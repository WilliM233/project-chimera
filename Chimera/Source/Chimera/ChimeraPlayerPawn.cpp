// Copyright 2026 Flux Meridian. All Rights Reserved.


#include "ChimeraPlayerPawn.h"
#include "Components/CapsuleComponent.h"
#include "DefaultMovementSet/CharacterMoverComponent.h"
#include "Backends/MoverNetworkPredictionLiaison.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

// Sets default values
AChimeraPlayerPawn::AChimeraPlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	SetReplicateMovement(false);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->InitCapsuleSize(34.f, 88.f);
	Capsule->SetCollisionProfileName(TEXT("Pawn"));
	RootComponent = Capsule;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	Mover = CreateDefaultSubobject<UCharacterMoverComponent>(TEXT("Mover"));
	MoverBackend = CreateDefaultSubobject<UMoverNetworkPredictionLiaisonComponent>(TEXT("MoverBackend"));
}

// Called when the game starts or when spawned
void AChimeraPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChimeraPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AChimeraPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AChimeraPlayerPawn::OnMove);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AChimeraPlayerPawn::OnLook);
	}
}

void AChimeraPlayerPawn::OnMove(const FInputActionValue& Value)
{
	CachedMoveInput = Value.Get<FVector2D>();
	//UE_LOG(LogTemp, Log, TEXT("Move: %s (size %.2f)"), *CachedMoveInput.ToString(), CachedMoveInput.Size());
}

void AChimeraPlayerPawn::OnLook(const FInputActionValue& Value)
{
	const FVector2D LookInput = Value.Get<FVector2D>();
	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(-LookInput.Y);
	//UE_LOG(LogTemp, Log, TEXT("Look: %s"), *CachedLookInput.ToString());
}

void AChimeraPlayerPawn::ProduceInput_Implementation(int32 SimTimeMs, FMoverInputCmdContext& InputCmdResult)
{
	FCharacterDefaultInputs& Inputs = InputCmdResult.InputCollection.FindOrAddMutableDataByType<FCharacterDefaultInputs>();

	// Camera yaw only
	const FRotator ControlYaw(0.f, GetControlRotation().Yaw, 0.f);
	const FVector Forward = FRotationMatrix(ControlYaw).GetUnitAxis(EAxis::X);
	const FVector Right	  = FRotationMatrix(ControlYaw).GetUnitAxis(EAxis::Y);

	// Magnitude preserved for analog sticks.
	const FVector WorldIntent = (Forward * CachedMoveInput.Y) + (Right * CachedMoveInput.X);

	Inputs.SetMoveInput(EMoveInputType::DirectionalIntent, WorldIntent);

	// Free-look: face where we're going.
	if (!WorldIntent.IsNearlyZero())
	{
		Inputs.OrientationIntent = WorldIntent.GetSafeNormal();
	}
	else
	{
		Inputs.OrientationIntent = FVector::ZeroVector;
	}

	CachedMoveInput = FVector2D::ZeroVector;
}



