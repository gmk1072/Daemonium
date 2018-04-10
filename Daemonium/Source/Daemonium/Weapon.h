// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class DAEMONIUM_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float damage;
	float weight;
	float attackSpeed;
	FString name;
	FString description;
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent* mesh;
};
