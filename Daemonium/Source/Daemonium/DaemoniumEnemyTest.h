// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DaemoniumEnemyTest.generated.h"

UCLASS()
class DAEMONIUM_API ADaemoniumEnemyTest : public ACharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	ADaemoniumEnemyTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser);

	float health;
	float armor;
	bool takenDamage;
	float invulnTime;

	float damageRad;
	
};
