// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("weapon"));
	mesh->SetRelativeLocation(FVector(0, 0, 0));


	health_bar_inner = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("health_bar_inner"));
	health_bar_inner->SetRelativeLocation(FVector(0, 0, 0));
	health_bar_inner->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	health_bar_outer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("health_bar_outer"));
	health_bar_outer->SetRelativeLocation(FVector(0, 0, 0));
	health_bar_outer->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

