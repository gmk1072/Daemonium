// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "DaemoniumCharacter.h"
#include "DaemoniumProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"


DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ADaemoniumCharacter

ADaemoniumCharacter::ADaemoniumCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetVisibility(false);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	FP_WeaponRoot = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponRoot"));
	FP_WeaponRoot->SetupAttachment(FirstPersonCameraComponent);
	FP_WeaponRoot->SetRelativeLocation(FVector(50.0f, 20.0f, -30.0f));
	FP_WeaponRoot->SetMobility(EComponentMobility::Movable);

	// Create a gun mesh component
	FP_Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("narsil"));
	FP_Weapon->SetMobility(EComponentMobility::Movable);
	FP_Weapon->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Weapon->bCastDynamicShadow = false;
	FP_Weapon->CastShadow = false;
	FP_Weapon->AttachToComponent(FP_WeaponRoot, FAttachmentTransformRules::KeepRelativeTransform);//FP_WeaponRoot, FAttachmentTransformRules::KeepWorldTransform);
	FP_Weapon->RelativeRotation = FRotator(-15.0f, 8.0f, 169.0f);
	FP_Weapon->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	FP_Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//FP_Weapon->SetRelativeLocation(FVector(0.0f, 100.0f, 0.0f));
	//FP_Weapon->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	//FP_Weapon->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	//FP_MuzzleLocation->SetupAttachment(FP_Weapon);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Weapon, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->Hand = EControllerHand::Right;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Create a gun and attach it to the right-hand VR controller.
	// Create a gun mesh component
	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	VR_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	VR_Gun->bCastDynamicShadow = false;
	VR_Gun->CastShadow = false;
	VR_Gun->SetupAttachment(R_MotionController);
	VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	VR_MuzzleLocation->SetupAttachment(VR_Gun);
	VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
	VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));		// Counteract the rotation of the VR gun model.

	attackIndex = 0;
	weaponDefaultLocation = FP_WeaponRoot->RelativeLocation; //Start interpolation at player's location
	attackEndLocation[0][0] = FP_WeaponRoot->RelativeLocation + FVector(0, 20, 50); //End interpolation at target's location
	attackEndRotation[0][0] = FRotator(60, 20, 40);
	attackEndLocation[0][1] = FP_WeaponRoot->RelativeLocation + FVector(40, -20, -50);
	attackEndRotation[0][1] = FRotator(-60, -40, 40);
	attackEndLocation[0][2] = FP_WeaponRoot->RelativeLocation;
	attackEndRotation[0][2] = FP_WeaponRoot->RelativeRotation;

	attackEndLocation[1][0] = FP_WeaponRoot->RelativeLocation + FVector(0, 20, -50); //End interpolation at target's location
	//attackEndRotation[1][0] = FRotator(142, -33, 22);
	FQuat quat = FQuat(FRotator(-140, 0, 89));
	attackEndRotation[1][0] = FRotator(quat);
	attackEndLocation[1][1] = FP_WeaponRoot->RelativeLocation + FVector(20, -50, 50);
	//attackEndRotation[1][1] = FRotator(37, 33, -157);
	quat = FQuat(FRotator(-160, -160, 180));
	attackEndRotation[1][1] = FRotator(quat);
	attackEndLocation[1][2] = FP_WeaponRoot->RelativeLocation;
	attackEndRotation[1][2] = FP_WeaponRoot->RelativeRotation;

	//stabby
	//attackEndLocation[1][0] = FP_WeaponRoot->RelativeLocation + FVector(0, 20, -50); //End interpolation at target's location
	//attackEndRotation[1][0] = FRotator(0, 20, 60);
	//attackEndLocation[1][1] = FP_WeaponRoot->RelativeLocation + FVector(40, -20, 50);
	//attackEndRotation[1][1] = FRotator(-60, -40, 40);
	//attackEndLocation[1][2] = FP_WeaponRoot->RelativeLocation;
	//attackEndRotation[1][2] = FP_WeaponRoot->RelativeRotation;


	attackEndLocation[2][0] = FP_WeaponRoot->RelativeLocation + FVector(0, 20, 50); //End interpolation at target's location
	attackEndRotation[2][0] = FRotator(60, 20, 40);
	attackEndLocation[2][1] = FP_WeaponRoot->RelativeLocation + FVector(40, -20, -50);
	attackEndRotation[2][1] = FRotator(-60, -40, 40);
	attackEndLocation[2][2] = FP_WeaponRoot->RelativeLocation;
	attackEndRotation[2][2] = FP_WeaponRoot->RelativeRotation;

	attackEndLocation[3][0] = FP_WeaponRoot->RelativeLocation + FVector(0, 20, 50); //End interpolation at target's location
	attackEndRotation[3][0] = FRotator(60, 20, 40);
	attackEndLocation[3][1] = FP_WeaponRoot->RelativeLocation + FVector(40, -20, -50);
	attackEndRotation[3][1] = FRotator(-60, -40, 40);
	attackEndLocation[3][2] = FP_WeaponRoot->RelativeLocation;
	attackEndRotation[3][2] = FP_WeaponRoot->RelativeRotation;

	blockLocation = FP_WeaponRoot->RelativeLocation + FVector(0, 20, 35);
	blockRotation = FRotator(89, 89, 0);
	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;

	moveSpeedModifier = 1.0f;
}

void ADaemoniumCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = true;

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	//FP_Weapon->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		VR_Gun->SetHiddenInGame(false, true);
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	}
}

void ADaemoniumCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsAttacking)
	{
		float interpSpeed = 25.0f;
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "attacking");
		attackDestinationLocation = FMath::VInterpTo(FP_WeaponRoot->RelativeLocation, attackEndLocation[attackRandomizer][attackIndex], FApp::GetDeltaTime(), interpSpeed);
		attackDestinationRotation = FMath::RInterpTo(FP_WeaponRoot->RelativeRotation, attackEndRotation[attackRandomizer][attackIndex], FApp::GetDeltaTime(), interpSpeed);
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::SanitizeFloat(FP_WeaponRoot->RelativeRotation.Yaw) + "," + FString::SanitizeFloat(FP_WeaponRoot->RelativeRotation.Pitch) + ", " + FString::SanitizeFloat(FP_WeaponRoot->RelativeRotation.Roll));
		//attackDestinationRotation = FMath::VInterpTo(FVector(FP_WeaponRoot->RelativeRotation.Yaw, FP_WeaponRoot->RelativeRotation.Pitch, FP_WeaponRoot->RelativeRotation.Roll), attackEndRotation[attackIndex], FApp::GetDeltaTime(), interpSpeed);
		FP_WeaponRoot->SetRelativeLocation(attackDestinationLocation, true);
		FP_WeaponRoot->SetRelativeRotation(attackDestinationRotation, true);// FRotator(attackDestinationRotation.X, attackDestinationRotation.Y, attackDestinationRotation.Z));
		if (FP_WeaponRoot->RelativeLocation.Equals(attackEndLocation[attackRandomizer][attackIndex], 1))//&& FP_WeaponRoot->RelativeRotation.Equals(FRotator(attackEndRotation[attackIndex].X, attackEndRotation[attackIndex].Y, attackEndRotation[attackIndex].Z), 1)
		{
			attackIndex++;
			if (attackIndex == sizeof(attackEndLocation[0]) / sizeof(attackEndLocation[0][0]))
			{
				bIsAttacking = false;
				FP_Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				attackIndex = 0;
			}
		}
	}

	if (bIsBlocking && !((FP_WeaponRoot->RelativeLocation.Equals(blockLocation, 1)) && (FP_WeaponRoot->RelativeRotation.Equals(blockRotation, 1))))
	{
		float interpSpeed = 25.0f;
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "blocking");
		blockDestinationLocation = FMath::VInterpTo(FP_WeaponRoot->RelativeLocation, blockLocation, FApp::GetDeltaTime(), interpSpeed);
		blockDestinationRotation = FMath::RInterpTo(FP_WeaponRoot->RelativeRotation, blockRotation, FApp::GetDeltaTime(), interpSpeed);
		FP_WeaponRoot->SetRelativeLocation(blockDestinationLocation);
		FP_WeaponRoot->SetRelativeRotation(blockDestinationRotation);
	}
	if (bIsStoppingBlock)
	{
		float interpSpeed = 25.0f;
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, "stopping block");
		blockDestinationLocation = FMath::VInterpTo(FP_WeaponRoot->RelativeLocation, weaponDefaultLocation, FApp::GetDeltaTime(), interpSpeed);
		blockDestinationRotation = FMath::RInterpTo(FP_WeaponRoot->RelativeRotation, FRotator(0, 0, 0), FApp::GetDeltaTime(), interpSpeed);
		FP_WeaponRoot->SetRelativeLocation(blockDestinationLocation);
		FP_WeaponRoot->SetRelativeRotation(blockDestinationRotation);
		if (FP_WeaponRoot->RelativeLocation.Equals(weaponDefaultLocation, 1))
		{
			bIsStoppingBlock = false;
		}
	}
	if (bIsSprinting)
	{
		float interpspeed = 3;
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, "sprinting ");
		sprintingFOVDestination = FMath::FInterpTo(FirstPersonCameraComponent->FieldOfView, 110, FApp::GetDeltaTime(), interpspeed);
		if (!FMath::IsNearlyEqual(FirstPersonCameraComponent->FieldOfView, sprintingFOVDestination, 0.1f))
		{
			FirstPersonCameraComponent->FieldOfView = sprintingFOVDestination;
		}
		if (GetCharacterMovement()->Velocity.Size() == 0) {
			bIsSprinting = false;
			bIsStoppingSprint = true;
		}
	}

	if (bIsStoppingSprint && !bIsSprinting)
	{
		float interpspeed = 3;
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Emerald, FString::SanitizeFloat(FirstPersonCameraComponent->FieldOfView));
		sprintingFOVDestination = FMath::FInterpTo(FirstPersonCameraComponent->FieldOfView, 90, FApp::GetDeltaTime(), interpspeed);
		FirstPersonCameraComponent->FieldOfView = sprintingFOVDestination;
		if (FMath::IsNearlyEqual(FirstPersonCameraComponent->FieldOfView, 90, 0.1f))
		{
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "stopped sprint");
			bIsStoppingSprint = false;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ADaemoniumCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ADaemoniumCharacter::OnFire);

	PlayerInputComponent->BindAction("Block", IE_Pressed, this, &ADaemoniumCharacter::OnStartBlock);
	PlayerInputComponent->BindAction("Block", IE_Released, this, &ADaemoniumCharacter::OnStopBlock);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ADaemoniumCharacter::OnStartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ADaemoniumCharacter::OnStopSprint);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ADaemoniumCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ADaemoniumCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADaemoniumCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ADaemoniumCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ADaemoniumCharacter::LookUpAtRate);
}

void ADaemoniumCharacter::OnFire()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "attack");
	/*float dt = 0.0f;
	int frames = 30;
	for (int i = 0; i < frames; i++) {
		dt += float(1 / frames);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, FString::SanitizeFloat(dt));
		FMath::VInterpTo(FP_WeaponRoot->RelativeLocation, FVector(FP_WeaponRoot->RelativeLocation + FVector(10, 10, 10)), 1, 1);
	}*/
	if (!bIsAttacking && !bIsBlocking && !bIsSprinting)
	{
		FP_Weapon->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		attackRandomizer = FMath::FRandRange(0, ((sizeof(attackEndLocation) / sizeof(attackEndLocation[0]))) / 2);
		if (attackEven) {
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan, "even");
			attackRandomizer *= 2;
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan, "odd");
			attackRandomizer *= 2;
			attackRandomizer += 1;
		}
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan, FString::SanitizeFloat(attackRandomizer));
		//attackRandomizer += ((sizeof(attackEndLocation) / sizeof(attackEndLocation[0]) - 1) / 2 * attackEven);
		attackEven = !attackEven;
		bIsStoppingBlock = false;
		bIsAttacking = true;
	}


	//FVector destinationLocation;
	//FVector FlowEndLocation;
	//FVector FlowStartLocation;

	//FlowStartLocation = FP_WeaponRoot->RelativeLocation; //Start interpolation at player's location
	//FlowEndLocation = FP_WeaponRoot->RelativeLocation + FVector(10, 200, 500); //End interpolation at target's location
	//float InterpSpeed = 0.1f;

	//destinationLocation = FMath::VInterpTo(FlowStartLocation, FlowEndLocation, FApp::GetDeltaTime(), InterpSpeed); //Move from start-finish in 0.5 seconds
	//FP_WeaponRoot->SetRelativeLocation(destinationLocation);    //Set the player's position to the calculated value




/*
GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, orig.ToString());
FVector rot = orig.RotateVector(FVector(45.0f, 0.0f, 0.0f));
GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, rot.ToString());
FP_WeaponRoot->SetWorldRotation(FRotator(rot.Y, rot.Z, rot.X));
GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FP_WeaponRoot->RelativeRotation.ToString());
/**/

/*
// try and fire a projectile
if (ProjectileClass != NULL)
{
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		if (bUsingMotionControllers)
		{
			const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
			const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
			World->SpawnActor<ADaemoniumProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
		}
		else
		{
			const FRotator SpawnRotation = GetControlRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			World->SpawnActor<ADaemoniumProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}
}

// try and play the sound if specified
if (FireSound != NULL)
{
	UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
}

// try and play a firing animation if specified
if (FireAnimation != NULL)
{
	// Get the animation object for the arms mesh
	UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
	if (AnimInstance != NULL)
	{
		AnimInstance->Montage_Play(FireAnimation, 1.f);
	}
}
/**/
}

void ADaemoniumCharacter::OnStartSprint()
{
	if (GetCharacterMovement()->Velocity.Size() != 0) {
		GetCharacterMovement()->MaxWalkSpeed *= 2;
		bIsSprinting = true;
	}
}

void ADaemoniumCharacter::OnStopSprint()
{
	if (bIsSprinting) {
		GetCharacterMovement()->MaxWalkSpeed *= .5;
		bIsSprinting = false;
		bIsStoppingSprint = true;
	}
}

void ADaemoniumCharacter::OnStartBlock()
{
	FP_Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bIsBlocking = true;
	bIsStoppingBlock = false;
	moveSpeedModifier = .6;
	if (bIsAttacking)
	{
		bIsAttacking = false;
		attackIndex = 0;
	}
}

void ADaemoniumCharacter::OnStopBlock()
{
	bIsBlocking = false;
	bIsStoppingBlock = true;
	moveSpeedModifier = 1;
}

void ADaemoniumCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ADaemoniumCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void ADaemoniumCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = false;
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void ADaemoniumCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void ADaemoniumCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value * moveSpeedModifier);
	}
}

void ADaemoniumCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value * moveSpeedModifier);
	}
}

void ADaemoniumCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ADaemoniumCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool ADaemoniumCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ADaemoniumCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &ADaemoniumCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ADaemoniumCharacter::TouchUpdate);
		return true;
	}

	return false;
}
