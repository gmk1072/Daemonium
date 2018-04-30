// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
#include "Pickup.h"
#include "Consumable.generated.h"

UCLASS()
class DAEMONIUM_API AConsumable : public APickup
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AConsumable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void Use();

	
	
};


