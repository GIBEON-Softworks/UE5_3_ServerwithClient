// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "HitUP_Online/CustomActors/NetActor.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeNetActor() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	SAMPLE_API UClass* Z_Construct_UClass_ANetActor();
	SAMPLE_API UClass* Z_Construct_UClass_ANetActor_NoRegister();
	UPackage* Z_Construct_UPackage__Script_Sample();
// End Cross Module References
	DEFINE_FUNCTION(ANetActor::execClearRoom)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->ClearRoom();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(ANetActor::execJoinRoom)
	{
		P_GET_PROPERTY(FIntProperty,Z_Param_RoomNumber);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->JoinRoom(Z_Param_RoomNumber);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(ANetActor::execReqLogin)
	{
		P_GET_PROPERTY(FStrProperty,Z_Param_ip);
		P_GET_PROPERTY(FStrProperty,Z_Param_port);
		P_GET_PROPERTY(FStrProperty,Z_Param_name);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->ReqLogin(Z_Param_ip,Z_Param_port,Z_Param_name);
		P_NATIVE_END;
	}
	void ANetActor::StaticRegisterNativesANetActor()
	{
		UClass* Class = ANetActor::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "ClearRoom", &ANetActor::execClearRoom },
			{ "JoinRoom", &ANetActor::execJoinRoom },
			{ "ReqLogin", &ANetActor::execReqLogin },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_ANetActor_ClearRoom_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ANetActor_ClearRoom_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "CustomActors/NetActor.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ANetActor_ClearRoom_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ANetActor, nullptr, "ClearRoom", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ANetActor_ClearRoom_Statics::Function_MetaDataParams), Z_Construct_UFunction_ANetActor_ClearRoom_Statics::Function_MetaDataParams) };
	UFunction* Z_Construct_UFunction_ANetActor_ClearRoom()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ANetActor_ClearRoom_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_ANetActor_JoinRoom_Statics
	{
		struct NetActor_eventJoinRoom_Parms
		{
			int32 RoomNumber;
		};
		static const UECodeGen_Private::FIntPropertyParams NewProp_RoomNumber;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_ANetActor_JoinRoom_Statics::NewProp_RoomNumber = { "RoomNumber", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NetActor_eventJoinRoom_Parms, RoomNumber), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ANetActor_JoinRoom_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ANetActor_JoinRoom_Statics::NewProp_RoomNumber,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ANetActor_JoinRoom_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "CustomActors/NetActor.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ANetActor_JoinRoom_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ANetActor, nullptr, "JoinRoom", nullptr, nullptr, Z_Construct_UFunction_ANetActor_JoinRoom_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ANetActor_JoinRoom_Statics::PropPointers), sizeof(Z_Construct_UFunction_ANetActor_JoinRoom_Statics::NetActor_eventJoinRoom_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ANetActor_JoinRoom_Statics::Function_MetaDataParams), Z_Construct_UFunction_ANetActor_JoinRoom_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ANetActor_JoinRoom_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_ANetActor_JoinRoom_Statics::NetActor_eventJoinRoom_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_ANetActor_JoinRoom()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ANetActor_JoinRoom_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_ANetActor_ReqLogin_Statics
	{
		struct NetActor_eventReqLogin_Parms
		{
			FString ip;
			FString port;
			FString name;
		};
		static const UECodeGen_Private::FStrPropertyParams NewProp_ip;
		static const UECodeGen_Private::FStrPropertyParams NewProp_port;
		static const UECodeGen_Private::FStrPropertyParams NewProp_name;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_ANetActor_ReqLogin_Statics::NewProp_ip = { "ip", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NetActor_eventReqLogin_Parms, ip), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_ANetActor_ReqLogin_Statics::NewProp_port = { "port", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NetActor_eventReqLogin_Parms, port), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_ANetActor_ReqLogin_Statics::NewProp_name = { "name", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(NetActor_eventReqLogin_Parms, name), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ANetActor_ReqLogin_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ANetActor_ReqLogin_Statics::NewProp_ip,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ANetActor_ReqLogin_Statics::NewProp_port,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ANetActor_ReqLogin_Statics::NewProp_name,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ANetActor_ReqLogin_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "CustomActors/NetActor.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ANetActor_ReqLogin_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ANetActor, nullptr, "ReqLogin", nullptr, nullptr, Z_Construct_UFunction_ANetActor_ReqLogin_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ANetActor_ReqLogin_Statics::PropPointers), sizeof(Z_Construct_UFunction_ANetActor_ReqLogin_Statics::NetActor_eventReqLogin_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ANetActor_ReqLogin_Statics::Function_MetaDataParams), Z_Construct_UFunction_ANetActor_ReqLogin_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_ANetActor_ReqLogin_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_ANetActor_ReqLogin_Statics::NetActor_eventReqLogin_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_ANetActor_ReqLogin()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ANetActor_ReqLogin_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ANetActor);
	UClass* Z_Construct_UClass_ANetActor_NoRegister()
	{
		return ANetActor::StaticClass();
	}
	struct Z_Construct_UClass_ANetActor_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ANetActor_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_Sample,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ANetActor_Statics::DependentSingletons) < 16);
	const FClassFunctionLinkInfo Z_Construct_UClass_ANetActor_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_ANetActor_ClearRoom, "ClearRoom" }, // 288301886
		{ &Z_Construct_UFunction_ANetActor_JoinRoom, "JoinRoom" }, // 3503899727
		{ &Z_Construct_UFunction_ANetActor_ReqLogin, "ReqLogin" }, // 51492929
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ANetActor_Statics::FuncInfo) < 2048);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ANetActor_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "CustomActors/NetActor.h" },
		{ "ModuleRelativePath", "CustomActors/NetActor.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ANetActor_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ANetActor>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_ANetActor_Statics::ClassParams = {
		&ANetActor::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		0,
		0,
		0x009000A4u,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ANetActor_Statics::Class_MetaDataParams), Z_Construct_UClass_ANetActor_Statics::Class_MetaDataParams)
	};
	UClass* Z_Construct_UClass_ANetActor()
	{
		if (!Z_Registration_Info_UClass_ANetActor.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ANetActor.OuterSingleton, Z_Construct_UClass_ANetActor_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_ANetActor.OuterSingleton;
	}
	template<> SAMPLE_API UClass* StaticClass<ANetActor>()
	{
		return ANetActor::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(ANetActor);
	struct Z_CompiledInDeferFile_FID_UE5_3_ServerwithClient_HitUP_Online_Source_HitUP_Online_CustomActors_NetActor_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UE5_3_ServerwithClient_HitUP_Online_Source_HitUP_Online_CustomActors_NetActor_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_ANetActor, ANetActor::StaticClass, TEXT("ANetActor"), &Z_Registration_Info_UClass_ANetActor, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ANetActor), 3090726430U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UE5_3_ServerwithClient_HitUP_Online_Source_HitUP_Online_CustomActors_NetActor_h_1588493476(TEXT("/Script/Sample"),
		Z_CompiledInDeferFile_FID_UE5_3_ServerwithClient_HitUP_Online_Source_HitUP_Online_CustomActors_NetActor_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UE5_3_ServerwithClient_HitUP_Online_Source_HitUP_Online_CustomActors_NetActor_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
