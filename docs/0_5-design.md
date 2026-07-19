# 0.5 — Design

> *The learner's game. Standing document — every chapter generates against this
> instead of a generic project. Born 2026-07-19 from the design conversation at
> `framework.pawns-controllers`. Grows as decisions are made; decisions get
> dated. Standing document — must live in project knowledge.*

## Vision

**Target feel:** RDR2-style weightiness — deliberate, grounded, animation-driven
movement — combined with free-running/traversal elements in the Assassin's
Creed vein. Character-first game; movement feel is the core pillar.

**Reference points:** Red Dead Redemption 2 (weight, groundedness), Assassin's
Creed (flow, traversal).

## Decisions locked (2026-07-19)

| Decision | Choice | Rationale |
|---|---|---|
| Character foundation | C++ `Character` subclass (`ChimeraCharacter`) | Movement feel lives in movement code + animation; wants the C++ floor with Blueprint iteration on top (the hybrid seam) |
| Skeleton | **UE5 standard skeleton** (Manny/Quinn) for all animation work | MetaHumans are animation-compatible with the standard skeleton; building against it keeps every hour of animation work portable — MetaHuman lands later as mesh-and-retarget, not rebuild |
| Player mesh (eventual) | MetaHuman | Swapped in via retarget once movement/animation foundation exists |
| Camera architecture | **One body, two rigs**: spring arm + camera (third person) and head-mounted camera (first person), runtime toggle | Third/first person is a camera question, not a character question; possession model makes this natural |
| First-person style | **Undecided — both doors held open** | True first person (visible body, head-bone camera) fits the RDR2 reference but costs real animation work; floating-arms is cheaper. Two-rig structure closes neither door. Decide when first-person work begins. |

## Deferred to spine slots

| Ambition | Where it lives |
|---|---|
| Weightiness (acceleration curves, turn rates) | `interaction.movement` → `interaction.advanced-movement` |
| Animation-driven weight | `systems.animation` → `systems.montages-root-motion` (root motion is the RDR2 mechanism) |
| Free-running / traversal | `interaction.advanced-movement` + eventual custom movement modes on CharacterMovementComponent |
| First/third person toggle | `interaction.cameras` |
| MetaHuman integration | Post-animation-foundation; retarget operation |

## Principles

- **One bite at a time** (learner's own words): the vision constrains
  foundations now (skeleton, class choice, camera structure) but does not get
  to demand features early. "Something moving around" arrives via the spine at
  Part IV, properly gated.
- Foundations are chosen so no future door closes; ambitions wait in their
  spine slots.
