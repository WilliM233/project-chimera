// Copyright 2026 Flux Meridian. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ChimeraTypes.h"
#include "ChimeraBodyAnimInstance.generated.h"


class UCharacterMoverComponent;
class AChimeraPlayerPawn;

/**
 * Anim instance for the Chimera body: publishes locomotion state read from
 * the Mover component, and runs the foot IK conformance solve - terrain
 * deltas and surface tilt per foot, pelvis drop to match. Everything the
 * AnimGraph consumes is published as plain properties for Fast Path reads.
 */
UCLASS()
class CHIMERA_API UChimeraBodyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    // -- Published for the AnimGraph (read via Fast Path) --

    UPROPERTY(BlueprintReadOnly, Category = "Chimera|Locomotion")
    float Speed = 0.f;

    UPROPERTY(BlueprintReadOnly, Category = "Chimera|Locomotion")
    bool bIsAirborne = false;

    UPROPERTY(BlueprintReadOnly, Category = "Chimera|Locomotion")
    bool bIsGrounded = true;

    UPROPERTY(BlueprintReadOnly, Category = "Chimera|Locomotion")
    float VerticalSpeed = 0.f;

    UPROPERTY(BlueprintReadOnly, Category = "Chimera|Locomotion")
    EChimeraGait Gait = EChimeraGait::Jog;

    UPROPERTY(BlueprintReadOnly, Category = "Chimera|Locomotion")
    bool bIsMoving = false;

    UPROPERTY(EditAnywhere, Category = "Chimera|Locomotion")
    float MovingSpeedThreshold = 10.f;

    UPROPERTY(Transient)
    TObjectPtr<AChimeraPlayerPawn> Pawn = nullptr;


    // Additive component-space offsets for the ik feet: the terrain delta
    // under each foot. Applied on top of the authored ik tracks - the
    // animation keeps the stride, we shift it vertically. Flat ground: zero.
    UPROPERTY(BlueprintReadOnly, Category = "Chimera|FootIK")
    FVector LeftFootIKOffset = FVector::ZeroVector;

    UPROPERTY(BlueprintReadOnly, Category = "Chimera|FootIK")
    FVector RightFootIKOffset = FVector::ZeroVector;

    // Additive component-space tilt for the ik feet: the rotation that
    // takes flat onto the surface under each foot. Applied on top of the
    // authored orientation. Flat ground: zero, animation untouched.
    UPROPERTY(BlueprintReadOnly, Category = "Chimera|FootIK")
    FRotator LeftFootIKRotationOffset = FRotator::ZeroRotator;

    UPROPERTY(BlueprintReadOnly, Category = "Chimera|FootIK")
    FRotator RightFootIKRotationOffset = FRotator::ZeroRotator;

    // Blend authority per foot: 0 = animation owns it (swing / airborne),
    // 1 = trace owns it (planted). Consumed as the Modify Bone alpha pins.
    UPROPERTY(BlueprintReadOnly, Category = "Chimera|FootIK")
    float LeftFootIKAlpha = 0.f;

    UPROPERTY(BlueprintReadOnly, Category = "Chimera|FootIK")
    float RightFootIKAlpha = 0.f;

    // Additive: the whole pose sinks by this so the longest-reaching leg
    // can touch its ground. Z is always <= 0; XY always zero. Published
    // as a vector so the graph feeds the Translation pin on Fast Path
    // with no assembly nodes.
    UPROPERTY(BlueprintReadOnly, Category = "Chimera|FootIK")
    FVector PelvisOffset = FVector::ZeroVector;

    // -- Foot IK tuning: every number Foot Placement hid in a panel, owned --

    // Trace window around each foot, in cm.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chimera|FootIK|Config")
    float TraceAboveFoot = 50.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chimera|FootIK|Config")
    float TraceBelowFoot = 75.f;

    // Corrections are clamped to this so bad trace data can't fold the legs.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chimera|FootIK|Config")
    float MaxFootAdjustment = 35.f;

    // Interp speeds (per second). Our Linear Stiffness knobs, honestly named.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chimera|FootIK|Config")
    float FootInterpSpeed = 13.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chimera|FootIK|Config")
    float PelvisInterpSpeed = 10.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chimera|FootIK|Config")
    float AlphaInterpSpeed = 10.f;

    // Clamp on the tilt angle itself (not per-axis) so bad normals can't
    // wrench the foot.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chimera|FootIK|Config")
    float MaxFootRotationDegrees = 30.f;

    // Rotation's own stiffness knob, same family as the others.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chimera|FootIK|Config")
    float FootRotationInterpSpeed = 10.f;

    // Surfaces steeper than this from horizontal aren't ground (riser
    // faces, corner kisses) - rejected outright, foot stays flat. Same
    // worldview as the movement system's walkable slope.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chimera|FootIK|Config")
    float WalkableNormalAngle = 45.f;

private:
    UPROPERTY()
    TObjectPtr<UCharacterMoverComponent> Mover;

    // Interpolation state. Vertical conformance in world-Z terms:
    float SmoothedLeftOffsetZ = 0.f;
    float SmoothedRightOffsetZ = 0.f;
    float SmoothedPelvisZ = 0.f;

    // Surface tilt, in the mesh component's frame (the space the Modify
    // Bone nodes apply rotation in):
    FRotator SmoothedLeftRotation = FRotator::ZeroRotator;
    FRotator SmoothedRightRotation = FRotator::ZeroRotator;

    // One foot's trace-and-solve; returns the smoothed values through refs.
    void UpdateFootIK(FName FootBone, float DeltaSeconds, float& SmoothedOffsetZ, FVector& OutOffset, FRotator& SmoothedRotation, FRotator& OutRotation);

};
