// Fill out your copyright notice in the Description page of Project Settings.

#include "Consumable.h"


// Sets default values
AConsumable::AConsumable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AConsumable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AConsumable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AConsumable::Use()
{
}

