// Copyright 2026 Flux Meridian. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MoverSimulationTypes.h"
#include "ChimeraPlayerPawn.generated.h"

class UCapsuleComponent;
class UCharacterMoverComponent;
class UMoverNetworkPredictionLiaisonComponent;
class USpringArmComponent;
class UCameraComponent;
class USceneComponent;
class UInputAction;
class UInputComponent;
class USkeletalMeshComponent;
class ULODSyncComponent;
class UMetaHumanComponentUE;
class UGroomComponent;

struct FInputActionValue;

USTRUCT(BlueprintType)
struct FCameraFraming
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Chimera|Camera")
	float TargetArmLength = 300.f;

	// Profile intent; the global envelope in Tick has final say.
	UPROPERTY(EditAnywhere, Category = "Chimera|Camera")
	float PitchMin = -60.f;

	UPROPERTY(EditAnywhere, Category = "Chimera|Camera")
	float PitchMax = 55.f;

	// Screen-space nudge at the camera end: Y = right, Z = up.
	UPROPERTY(EditAnywhere, Category = "Chimera|Camera")
	FVector SocketOffset = FVector::ZeroVector;
};

UENUM()
enum class ECameraDriverMode : uint8
{
	GaitDriven,
	Manual
};

UCLASS()
class CHIMERA_API AChimeraPlayerPawn : public APawn, public IMoverInputProducerInterface
{
	GENERATED_BODY()

public:
	AChimeraPlayerPawn();

protected:
	virtual void BeginPlay() override;

	/* Movement */
	UPROPERTY(VisibleAnywhere, Category = "Chimera|Movement")
	TObjectPtr<UCapsuleComponent> Capsule;

	UPROPERTY(VisibleAnywhere, Category = "Chimera|Movement")
	TObjectPtr<UCharacterMoverComponent> Mover;
	UPROPERTY(VisibleAnywhere, Category = "Chimera|Movement")
	TObjectPtr<UMoverNetworkPredictionLiaisonComponent> MoverBackend;

	/* Camera */
	UPROPERTY(VisibleAnywhere, Category = "Chimera|Camera")
	TObjectPtr<USceneComponent> VisualRoot;
	UPROPERTY(VisibleAnywhere, Category = "Chimera|Camera")
	TObjectPtr<USpringArmComponent> SpringArm;
	UPROPERTY(VisibleAnywhere, Category = "Chimera|Camera")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, Category = "Chimera|Camera")
	FCameraFraming WalkFraming{ 220.f, -50.f, 45.f, FVector(0.f, 50.f, 12.f) };
	UPROPERTY(EditAnywhere, Category = "Chimera|Camera")
	FCameraFraming JogFraming{ 280.f, -55.f, 50.f, FVector(0.f, 40.f, 10.f) };
	UPROPERTY(EditAnywhere, Category = "Chimera|Camera")
	FCameraFraming RunFraming{ 340.f, -60.f, 55.f, FVector(0.f, 25.f, 6.f) };
	UPROPERTY(EditAnywhere, Category = "Chimera|Camera")
	FCameraFraming SprintFraming{ 420.f, -70.f, 60.f, FVector(0.f, 0.f, 0.f) };

	// The law. Profiles express intent; nothing escapes these.
	UPROPERTY(EditAnywhere, Category = "Chimera|Camera")
	float GlobalPitchMin = -75.f;
	UPROPERTY(EditAnywhere, Category = "Chimera|Camera")
	float GlobalPitchMax = 70.f;

	UPROPERTY(EditAnywhere, Category = "Chimera|Camera")
	float FramingInterpSpeed = 5.f;

	/* Appearance */
	UPROPERTY(VisibleAnywhere, Category = "Chimera|Appearance")
	TObjectPtr<USkeletalMeshComponent> Body;
	UPROPERTY(VisibleAnywhere, Category = "Chimera|Appearance")
	TObjectPtr<USkeletalMeshComponent> Face;
	UPROPERTY(VisibleAnywhere, Category = "Chimera|Appearance")
	TObjectPtr<ULODSyncComponent> LODSync;
	UPROPERTY(VisibleAnywhere, Category = "Chimera|Appearance")
	TObjectPtr<UMetaHumanComponentUE> MetaHuman;
	UPROPERTY(VisibleAnywhere, Category = "Chimera|Appearance")
	TObjectPtr<UGroomComponent> Eyelashes;
	UPROPERTY(VisibleAnywhere, Category = "Chimera|Appearance")
	TObjectPtr<UGroomComponent> Eyebrows;
	UPROPERTY(VisibleAnywhere, Category = "Chimera|Appearance")
	TObjectPtr<UGroomComponent> Beard;
	UPROPERTY(VisibleAnywhere, Category = "Chimera|Appearance")
	TObjectPtr<UGroomComponent> Fuzz;
	UPROPERTY(VisibleAnywhere, Category = "Chimera|Appearance")
	TObjectPtr<UGroomComponent> Hair;
	UPROPERTY(VisibleAnywhere, Category = "Chimera|Appearance")
	TObjectPtr<UGroomComponent> Mustache;

	/* Input */
	UPROPERTY(EditDefaultsOnly, Category = "Chimera|Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Chimera|Input")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "Chimera|Input")
	TObjectPtr<UInputAction> ToggleWalkAction;
	UPROPERTY(EditDefaultsOnly, Category = "Chimera|Input")
	TObjectPtr<UInputAction> SpeedUpAction;
	UPROPERTY(EditDefaultsOnly, Category = "Chimera|Input")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "Chimera|Input")
	TObjectPtr<UInputAction> CycleCameraAction;


	UPROPERTY(EditAnywhere, Category = "Chimera|Input")
	bool bWalkMode = false;
	UPROPERTY(EditAnywhere, Category = "Chimera|Input")
	bool bSpeedUp = false;
	UPROPERTY(EditAnywhere, Category = "Chimera|Input")
	float WalkModeMaxSpeed = 200.f;
	UPROPERTY(EditAnywhere, Category = "Chimera|Input")
	float RunModeMaxSpeed = 500.f;
	UPROPERTY(EditAnywhere, Category = "Chimera|Input")
	float JogMaxSpeed = 450.f;
	UPROPERTY(EditAnywhere, Category = "Chimera|Input")
	float SprintMaxSpeed = 700.f;
	
	bool bJumpJustPressed = false;
	
	void OnMove(const FInputActionValue& Value);
	void OnMoveCompleted(const FInputActionValue& Value);
	void OnLook(const FInputActionValue& Value);
	void OnWalkToggle(const FInputActionValue& Value);
	void OnSpeedUp(const FInputActionValue& Value);
	void OnSlowDown(const FInputActionValue& Value);
	void OnJump(const FInputActionValue& Value);
	void OnCycleCamera(const FInputActionValue& Value);
	virtual void Tick(float DeltaSeconds) override;
	FVector2D CachedMoveInput = FVector2D::ZeroVector;
	FCameraFraming CurrentFraming;
	FCameraFraming TargetFraming;
	ECameraDriverMode CameraDriverMode = ECameraDriverMode::GaitDriven;
	int32 ManualFramingIndex = 0;


	// Seam between input layer and mover
	virtual void ProduceInput_Implementation(int32 SimTimeMs, FMoverInputCmdContext& InputCmdResult) override;

public:	

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	void ApplyGaitSettings();
};
