// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzelPlatformsGameInstance.h"
#include "Engine/Engine.h"

UPuzzelPlatformsGameInstance::UPuzzelPlatformsGameInstance(const FObjectInitializer & ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Constructor"));
}


void UPuzzelPlatformsGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Init"));
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