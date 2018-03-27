// Fill out your copyright notice in the Description page of Project Settings.

#include "Sword.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

ASword::ASword()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/PlaceholderAssets/narsil/narsil.narsil'"));
	UStaticMesh* Asset = MeshAsset.Object;
	mesh->SetStaticMesh(Asset);
}