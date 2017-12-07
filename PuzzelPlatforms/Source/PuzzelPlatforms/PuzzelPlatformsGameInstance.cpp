// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzelPlatformsGameInstance.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "PlatformTrigger.h"
#include "Blueprint/UserWidget.h"

UPuzzelPlatformsGameInstance::UPuzzelPlatformsGameInstance(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));

	if (ensure(MainMenuBPClass.Class != NULL))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Class found: %s"), *PlatformTriggerBPClass.Class->GetName());
		MainMenuClass = MainMenuBPClass.Class;
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
		UUserWidget* MenuWidget = CreateWidget<UUserWidget>(this, MainMenuClass);
		MenuWidget->AddToViewport();

		APlayerController* PlayerController = GetFirstLocalPlayerController();
		if (ensure(PlayerController != nullptr))
		{
			FInputModeUIOnly InputModeUIOnly;
			PlayerController->SetInputMode(InputModeUIOnly);
			PlayerController->bShowMouseCursor = true;
		}
	}
}

void UPuzzelPlatformsGameInstance::Host()
{
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
	UEngine* Engine = GetEngine();
	if (!ensure(Engine)) { return; }

	FString OutputMsg = FString::Printf(TEXT("Joining %s"), *Address);
	Engine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, OutputMsg);

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) { return; }

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}