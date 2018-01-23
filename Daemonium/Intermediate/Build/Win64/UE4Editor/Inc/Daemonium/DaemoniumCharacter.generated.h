// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "ObjectMacros.h"
#include "ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef DAEMONIUM_DaemoniumCharacter_generated_h
#error "DaemoniumCharacter.generated.h already included, missing '#pragma once' in DaemoniumCharacter.h"
#endif
#define DAEMONIUM_DaemoniumCharacter_generated_h

#define Daemonium_Source_Daemonium_DaemoniumCharacter_h_14_RPC_WRAPPERS
#define Daemonium_Source_Daemonium_DaemoniumCharacter_h_14_RPC_WRAPPERS_NO_PURE_DECLS
#define Daemonium_Source_Daemonium_DaemoniumCharacter_h_14_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesADaemoniumCharacter(); \
	friend DAEMONIUM_API class UClass* Z_Construct_UClass_ADaemoniumCharacter(); \
public: \
	DECLARE_CLASS(ADaemoniumCharacter, ACharacter, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/Daemonium"), NO_API) \
	DECLARE_SERIALIZER(ADaemoniumCharacter) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define Daemonium_Source_Daemonium_DaemoniumCharacter_h_14_INCLASS \
private: \
	static void StaticRegisterNativesADaemoniumCharacter(); \
	friend DAEMONIUM_API class UClass* Z_Construct_UClass_ADaemoniumCharacter(); \
public: \
	DECLARE_CLASS(ADaemoniumCharacter, ACharacter, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/Daemonium"), NO_API) \
	DECLARE_SERIALIZER(ADaemoniumCharacter) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define Daemonium_Source_Daemonium_DaemoniumCharacter_h_14_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API ADaemoniumCharacter(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(ADaemoniumCharacter) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ADaemoniumCharacter); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ADaemoniumCharacter); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ADaemoniumCharacter(ADaemoniumCharacter&&); \
	NO_API ADaemoniumCharacter(const ADaemoniumCharacter&); \
public:


#define Daemonium_Source_Daemonium_DaemoniumCharacter_h_14_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ADaemoniumCharacter(ADaemoniumCharacter&&); \
	NO_API ADaemoniumCharacter(const ADaemoniumCharacter&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ADaemoniumCharacter); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ADaemoniumCharacter); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(ADaemoniumCharacter)


#define Daemonium_Source_Daemonium_DaemoniumCharacter_h_14_PRIVATE_PROPERTY_OFFSET \
	FORCEINLINE static uint32 __PPO__Mesh1P() { return STRUCT_OFFSET(ADaemoniumCharacter, Mesh1P); } \
	FORCEINLINE static uint32 __PPO__FP_Gun() { return STRUCT_OFFSET(ADaemoniumCharacter, FP_Gun); } \
	FORCEINLINE static uint32 __PPO__FP_MuzzleLocation() { return STRUCT_OFFSET(ADaemoniumCharacter, FP_MuzzleLocation); } \
	FORCEINLINE static uint32 __PPO__VR_Gun() { return STRUCT_OFFSET(ADaemoniumCharacter, VR_Gun); } \
	FORCEINLINE static uint32 __PPO__VR_MuzzleLocation() { return STRUCT_OFFSET(ADaemoniumCharacter, VR_MuzzleLocation); } \
	FORCEINLINE static uint32 __PPO__FirstPersonCameraComponent() { return STRUCT_OFFSET(ADaemoniumCharacter, FirstPersonCameraComponent); } \
	FORCEINLINE static uint32 __PPO__R_MotionController() { return STRUCT_OFFSET(ADaemoniumCharacter, R_MotionController); } \
	FORCEINLINE static uint32 __PPO__L_MotionController() { return STRUCT_OFFSET(ADaemoniumCharacter, L_MotionController); }


#define Daemonium_Source_Daemonium_DaemoniumCharacter_h_11_PROLOG
#define Daemonium_Source_Daemonium_DaemoniumCharacter_h_14_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	Daemonium_Source_Daemonium_DaemoniumCharacter_h_14_PRIVATE_PROPERTY_OFFSET \
	Daemonium_Source_Daemonium_DaemoniumCharacter_h_14_RPC_WRAPPERS \
	Daemonium_Source_Daemonium_DaemoniumCharacter_h_14_INCLASS \
	Daemonium_Source_Daemonium_DaemoniumCharacter_h_14_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define Daemonium_Source_Daemonium_DaemoniumCharacter_h_14_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	Daemonium_Source_Daemonium_DaemoniumCharacter_h_14_PRIVATE_PROPERTY_OFFSET \
	Daemonium_Source_Daemonium_DaemoniumCharacter_h_14_RPC_WRAPPERS_NO_PURE_DECLS \
	Daemonium_Source_Daemonium_DaemoniumCharacter_h_14_INCLASS_NO_PURE_DECLS \
	Daemonium_Source_Daemonium_DaemoniumCharacter_h_14_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID Daemonium_Source_Daemonium_DaemoniumCharacter_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
