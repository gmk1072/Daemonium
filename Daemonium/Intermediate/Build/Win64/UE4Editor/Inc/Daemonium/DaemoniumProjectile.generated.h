// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "ObjectMacros.h"
#include "ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UPrimitiveComponent;
class AActor;
struct FVector;
struct FHitResult;
#ifdef DAEMONIUM_DaemoniumProjectile_generated_h
#error "DaemoniumProjectile.generated.h already included, missing '#pragma once' in DaemoniumProjectile.h"
#endif
#define DAEMONIUM_DaemoniumProjectile_generated_h

#define Daemonium_Source_Daemonium_DaemoniumProjectile_h_12_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execOnHit) \
	{ \
		P_GET_OBJECT(UPrimitiveComponent,Z_Param_HitComp); \
		P_GET_OBJECT(AActor,Z_Param_OtherActor); \
		P_GET_OBJECT(UPrimitiveComponent,Z_Param_OtherComp); \
		P_GET_STRUCT(FVector,Z_Param_NormalImpulse); \
		P_GET_STRUCT_REF(FHitResult,Z_Param_Out_Hit); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->OnHit(Z_Param_HitComp,Z_Param_OtherActor,Z_Param_OtherComp,Z_Param_NormalImpulse,Z_Param_Out_Hit); \
		P_NATIVE_END; \
	}


#define Daemonium_Source_Daemonium_DaemoniumProjectile_h_12_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execOnHit) \
	{ \
		P_GET_OBJECT(UPrimitiveComponent,Z_Param_HitComp); \
		P_GET_OBJECT(AActor,Z_Param_OtherActor); \
		P_GET_OBJECT(UPrimitiveComponent,Z_Param_OtherComp); \
		P_GET_STRUCT(FVector,Z_Param_NormalImpulse); \
		P_GET_STRUCT_REF(FHitResult,Z_Param_Out_Hit); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		this->OnHit(Z_Param_HitComp,Z_Param_OtherActor,Z_Param_OtherComp,Z_Param_NormalImpulse,Z_Param_Out_Hit); \
		P_NATIVE_END; \
	}


#define Daemonium_Source_Daemonium_DaemoniumProjectile_h_12_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesADaemoniumProjectile(); \
	friend DAEMONIUM_API class UClass* Z_Construct_UClass_ADaemoniumProjectile(); \
public: \
	DECLARE_CLASS(ADaemoniumProjectile, AActor, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/Daemonium"), NO_API) \
	DECLARE_SERIALIZER(ADaemoniumProjectile) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC}; \
	static const TCHAR* StaticConfigName() {return TEXT("Game");} \



#define Daemonium_Source_Daemonium_DaemoniumProjectile_h_12_INCLASS \
private: \
	static void StaticRegisterNativesADaemoniumProjectile(); \
	friend DAEMONIUM_API class UClass* Z_Construct_UClass_ADaemoniumProjectile(); \
public: \
	DECLARE_CLASS(ADaemoniumProjectile, AActor, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/Daemonium"), NO_API) \
	DECLARE_SERIALIZER(ADaemoniumProjectile) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC}; \
	static const TCHAR* StaticConfigName() {return TEXT("Game");} \



#define Daemonium_Source_Daemonium_DaemoniumProjectile_h_12_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API ADaemoniumProjectile(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(ADaemoniumProjectile) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ADaemoniumProjectile); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ADaemoniumProjectile); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ADaemoniumProjectile(ADaemoniumProjectile&&); \
	NO_API ADaemoniumProjectile(const ADaemoniumProjectile&); \
public:


#define Daemonium_Source_Daemonium_DaemoniumProjectile_h_12_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ADaemoniumProjectile(ADaemoniumProjectile&&); \
	NO_API ADaemoniumProjectile(const ADaemoniumProjectile&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ADaemoniumProjectile); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ADaemoniumProjectile); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(ADaemoniumProjectile)


#define Daemonium_Source_Daemonium_DaemoniumProjectile_h_12_PRIVATE_PROPERTY_OFFSET \
	FORCEINLINE static uint32 __PPO__CollisionComp() { return STRUCT_OFFSET(ADaemoniumProjectile, CollisionComp); } \
	FORCEINLINE static uint32 __PPO__ProjectileMovement() { return STRUCT_OFFSET(ADaemoniumProjectile, ProjectileMovement); }


#define Daemonium_Source_Daemonium_DaemoniumProjectile_h_9_PROLOG
#define Daemonium_Source_Daemonium_DaemoniumProjectile_h_12_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	Daemonium_Source_Daemonium_DaemoniumProjectile_h_12_PRIVATE_PROPERTY_OFFSET \
	Daemonium_Source_Daemonium_DaemoniumProjectile_h_12_RPC_WRAPPERS \
	Daemonium_Source_Daemonium_DaemoniumProjectile_h_12_INCLASS \
	Daemonium_Source_Daemonium_DaemoniumProjectile_h_12_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define Daemonium_Source_Daemonium_DaemoniumProjectile_h_12_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	Daemonium_Source_Daemonium_DaemoniumProjectile_h_12_PRIVATE_PROPERTY_OFFSET \
	Daemonium_Source_Daemonium_DaemoniumProjectile_h_12_RPC_WRAPPERS_NO_PURE_DECLS \
	Daemonium_Source_Daemonium_DaemoniumProjectile_h_12_INCLASS_NO_PURE_DECLS \
	Daemonium_Source_Daemonium_DaemoniumProjectile_h_12_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID Daemonium_Source_Daemonium_DaemoniumProjectile_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
