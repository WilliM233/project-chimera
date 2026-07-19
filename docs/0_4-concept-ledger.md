# 0.4 — Concept Ledger

> *The permanent record of what the learner has demonstrated, keyed by slug.
> Born 2026-07-19 at the first formal close-out (`framework.actor-lifecycle`).
> Standing document — must live in project knowledge to survive container resets.*
>
> **Entry rules:** one entry per closed concept, appended at close-out, never
> rewritten (corrections append a note). `evidence` records what was actually
> demonstrated — it is written from the learner's own close-out narration, not
> from the chapter text. Concepts closed before the ledger existed are
> backfilled with `gate: loose`.

## Ledger

### `foundation.install` — closed 2026-07-18 — gate: loose
Engine 5.8 + toolchain installed and verified by first project launch.
Folded into the chapter-1 blend.

### `foundation.version-control` — closed 2026-07-18 — gate: loose
Git + Git LFS from the command line (manual-seams). `.gitignore`/`.gitattributes`
authored and committed first, alone. LFS proven in the field twice (`Sandbox.umap`,
verified via `git lfs ls-files`). Learner self-debugged repo-root path relativity
(`Chimera/Config/...`). Deliberate-commit habit for binary assets established.

### `foundation.project-setup` — closed 2026-07-18 — gate: loose
Blank C++ project, no starter content. First developer decision made unprompted:
created `Content/Maps/Sandbox` prototype level, set Editor Startup Map and Game
Default Map, read the resulting `DefaultEngine.ini` diff. `Content/Maps/` folder
discipline from map one.

### `foundation.editor` — closed 2026-07-18 — gate: loose (compressed)
Prior familiarity self-reported (upper beginner); chapter compressed to fit.
Details panel, Outliner, Content Browser fluency confirmed through use in
subsequent chapters rather than dedicated exercises.

### `foundation.viewport` — closed 2026-07-18 — gate: loose (compressed)
Navigation fluency confirmed by learner; validated through sandbox build-out.

### `framework.actors` — closed 2026-07-19 — gate: loose
Populated Sandbox with test actors (lamp post, staircase, slalom walls).
Instance-vs-asset distinction (StaticMeshActor pointing at mesh assets).
Outliner folder hygiene. Deliberate map commit.

### `framework.components` — closed 2026-07-18 — gate: loose
Composite actor built by hand from an empty Actor: StaticMeshComponent post +
child PointLightComponent. Root vs. child, world vs. relative transform
demonstrated by moving parent and nudging child. UActorComponent /
USceneComponent / UPrimitiveComponent hierarchy filed. Component-tree reading
established as reflex.

### `framework.actor-lifecycle` — closed 2026-07-19 — gate: formal DoD ✦ first formal close-out
First C++ class written (`LifecycleProbe`): UE_LOG in constructor, BeginPlay,
Tick, and a hand-added EndPlay override with reason logging. Evidence from
learner narration:
- Constructor fires at editor drop-in, and *again* at Play — PIE duplicates the
  editor world (construct + copy properties), so two births, one BeginPlay.
- BeginPlay understood as the life-start signal vs. constructor as object-build.
- DeltaSeconds as frame-to-frame time; Tick flood observed raw, then throttled
  with a time accumulator (first deliberate Tick-cost management). Frame-rate
  independence corrected into the record at the gate.
- EndPlay reason 2 (EndPlayInEditor: world died, took the Actor) vs. reason 0
  (Destroyed via `destroyall`: Actor died, world survived) — separate lifespans
  witnessed both ways.
Incidental learnings banked: macro error shrapnel (only the first error on a
macro line is real), commit vs. physical memory in build tooling, squiggle-says-
no-compiler-decides, copyright notice mechanism (`DefaultGame.ini`), split
commits for unrelated changes.

### `framework.pawns-controllers` — closed 2026-07-19 — gate: formal DoD
Pawn/Controller/Character split demonstrated live. `ChimeraCharacter` C++ class
created (parent: Character) with `PossessedBy`/`UnPossessed` overrides logging
the marriage. Evidence:
- DefaultPawn ghost identified and inspected (engine-spawned free-flyer when no
  pawn is claimed); F8 eject/repossess cycle logged: possessed → unpossessed →
  repossessed, same body, same controller.
- Stowaway diagnosed from the Outliner: `Auto Possess AI: Placed in World`
  (Character default) spawned an AIController into the placed pawn before any
  player involvement. Two stillnesses distinguished at the gate: unpossessed
  pawn *can't* act; AI-possessed pawn with empty controller *won't*.
- Manual possession achieved via `Auto Possess Player: Player 0` after the
  stock `possess` console command proved nonexistent in UE5 (silent
  non-recognition noted as console behavior).
- Correction at gate: learner initially placed the will inside the pawn's
  wiring; corrected — controllers are independent Actors, persist unpossessed
  (the F8 free-flight *is* the detached controller), and can re-marry bodies.
Long-term design conversation occurred here: see `0.5-design.md`.

### `framework.gamemode` — closed 2026-07-19 — gate: formal DoD
`ChimeraGameMode` (parent: GameModeBase) with `DefaultPawnClass =
AChimeraCharacter::StaticClass()` in constructor; installed via Project
Settings → Maps & Modes (→ `DefaultEngine.ini`, plain-text diff hymn, verse
four). Auto Possess Player checkbox retired; placed zombie pawn deleted
deliberately. Verified: fresh ChimeraCharacter instance spawned at PlayerStart
each session, possessed by PlayerController_0, F8 in/out working.
- Correction at gate: learner conflated the two auto-possess properties
  (Auto Possess AI spawned the stowaway; Auto Possess Player was the retired
  instance-fact) and described GameMode as possessing the placed pawn —
  corrected: GameMode *spawns a new pawn* from DefaultPawnClass; instance-fact
  became class-rule. First contact with `StaticClass()` / reflection, deferred
  to scripting.unreal-cpp.
**Part II closed at this entry.**
