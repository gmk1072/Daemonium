// Fill out your copyright notice in the Description page of Project Settings.

#include "DaemoniumEnemyTest.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

// Sets default values
ADaemoniumEnemyTest::ADaemoniumEnemyTest()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	health = 100.0f;
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

}


// Called to bind functionality to input
void ADaemoniumEnemyTest::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADaemoniumEnemyTest::TakeDamage(float damage)
{
	health -= armor * damage;
}

