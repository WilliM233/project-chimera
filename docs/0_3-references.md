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

## Standing documents

| Doc | Purpose |
|---|---|
| `0.0-toc.md` | 43-concept spine, slug-based, UE 5.8 target |
| `0.1-voices.md` | Voice A (org memo) / B (mentor) / C (reference) |
| `0.2-ticket-schema.md` | Framework ticket schema |
| `0.3-references.md` | This document — live pointers |
| Concept ledger | Not yet born — minted at first ticket close-out |

## Chapter log

| # | Slug | Status | Ticket/Issue |
|---|---|---|---|
| 1 | foundation.project-setup + foundation.version-control | In progress — first launch underway | — |

*Note: run opened loose — chapter 1 delivered conversationally, blending
project-setup and version-control rather than as separate ticketed chapters.
Structure to be adopted through use, not upfront.*

## Key technical references established

- `.gitignore` and `.gitattributes` for Unreal — delivered in chapter 1;
  canonical copies live in the learner repo root
- Ignore list: `Binaries/ Intermediate/ DerivedDataCache/ Saved/ Build/` + IDE/OS noise
- LFS tracks: `*.uasset *.umap` + raw binary imports (`*.fbx *.png *.wav`)
- First-commit rule: ignore/attributes files committed alone, before any content

## Open threads

- Concept ledger to be minted at first close-out
- Part A generator skill design conversation still pending (flagged last session)
- Lore (Epic's VCS) parked as "worth evaluating someday" — Git chosen for GitHub-ticket synergy
