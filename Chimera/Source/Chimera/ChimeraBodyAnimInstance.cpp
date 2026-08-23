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
		UpdateFootIK(TEXT("foot_l"), DeltaSeconds, SmoothedLeftOffsetZ, LeftFootIKOffset);
		UpdateFootIK(TEXT("foot_r"), DeltaSeconds, SmoothedRightOffsetZ, RightFootIKOffset);
	}
	else
	{
		// Airborne: ease the offsets home so landing starts from neutral.
		SmoothedLeftOffsetZ = FMath::FInterpTo(SmoothedLeftOffsetZ, 0.f, DeltaSeconds, FootInterpSpeed);
		SmoothedRightOffsetZ = FMath::FInterpTo(SmoothedRightOffsetZ, 0.f, DeltaSeconds, FootInterpSpeed);
	}

	// Pelvis sinks by the most-negative offset so the low-side leg can reach.
	// Positive offsets need no pelvis help - Leg IK bends that knee instead.
	const float TargetPelvisZ = FMath::Min3(SmoothedLeftOffsetZ, SmoothedRightOffsetZ, 0.f);
	SmoothedPelvisZ = FMath::FInterpTo(SmoothedPelvisZ, TargetPelvisZ, DeltaSeconds, PelvisInterpSpeed);
	PelvisOffsetZ = SmoothedPelvisZ;
}

void UChimeraBodyAnimInstance::UpdateFootIK(FName FootBone, float DeltaSeconds, float& SmoothedOffsetZ, FVector& OutOffset)
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
}
