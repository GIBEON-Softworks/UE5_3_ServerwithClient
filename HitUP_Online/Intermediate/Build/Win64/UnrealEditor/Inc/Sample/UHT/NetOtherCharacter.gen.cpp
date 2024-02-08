// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "HitUP_Online/Characters/NetOtherCharacter.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeNetOtherCharacter() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_ACharacter();
	SAMPLE_API UClass* Z_Construct_UClass_ANetOtherCharacter();
	SAMPLE_API UClass* Z_Construct_UClass_ANetOtherCharacter_NoRegister();
	UPackage* Z_Construct_UPackage__Script_Sample();
// End Cross Module References
	void ANetOtherCharacter::StaticRegisterNativesANetOtherCharacter()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ANetOtherCharacter);
	UClass* Z_Construct_UClass_ANetOtherCharacter_NoRegister()
	{
		return ANetOtherCharacter::StaticClass();
	}
	struct Z_Construct_UClass_ANetOtherCharacter_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ANetOtherCharacter_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_ACharacter,
		(UObject* (*)())Z_Construct_UPackage__Script_Sample,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ANetOtherCharacter_Statics::DependentSingletons) < 16);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ANetOtherCharacter_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Navigation" },
		{ "IncludePath", "Characters/NetOtherCharacter.h" },
		{ "ModuleRelativePath", "Characters/NetOtherCharacter.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ANetOtherCharacter_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ANetOtherCharacter>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_ANetOtherCharacter_Statics::ClassParams = {
		&ANetOtherCharacter::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009000A4u,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ANetOtherCharacter_Statics::Class_MetaDataParams), Z_Construct_UClass_ANetOtherCharacter_Statics::Class_MetaDataParams)
	};
	UClass* Z_Construct_UClass_ANetOtherCharacter()
	{
		if (!Z_Registration_Info_UClass_ANetOtherCharacter.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ANetOtherCharacter.OuterSingleton, Z_Construct_UClass_ANetOtherCharacter_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_ANetOtherCharacter.OuterSingleton;
	}
	template<> SAMPLE_API UClass* StaticClass<ANetOtherCharacter>()
	{
		return ANetOtherCharacter::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(ANetOtherCharacter);
	ANetOtherCharacter::~ANetOtherCharacter() {}
	struct Z_CompiledInDeferFile_FID_UE5_3_ServerwithClient_HitUP_Online_Source_HitUP_Online_Characters_NetOtherCharacter_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UE5_3_ServerwithClient_HitUP_Online_Source_HitUP_Online_Characters_NetOtherCharacter_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_ANetOtherCharacter, ANetOtherCharacter::StaticClass, TEXT("ANetOtherCharacter"), &Z_Registration_Info_UClass_ANetOtherCharacter, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ANetOtherCharacter), 1567827170U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UE5_3_ServerwithClient_HitUP_Online_Source_HitUP_Online_Characters_NetOtherCharacter_h_1906236056(TEXT("/Script/Sample"),
		Z_CompiledInDeferFile_FID_UE5_3_ServerwithClient_HitUP_Online_Source_HitUP_Online_Characters_NetOtherCharacter_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UE5_3_ServerwithClient_HitUP_Online_Source_HitUP_Online_Characters_NetOtherCharacter_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
