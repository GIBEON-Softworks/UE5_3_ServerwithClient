// copyright by Choi SeungWoo

#include "HitUPGameMode.h"

#include "Engine.h"
#include "UObject/ConstructorHelpers.h"
#include <Blueprint/UserWidget.h>

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"

#include "Kismet/GameplayStatics.h" // 레벨 이동

#include "Misc/SecureHash.h"
#include "Misc/Base64.h"

#include "Json.h" // 20240222 추가

#include "GenericPlatform/GenericPlatformMisc.h"

#include "Widgets/BP_HipUpGameInstance.h"

AHitUPGameMode::AHitUPGameMode()
{

			
}


void AHitUPGameMode::BeginPlay()
{
	Super::BeginPlay();

	FString current = GetWorld()->GetMapName();
	//GEngine->AddOnScreenDebugMessage(-3, 30.0f, FColor::Red, current);

	UUserWidget* StartWidget = nullptr;

	// UEDPIE_0_
	if (GetWorld()->GetMapName() == "UEDPIE_0_Lv_Lobby01")
	{
		// 현재 레벨을 비교하여 각 레벨의 설정을 맞춰준 다
		StartWidget = CreateWidget<UUserWidget>(GetWorld(), Level1StartWidgetClass);
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		ChangeMenuWidget(Level1StartWidgetClass);

		GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Purple, TEXT("현재 Level : Lv_Lobby01"));
	}
	else if (GetWorld()->GetMapName() == "UEDPIE_0_Lv_Map01")
	{
		StartWidget = nullptr; //CreateWidget<UUserWidget>(GetWorld(), Level2StartWidgetClass);
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;

		GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Purple, TEXT("현재 Level : Lv_Map01"));
	}
	else if (GetWorld()->GetMapName() == "UEDPIE_0_Lv_Map02")
	{
		StartWidget = CreateWidget<UUserWidget>(GetWorld(), Level1StartWidgetClass);; //CreateWidget<UUserWidget>(GetWorld(), Level2StartWidgetClass);
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		ChangeMenuWidget(Level1StartWidgetClass);

		GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Purple, TEXT("현재 Level : Lv_Map02"));
	}
	//ChangeMenuWidget(StartingWidgetClass);
}

void AHitUPGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget != nullptr)
	{
		// UUserWidget에서 제공하는 RemoveFromViewport()를 사용해서
		// viewoport에 있는 UI를 제거한다
		CurrentWidget->RemoveFromParent(); //240219 : RemoveFromViewport -> RemoveFromParent 변경 viewport는 이전 사항
		CurrentWidget = nullptr;

		GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Blue, TEXT("input ChangeMenuWidget"));
	}

	if (NewWidgetClass != nullptr)
	{
		// 매개변수로 받은 NewWidgetClass가 유효한지 확인하고,

		// CurrentWidget함수로 NewWidgetClass를 저장하고
		CurrentWidget = CreateWidget(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr)
		{
			// 새 위젯을 만든다
			CurrentWidget->AddToViewport();
		}
	}
}

void AHitUPGameMode::ChangeLevel(const FString& LevelName, UWorld* World)
{
	GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Red, TEXT("input ChangeLevel"));

	/*FString NextLevelName = LevelName;*/

	// level 전환 매서드 - 최승우 
	//							여기를 거쳐서 -> 다음 레벨로 이동 한다
	
	UGameplayStatics::OpenLevel(World, FName(*LevelName));
	if (World)
	{
		// 레벨 전환 메서드
	}
	else
	{
		
		// 월드를 얻어오지 못한 경우 에러 처리
		UE_LOG(LogTemp, Error, TEXT("Failed to get world."));
	}
}

void AHitUPGameMode::CalledWeb()
{

}

void AHitUPGameMode::CallLogin(const FString& Id, const FString& Password)
{
	//FString Url = TEXT("http://192.168.0.118:8000/jun/test/7");
	//FString Url = TEXT("http://hitup.shop:8000/api/signin");
	FString Url = TEXT("https://hitup.shop/api/signin");

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField(TEXT("id"), Id);
	JsonObject->SetStringField(TEXT("password"), Password);

	FString JsonData;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonData);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

	// HTTP 요청 객체 생성
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("POST");
	HttpRequest->SetURL(Url);
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetContentAsString(JsonData);

	// 1. 로딩 창을 먼저 보여준다

	// 요청 완료 후 호출될 콜백 함수 설정 ( 대기 )
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &AHitUPGameMode::LoginOnHttpRequestComplete);

	// 요청 보내기
	HttpRequest->ProcessRequest();

	// 요청 보내기 
	//if (HttpRequest->ProcessRequest())
	//{
			// 요청 성공시
			// GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Green, TEXT(" Post Call Web Browser"));
	//}
	//else
	//{
			// 요청 실패시
	//}


}

void AHitUPGameMode::CallJoin(const FString& Id, const FString& email, const FString& Password)
{
	//FString Url = TEXT("http://hitup.shop:8000/api/signup");
	FString Url = TEXT("https://hitup.shop/api/signup");

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField(TEXT("id"), Id);
	JsonObject->SetStringField(TEXT("email"), email);
	JsonObject->SetStringField(TEXT("password"), Password);

	FString JsonData;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonData);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

	// HTTP 요청 객체 생성
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("POST");
	HttpRequest->SetURL(Url);
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetContentAsString(JsonData);

	// 1. 로딩 창을 먼저 보여준다

	// 요청 완료 후 호출될 콜백 함수 설정 ( 대기 )
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &AHitUPGameMode::JoinOnHttpRequestComplete);

	// 요청 보내기
	HttpRequest->ProcessRequest();

	// 요청 보내기 
	//if (HttpRequest->ProcessRequest())
	//{
			// 요청 성공시
			// GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Green, TEXT(" Post Call Web Browser"));
	//}
	//else
	//{
			// 요청 실패시
	//}

}

void AHitUPGameMode::ClickJoin(const FString& token, const int32 click_Point)
{
	//FString Url = TEXT("http://hitup.shop:8000/api/click/up");
	FString Url = TEXT("https://hitup.shop/api/click/up");

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	//JsonObject->SetStringField(TEXT("id"), click_Point);
	JsonObject->SetNumberField(TEXT("click"), click_Point);
	

	FString JsonData;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonData);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

	// HTTP 요청 객체 생성
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("POST");
	HttpRequest->SetURL(Url);

	UBP_HipUpGameInstance* GameInstance = Cast<UBP_HipUpGameInstance>(GetGameInstance());

	// 헤더의 토큰 추가						Bearer + "token"
	//FString AuthorizationHeaderValue = TEXT("Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ0b2tlbl90eXBlIjoiYWNjZXNzIiwiZXhwIjoxNzA4NzczNjY0LCJpYXQiOjE3MDg2ODcyNjQsImp0aSI6ImY1ZDY4NDQ1M2FlZTQ2ZjQ4MDU3NjZmNGFkNGNiNmFhIiwidWlkIjoyfQ.n3-rIpVUe84zF1ckFeFTISyDPNwHNEy3gWco5qYqvY8");
	FString AuthorizationHeaderValue = TEXT("Bearer " + GameInstance->LoadData());
	//HttpRequest->SetHeader(TEXT("Authorization"), AuthorizationHeaderValue);
	HttpRequest->SetHeader(TEXT("Authorization"), AuthorizationHeaderValue);

	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetContentAsString(JsonData);

	// 1. 로딩 창을 먼저 보여준다

	// 요청 완료 후 호출될 콜백 함수 설정 ( 대기 )
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &AHitUPGameMode::ClickOnHttpRequestComplete);

	// 요청 보내기
	HttpRequest->ProcessRequest();

	// 요청 보내기 
	//if (HttpRequest->ProcessRequest())
	//{
			// 요청 성공시
			// GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Green, TEXT(" Post Call Web Browser"));
	//}
	//else
	//{
			// 요청 실패시
	//}
}

// HTTP Log요청 완료 시 호출되는 콜백 함수
void AHitUPGameMode::LoginOnHttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	if (bSuccess && Response.IsValid())
	{
		// 응답이 성공적으로 받아졌을 때 처리하는 코드
		FString ResponseData = Response->GetContentAsString();

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseData);
		
		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			TSharedPtr<FJsonObject> ResultObject = JsonObject->GetObjectField(TEXT("result"));		

			if (ResultObject->HasField("token"))
			{
				// "token" 값을 넣을 변수 가져오기
				UBP_HipUpGameInstance* GameInstance = Cast<UBP_HipUpGameInstance>(GetGameInstance());
				
				//GameInstance = Cast<UBP_HipUpGameInstance>(GetGameInstance());

				if (GameInstance)
				{
					GameInstance->saveToken(ResultObject->GetStringField("token"));
					
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Null GameInstance"));
				}
				//my_token = ResultObject->GetStringField("token");
			}
			
			// 데이터 담기
			int32 Code = JsonObject->GetNumberField("code");
			World = GetWorld();

			// 2. 로딩 창 꺼주면서 ---> 레벨 이동
			if (Code == 2000)
			{
				// 로그인 성공 데이터가 어떻게 넘어오는지 확인 
				//GEngine->AddOnScreenDebugMessage(-10, 10.0f, FColor::Blue, TEXT(" Code : %d "), Code);
				UE_LOG(LogTemp, Warning, TEXT("Code: %d"), Code);


				// 1. 승인 요청이 완료 된다면, 
				// 2. 받아온 token 데이터를 저장 한다  

				// 3.  레벨을 넘어가고
				ChangeLevel("Lv_Map02", World);
				//ChangeLevel("Lv_Map02", World);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Code: %d"), Code);
				/*
				2000	성공	요청 성공			
				4000	실패	Bad Request			
				4102	실패	닉네임 중복			
				4105	실패	닉네임을 입력해주세요.			
				4107	실패	닉네임은 2자 이상 20자 이하여야 합니다.			
				4900	실패	인증 토큰이 존재하지 않음			
				4901	실패	인증 토큰 항목이 존재하지 않음			
				4902	실패	유효하지 않은 토큰			
				*/
			}

		}
		else
		{
			// 연결이 안됐을때, 
			// 1. 파싱해서 오류 메세지를 출력 한다
			GEngine->AddOnScreenDebugMessage(-10, 10.0f, FColor::Red, ResponseData);
		}
	}
	else
	{
		// 요청이 실패했을 때 처리하는 코드
		//UE_LOG(LogTemp, Error, TEXT("HTTP Request failed"));
		GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Red, TEXT("HTTP Request failed"));
	}
}


void AHitUPGameMode::CallRank()
{
	//FString Url = TEXT("http://hitup.shop:8000/api/signup");
	FString Url = TEXT("https://hitup.shop/api/click/ranking/10");

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	FString JsonData;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonData);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

	// HTTP 요청 객체 생성
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("POST");
	HttpRequest->SetURL(Url);
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetContentAsString(JsonData);

	// 1. 로딩 창을 먼저 보여준다

	// 요청 완료 후 호출될 콜백 함수 설정 ( 대기 )
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &AHitUPGameMode::RankOnHttpRequestComplete);

	// 요청 보내기
	HttpRequest->ProcessRequest();

	// 요청 보내기 
	//if (HttpRequest->ProcessRequest())
	//{
			// 요청 성공시
			// GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Green, TEXT(" Post Call Web Browser"));
	//}
	//else
	//{
			// 요청 실패시
	//}
}

// HTTP Join요청 완료 시 호출되는 콜백 함수
void AHitUPGameMode::JoinOnHttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	if (bSuccess && Response.IsValid())
	{
		// 응답이 성공적으로 받아졌을 때 처리하는 코드
		FString ResponseData = Response->GetContentAsString();

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseData);

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			// 데이터 담기
			int32 Code = JsonObject->GetNumberField("code");
			FString Message = JsonObject->GetStringField("message");

			switch (Code)
			{
			case 2000:
				UE_LOG(LogTemp, Warning, TEXT("message : %s"), *Message);
				break;
			case 4000:
				UE_LOG(LogTemp, Warning, TEXT("message : %s"), *Message);
				break;
			case 4100:
				UE_LOG(LogTemp, Warning, TEXT("message : %s"), *Message);
				break;
			case 4101:
				UE_LOG(LogTemp, Warning, TEXT("message : %s"), *Message);
				break;
			case 4103:
				UE_LOG(LogTemp, Warning, TEXT("message : %s"), *Message);
				break;
			case 4104:
				UE_LOG(LogTemp, Warning, TEXT("message : %s"), *Message);
				break;
			case 4106:
				UE_LOG(LogTemp, Warning, TEXT("message : %s"), *Message);
				break;
			}
			/*
			2000	성공	요청 성공
			4000	실패	Bad Request
			4100	실패	이메일 중복
			4101	실패	아이디 중복
			4103	실패	이메일을 입력해주세요.
			4104	실패	ID를 입력해주세요.
			4106	실패	비밀번호를 입력해주세요.
			*/
		}
		else
		{
			// 연결이 안됐을때, 
			// 1. 파싱해서 오류 메세지를 출력 한다
			GEngine->AddOnScreenDebugMessage(-10, 2.0f, FColor::Red, ResponseData);
		}
	}
	else
	{
		// 요청이 실패했을 때 처리하는 코드
		//UE_LOG(LogTemp, Error, TEXT("HTTP Request failed"));
		GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Red, TEXT("HTTP Request failed"));
	}
}

void AHitUPGameMode::ClickOnHttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	if (bSuccess && Response.IsValid())
	{
		// 응답이 성공적으로 받아졌을 때 처리하는 코드
		FString ResponseData = Response->GetContentAsString();

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseData);

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			// 데이터 담기
			int32 Code = JsonObject->GetNumberField("code");
			FString Message = JsonObject->GetStringField("message");

			switch (Code)
			{
			case 2000:
				UE_LOG(LogTemp, Warning, TEXT("message : %s"), *Message);
				break;
			case 4000:
				UE_LOG(LogTemp, Warning, TEXT("message : %s"), *Message);
				break;
			case 4001:
				UE_LOG(LogTemp, Warning, TEXT("message : %s"), *Message);
				break;
			case 4900:
				UE_LOG(LogTemp, Warning, TEXT("message : %s"), *Message);
				break;
			case 4901:
				UE_LOG(LogTemp, Warning, TEXT("message : %s"), *Message);
				break;
			case 4902:
				UE_LOG(LogTemp, Warning, TEXT("message : %s"), *Message);
				break;
			case 5200:
				UE_LOG(LogTemp, Warning, TEXT("message : %s"), *Message);
				break;
			}
			/*
			2000	성공	요청 성공	
			4000	실패	Bad Request	
			4001	실패	잘못된 입력입니다.	
			4900	실패	인증 토큰이 존재하지 않음	
			4901	실패	인증 토큰 항목이 존재하지 않음	
			4902	실패	유효하지 않은 토큰	
			5200	오류	데이터베이스 오류	
			*/
		}
		else
		{
			// 연결이 안됐을때, 
			// 1. 파싱해서 오류 메세지를 출력 한다
			GEngine->AddOnScreenDebugMessage(-10, 2.0f, FColor::Red, ResponseData);
		}
	}
	else
	{
		// 요청이 실패했을 때 처리하는 코드
		//UE_LOG(LogTemp, Error, TEXT("HTTP Request failed"));
		GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Red, TEXT("HTTP Request failed"));
	}
}

void AHitUPGameMode::RankOnHttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	FString ResponseData = Response->GetContentAsString();

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseData);


	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		// "result" 필드가 있는지 확인
		if (JsonObject->HasField("result"))
		{
			// "result" 필드의 배열 가져오기
			TArray<TSharedPtr<FJsonValue>> ResultArray = JsonObject->GetArrayField("result");
			UBP_HipUpGameInstance* GameInstance = Cast<UBP_HipUpGameInstance>(GetGameInstance());
			
			GameInstance->vRank_data.Reset();

			// 각 필드의 배열 생성
			TArray<int32> UserIds;
			TArray<int32> ClickCounts;
			TArray<int32> Ranks;

			// 각 항목 순회하며 데이터 파싱 및 배열에 추가
			for (const TSharedPtr<FJsonValue>& ResultItem : ResultArray)
			{
				TSharedPtr<FJsonObject> ResultObject = ResultItem->AsObject();
				if (ResultObject.IsValid())
				{
					// 데이터 파싱
					int32 UserId = ResultObject->GetIntegerField("user_id");
					int32 ClickCount = ResultObject->GetIntegerField("click_cnt");
					int32 Rank = ResultObject->GetIntegerField("rank");

					// 배열에 추가
					UserIds.Add(UserId);
					ClickCounts.Add(ClickCount);
					Ranks.Add(Rank);

					GameInstance->vRank_data.Add(FString::Printf(TEXT("user_id : %d click_cnt : %d rank : %d"), UserId, ClickCount, Rank));
				}
			}
		

			// 데이터 출력
			for (int32 Index = 0; Index < ResultArray.Num(); ++Index)
			{
				UE_LOG(LogTemp, Warning, TEXT("User ID: %d, Click Count: %d, Rank: %d"),
					UserIds[Index], ClickCounts[Index], Ranks[Index]);

				UE_LOG(LogTemp, Warning, TEXT("Rank_Data : %s"), *GameInstance->vRank_data[Index]);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("JSON 파싱 오류: 'result' 필드가 없습니다."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("JSON 파싱 오류: 올바르지 않은 JSON 형식입니다."));
	}

}

// 192.168.0.118



