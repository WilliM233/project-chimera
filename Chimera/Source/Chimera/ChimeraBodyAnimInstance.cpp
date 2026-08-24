// Copyright 2026 Flux Meridian. All Rights Reserved.


#include "ChimeraBodyAnimInstance.h"
#include "DefaultMovementSet/CharacterMoverComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"

void UChimeraBodyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// GetOwningActor + FindComponentByClass rather than TryGetPawnOwner:
	// works on any actor class, and returns null without complaint on actors
	// that have no Mover (preview windows, placed statues). Null is a legal
	// state here - every consumer checks before use
	if (const AActor* Owner = GetOwningActor())
	{
		Mover = Owner->FindComponentByClass<UCharacterMoverComponent>();
	}
}

void UChimeraBodyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Mover)
	{
		return;
	}

	Speed = Mover->GetVelocity().Size2D();
	bIsAirborne = Mover->IsAirborne();
	bIsGrounded = !bIsAirborne;

	// --- Foot IK: conformance, not locking. Offsets are the terrain delta
	//     under each foot relative to the character's own ground plane. On
	//     flat ground they are zero and the animation plays untouched. ---

	const float TargetAlpha = bIsGrounded ? 1.f : 0.f;
	LeftFootIKAlpha = FMath::FInterpTo(LeftFootIKAlpha, TargetAlpha, DeltaSeconds, AlphaInterpSpeed);
	RightFootIKAlpha = FMath::FInterpTo(RightFootIKAlpha, TargetAlpha, DeltaSeconds, AlphaInterpSpeed);

	if (bIsGrounded)
	{
		UpdateFootIK(TEXT("foot_l"), DeltaSeconds, SmoothedLeftOffsetZ, LeftFootIKOffset, SmoothedLeftRotation, LeftFootIKRotationOffset);
		UpdateFootIK(TEXT("foot_r"), DeltaSeconds, SmoothedRightOffsetZ, RightFootIKOffset, SmoothedRightRotation, RightFootIKRotationOffset);
	}
	else
	{
		// Airborne: ease offsets and rotations home, and republish, so
		// landing starts from neutral.
		SmoothedLeftOffsetZ = FMath::FInterpTo(SmoothedLeftOffsetZ, 0.f, DeltaSeconds, FootInterpSpeed);
		SmoothedRightOffsetZ = FMath::FInterpTo(SmoothedRightOffsetZ, 0.f, DeltaSeconds, FootInterpSpeed);
		SmoothedLeftRotation = FMath::RInterpTo(SmoothedLeftRotation, FRotator::ZeroRotator, DeltaSeconds, FootRotationInterpSpeed);
		SmoothedRightRotation = FMath::RInterpTo(SmoothedRightRotation, FRotator::ZeroRotator, DeltaSeconds, FootRotationInterpSpeed);
		LeftFootIKRotationOffset = SmoothedLeftRotation;
		RightFootIKRotationOffset = SmoothedRightRotation;
	}

	// Pelvis sinks by the most-negative offset so the low-side leg can reach.
	// Positive offsets need no pelvis help - Leg IK bends that knee instead.
	const float TargetPelvisZ = FMath::Min3(SmoothedLeftOffsetZ, SmoothedRightOffsetZ, 0.f);
	SmoothedPelvisZ = FMath::FInterpTo(SmoothedPelvisZ, TargetPelvisZ, DeltaSeconds, PelvisInterpSpeed);
	PelvisOffset = FVector(0.f, 0.f, SmoothedPelvisZ);
}

void UChimeraBodyAnimInstance::UpdateFootIK(FName FootBone, float DeltaSeconds, float& SmoothedOffsetZ, FVector& OutOffset, FRotator& SmoothedRotation, FRotator& OutRotation)
{
	USkeletalMeshComponent* Mesh = GetSkelMeshComponent();
	UWorld* World = GetWorld();
	if (!Mesh || !World)
	{
		return;
	}

	const FVector FootWorld = Mesh->GetSocketLocation(FootBone);

	// The character's own ground plane: the mesh component origin sits at the
	// capsule bottom by construction (the -CapsuleHalfHeight offset), so its
	// world Z *is* "the ground under the character".
	const float RootPlaneZ = Mesh->GetComponentLocation().Z;

	FHitResult Hit;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(ChimeraFootIK), /*bTraceComplex*/ false);
	Params.AddIgnoredActor(Mesh->GetOwner());

	// Trace at the foot's XY, through the root plane's Z - the window is
	// about where ground could plausibly be, not about where the foot is.
	const FVector TraceStart(FootWorld.X, FootWorld.Y, RootPlaneZ + TraceAboveFoot);
	const FVector TraceEnd(FootWorld.X, FootWorld.Y, RootPlaneZ - TraceBelowFoot);

	const bool bHit = World->LineTraceSingleByChannel(
		Hit, TraceStart, TraceEnd, ECC_Visibility, Params);

	// The terrain delta: how much higher/lower the ground under this foot is
	// than the ground the character stands on. Flat ground: zero, IK inert.
	float OffsetZ = bHit ? (Hit.ImpactPoint.Z - RootPlaneZ) : 0.f;
	OffsetZ = FMath::Clamp(OffsetZ, -MaxFootAdjustment, MaxFootAdjustment);

	SmoothedOffsetZ = FMath::FInterpTo(SmoothedOffsetZ, OffsetZ, DeltaSeconds, FootInterpSpeed);

	OutOffset = FVector(0.f, 0.f, SmoothedOffsetZ);

	// -- Rotation: tilt the foot to the surface it stands on --
	
	// The ground's "up" under this foot. Defaults to world up,
	// which produces zero rotation - the inert state, same as flat ground.
	FVector GroundNormal = FVector::UpVector;
	if (bHit)
	{
		// Walkable check: a normal's Z is its dot with world up, so
		// acos(Z) is the surface's angle from horizontal. Steeper than
		// walkable (riser faces, corner kisses) isn't ground - reject
		// outright and stay flat rather than clamping a wall.
		const float SurfaceAngle = FMath::RadiansToDegrees(
			FMath::Acos(FMath::Clamp(Hit.ImpactNormal.Z, -1.f, 1.f)));
		if (SurfaceAngle <= WalkableNormalAngle)
		{
			GroundNormal = Hit.ImpactNormal;
		}
	}

	// The tilt we want, as a world-space delta: whatever rotation takes
	// world-up onto the ground's up. Flat ground: identity, inert.
	FQuat WorldDelta = FQuat::FindBetweenNormals(FVector::UpVector, GroundNormal);

	// Clamp the tilt itself, not per-axis components.
	FVector TiltAxis; float TiltAngle;
	WorldDelta.ToAxisAndAngle(TiltAxis, TiltAngle);
	const float MaxRadians = FMath::DegreesToRadians(MaxFootRotationDegrees);
	if (TiltAngle > MaxRadians)
	{
		WorldDelta = FQuat(TiltAxis, MaxRadians);
	}

	// Re-express the world delta in the mesh component's frame - the
	// space the Modify Bone node applies rotation in. The conjugation
	// is where the mesh's -90 yaw gets accounted for.
	const FQuat CompQuat = Mesh->GetComponentQuat();
	const FQuat ComponentDelta = CompQuat.Inverse() * WorldDelta * CompQuat;

	const FRotator TargetRotation = ComponentDelta.Rotator();
	SmoothedRotation = FMath::RInterpTo(SmoothedRotation, TargetRotation, DeltaSeconds, FootRotationInterpSpeed);
	OutRotation = SmoothedRotation;
}
