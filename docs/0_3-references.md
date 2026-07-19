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
| Run started | 2026-07-18 |
| Studio name (fiction) | Flux Meridian |
| Copyright line | `Copyright 2026 Flux Meridian. All Rights Reserved.` |

## Standing documents

| Doc | Purpose |
|---|---|
| `0.0-toc.md` | 43-concept spine, slug-based, UE 5.8 target |
| `0.1-voices.md` | Voice A (org memo) / B (mentor) / C (reference) |
| `0.2-ticket-schema.md` | Framework ticket schema — ⚠ not currently in project knowledge |
| `0.3-references.md` | This document — live pointers |
| `0.4-concept-ledger.md` | Concept ledger — born 2026-07-19 at first formal close-out |
| `0.5-design.md` | The learner's game — vision, locked decisions, deferred ambitions |

## Chapter log

| # | Slug(s) | Status | Gate |
|---|---|---|---|
| 1 | foundation.install + version-control + project-setup | Closed 2026-07-18 | loose |
| 2 | foundation.editor + foundation.viewport | Closed 2026-07-18 (compressed — prior familiarity) | loose |
| 3 | framework.actors | Closed 2026-07-19 | loose |
| 4 | framework.components | Closed 2026-07-18 | loose |
| 5 | framework.actor-lifecycle | Closed 2026-07-19 | **formal DoD — first** |
| 6 | framework.pawns-controllers | Closed 2026-07-19 | formal DoD |
| 7 | framework.gamemode | Closed 2026-07-19 — **Part II complete** | formal DoD |
| 8 | scripting.blueprints | Next up — opens Part III | — |

*Run opened loose — early chapters delivered conversationally. First formal
Definition-of-Done gate passed at actor-lifecycle; ledger minted there.
Structure being adopted through use, as designed.*

## Key technical references established

- `.gitignore` / `.gitattributes` canonical copies in learner repo root;
  ignore list `Binaries/ Intermediate/ DerivedDataCache/ Saved/ Build/` + IDE/OS
  noise; LFS tracks `*.uasset *.umap` + raw binary imports
- First-commit rule: ignore/attributes committed alone, before content
- `Content/Maps/Sandbox` — prototype slab, Editor Startup + Game Default map;
  landfill-in-waiting, commits deliberate (binary, no diff)
- `LifecycleProbe` class — permanent Sandbox resident, throttled Tick;
  working reference for lifecycle overrides
- Copyright notice set in Project Settings → writes `DefaultGame.ini`;
  retroactive-never (existing files swept by hand)
- Split-commit hygiene: unrelated changes = separate commits
- `ChimeraCharacter` (C++, parent Character) — the player class; PossessedBy/
  UnPossessed logging in place; grows into the real player per `0.5-design.md`
- `ChimeraGameMode` (C++, parent GameModeBase) — DefaultPawnClass rule;
  installed as project default via Maps & Modes
- Console gotcha: no stock `possess` command in UE5; unknown commands fail
  *silently* — no response usually means no such command

## Open threads

- **`0.2-ticket-schema.md` missing from project knowledge** — needs re-upload
  before Part A generator design work
- Part A generator skill design conversation still pending (flagged two sessions ago)
- Lore (Epic's VCS) parked as "worth evaluating someday"
- Audio hum for the lamp post pocketed for `systems.audio` (Management need not know)
