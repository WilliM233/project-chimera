"""
Chimera — Body Set Skeleton Lock Verification
==============================================
Diffs the reference-pose skeleton of every endpoint body against the anchor.
Run AFTER the anchor-skeleton import step (body-set spec §8.3); zero delta = lock held.
Also useful BEFORE import to see the authored per-body joint drift (expect the fem
clavicle mismatch and the Masc_Large strain to show up there).

Usage: Editor > Tools > Execute Python Script (or paste into the Python console).
Requires the Python Editor Script Plugin (on by default in 5.x).

Why component-level reads: all bodies share the USkeleton asset (metahuman_base_skel),
so skeleton-asset APIs (e.g. AnimPoseExtensions.get_reference_pose) return the same
pose for every body — useless for this diff. Each USkeletalMesh carries its own
reference skeleton; SkinnedMeshComponent.get_ref_pose_position() reads it per-mesh.
Positions are parent-relative (local space), so per-bone deltas capture both joint
placement and bone length (length = |local position|).

API sanity check if anything errors:
    help(unreal.SkinnedMeshComponent.get_ref_pose_position)
    help(unreal.SkeletalMeshComponent.set_skeletal_mesh_asset)
"""

import unreal

# ----------------------------------------------------------------------------- config

ANCHOR_MESH = "/Game/Player/MetaHuman/MHC_Chimera_Neutral_Average/Body/SKM_MHC_Chimera_Neutral_Average_BodyMesh.SKM_MHC_Chimera_Neutral_Average_BodyMesh"  # TODO: anchor body SKM path

ENDPOINT_MESHES = [
    # TODO: fill in as endpoints are assembled + skeleton-imported
    # "/Game/MetaHumans/MHC_Chimera_Masc_Average/...",
    # "/Game/MetaHumans/MHC_Chimera_Fem_Average/...",
]

USE_SELECTION = True   # if True and content-browser selection is non-empty, diff the
                       # selected skeletal meshes against ANCHOR_MESH instead of the list

POS_TOLERANCE_CM = 0.001   # max allowed per-axis-combined positional delta (cm)
LEN_TOLERANCE_CM = 0.001   # max allowed bone-length delta (cm)
TOP_OFFENDERS = 10         # how many worst bones to print per failing body

EXCLUDE_BONES = {"ik_foot_root", "ik_foot_l", "ik_foot_r"}
# PERMANENT, BY CONTRACT (decision 2026-08-30): the anchor carries these three
# hand-restored IK bones; MHC's From Template import validator rejects nonstandard
# hierarchy, so imports run from the stripped _Pipeline/SKM_Anchor_ImportSource_NoIK
# and endpoints legitimately lack them. Endpoints are morph donors / clothing source
# bodies — never animated, never possessed — and IK bones carry no skinning, so the
# difference is functionally void. The exclusion is part of the verified contract,
# not a hole in it. Known blind spot (accepted): a bone with one of these names on
# BOTH sides would be skipped entirely.

# --------------------------------------------------------------------------- internals

def _spawn_reader():
    eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    actor = eas.spawn_actor_from_class(
        unreal.SkeletalMeshActor, unreal.Vector(0.0, 0.0, -100000.0)
    )
    return eas, actor


def read_ref_skeleton(mesh, component):
    """Returns {bone_name: (local_ref_pos: Vector, parent_name: str)} for a mesh."""
    component.set_skeletal_mesh_asset(mesh)
    out = {}
    for i in range(component.get_num_bones()):
        name = str(component.get_bone_name(i))
        if name in EXCLUDE_BONES:
            continue
        pos = component.get_ref_pose_position(i)   # parent-relative, reference pose
        parent = str(component.get_parent_bone(component.get_bone_name(i)))
        out[name] = (pos, parent)
    return out


def diff_body(anchor_data, body_data, body_label):
    anchor_names = set(anchor_data.keys())
    body_names = set(body_data.keys())

    missing = anchor_names - body_names
    extra = body_names - anchor_names
    if missing or extra:
        unreal.log_error(f"[{body_label}] BONE SET MISMATCH — lock FAILED before transforms")
        if missing:
            unreal.log_error(f"  missing vs anchor: {sorted(missing)}")
        if extra:
            unreal.log_error(f"  extra vs anchor:   {sorted(extra)}")
        return False

    rows = []  # (pos_delta, len_delta, bone, parent_changed)
    for name in anchor_names:
        a_pos, a_parent = anchor_data[name]
        b_pos, b_parent = body_data[name]
        d = b_pos - a_pos
        pos_delta = d.length()
        len_delta = abs(b_pos.length() - a_pos.length())
        rows.append((pos_delta, len_delta, name, a_parent != b_parent))

    rows.sort(reverse=True)
    max_pos, max_len = rows[0][0], max(r[1] for r in rows)
    hierarchy_broken = [r[2] for r in rows if r[3]]

    ok = (max_pos <= POS_TOLERANCE_CM and max_len <= LEN_TOLERANCE_CM
          and not hierarchy_broken)

    verdict = "LOCK HELD" if ok else "LOCK FAILED"
    log = unreal.log if ok else unreal.log_error
    log(f"[{body_label}] {verdict} — max pos delta {max_pos:.6f} cm, "
        f"max length delta {max_len:.6f} cm over {len(rows)} bones")

    if hierarchy_broken:
        unreal.log_error(f"  parent changed on: {hierarchy_broken}")
    if not ok:
        unreal.log_error(f"  worst {TOP_OFFENDERS} bones (pos delta cm | length delta cm):")
        for pos_delta, len_delta, name, _ in rows[:TOP_OFFENDERS]:
            unreal.log_error(f"    {name:32s} {pos_delta:10.6f} | {len_delta:10.6f}")
    return ok


def _gather_targets():
    if USE_SELECTION:
        selected = [a for a in unreal.EditorUtilityLibrary.get_selected_assets()
                    if isinstance(a, unreal.SkeletalMesh)]
        if selected:
            return [(m.get_path_name(), m) for m in selected]
    loaded = []
    for path in ENDPOINT_MESHES:
        mesh = unreal.load_asset(path)
        if mesh is None:
            unreal.log_error(f"could not load: {path}")
        else:
            loaded.append((path, mesh))
    return loaded


def main():
    anchor = unreal.load_asset(ANCHOR_MESH)
    if anchor is None:
        unreal.log_error(f"anchor not found: {ANCHOR_MESH}")
        return

    targets = _gather_targets()
    if not targets:
        unreal.log_error("no endpoint meshes to diff (fill ENDPOINT_MESHES or select "
                         "skeletal meshes in the content browser)")
        return

    eas, actor = _spawn_reader()
    try:
        comp = actor.skeletal_mesh_component
        anchor_data = read_ref_skeleton(anchor, comp)
        unreal.log(f"anchor: {ANCHOR_MESH} ({len(anchor_data)} bones)")

        results = {}
        for path, mesh in targets:
            label = path.rsplit("/", 1)[-1]
            results[label] = diff_body(anchor_data, read_ref_skeleton(mesh, comp), label)
    finally:
        eas.destroy_actor(actor)

    unreal.log("=" * 60)
    passed = sum(1 for v in results.values() if v)
    unreal.log(f"skeleton lock: {passed}/{len(results)} bodies passed")
    for label, ok in sorted(results.items()):
        unreal.log(f"  {'PASS' if ok else 'FAIL':4s}  {label}")


main()
