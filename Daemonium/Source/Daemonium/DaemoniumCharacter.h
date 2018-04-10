// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MeleeWeapon.h"
#include "RangedWeapon.h"
#include "Weapon.h"
#include "CoreMinimal.h"
#include "DaemoniumEnemyTest.h"
#include "GameFramework/Character.h"
#include "DaemoniumCharacter.generated.h"

class UInputComponent;

UCLASS(config = Game)
class ADaemoniumCharacter : public ACharacter
{
	GENERATED_BODY()

		/** Pawn mesh: 1st person view (arms; seen only by self) */
		UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* FP_WeaponRoot;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent* FP_Weapon;
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent* Health_bar_outer;
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent* Health_bar_inner;
	//class USkeletalMeshComponent* FP_Weapon;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* FP_MuzzleLocation;

	/** Gun mesh: VR view (attached to the VR controller directly, no arm, just the actual gun) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* VR_Gun;

	/** Location on VR gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* VR_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;

	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UMotionControllerComponent* R_MotionController;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UMotionControllerComponent* L_MotionController;

public:
	ADaemoniumCharacter();

protected:
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ADaemoniumProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* FireAnimation;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		uint32 bUsingMotionControllers : 1;


	UFUNCTION(BlueprintNativeEvent, Category = "HUD functions")
	void updateStaminaBar();
	void updateStaminaBar_Implementation();
	UFUNCTION(BlueprintNativeEvent, Category = "HUD functions")
	void ShowStaminaBar(bool show);
	void ShowStaminaBar_Implementation(bool show);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats)
	float stamina;
protected:

	/** Fires a projectile. */
	void OnFire();
	void OnFireRelease();
	void UpdateHealthScreenEffect();
	void UpdateHealthBar();
	void OnStartSprint();
	void OnStopSprint();
	void RegainStamina(float staminaGained);

	void Holster();
	void Interact();
	void PickedUpWeapon(AActor* actor);
	void ReplaceWeapon(AActor* actor);
	void OnStartBlock();
	void OnStopBlock();


	void Dodge();

	void becomeInvincible(float time);

	bool drainStamina(float staminaCost, bool drainAnyway);
	
	/** Resets HMD orientation and position in VR. */
	void OnResetVR();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	* Called via input to turn at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to turn look up/down at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);

	struct TouchData
	{
		TouchData() { bIsPressed = false; Location = FVector::ZeroVector; }
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);

	UFUNCTION()
		void OnSwordOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	TouchData	TouchItem;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/*
	* Configures input for touchscreen devices if there is a valid touch interface for doing so
	*
	* @param	InputComponent	The input component pointer to bind controls to
	* @returns true if touch controls were enabled.
	*/
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

	bool bIsAttacking;
	bool bIsCharging;
	bool bIsBlocking;
	bool bIsStoppingBlock;
	bool bIsSprinting;
	bool bIsStoppingSprint;
	bool bDodged;
	float dodgeCooldown;
	float staminaRegen;
	float staminaRegenCooldown;
	bool bStaminaCooldown;
	bool binvincible;
	float invincibillityTimer;
	float health;
	bool bIsDead;
	bool bIsHolstered;

	AGameModeBase* gameMode;

	AWeapon* currentWeapon;


	FColor debugAttackColor;

	FVector attackDestinationLocation;
	FRotator attackDestinationRotation;
	FVector attackEndLocation[4][3];
	FRotator attackEndRotation[4][3];
	FVector chargeEndLocation[3];
	FRotator chargeEndRotation[3];
	float timeCharged;
	int attackRandomizer;
	bool attackEven;

	FVector blockLocation;
	FRotator blockRotation;
	FVector blockDestinationLocation;
	FRotator blockDestinationRotation;

	int attackIndex;
	FVector weaponDefaultLocation;

	float moveSpeedModifier;
	float sprintingFOVDestination;

	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser);

public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

