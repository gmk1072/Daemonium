// Fill out your copyright notice in the Description page of Project Settings.

#include "FireStaff.h"




#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

AFireStaff::AFireStaff()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/PlaceholderAssets/staff/staff_staff.staff_staff'"));
	UStaticMesh* Asset = MeshAsset.Object;
	mesh->SetStaticMesh(Asset);
}