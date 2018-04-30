// Fill out your copyright notice in the Description page of Project Settings.

#include "DaemoniumEnemyTest.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "DaemoniumCharacter.h"


// Sets default values
ADaemoniumEnemyTest::ADaemoniumEnemyTest()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	health = 100;
	armor = 1.0;

}

// Called when the game starts or when spawned
void ADaemoniumEnemyTest::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADaemoniumEnemyTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (takenDamage) {
		invulnTime += DeltaTime;
	}
	if (invulnTime >= .2) {
		takenDamage = false;
		invulnTime = 0;
	}
	/*DrawDebugSphere(
		GetWorld(),
		RootComponent->GetComponentToWorld().GetLocation(),
		300,
		32,
		FColor(255, 255, 255)
	);
	DrawDebugSphere(
		GetWorld(),
		RootComponent->GetComponentToWorld().GetLocation(),
		damageRad,
		32,
		FColor(255, 0, 0)
	);
	damageRad += (60 * DeltaTime);
	if (damageRad >= 300) {
		TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
		FDamageEvent DamageEvent(ValidDamageTypeClass);
		UGameplayStatics::ApplyRadialDamage(GetWorld(), 20, RootComponent->GetComponentToWorld().GetLocation(), damageRad, ValidDamageTypeClass, TArray<AActor*>(), this, GetController(), true, ECC_Destructible);
		damageRad = 0;
	}*/
}


// Called to bind functionality to input
void ADaemoniumEnemyTest::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ADaemoniumEnemyTest::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	if (DamageCauser != Cast<ADaemoniumEnemyTest>(DamageCauser))
	{
		if (takenDamage == false) {
			// Call the base class - this will tell us how much damage to apply  
			const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
			if (ActualDamage > 0.f)
			{
				health -= ActualDamage;
				takenDamage = true;
				// If the damage depletes our health set our lifespan to zero - which will destroy the actor  
				if (health <= 0.f)
				{
					SetLifeSpan(0.001f);
				}
			}

			return ActualDamage;
		}
	}
	return 0;
}

