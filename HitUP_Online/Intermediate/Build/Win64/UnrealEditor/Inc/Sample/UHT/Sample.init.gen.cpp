// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeSample_init() {}
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_Sample;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_Sample()
	{
		if (!Z_Registration_Info_UPackage__Script_Sample.OuterSingleton)
		{
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/Sample",
				nullptr,
				0,
				PKG_CompiledIn | 0x00000000,
				0xC1D35C91,
				0xD1C9632C,
				METADATA_PARAMS(0, nullptr)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_Sample.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_Sample.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_Sample(Z_Construct_UPackage__Script_Sample, TEXT("/Script/Sample"), Z_Registration_Info_UPackage__Script_Sample, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0xC1D35C91, 0xD1C9632C));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
