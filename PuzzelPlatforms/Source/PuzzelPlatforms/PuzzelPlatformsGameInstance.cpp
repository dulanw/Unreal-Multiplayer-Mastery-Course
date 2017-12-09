// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzelPlatformsGameInstance.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "PlatformTrigger.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MenuWidget.h"

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
	UE_LOG(LogTemp, Warning, TEXT("Class Initiated, found: %s"),*MainMenuClass->GetName());
}

void UPuzzelPlatformsGameInstance::LoadMenu()
{
	if (ensure(MainMenuClass != NULL))
	{
		MainMenuWidget = CreateWidget<UMenuWidget>(this, MainMenuClass);
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

void UPuzzelPlatformsGameInstance::Host()
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

	FString InURL = FString::Printf(TEXT("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen"));
	World->ServerTravel(InURL);


}

void UPuzzelPlatformsGameInstance::Join(const FString& Address)
{
	if (MainMenuWidget != nullptr)
	{
		MainMenuWidget->TearDown();
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine)) { return; }

	FString OutputMsg = FString::Printf(TEXT("Joining %s"), *Address);
	Engine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, OutputMsg);

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) { return; }

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UPuzzelPlatformsGameInstance::QuitToMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) { return; }

	PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}
