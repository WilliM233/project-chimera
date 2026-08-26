// Copyright 2026 Flux Meridian. All Rights Reserved.


#include "ChimeraPlayerPawn.h"
#include "Components/CapsuleComponent.h"
#include "DefaultMovementSet/CharacterMoverComponent.h"
#include "Backends/MoverNetworkPredictionLiaison.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Components/LODSyncComponent.h"
#include "MetaHumanComponentUE.h"
#include "DefaultMovementSet/Settings/CommonLegacyMovementSettings.h"
#include "GroomComponent.h"

// Capsule sized to the MetaHuman body mesh (~177.4cm tall). Half-height drives the body mesh offset below - change one and the other follows.
static constexpr float CapsuleHalfHeight = 88.7f;

// Widen if shoulders clip through walls; the mannequin's 34 is only a starting point.
static constexpr float CapsuleRadius = 34.f; 

// Third-person follow distance.
static constexpr float SpringArmLength = 300.f;

// MetaHuman skeletons face -Y; Unreal's forward is +X. This rotates the mesh to match.
static constexpr float MeshForwardYaw = -90.f;

// The MetaHuman body has 4 LODs; the face and grooms have 8. LODSync drives everything from the body's count so they never fall out of step.
static constexpr int32 BodyLODCount = 4;

AChimeraPlayerPawn::AChimeraPlayerPawn()
{
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;


	// Mover replicates movement through its own network model, so actor-level movement replication would fight it.
	SetReplicateMovement(false);

	// --- Hierarchy: order matters. The root must exist before anything attaches to it, and VisualRoot before anything attaches beneath it.
	// Reordering these compiles fine and silently produces a broken hierarchy.

	// The capsule is Mover's updated component: what actually moves and collides.
	// It advances in discrete simulation steps, so it is deliberately NOT what the camera or mesh hang from.
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->InitCapsuleSize(CapsuleRadius, CapsuleHalfHeight);
	Capsule->SetCollisionProfileName(TEXT("Pawn"));
	RootComponent = Capsule;

	// Mover writes its smoothing offset here (see SetPrimaryVisualComponent in BeginPlay).
	// Everything visual sits underneath so mesh and camera share one interpolated transform.
	// Empty and identity by design - do not delete: without it the camera rides the stepped capsule and the whole view judders.
	VisualRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VisualRoot"));
	VisualRoot->SetupAttachment(RootComponent);

	// On VisualRoot rather than Body: needs the smoothed transform,
	// but hanging it off the mesh would make the camera inherit animation bob.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(VisualRoot);
	SpringArm->TargetArmLength = SpringArmLength;

	// Mouse look drives the arm; the camera just rides the socket.
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;


	Body = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(VisualRoot);

	// Expressed as a relationship, not a number: feet sit at the capsule's bottom,
	// so resizing the capsule keeps the mesh grounded. Don't flatten to a literal.
	Body->SetRelativeLocation(FVector(0.f, 0.f, -CapsuleHalfHeight));
	Body->SetRelativeRotation(FRotator(0.f, MeshForwardYaw, 0.f));

	// Deliberately bare. The MetaHuman component finds Body and Face by component
	// name at registration and wires the face rig itself - no leader pose, 
	// the facial skeleton isn't a subset of the body's.
	Face = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Face"));
	Face->SetupAttachment(Body);

	Eyelashes = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyelashes"));
	Eyelashes->SetupAttachment(Face);

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(Face);

	Beard = CreateDefaultSubobject<UGroomComponent>(TEXT("Beard"));
	Beard->SetupAttachment(Face);

	Fuzz = CreateDefaultSubobject<UGroomComponent>(TEXT("Fuzz"));
	Fuzz->SetupAttachment(Face);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(Face);

	Mustache = CreateDefaultSubobject<UGroomComponent>(TEXT("Mustache"));
	Mustache->SetupAttachment(Face);

	// Both components resolve their targets by component name, so renaming the
	// Body or Face subobjects silently breaks them.
	LODSync = CreateDefaultSubobject<ULODSyncComponent>(TEXT("LODSync"));
	LODSync->NumLODs = BodyLODCount;

	FComponentSync BodySync;
	BodySync.Name = TEXT("Body");
	BodySync.SyncOption = ESyncOption::Drive;
	LODSync->ComponentsToSync.Add(BodySync);

	FComponentSync FaceSync;
	FaceSync.Name = TEXT("Face");
	FaceSync.SyncOption = ESyncOption::Drive;
	LODSync->ComponentsToSync.Add(FaceSync);

	FComponentSync HairSync;
	HairSync.Name = TEXT("Hair");
	HairSync.SyncOption = ESyncOption::Passive;
	LODSync->ComponentsToSync.Add(HairSync);

	FComponentSync BeardSync;
	BeardSync.Name = TEXT("Beard");
	BeardSync.SyncOption = ESyncOption::Passive;
	LODSync->ComponentsToSync.Add(BeardSync);

	FComponentSync MustacheSync;
	MustacheSync.Name = TEXT("Mustache");
	MustacheSync.SyncOption = ESyncOption::Passive;
	LODSync->ComponentsToSync.Add(MustacheSync);

	FComponentSync EyebrowsSync;
	EyebrowsSync.Name = TEXT("Eyebrows");
	EyebrowsSync.SyncOption = ESyncOption::Passive;
	LODSync->ComponentsToSync.Add(EyebrowsSync);

	FComponentSync EyelashesSync;
	EyelashesSync.Name = TEXT("Eyelashes");
	EyelashesSync.SyncOption = ESyncOption::Passive;
	LODSync->ComponentsToSync.Add(EyelashesSync);

	// Groom LODs run 0-7 against the body's 0-3; these mappings pick which
	// groom LOD each sync level selects, mirroring Epic's own MetaHuman BPs.
	FLODMappingData GroomMapping;
	GroomMapping.Mapping = { 1, 3, 5, 7 };
	LODSync->CustomLODMapping.Add(TEXT("Hair"), GroomMapping);
	LODSync->CustomLODMapping.Add(TEXT("Beard"), GroomMapping);
	LODSync->CustomLODMapping.Add(TEXT("Mustache"), GroomMapping);
	LODSync->CustomLODMapping.Add(TEXT("Eyebrows"), GroomMapping);
	LODSync->CustomLODMapping.Add(TEXT("Eyelashes"), GroomMapping);

	MetaHuman = CreateDefaultSubobject<UMetaHumanComponentUE>(TEXT("MetaHuman"));

	Mover = CreateDefaultSubobject<UCharacterMoverComponent>(TEXT("Mover"));
	MoverBackend = CreateDefaultSubobject<UMoverNetworkPredictionLiaisonComponent>(TEXT("MoverBackend"));
}

void AChimeraPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	// Must happen after Mover registers; setting it in the constructor doesn't take.
	Mover->SetPrimaryVisualComponent(VisualRoot);
	ApplySpeedForCurrentGait();
}

void AChimeraPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(MoveAction,		ETriggerEvent::Triggered, this, &AChimeraPlayerPawn::OnMove);
		EnhancedInput->BindAction(MoveAction,		ETriggerEvent::Completed, this, &AChimeraPlayerPawn::OnMoveCompleted);
		EnhancedInput->BindAction(LookAction,		ETriggerEvent::Triggered, this, &AChimeraPlayerPawn::OnLook);
		EnhancedInput->BindAction(ToggleWalkAction, ETriggerEvent::Started,	  this, &AChimeraPlayerPawn::OnWalkToggle);
		EnhancedInput->BindAction(SpeedUpAction,	ETriggerEvent::Started,	  this, &AChimeraPlayerPawn::OnSpeedUp);
		EnhancedInput->BindAction(SpeedUpAction,	ETriggerEvent::Completed, this, &AChimeraPlayerPawn::OnSlowDown);
	}
}

void AChimeraPlayerPawn::ApplySpeedForCurrentGait()
{
	// The 2x2 gait grid resolves here and only here:
	// gait family from bWalkMode, step-up from bSpeedUp.
	const float NewMaxSpeed = bWalkMode
		? (bSpeedUp ? JogMaxSpeed : WalkModeMaxSpeed)
		: (bSpeedUp ? SprintMaxSpeed : RunModeMaxSpeed);

	if (UCommonLegacyMovementSettings* Settings = Mover->FindSharedSettings_Mutable<UCommonLegacyMovementSettings>())
	{
		Settings->MaxSpeed = NewMaxSpeed;
	}
}

void AChimeraPlayerPawn::OnMove(const FInputActionValue& Value)
{
	CachedMoveInput = Value.Get<FVector2D>();
}

void AChimeraPlayerPawn::OnMoveCompleted(const FInputActionValue& Value)
{
	CachedMoveInput = FVector2D::ZeroVector;
}

void AChimeraPlayerPawn::OnLook(const FInputActionValue& Value)
{
	const FVector2D LookInput = Value.Get<FVector2D>();
	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(-LookInput.Y);
}

void AChimeraPlayerPawn::OnWalkToggle(const FInputActionValue& Value)
{
	bWalkMode = !bWalkMode;
	ApplySpeedForCurrentGait();
}

void AChimeraPlayerPawn::OnSpeedUp(const FInputActionValue& Value)
{
	bSpeedUp = true;
	ApplySpeedForCurrentGait();
}

void AChimeraPlayerPawn::OnSlowDown(const FInputActionValue& Value)
{
	bSpeedUp = false;
	ApplySpeedForCurrentGait();
}

void AChimeraPlayerPawn::ProduceInput_Implementation(int32 SimTimeMs, FMoverInputCmdContext& InputCmdResult)
{
	FCharacterDefaultInputs& Inputs = InputCmdResult.InputCollection.FindOrAddMutableDataByType<FCharacterDefaultInputs>();

	// Camera Yaw only - pitch in the basis would tilt movement into the ground when looking down.
	const FRotator ControlYaw(0.f, GetControlRotation().Yaw, 0.f);
	const FRotationMatrix YawMatrix(ControlYaw);

	const FVector Forward = YawMatrix.GetUnitAxis(EAxis::X);
	const FVector Right	  = YawMatrix.GetUnitAxis(EAxis::Y);

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
}



