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

struct FInputActionValue;

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

	/* Appearance */
	UPROPERTY(VisibleAnywhere, Category = "Chimera|Appearance")
	TObjectPtr<USkeletalMeshComponent> Body;
	UPROPERTY(VisibleAnywhere, Category = "Chimera|Appearance")
	TObjectPtr<USkeletalMeshComponent> Face;
	UPROPERTY(VisibleAnywhere, Category = "Chimera|Appearance")
	TObjectPtr<ULODSyncComponent> LODSync;
	UPROPERTY(VisibleAnywhere, Category = "Chimera|Appearance")
	TObjectPtr<UMetaHumanComponentUE> MetaHuman;

	/* Input */
	UPROPERTY(EditDefaultsOnly, Category = "Chimera|Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Chimera|Input")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "Chimera|Input")
	TObjectPtr<UInputAction> ToggleWalkAction;
	UPROPERTY(EditDefaultsOnly, Category = "Chimera|Input")
	TObjectPtr<UInputAction> SpeedUpAction;


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
	float SprintMaxSpeed = 800.f;
	
	void OnMove(const FInputActionValue& Value);
	void OnMoveCompleted(const FInputActionValue& Value);
	void OnLook(const FInputActionValue& Value);
	void OnWalkToggle(const FInputActionValue& Value);
	void OnSpeedUp(const FInputActionValue& Value);
	void OnSlowDown(const FInputActionValue& Value);
	FVector2D CachedMoveInput = FVector2D::ZeroVector;

	// Seam between input layer and mover
	virtual void ProduceInput_Implementation(int32 SimTimeMs, FMoverInputCmdContext& InputCmdResult) override;

public:	

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	void ApplySpeedForCurrentGait();
};
