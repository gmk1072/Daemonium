// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "DaemoniumProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine//Classes/Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

ADaemoniumProjectile::ADaemoniumProjectile()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ADaemoniumProjectile::OnOverlap);		  // set up a notification for when this component hits something blocking
	//CollisionComp->OnComponentHit.AddDynamic(this, &ADaemoniumProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = mesh;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = mesh;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	/*static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/PlaceholderAssets/sphere/sphereMesh.sphereMesh'"));
	UStaticMesh* Asset = MeshAsset.Object;
	mesh->SetStaticMesh(Asset);*/

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sphereMesh"));
	mesh->SetStaticMesh(nullptr);
	mesh->SetMobility(EComponentMobility::Movable);
	mesh->bCastDynamicShadow = false;
	mesh->CastShadow = false;
	mesh->BodyInstance.SetCollisionProfileName("Projectile");

	mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	/*
	mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	*/
	//mesh->OnComponentHit.AddDynamic(this, &ADaemoniumProjectile::OnHit);		// set up a notification for when this component hits something blocking
	//FP_Weapon->OnComponentBeginOverlap.AddDynamic(this, &ADaemoniumCharacter::OnSwordOverlap);
	//mesh->OnComponentBeginOverlap.AddDynamic(this, &ADaemoniumProjectile::OnOverlap);

	CollisionComp->SetupAttachment(mesh);
	//CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ADaemoniumProjectile::OnOverlap);


	partclSystem = CreateDefaultSubobject<UParticleSystem>(TEXT("FireBallExplosion"));
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "destroyed");

}

void ADaemoniumProjectile::Init(float dmg, float rad, AActor* owner)
{
	damage = dmg;
	radius = rad < 50 ? 50 : rad;
	this->SetOwner(owner);
}

//void ADaemoniumCharacter::OnSwordOverlap(class UPrimitiveComponent* OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)

void ADaemoniumProjectile::OnOverlap(class UPrimitiveComponent* OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Only add impulse and destroy projectile if we hit a physics
	TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>();
	FDamageEvent DamageEvent(ValidDamageTypeClass);
	TArray<AActor*> arr = TArray<AActor*>();
	//arr.Add(GetOwner());
	//UGameplayStatics::ApplyRadialDamageWithFalloff((UObject*)GetWorld(), damage, 1, mesh->GetComponentLocation(), 50, radius, .6, ValidDamageTypeClass, arr, this, (AController*)0, ECC_WorldDynamic);

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::SanitizeFloat(radius));
	arr.Add(GetOwner());
	if (OtherActor != GetOwner())
	{
		UGameplayStatics::ApplyRadialDamageWithFalloff((UObject*)GetWorld(), damage, 1, mesh->GetComponentLocation(), 50, radius, 1, ValidDamageTypeClass, arr, this, (AController*)0, ECC_WorldDynamic);
		//OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), partclSystem, GetActorLocation(), FRotator::ZeroRotator, true);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "destroyed");

		Destroy();

	}
}

void ADaemoniumProjectile::OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>();
	FDamageEvent DamageEvent(ValidDamageTypeClass);
	TArray<AActor*> arr = TArray<AActor*>();
	//arr.Add(GetOwner());
	//UGameplayStatics::ApplyRadialDamageWithFalloff((UObject*)GetWorld(), damage, 1, mesh->GetComponentLocation(), 50, radius, .6, ValidDamageTypeClass, arr, this, (AController*)0, ECC_WorldDynamic);

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::SanitizeFloat(radius));
	arr.Add(GetOwner());
	UGameplayStatics::ApplyRadialDamageWithFalloff((UObject*)GetWorld(), damage, 1, mesh->GetComponentLocation(), 50, radius, .6, ValidDamageTypeClass, arr, this, (AController*)0, ECC_WorldDynamic);
	if (OtherActor != GetOwner())
	{
		//OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "destroyed");

		Destroy();
	}
}
