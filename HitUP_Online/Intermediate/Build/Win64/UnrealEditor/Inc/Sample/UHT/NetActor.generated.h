// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "CustomActors/NetActor.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef SAMPLE_NetActor_generated_h
#error "NetActor.generated.h already included, missing '#pragma once' in NetActor.h"
#endif
#define SAMPLE_NetActor_generated_h

#define FID_UE5_3_ServerwithClient_HitUP_Online_Source_HitUP_Online_CustomActors_NetActor_h_17_SPARSE_DATA
#define FID_UE5_3_ServerwithClient_HitUP_Online_Source_HitUP_Online_CustomActors_NetActor_h_17_SPARSE_DATA_PROPERTY_ACCESSORS
#define FID_UE5_3_ServerwithClient_HitUP_Online_Source_HitUP_Online_CustomActors_NetActor_h_17_EDITOR_ONLY_SPARSE_DATA_PROPERTY_ACCESSORS
#define FID_UE5_3_ServerwithClient_HitUP_Online_Source_HitUP_Online_CustomActors_NetActor_h_17_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execClearRoom); \
	DECLARE_FUNCTION(execJoinRoom); \
	DECLARE_FUNCTION(execReqLogin);


#define FID_UE5_3_ServerwithClient_HitUP_Online_Source_HitUP_Online_CustomActors_NetActor_h_17_ACCESSORS
#define FID_UE5_3_ServerwithClient_HitUP_Online_Source_HitUP_Online_CustomActors_NetActor_h_17_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesANetActor(); \
	friend struct Z_Construct_UClass_ANetActor_Statics; \
public: \
	DECLARE_CLASS(ANetActor, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/Sample"), NO_API) \
	DECLARE_SERIALIZER(ANetActor)


#define FID_UE5_3_ServerwithClient_HitUP_Online_Source_HitUP_Online_CustomActors_NetActor_h_17_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ANetActor(ANetActor&&); \
	NO_API ANetActor(const ANetActor&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ANetActor); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ANetActor); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(ANetActor)


#define FID_UE5_3_ServerwithClient_HitUP_Online_Source_HitUP_Online_CustomActors_NetActor_h_14_PROLOG
#define FID_UE5_3_ServerwithClient_HitUP_Online_Source_HitUP_Online_CustomActors_NetActor_h_17_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_UE5_3_ServerwithClient_HitUP_Online_Source_HitUP_Online_CustomActors_NetActor_h_17_SPARSE_DATA \
	FID_UE5_3_ServerwithClient_HitUP_Online_Source_HitUP_Online_CustomActors_NetActor_h_17_SPARSE_DATA_PROPERTY_ACCESSORS \
	FID_UE5_3_ServerwithClient_HitUP_Online_Source_HitUP_Online_CustomActors_NetActor_h_17_EDITOR_ONLY_SPARSE_DATA_PROPERTY_ACCESSORS \
	FID_UE5_3_ServerwithClient_HitUP_Online_Source_HitUP_Online_CustomActors_NetActor_h_17_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_UE5_3_ServerwithClient_HitUP_Online_Source_HitUP_Online_CustomActors_NetActor_h_17_ACCESSORS \
	FID_UE5_3_ServerwithClient_HitUP_Online_Source_HitUP_Online_CustomActors_NetActor_h_17_INCLASS_NO_PURE_DECLS \
	FID_UE5_3_ServerwithClient_HitUP_Online_Source_HitUP_Online_CustomActors_NetActor_h_17_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> SAMPLE_API UClass* StaticClass<class ANetActor>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UE5_3_ServerwithClient_HitUP_Online_Source_HitUP_Online_CustomActors_NetActor_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
