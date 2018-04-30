// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "Components/BoxComponent.h"
//#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "Pickup.h"
#include "Weapon.generated.h"

UCLASS()
class DAEMONIUM_API AWeapon : public APickup
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeapon();

protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float damage;
	float attackSpeed;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent* health_bar_inner;
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent* health_bar_outer;
};
