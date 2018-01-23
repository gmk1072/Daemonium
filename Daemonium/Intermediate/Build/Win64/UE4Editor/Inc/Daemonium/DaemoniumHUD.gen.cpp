// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "DaemoniumHUD.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeDaemoniumHUD() {}
// Cross Module References
	DAEMONIUM_API UClass* Z_Construct_UClass_ADaemoniumHUD_NoRegister();
	DAEMONIUM_API UClass* Z_Construct_UClass_ADaemoniumHUD();
	ENGINE_API UClass* Z_Construct_UClass_AHUD();
	UPackage* Z_Construct_UPackage__Script_Daemonium();
// End Cross Module References
	void ADaemoniumHUD::StaticRegisterNativesADaemoniumHUD()
	{
	}
	UClass* Z_Construct_UClass_ADaemoniumHUD_NoRegister()
	{
		return ADaemoniumHUD::StaticClass();
	}
	UClass* Z_Construct_UClass_ADaemoniumHUD()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			static UObject* (*const DependentSingletons[])() = {
				(UObject* (*)())Z_Construct_UClass_AHUD,
				(UObject* (*)())Z_Construct_UPackage__Script_Daemonium,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
				{ "HideCategories", "Rendering Actor Input Replication" },
				{ "IncludePath", "DaemoniumHUD.h" },
				{ "ModuleRelativePath", "DaemoniumHUD.h" },
				{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
			};
#endif
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<ADaemoniumHUD>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&ADaemoniumHUD::StaticClass,
				DependentSingletons, ARRAY_COUNT(DependentSingletons),
				0x0080028Cu,
				nullptr, 0,
				nullptr, 0,
				"Game",
				&StaticCppClassTypeInfo,
				nullptr, 0,
				METADATA_PARAMS(Class_MetaDataParams, ARRAY_COUNT(Class_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUClass(OuterClass, ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(ADaemoniumHUD, 3491565062);
	static FCompiledInDefer Z_CompiledInDefer_UClass_ADaemoniumHUD(Z_Construct_UClass_ADaemoniumHUD, &ADaemoniumHUD::StaticClass, TEXT("/Script/Daemonium"), TEXT("ADaemoniumHUD"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ADaemoniumHUD);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
