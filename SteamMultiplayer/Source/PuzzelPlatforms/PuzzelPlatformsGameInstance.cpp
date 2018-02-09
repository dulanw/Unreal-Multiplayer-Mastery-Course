// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzelPlatformsGameInstance.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "PlatformTrigger.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MenuWidget.h"
#include "MenuSystem/MainMenu.h"
#include "OnlineSessionSettings.h"

const static FName SESSION_NAME = TEXT("GameSession");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

UPuzzelPlatformsGameInstance::UPuzzelPlatformsGameInstance(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));

	if (ensure(MainMenuBPClass.Class != NULL))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Class found: %s"), *PlatformTriggerBPClass.Class->GetName());
		MainMenuClass = MainMenuBPClass.Class;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));

	if (ensure(InGameMenuBPClass.Class != NULL))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Class found: %s"), *PlatformTriggerBPClass.Class->GetName());
		InGameMenuClass = InGameMenuBPClass.Class;
	}


	//UE_LOG(LogTemp, Warning, TEXT("GameInstance Constructor"));
}


void UPuzzelPlatformsGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Init"));
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Online Subsystem found: %s"), *OnlineSubsystem->GetSubsystemName().ToString());
		SessionInterface = OnlineSubsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->CreateSession
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzelPlatformsGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzelPlatformsGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzelPlatformsGameInstance::OnFindSessionComplete);		
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzelPlatformsGameInstance::OnJoinSessionComplete);
		}
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("SubSystem NOT FOUND"));
}

void UPuzzelPlatformsGameInstance::StartSession()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->StartSession(SESSION_NAME);
	}
}

void UPuzzelPlatformsGameInstance::LoadMenuWidget()
{
	if (ensure(MainMenuClass != NULL))
	{
		MainMenuWidget = CreateWidget<UMainMenu>(this, MainMenuClass);
		MainMenuWidget->SetMenuInterface(this);
		MainMenuWidget->Setup();
	}
}

void UPuzzelPlatformsGameInstance::LoadInGameMenu()
{
	if (ensure(InGameMenuClass != NULL))
	{
		UMenuWidget* InGameMenuWidget = CreateWidget<UMenuWidget>(this, InGameMenuClass);
		InGameMenuWidget->SetMenuInterface(this);
		InGameMenuWidget->Setup();
		//UUserWidget* InGameMenuWidget = CreateWidget<UUserWidget>(this, InGameMenuClass);
		//InGameMenuWidget->SetMenuInterface(this);
		//InGameMenuWidget->Setup();
	}
}

void UPuzzelPlatformsGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			SessionSettings.bIsLANMatch = true;
		}
		else
		{
			SessionSettings.bIsLANMatch = false;
		}
		
		SessionSettings.NumPublicConnections = 5;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bAllowJoinInProgress = false;
		SessionSettings.bUsesPresence = true;
		SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UPuzzelPlatformsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (Result == EOnJoinSessionCompleteResult::Success && SessionInterface.IsValid())
	{
		FString ConnectionInfo;
		if (SessionInterface->GetResolvedConnectString(SessionName, ConnectionInfo))
		{
			UEngine* Engine = GetEngine();
			if (!ensure(Engine)) { return; }

			FString OutputMsg = FString::Printf(TEXT("Joining %s"), *ConnectionInfo);
			Engine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, OutputMsg);


			APlayerController* PlayerController = GetFirstLocalPlayerController();
			if (!ensure(PlayerController != nullptr)) { return; }

			PlayerController->ClientTravel(ConnectionInfo, ETravelType::TRAVEL_Absolute);
		}

	}
}

void UPuzzelPlatformsGameInstance::Host(FString ServerName)
{
	DesiredServerName = ServerName;
	if (SessionInterface.IsValid())
	{
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
}

void UPuzzelPlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		CreateSession();
	}
}

void UPuzzelPlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		if (MainMenuWidget != nullptr)
		{
			MainMenuWidget->TearDown();
		}

		UEngine* Engine = GetEngine();
		if (!ensure(Engine != nullptr)) { return; }
		Engine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Hosting"));

		UWorld * World = GetWorld();
		if (!ensure(World != nullptr)) { return; }

		FString InURL = FString::Printf(TEXT("/Game/PuzzelPlatforms/Maps/Lobby?listen"));
		World->ServerTravel(InURL);
	}
}

void UPuzzelPlatformsGameInstance::RefreshServerList()
{
	UE_LOG(LogTemp, Warning, TEXT("Starting Find Session"));
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		SessionSearch->MaxSearchResults = 100;
		//SessionSearch->bIsLanQuery = false;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UPuzzelPlatformsGameInstance::OnFindSessionComplete(bool Success)
{
	if (Success && SessionSearch.IsValid() && MainMenuWidget != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Sessions Found"));
		TArray<FServerData> ServerDatas;
		TArray<FOnlineSessionSearchResult>& SearchResults = SessionSearch->SearchResults;
		for (FOnlineSessionSearchResult& SearchResult : SearchResults)
		{
			FServerData ServerData;
			FString ServerName;

			if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
				ServerData.Name = ServerName;
			else
				ServerData.Name = TEXT("Server Name Not Found!");
			
			ServerData.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
			ServerData.CurrentPlayers = (ServerData.MaxPlayers - SearchResult.Session.NumOpenPublicConnections);
			ServerData.HostUsername = SearchResult.Session.OwningUserName;
			ServerDatas.Add(ServerData);
		}

		MainMenuWidget->SetServerList(ServerDatas);
	}
}

void UPuzzelPlatformsGameInstance::Join(uint8 index)
{
	if (!SessionInterface.IsValid()) { return; }
	if (!SessionSearch.IsValid()) { return; }

	if (MainMenuWidget != nullptr)
	{
		MainMenuWidget->TearDown();
	}

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[index]);
}

void UPuzzelPlatformsGameInstance::QuitToMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) { return; }

	PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}
