# 0.3 — References

> *Running document of live pointers for L's Chimera run. Grows with use.
> Standing document — must live in project knowledge to survive container resets.*

## The Run

| Item | Value |
|---|---|
| Learner repo | https://github.com/WilliM233/project-chimera |
| Engine target | UE 5.8 |
| Project template | Blank, C++, no starter content |
| Version control | Git + Git LFS, command-line driven (manual-seams) |
| Repo layout | Repo root > `Chimera/` (project) + `docs/` (standing documents) |
| Run started | 2026-07-18 |

## Standing documents

| Doc | Purpose |
|---|---|
| `0.0-toc.md` | 43-concept spine, slug-based, UE 5.8 target |
| `0.1-voices.md` | Voice A (org memo) / B (mentor) / C (reference) |
| `0.2-ticket-schema.md` | Framework ticket schema |
| `0.3-references.md` | This document — live pointers; canonical copy in repo `docs/` |
| Concept ledger | Not yet born — minted at first ticket close-out |

## Chapter log

| # | Slug(s) | Status | Notes |
|---|---|---|---|
| 1 | foundation.project-setup + foundation.version-control | **Closed** 2026-07-18 | Blended chapter; `.slnx` leak caught & fixed; first-commit rule executed |
| 2 | foundation.editor + foundation.viewport | **Closed** 2026-07-18 | Compressed — learner has prior editor familiarity (self-assessed upper beginner) |
| 3 | framework.actors | **Closed** 2026-07-18 | Sandbox furnished; brush→static-mesh conversion; asset naming conventions |
| 4 | framework.components | **Closed** 2026-07-18 | Composite lamp post built by hand; component tree literacy; relative transforms |
| 5 | framework.actor-lifecycle | **Next** | Opens with memo next session; first C++ chapter; meatier — flagged to start fresh |

*Note: run opened loose — chapters delivered conversationally rather than as
GitHub-ticketed chapters. Structure to be adopted through use, not upfront.*

## Project decisions log

- **Sandbox-first**: default Open World map rejected for prototyping; flat Basic-template
  level `Content/Maps/Sandbox.umap` created and set as both Editor Startup Map and
  Game Default Map (written to `Config/DefaultEngine.ini`)
- **Content taxonomy started**: `Content/Maps/` for levels, `Content/Meshes/` for meshes —
  folders minted as needed, one at a time
- **Brush geometry avoided**: BSP brushes converted to static meshes when encountered;
  primitives preferred for blockout
- **Naming discipline**: actors named for role in scene (`Ramp_10d`, `LampPost_1`);
  assets named by type prefix (`SM_Stairs_Sandbox`); Outliner organized with folders

## Key technical references established

- `.gitignore` and `.gitattributes` for Unreal — canonical copies in learner repo root
- Ignore list: `Binaries/ Intermediate/ DerivedDataCache/ Saved/ Build/` + IDE/OS noise
  + **`*.slnx`** (5.8-era VS solution format — engine-version drift caught in the field)
- LFS tracks: `*.uasset *.umap` + raw binary imports (`*.fbx *.png *.wav`) — verified
  working twice (`Sandbox.umap`, `SM_Stairs_Sandbox.uasset`)
- First-commit rule: ignore/attributes files committed alone, before any content
- Path note: Git commands run from repo root; project paths prefixed `Chimera/`
- Diagnostic tools taught: `git check-ignore -v`, `git lfs ls-files`, `git status --ignored`
- Unreal path conventions: `/Game/` = project `Content/`; `Package.ObjectName` format
- Asset prefix convention: `SM_ SK_ M_ MI_ T_ BP_ WBP_ A_/AM_`
- Component hierarchy: `UActorComponent` (logic) → `USceneComponent` (+transform) →
  `UPrimitiveComponent` (+geometry/collision)
- Editor snapping: no native nudge key; snap-drag workflow, `Ctrl+End` snap-to-grid,
  `End` snap-to-floor, `V` vertex snapping

## Open threads

- Concept ledger to be minted at first close-out — **note: four chapters have now closed
  conversationally without it; decide at next session whether to backfill or start
  ledger from chapter 5 forward**
- Part A generator skill design conversation still pending (flagged two sessions ago)
- Ticket/GitHub-issue delivery not yet in use — structure adopting through use
- Lore (Epic's VCS) parked as "worth evaluating someday"
- Reusable actor classes (Blueprint vs C++) — flagged as the natural question after
  the hand-built lamp post; lands in Part III
