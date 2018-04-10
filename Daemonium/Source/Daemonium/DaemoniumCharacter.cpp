// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "DaemoniumCharacter.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "DaemoniumProjectile.h"
#include "DaemoniumGameMode.h"
#include "DaemoniumHUD.h"
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
#include "DrawDebugHelpers.h"
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
	FP_Weapon->SetStaticMesh(nullptr);
	FP_Weapon->SetMobility(EComponentMobility::Movable);
	FP_Weapon->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Weapon->bCastDynamicShadow = false;
	FP_Weapon->CastShadow = false;
	FP_Weapon->AttachToComponent(FP_WeaponRoot, FAttachmentTransformRules::KeepRelativeTransform);//FP_WeaponRoot, FAttachmentTransformRules::KeepWorldTransform);
	FP_Weapon->RelativeRotation = FRotator(-15.0f, 8.0f, 169.0f);
	FP_Weapon->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	FP_Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Health_bar_outer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("health_bar_outer"));
	Health_bar_outer->SetStaticMesh(nullptr);
	Health_bar_outer->SetMobility(EComponentMobility::Movable);
	Health_bar_outer->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	Health_bar_outer->bCastDynamicShadow = false;
	Health_bar_outer->CastShadow = false;
	Health_bar_outer->AttachToComponent(FP_Weapon, FAttachmentTransformRules::KeepRelativeTransform);//FP_WeaponRoot, FAttachmentTransformRules::KeepWorldTransform);
																									 //Health_bar_outer->RelativeRotation = FP_Weapon->RelativeRotation;
	Health_bar_outer->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	Health_bar_inner = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("health_bar_inner"));
	Health_bar_inner->SetStaticMesh(nullptr);
	Health_bar_inner->SetMobility(EComponentMobility::Movable);
	Health_bar_inner->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	Health_bar_inner->bCastDynamicShadow = false;
	Health_bar_inner->CastShadow = false;
	Health_bar_inner->AttachToComponent(FP_Weapon, FAttachmentTransformRules::KeepRelativeTransform);//FP_WeaponRoot, FAttachmentTransformRules::KeepWorldTransform);
																									 //Health_bar_inner->RelativeRotation = FP_Weapon->RelativeRotation;
	Health_bar_inner->SetCollisionEnabled(ECollisionEnabled::NoCollision);



	//FP_Weapon->SetRelativeLocation(FVector(0.0f, 100.0f, 0.0f));
	//FP_Weapon->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	//FP_Weapon->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FirstPersonCameraComponent);
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


	chargeEndLocation[0] = FP_WeaponRoot->RelativeLocation + FVector(0, 20, 50); //End interpolation at target's location
	chargeEndRotation[0] = FRotator(60, 20, 40);
	chargeEndLocation[1] = FP_WeaponRoot->RelativeLocation + FVector(40, -20, -50);
	chargeEndRotation[1] = FRotator(-60, -40, 40);
	chargeEndLocation[2] = FP_WeaponRoot->RelativeLocation;
	chargeEndRotation[2] = FP_WeaponRoot->RelativeRotation;

	//stabby
	//attackEndLocation[1][0] = FP_WeaponRoot->RelativeLocation + FVector(0, 20, -50); //End interpolation at target's location
	//attackEndRotation[1][0] = FRotator(0, 20, 60);
	//attackEndLocation[1][1] = FP_WeaponRoot->RelativeLocation + FVector(40, -20, 50);
	//attackEndRotation[1][1] = FRotator(-60, -40, 40);
	//attackEndLocation[1][2] = FP_WeaponRoot->RelativeLocation;
	//attackEndRotation[1][2] = FP_WeaponRoot->RelativeRotation;


	attackEndLocation[1][0] = FP_WeaponRoot->RelativeLocation + FVector(0, -60, 50); //End interpolation at target's location
	attackEndRotation[1][0] = FRotator(0, 0, -30);
	attackEndLocation[1][1] = FP_WeaponRoot->RelativeLocation + FVector(40, 40, -50);
	attackEndRotation[1][1] = FRotator(-40, 60, -30);
	attackEndLocation[1][2] = FP_WeaponRoot->RelativeLocation;
	attackEndRotation[1][2] = FP_WeaponRoot->RelativeRotation;

	attackEndLocation[2][0] = FP_WeaponRoot->RelativeLocation + FVector(0, 30, -60); //End interpolation at target's location
	FQuat quat = FQuat(FRotator(-120, 0, 70));
	attackEndRotation[2][0] = FRotator(quat);
	attackEndLocation[2][1] = FP_WeaponRoot->RelativeLocation + FVector(20, -60, 60);
	quat = FQuat(FRotator(-160, -160, 150));
	attackEndRotation[2][1] = FRotator(quat);
	attackEndLocation[2][2] = FP_WeaponRoot->RelativeLocation;
	attackEndRotation[2][2] = FP_WeaponRoot->RelativeRotation;

	attackEndLocation[3][0] = FP_WeaponRoot->RelativeLocation + FVector(0, -60, -20); //End interpolation at target's location
	attackEndRotation[3][0] = FRotator(0, 0, -110);
	attackEndLocation[3][1] = FP_WeaponRoot->RelativeLocation + FVector(0, 20, 50);
	attackEndRotation[3][1] = FRotator(40, 60, -110);
	attackEndLocation[3][2] = FP_WeaponRoot->RelativeLocation;
	attackEndRotation[3][2] = FP_WeaponRoot->RelativeRotation;

	blockLocation = FP_WeaponRoot->RelativeLocation + FVector(0, 20, 35);
	blockRotation = FRotator(89, 89, 0);
	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;

	moveSpeedModifier = 1.0f;

	stamina = 100.0f;
	staminaRegen = stamina / 3.5f;

	health = 100.0f;

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
	FP_Weapon->OnComponentBeginOverlap.AddDynamic(this, &ADaemoniumCharacter::OnSwordOverlap);

	FirstPersonCameraComponent->PostProcessSettings.bOverride_VignetteIntensity = true;
	FirstPersonCameraComponent->PostProcessSettings.VignetteIntensity = 0.0f;
}

void ADaemoniumCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsAttacking)
	{
		if (currentWeapon != nullptr)
		{
			if (currentWeapon->IsA(AMeleeWeapon::StaticClass())) {

				float interpSpeed = 25.0f;
				//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "attacking");
				attackDestinationLocation = FMath::VInterpTo(FP_WeaponRoot->RelativeLocation, attackEndLocation[attackRandomizer][attackIndex], FApp::GetDeltaTime(), interpSpeed);
				attackDestinationRotation = FMath::RInterpTo(FP_WeaponRoot->RelativeRotation, attackEndRotation[attackRandomizer][attackIndex], FApp::GetDeltaTime(), interpSpeed);
				//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::SanitizeFloat(FP_WeaponRoot->RelativeRotation.Yaw) + "," + FString::SanitizeFloat(FP_WeaponRoot->RelativeRotation.Pitch) + ", " + FString::SanitizeFloat(FP_WeaponRoot->RelativeRotation.Roll));
				//attackDestinationRotation = FMath::VInterpTo(FVector(FP_WeaponRoot->RelativeRotation.Yaw, FP_WeaponRoot->RelativeRotation.Pitch, FP_WeaponRoot->RelativeRotation.Roll), attackEndRotation[attackIndex], FApp::GetDeltaTime(), interpSpeed);
				FP_WeaponRoot->SetRelativeLocation(attackDestinationLocation, true);
				FP_WeaponRoot->SetRelativeRotation(attackDestinationRotation, true);// FRotator(attackDestinationRotation.X, attackDestinationRotation.Y, attackDestinationRotation.Z));

				DrawDebugPoint(
					GetWorld(),
					FP_WeaponRoot->GetComponentToWorld().GetLocation() + (FP_Weapon->GetUpVector() * -80),
					20,  					//size
					debugAttackColor,
					//FColor(255, 0, 255),  //pink
					false,//persistent (never goes away)
					5.0 					//point leaves a trail on moving object
				);
				DrawDebugPoint(
					GetWorld(),
					FP_WeaponRoot->GetComponentToWorld().GetLocation() + (FP_Weapon->GetUpVector() * -60),
					20,  					//size
					debugAttackColor,
					//FColor(255, 0, 255),  //pink
					false,//persistent (never goes away)
					5.0 					//point leaves a trail on moving object
				);
				DrawDebugPoint(
					GetWorld(),
					FP_WeaponRoot->GetComponentToWorld().GetLocation() + (FP_Weapon->GetUpVector() * -40),
					20,  					//size
					debugAttackColor,
					//FColor(255, 0, 255),  //pink
					false,//persistent (never goes away)
					5.0 					//point leaves a trail on moving object
				);
				DrawDebugPoint(
					GetWorld(),
					FP_WeaponRoot->GetComponentToWorld().GetLocation() + (FP_Weapon->GetUpVector() * -20),
					20,  					//size
					debugAttackColor,
					//FColor(255, 0, 255),  //pink
					false,//persistent (never goes away)
					5.0 					//point leaves a trail on moving object
				);

				if (FP_WeaponRoot->RelativeLocation.Equals(attackEndLocation[attackRandomizer][attackIndex], 1))//&& FP_WeaponRoot->RelativeRotation.Equals(FRotator(attackEndRotation[attackIndex].X, attackEndRotation[attackIndex].Y, attackEndRotation[attackIndex].Z), 1)
				{
					attackIndex++;
					if (attackIndex == sizeof(attackEndLocation[0]) / sizeof(attackEndLocation[0][0]))
					{
						bIsAttacking = false;

						attackIndex = 0;
					}
					else if (attackIndex == (sizeof(attackEndLocation[0]) / sizeof(attackEndLocation[0][0])) - 1)
					{
						FP_Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
					}
				}
			}
			else if (currentWeapon->IsA(ARangedWeapon::StaticClass()))
			{
				FP_Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				if (bIsCharging)
				{

					float interpSpeed = 3.0f;
					//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "attacking");
					attackDestinationLocation = FMath::VInterpTo(FP_WeaponRoot->RelativeLocation, chargeEndLocation[attackIndex], FApp::GetDeltaTime(), interpSpeed);
					attackDestinationRotation = FMath::RInterpTo(FP_WeaponRoot->RelativeRotation, chargeEndRotation[attackIndex], FApp::GetDeltaTime(), interpSpeed);
					//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::SanitizeFloat(FP_WeaponRoot->RelativeRotation.Yaw) + "," + FString::SanitizeFloat(FP_WeaponRoot->RelativeRotation.Pitch) + ", " + FString::SanitizeFloat(FP_WeaponRoot->RelativeRotation.Roll));
					//attackDestinationRotation = FMath::VInterpTo(FVector(FP_WeaponRoot->RelativeRotation.Yaw, FP_WeaponRoot->RelativeRotation.Pitch, FP_WeaponRoot->RelativeRotation.Roll), attackEndRotation[attackIndex], FApp::GetDeltaTime(), interpSpeed);
					FP_WeaponRoot->SetRelativeLocation(attackDestinationLocation, true);
					FP_WeaponRoot->SetRelativeRotation(attackDestinationRotation, true);// FRotator(attackDestinationRotation.X, attackDestinationRotation.Y, attackDestinationRotation.Z));

					if (FP_WeaponRoot->RelativeLocation.Equals(chargeEndLocation[attackIndex], 3))//&& FP_WeaponRoot->RelativeRotation.Equals(FRotator(attackEndRotation[attackIndex].X, attackEndRotation[attackIndex].Y, attackEndRotation[attackIndex].Z), 1)
					{

					}
					else
					{
						timeCharged += DeltaTime;
					}

				}
				else
				{
					float interpSpeed = 25.0f;
					//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "attacking");
					attackDestinationLocation = FMath::VInterpTo(FP_WeaponRoot->RelativeLocation, chargeEndLocation[attackIndex], FApp::GetDeltaTime(), interpSpeed);
					attackDestinationRotation = FMath::RInterpTo(FP_WeaponRoot->RelativeRotation, chargeEndRotation[attackIndex], FApp::GetDeltaTime(), interpSpeed);
					//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::SanitizeFloat(FP_WeaponRoot->RelativeRotation.Yaw) + "," + FString::SanitizeFloat(FP_WeaponRoot->RelativeRotation.Pitch) + ", " + FString::SanitizeFloat(FP_WeaponRoot->RelativeRotation.Roll));
					//attackDestinationRotation = FMath::VInterpTo(FVector(FP_WeaponRoot->RelativeRotation.Yaw, FP_WeaponRoot->RelativeRotation.Pitch, FP_WeaponRoot->RelativeRotation.Roll), attackEndRotation[attackIndex], FApp::GetDeltaTime(), interpSpeed);
					FP_WeaponRoot->SetRelativeLocation(attackDestinationLocation, true);
					FP_WeaponRoot->SetRelativeRotation(attackDestinationRotation, true);// FRotator(attackDestinationRotation.X, attackDestinationRotation.Y, attackDestinationRotation.Z));

					if (FP_WeaponRoot->RelativeLocation.Equals(chargeEndLocation[attackIndex], 1))//&& FP_WeaponRoot->RelativeRotation.Equals(FRotator(attackEndRotation[attackIndex].X, attackEndRotation[attackIndex].Y, attackEndRotation[attackIndex].Z), 1)
					{
						attackIndex++;
						if (attackIndex == sizeof(chargeEndLocation) / sizeof(chargeEndLocation[0]))
						{
							bIsAttacking = false;

							attackIndex = 0;
						}
					}
				}
			}
		}
	}

	if (bIsBlocking && !((FP_WeaponRoot->RelativeLocation.Equals(blockLocation, 1)) && (FP_WeaponRoot->RelativeRotation.Equals(blockRotation, 1))))
	{
		float interpSpeed = 25.0f;
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "blocking");
		blockDestinationLocation = FMath::VInterpTo(FP_WeaponRoot->RelativeLocation, blockLocation, FApp::GetDeltaTime(), interpSpeed);
		blockDestinationRotation = FMath::RInterpTo(FP_WeaponRoot->RelativeRotation, blockRotation, FApp::GetDeltaTime(), interpSpeed);
		FP_WeaponRoot->SetRelativeLocation(blockDestinationLocation);
		FP_WeaponRoot->SetRelativeRotation(blockDestinationRotation);
	}
	if (bIsStoppingBlock)
	{
		float interpSpeed = 25.0f;
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, "stopping block");
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
		if (drainStamina(10 * FApp::GetDeltaTime(), false)) {
			float interpspeed = 3;
			//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, "sprinting ");
			sprintingFOVDestination = FMath::FInterpTo(FirstPersonCameraComponent->FieldOfView, 100, FApp::GetDeltaTime(), interpspeed);
			if (!FMath::IsNearlyEqual(FirstPersonCameraComponent->FieldOfView, 100, 0.1f))
			{
				FirstPersonCameraComponent->FieldOfView = sprintingFOVDestination;
			}
			if (GetCharacterMovement()->Velocity.Size() == 0) {
				OnStopSprint();
			}
		}
		else {
			OnStopSprint();
		}
	}

	if (bIsStoppingSprint && !bIsSprinting)
	{
		float interpspeed = 10;
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Emerald, FString::SanitizeFloat(FirstPersonCameraComponent->FieldOfView));
		sprintingFOVDestination = FMath::FInterpTo(FirstPersonCameraComponent->FieldOfView, 90, FApp::GetDeltaTime(), interpspeed);
		FirstPersonCameraComponent->FieldOfView = sprintingFOVDestination;
		if (FMath::IsNearlyEqual(FirstPersonCameraComponent->FieldOfView, 90, 0.1f))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "stopped sprint");
			bIsStoppingSprint = false;
		}
	}

	if (bDodged) {
		dodgeCooldown += FApp::GetDeltaTime();
		if (dodgeCooldown >= 1.6) {
			dodgeCooldown = 0;
			bDodged = false;
		}
	}

	if (binvincible) {
		invincibillityTimer -= FApp::GetDeltaTime();
		if (invincibillityTimer <= 0) {
			invincibillityTimer = 0;
			binvincible = false;
		}
		GEngine->AddOnScreenDebugMessage(-1, FApp::GetDeltaTime(), FColor::Red, "invincible");
	}

	if (bStaminaCooldown) {
		staminaRegenCooldown += FApp::GetDeltaTime();
		if (staminaRegenCooldown >= 1.0) {
			staminaRegenCooldown = 0;
			bStaminaCooldown = false;
		}
	}
	else {
		if (stamina < 100) {
			if (bIsBlocking)
				RegainStamina((staminaRegen / 2.0f) * FApp::GetDeltaTime());
			else
				RegainStamina(staminaRegen * FApp::GetDeltaTime());
			if (stamina > 100) {
				stamina = 100;
				if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->IsInputKeyDown(EKeys::LeftShift)) {
					OnStartSprint();
				}
			}
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, FApp::GetDeltaTime(), FColor::Emerald, FString::SanitizeFloat(stamina));
	if (bIsDead) {
		GEngine->AddOnScreenDebugMessage(-1, FApp::GetDeltaTime(), FColor::Red, "You're Dead");
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, FApp::GetDeltaTime(), FColor::Red, FString::SanitizeFloat(health));
	}

}

//////////////////////////////////////////////////////////////////////////
// Input

void ADaemoniumCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ADaemoniumCharacter::Dodge);// &ACharacter::Jump);
	/*
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);*/

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ADaemoniumCharacter::OnFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ADaemoniumCharacter::OnFireRelease);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ADaemoniumCharacter::Interact);

	PlayerInputComponent->BindAction("Holster", IE_Pressed, this, &ADaemoniumCharacter::Holster);

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
	if (!bIsAttacking && !bIsBlocking && !bIsSprinting && !bIsHolstered)
	{
		if (currentWeapon != nullptr)
		{
			if (currentWeapon->IsA(AMeleeWeapon::StaticClass()))
			{
				FP_Weapon->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				attackRandomizer = FMath::FRandRange(0, ((sizeof(attackEndLocation) / sizeof(attackEndLocation[0]))) / 2);
				if (attackEven) {
					attackRandomizer *= 2;
					GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan, FString::SanitizeFloat(attackRandomizer));
				}
				else {
					attackRandomizer *= 2;
					attackRandomizer += 1;
					GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan, FString::SanitizeFloat(attackRandomizer));
				}
				//attackRandomizer = 1;
				debugAttackColor = FColor(FMath::FRandRange(0, 255), FMath::FRandRange(0, 255), FMath::FRandRange(0, 255));
				//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan, FString::SanitizeFloat(attackRandomizer));
				attackEven = !attackEven;
				bIsStoppingBlock = false;
				bIsAttacking = true;
			}
			if (currentWeapon->IsA(ARangedWeapon::StaticClass()))
			{
				bIsCharging = true;
				bIsStoppingBlock = false;
				bIsAttacking = true;
			}
		}
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

void ADaemoniumCharacter::OnFireRelease()
{
	if (bIsAttacking && !bIsBlocking && !bIsSprinting && !bIsHolstered)
	{
		if (currentWeapon != nullptr)
		{
			if (currentWeapon->IsA(ARangedWeapon::StaticClass()))
			{
				bIsCharging = false;
				attackIndex = 1;


				const FRotator SpawnRotation = FirstPersonCameraComponent->GetComponentRotation();//GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = (FirstPersonCameraComponent->GetForwardVector() * 20 + FirstPersonCameraComponent->GetComponentLocation());// (FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				const FTransform spawnTransform = FTransform(SpawnRotation, SpawnLocation);

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.Owner = this;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// spawn the projectile at the muzzle
				//GetWorld()->SpawnActor<ADaemoniumProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

				float radius = timeCharged * 300;
				float damage = timeCharged * 27;
				timeCharged = 0;

				auto MyDeferredActor = Cast<ADaemoniumProjectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileClass, spawnTransform));
				if (MyDeferredActor != nullptr)
				{
					MyDeferredActor->Init(damage, radius, this);

					UGameplayStatics::FinishSpawningActor(MyDeferredActor, spawnTransform);
					GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, "shot");
				}

			}
		}
	}
}

void ADaemoniumCharacter::UpdateHealthScreenEffect()
{
	float val = FMath::Pow(100 - health, 2);
	FirstPersonCameraComponent->PostProcessSettings.VignetteIntensity = FMath::GetMappedRangeValueClamped(FVector2D(0, 10000), FVector2D(0, 3), val);
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::SanitizeFloat(val));
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::SanitizeFloat(FirstPersonCameraComponent->PostProcessSettings.VignetteIntensity));
}

void ADaemoniumCharacter::UpdateHealthBar()
{
	Health_bar_outer->SetRelativeScale3D(FVector(1, 1, health / 100));
}

void ADaemoniumCharacter::OnStartSprint()
{
	if (GetCharacterMovement()->Velocity.Size() != 0) {
		GetCharacterMovement()->MaxWalkSpeed *= 1.6;
		bIsSprinting = true;
	}
}

void ADaemoniumCharacter::OnStopSprint()
{
	if (bIsSprinting) {
		GetCharacterMovement()->MaxWalkSpeed /= 1.6;
		bIsSprinting = false;
		bIsStoppingSprint = true;
	}
}

void ADaemoniumCharacter::Holster()
{
	if (currentWeapon != nullptr)
	{

		if (bIsHolstered)
		{
			bIsHolstered = false;
			FP_Weapon->SetStaticMesh(currentWeapon->mesh->GetStaticMesh());
			Health_bar_inner->SetStaticMesh(currentWeapon->health_bar_inner->GetStaticMesh());
			Health_bar_outer->SetStaticMesh(currentWeapon->health_bar_outer->GetStaticMesh());
		}
		else
		{
			bIsHolstered = true;
			FP_Weapon->SetStaticMesh(nullptr);
			Health_bar_inner->SetStaticMesh(nullptr);
			Health_bar_outer->SetStaticMesh(nullptr);
		}
	}
}

void ADaemoniumCharacter::Interact()
{
	DrawDebugSphere(
		GetWorld(),
		FirstPersonCameraComponent->GetForwardVector() + FirstPersonCameraComponent->GetComponentLocation(),
		50,
		32,
		FColor(255, 255, 255),
		true,
		1
	);
	DrawDebugSphere(
		GetWorld(),
		FirstPersonCameraComponent->GetForwardVector() * 150.0f + FirstPersonCameraComponent->GetComponentLocation(),
		50,
		32,
		FColor(255, 0, 255),
		true,
		1
	);
	FCollisionShape MySphere = FCollisionShape::MakeSphere(50.0f);
	TArray<FHitResult> OutResults = TArray<FHitResult>();
	GetWorld()->SweepMultiByChannel(OutResults, FirstPersonCameraComponent->GetForwardVector() + FirstPersonCameraComponent->GetComponentLocation(), FirstPersonCameraComponent->GetForwardVector() * 150.0f + FirstPersonCameraComponent->GetComponentLocation(), FQuat::Identity, ECollisionChannel::ECC_WorldDynamic, MySphere);
	//for (int32 i = 0; i < OutResults.Num; i++) {
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::SanitizeFloat((float)(OutResults.Num())));
	for (FHitResult result : OutResults) {
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "hit");
		if (result.GetActor() != nullptr)
		{
			if (result.GetActor()->IsA(AWeapon::StaticClass())) {
				PickedUpWeapon(result.GetActor());
			}

			//if (result.GetActor()->IsA(AMeleeWeapon::StaticClass())) {
			//	currentWeapon = (AMeleeWeapon*)(result.GetActor());
			//	//PickedUpWeapon();
			//	//result.GetActor()->Destroy();
			//}
			//else if (result.GetActor()->IsA(ARangedWeapon::StaticClass())) {
			//	currentWeapon = (ARangedWeapon*)(result.GetActor());
			//	//PickedUpWeapon();
			//	//result.GetActor()->Destroy();
			//}
		}
	}
}

void ADaemoniumCharacter::PickedUpWeapon(AActor* actor)
{
	bIsHolstered = false;
	if (actor->IsA(AMeleeWeapon::StaticClass())) {
		if (currentWeapon != nullptr)
		{
			ReplaceWeapon(actor);
		}
		else
		{
			actor->SetActorHiddenInGame(true);
			actor->SetActorEnableCollision(false);
		}
		currentWeapon = (AMeleeWeapon*)(actor);// result.GetActor());
		FP_Weapon->SetStaticMesh(currentWeapon->mesh->GetStaticMesh());
		Health_bar_inner->SetStaticMesh(currentWeapon->health_bar_inner->GetStaticMesh());
		Health_bar_outer->SetStaticMesh(currentWeapon->health_bar_outer->GetStaticMesh());
	}
	else if (actor->IsA(ARangedWeapon::StaticClass()))
	{
		if (currentWeapon != nullptr)
		{
			ReplaceWeapon(actor);
		}
		else
		{
			actor->SetActorHiddenInGame(true);
			actor->SetActorEnableCollision(false);
		}
		currentWeapon = (ARangedWeapon*)(actor);
		FP_Weapon->SetStaticMesh(currentWeapon->mesh->GetStaticMesh());
		Health_bar_inner->SetStaticMesh(currentWeapon->health_bar_inner->GetStaticMesh());
		Health_bar_outer->SetStaticMesh(currentWeapon->health_bar_outer->GetStaticMesh());
	}
}

void ADaemoniumCharacter::ReplaceWeapon(AActor* actor)
{
	actor->SetActorHiddenInGame(true);
	actor->SetActorEnableCollision(false);

	currentWeapon->SetActorLocation(actor->GetActorLocation());
	currentWeapon->SetActorHiddenInGame(false);
	currentWeapon->SetActorEnableCollision(true);
	currentWeapon->health_bar_inner->SetRelativeLocation(FVector(0, 0, 0));
	currentWeapon->health_bar_outer->SetRelativeLocation(FVector(0, 0, 0));
	//AWeapon* temp = (AWeapon*)(GetWorld()->SpawnActor(AWeapon::StaticClass(),&(actor->GetTransform()), FActorSpawnParameters())); // , NAME_None, NULL, NULL, NULL, false, false, NULL, NULL));
	//temp = currentWeapon;
}

void ADaemoniumCharacter::OnStartBlock()
{
	if (!bIsHolstered) {
		FP_Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		bIsBlocking = true;
		bIsStoppingBlock = false;
		moveSpeedModifier = .6;
		if (bIsAttacking)
		{
			bIsAttacking = false;
			attackIndex = 0;
		}
		ShowStaminaBar(true);
	}
}

void ADaemoniumCharacter::OnStopBlock()
{
	bIsBlocking = false;
	bIsStoppingBlock = true;
	moveSpeedModifier = 1;
	ShowStaminaBar(false);
}

void ADaemoniumCharacter::RegainStamina(float staminaGained)
{
	stamina += staminaGained;
	updateStaminaBar();

}

void ADaemoniumCharacter::updateStaminaBar_Implementation()
{
}

void ADaemoniumCharacter::ShowStaminaBar_Implementation(bool show)
{

}



void ADaemoniumCharacter::Dodge()
{
	if (!bDodged && !GetCharacterMovement()->IsFalling() && !(GetCharacterMovement()->Velocity.Size() == 0) && drainStamina(20, false)) {

		becomeInvincible(.5);
		bDodged = true;
		//GetRootPrimitiveComponent()->AddForce(GetActorForwardVector() * 200.0f);
		FVector direction = FVector(GetCharacterMovement()->Velocity.X, GetCharacterMovement()->Velocity.Y, 0);// .Normalize());// .X, GetCharacterMovement()->Velocity.Y, 0);
		direction.Normalize();
		direction.Z = 0;
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, "dodge: " + direction.ToString());
		direction = direction * 6000;
		GetCharacterMovement()->AddImpulse(direction, true);
	}
}

void ADaemoniumCharacter::becomeInvincible(float time)
{
	binvincible = true;
	if (invincibillityTimer < time) {
		invincibillityTimer = time;
	}
}

bool ADaemoniumCharacter::drainStamina(float staminaCost, bool drainAnyway)
{
	if (stamina - staminaCost < 0) {
		if (drainAnyway) {
			staminaRegenCooldown = 0;
			bStaminaCooldown = true;
			stamina = 0;
			updateStaminaBar();
		}
		return false;
	}
	else {
		stamina -= staminaCost;
		staminaRegenCooldown = 0;
		bStaminaCooldown = true;
		updateStaminaBar();
		return true;
	}
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

void ADaemoniumCharacter::OnSwordOverlap(class UPrimitiveComponent* OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

	TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
	FDamageEvent DamageEvent(ValidDamageTypeClass);

	const float DamageAmount = 15;
	//OtherActor->TakeDamage(DamageAmount, DamageEvent, GetController(), this);



	UGameplayStatics::ApplyRadialDamage(GetWorld(), DamageAmount, FP_WeaponRoot->GetComponentToWorld().GetLocation() + (FP_Weapon->GetUpVector() * -120), 30, ValidDamageTypeClass, TArray<AActor*>(), this, GetController(), true, ECC_Destructible);
	UGameplayStatics::ApplyRadialDamage(GetWorld(), DamageAmount, FP_WeaponRoot->GetComponentToWorld().GetLocation() + (FP_Weapon->GetUpVector() * -90), 30, ValidDamageTypeClass, TArray<AActor*>(), this, GetController(), true, ECC_Destructible);
	UGameplayStatics::ApplyRadialDamage(GetWorld(), DamageAmount, FP_WeaponRoot->GetComponentToWorld().GetLocation() + (FP_Weapon->GetUpVector() * -60), 30, ValidDamageTypeClass, TArray<AActor*>(), this, GetController(), true, ECC_Destructible);
	UGameplayStatics::ApplyRadialDamage(GetWorld(), DamageAmount, FP_WeaponRoot->GetComponentToWorld().GetLocation() + (FP_Weapon->GetUpVector() * -30), 30, ValidDamageTypeClass, TArray<AActor*>(), this, GetController(), true, ECC_Destructible);

	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, "hit enemy ");

		ADaemoniumEnemyTest* DET = Cast<ADaemoniumEnemyTest>(OtherActor);
		if (DET) {
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, "hit enemy " + FString::SanitizeFloat(DET->health));
		}
	}
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

float ADaemoniumCharacter::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (!binvincible) {
		const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
		if (ActualDamage > 0.f)
		{
			if (bIsBlocking)
			{
				drainStamina(ActualDamage * .6, true);
				health -= (ActualDamage * .1);
			}
			else
			{
				health -= ActualDamage;
			}
			// If the damage depletes our health set our lifespan to zero - which will destroy the actor  
			if (health <= 0.f)
			{
				bIsDead = true;
				health = 0;
			}
		}
		UpdateHealthScreenEffect();
		UpdateHealthBar();
		return ActualDamage;
	}
	else {
		return 0;
	}
}
