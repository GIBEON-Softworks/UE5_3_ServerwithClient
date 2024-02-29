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

#include "Kismet/GameplayStatics.h" // ���� �̵�

#include "Misc/SecureHash.h"
#include "Misc/Base64.h"

#include "Json.h" // 20240222 �߰�

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
		// ���� ������ ���Ͽ� �� ������ ������ ������ ��
		StartWidget = CreateWidget<UUserWidget>(GetWorld(), Level1StartWidgetClass);
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		ChangeMenuWidget(Level1StartWidgetClass);

		GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Purple, TEXT("���� Level : Lv_Lobby01"));
	}
	else if (GetWorld()->GetMapName() == "UEDPIE_0_Lv_Map01")
	{
		StartWidget = nullptr; //CreateWidget<UUserWidget>(GetWorld(), Level2StartWidgetClass);
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;

		GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Purple, TEXT("���� Level : Lv_Map01"));
	}
	else if (GetWorld()->GetMapName() == "UEDPIE_0_Lv_Map02")
	{
		StartWidget = CreateWidget<UUserWidget>(GetWorld(), Level1StartWidgetClass);; //CreateWidget<UUserWidget>(GetWorld(), Level2StartWidgetClass);
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		ChangeMenuWidget(Level1StartWidgetClass);

		GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Purple, TEXT("���� Level : Lv_Map02"));
	}
	//ChangeMenuWidget(StartingWidgetClass);
}

void AHitUPGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget != nullptr)
	{
		// UUserWidget���� �����ϴ� RemoveFromViewport()�� ����ؼ�
		// viewoport�� �ִ� UI�� �����Ѵ�
		CurrentWidget->RemoveFromParent(); //240219 : RemoveFromViewport -> RemoveFromParent ���� viewport�� ���� ����
		CurrentWidget = nullptr;

		GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Blue, TEXT("input ChangeMenuWidget"));
	}

	if (NewWidgetClass != nullptr)
	{
		// �Ű������� ���� NewWidgetClass�� ��ȿ���� Ȯ���ϰ�,

		// CurrentWidget�Լ��� NewWidgetClass�� �����ϰ�
		CurrentWidget = CreateWidget(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr)
		{
			// �� ������ �����
			CurrentWidget->AddToViewport();
		}
	}
}

void AHitUPGameMode::ChangeLevel(const FString& LevelName, UWorld* World)
{
	GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Red, TEXT("input ChangeLevel"));

	/*FString NextLevelName = LevelName;*/

	// level ��ȯ �ż��� - �ֽ¿� 
	//							���⸦ ���ļ� -> ���� ������ �̵� �Ѵ�
	
	UGameplayStatics::OpenLevel(World, FName(*LevelName));
	if (World)
	{
		// ���� ��ȯ �޼���
	}
	else
	{
		
		// ���带 ������ ���� ��� ���� ó��
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

	// HTTP ��û ��ü ����
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("POST");
	HttpRequest->SetURL(Url);
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetContentAsString(JsonData);

	// 1. �ε� â�� ���� �����ش�

	// ��û �Ϸ� �� ȣ��� �ݹ� �Լ� ���� ( ��� )
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &AHitUPGameMode::LoginOnHttpRequestComplete);

	// ��û ������
	HttpRequest->ProcessRequest();

	// ��û ������ 
	//if (HttpRequest->ProcessRequest())
	//{
			// ��û ������
			// GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Green, TEXT(" Post Call Web Browser"));
	//}
	//else
	//{
			// ��û ���н�
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

	// HTTP ��û ��ü ����
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("POST");
	HttpRequest->SetURL(Url);
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetContentAsString(JsonData);

	// 1. �ε� â�� ���� �����ش�

	// ��û �Ϸ� �� ȣ��� �ݹ� �Լ� ���� ( ��� )
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &AHitUPGameMode::JoinOnHttpRequestComplete);

	// ��û ������
	HttpRequest->ProcessRequest();

	// ��û ������ 
	//if (HttpRequest->ProcessRequest())
	//{
			// ��û ������
			// GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Green, TEXT(" Post Call Web Browser"));
	//}
	//else
	//{
			// ��û ���н�
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

	// HTTP ��û ��ü ����
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("POST");
	HttpRequest->SetURL(Url);

	UBP_HipUpGameInstance* GameInstance = Cast<UBP_HipUpGameInstance>(GetGameInstance());

	// ����� ��ū �߰�						Bearer + "token"
	//FString AuthorizationHeaderValue = TEXT("Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ0b2tlbl90eXBlIjoiYWNjZXNzIiwiZXhwIjoxNzA4NzczNjY0LCJpYXQiOjE3MDg2ODcyNjQsImp0aSI6ImY1ZDY4NDQ1M2FlZTQ2ZjQ4MDU3NjZmNGFkNGNiNmFhIiwidWlkIjoyfQ.n3-rIpVUe84zF1ckFeFTISyDPNwHNEy3gWco5qYqvY8");
	FString AuthorizationHeaderValue = TEXT("Bearer " + GameInstance->LoadData());
	//HttpRequest->SetHeader(TEXT("Authorization"), AuthorizationHeaderValue);
	HttpRequest->SetHeader(TEXT("Authorization"), AuthorizationHeaderValue);

	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetContentAsString(JsonData);

	// 1. �ε� â�� ���� �����ش�

	// ��û �Ϸ� �� ȣ��� �ݹ� �Լ� ���� ( ��� )
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &AHitUPGameMode::ClickOnHttpRequestComplete);

	// ��û ������
	HttpRequest->ProcessRequest();

	// ��û ������ 
	//if (HttpRequest->ProcessRequest())
	//{
			// ��û ������
			// GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Green, TEXT(" Post Call Web Browser"));
	//}
	//else
	//{
			// ��û ���н�
	//}
}

// HTTP Log��û �Ϸ� �� ȣ��Ǵ� �ݹ� �Լ�
void AHitUPGameMode::LoginOnHttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	if (bSuccess && Response.IsValid())
	{
		// ������ ���������� �޾����� �� ó���ϴ� �ڵ�
		FString ResponseData = Response->GetContentAsString();

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseData);
		
		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			TSharedPtr<FJsonObject> ResultObject = JsonObject->GetObjectField(TEXT("result"));		

			if (ResultObject->HasField("token"))
			{
				// "token" ���� ���� ���� ��������
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
			
			// ������ ���
			int32 Code = JsonObject->GetNumberField("code");
			World = GetWorld();

			// 2. �ε� â ���ָ鼭 ---> ���� �̵�
			if (Code == 2000)
			{
				// �α��� ���� �����Ͱ� ��� �Ѿ������ Ȯ�� 
				//GEngine->AddOnScreenDebugMessage(-10, 10.0f, FColor::Blue, TEXT(" Code : %d "), Code);
				UE_LOG(LogTemp, Warning, TEXT("Code: %d"), Code);


				// 1. ���� ��û�� �Ϸ� �ȴٸ�, 
				// 2. �޾ƿ� token �����͸� ���� �Ѵ�  

				// 3.  ������ �Ѿ��
				ChangeLevel("Lv_Map02", World);
				//ChangeLevel("Lv_Map02", World);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Code: %d"), Code);
				/*
				2000	����	��û ����			
				4000	����	Bad Request			
				4102	����	�г��� �ߺ�			
				4105	����	�г����� �Է����ּ���.			
				4107	����	�г����� 2�� �̻� 20�� ���Ͽ��� �մϴ�.			
				4900	����	���� ��ū�� �������� ����			
				4901	����	���� ��ū �׸��� �������� ����			
				4902	����	��ȿ���� ���� ��ū			
				*/
			}

		}
		else
		{
			// ������ �ȵ�����, 
			// 1. �Ľ��ؼ� ���� �޼����� ��� �Ѵ�
			GEngine->AddOnScreenDebugMessage(-10, 10.0f, FColor::Red, ResponseData);
		}
	}
	else
	{
		// ��û�� �������� �� ó���ϴ� �ڵ�
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

	// HTTP ��û ��ü ����
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("POST");
	HttpRequest->SetURL(Url);
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetContentAsString(JsonData);

	// 1. �ε� â�� ���� �����ش�

	// ��û �Ϸ� �� ȣ��� �ݹ� �Լ� ���� ( ��� )
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &AHitUPGameMode::RankOnHttpRequestComplete);

	// ��û ������
	HttpRequest->ProcessRequest();

	// ��û ������ 
	//if (HttpRequest->ProcessRequest())
	//{
			// ��û ������
			// GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Green, TEXT(" Post Call Web Browser"));
	//}
	//else
	//{
			// ��û ���н�
	//}
}

// HTTP Join��û �Ϸ� �� ȣ��Ǵ� �ݹ� �Լ�
void AHitUPGameMode::JoinOnHttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	if (bSuccess && Response.IsValid())
	{
		// ������ ���������� �޾����� �� ó���ϴ� �ڵ�
		FString ResponseData = Response->GetContentAsString();

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseData);

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			// ������ ���
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
			2000	����	��û ����
			4000	����	Bad Request
			4100	����	�̸��� �ߺ�
			4101	����	���̵� �ߺ�
			4103	����	�̸����� �Է����ּ���.
			4104	����	ID�� �Է����ּ���.
			4106	����	��й�ȣ�� �Է����ּ���.
			*/
		}
		else
		{
			// ������ �ȵ�����, 
			// 1. �Ľ��ؼ� ���� �޼����� ��� �Ѵ�
			GEngine->AddOnScreenDebugMessage(-10, 2.0f, FColor::Red, ResponseData);
		}
	}
	else
	{
		// ��û�� �������� �� ó���ϴ� �ڵ�
		//UE_LOG(LogTemp, Error, TEXT("HTTP Request failed"));
		GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Red, TEXT("HTTP Request failed"));
	}
}

void AHitUPGameMode::ClickOnHttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	if (bSuccess && Response.IsValid())
	{
		// ������ ���������� �޾����� �� ó���ϴ� �ڵ�
		FString ResponseData = Response->GetContentAsString();

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseData);

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			// ������ ���
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
			2000	����	��û ����	
			4000	����	Bad Request	
			4001	����	�߸��� �Է��Դϴ�.	
			4900	����	���� ��ū�� �������� ����	
			4901	����	���� ��ū �׸��� �������� ����	
			4902	����	��ȿ���� ���� ��ū	
			5200	����	�����ͺ��̽� ����	
			*/
		}
		else
		{
			// ������ �ȵ�����, 
			// 1. �Ľ��ؼ� ���� �޼����� ��� �Ѵ�
			GEngine->AddOnScreenDebugMessage(-10, 2.0f, FColor::Red, ResponseData);
		}
	}
	else
	{
		// ��û�� �������� �� ó���ϴ� �ڵ�
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
		// "result" �ʵ尡 �ִ��� Ȯ��
		if (JsonObject->HasField("result"))
		{
			// "result" �ʵ��� �迭 ��������
			TArray<TSharedPtr<FJsonValue>> ResultArray = JsonObject->GetArrayField("result");
			UBP_HipUpGameInstance* GameInstance = Cast<UBP_HipUpGameInstance>(GetGameInstance());
			
			GameInstance->vRank_data.Reset();

			// �� �ʵ��� �迭 ����
			TArray<int32> UserIds;
			TArray<int32> ClickCounts;
			TArray<int32> Ranks;

			// �� �׸� ��ȸ�ϸ� ������ �Ľ� �� �迭�� �߰�
			for (const TSharedPtr<FJsonValue>& ResultItem : ResultArray)
			{
				TSharedPtr<FJsonObject> ResultObject = ResultItem->AsObject();
				if (ResultObject.IsValid())
				{
					// ������ �Ľ�
					int32 UserId = ResultObject->GetIntegerField("user_id");
					int32 ClickCount = ResultObject->GetIntegerField("click_cnt");
					int32 Rank = ResultObject->GetIntegerField("rank");

					// �迭�� �߰�
					UserIds.Add(UserId);
					ClickCounts.Add(ClickCount);
					Ranks.Add(Rank);

					GameInstance->vRank_data.Add(FString::Printf(TEXT("user_id : %d click_cnt : %d rank : %d"), UserId, ClickCount, Rank));
				}
			}
		

			// ������ ���
			for (int32 Index = 0; Index < ResultArray.Num(); ++Index)
			{
				UE_LOG(LogTemp, Warning, TEXT("User ID: %d, Click Count: %d, Rank: %d"),
					UserIds[Index], ClickCounts[Index], Ranks[Index]);

				UE_LOG(LogTemp, Warning, TEXT("Rank_Data : %s"), *GameInstance->vRank_data[Index]);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("JSON �Ľ� ����: 'result' �ʵ尡 �����ϴ�."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("JSON �Ľ� ����: �ùٸ��� ���� JSON �����Դϴ�."));
	}

}

// 192.168.0.118



