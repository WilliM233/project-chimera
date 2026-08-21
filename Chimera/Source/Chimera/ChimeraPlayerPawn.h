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
class UInputAction;
class USkeletalMeshComponent;
class ULODSyncComponent;
class UMetaHumanComponentUE;

struct FInputActionValue;

UCLASS()
class CHIMERA_API AChimeraPlayerPawn : public APawn, public IMoverInputProducerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AChimeraPlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Chimera|Movement")
	TObjectPtr<UCapsuleComponent> Capsule;

	UPROPERTY(VisibleAnywhere, Category = "Chimera|Movement")
	TObjectPtr<UCharacterMoverComponent> Mover;
	UPROPERTY(VisibleAnywhere, Category = "Chimera|Movement")
	TObjectPtr<UMoverNetworkPredictionLiaisonComponent> MoverBackend;

	UPROPERTY(VisibleAnywhere, Category = "Chimera|Camera")
	TObjectPtr<USpringArmComponent> SpringArm;
	UPROPERTY(VisibleAnywhere, Category = "Chimera|Camera")
	TObjectPtr<UCameraComponent> Camera;
	UPROPERTY(VisibleAnywhere, Category = "Chimera|Camera")
	TObjectPtr<USceneComponent> VisualRoot;

	UPROPERTY(EditDefaultsOnly, Category = "Chimera|Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Chimera|Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(VisibleAnywhere, Category = "Chimera|Appearance")
	TObjectPtr<USkeletalMeshComponent> Body;
	UPROPERTY(VisibleAnywhere, Category = "Chimera|Appearance")
	TObjectPtr<USkeletalMeshComponent> Face;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Chimera|Appearance")
	TObjectPtr<ULODSyncComponent> LODSync;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Chimera|Appearance")
	TObjectPtr<UMetaHumanComponentUE> MetaHuman;
	
	void OnMove(const FInputActionValue& Value);
	void OnMoveCompleted(const FInputActionValue& Value);
	void OnLook(const FInputActionValue& Value);

	FVector2D CachedMoveInput = FVector2D::ZeroVector;

	virtual void ProduceInput_Implementation(int32 SimTimeMs, FMoverInputCmdContext& InputCmdResult) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
