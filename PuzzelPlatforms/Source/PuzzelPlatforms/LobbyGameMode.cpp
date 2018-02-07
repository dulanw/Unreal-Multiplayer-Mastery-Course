// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"
#include "TimerManager.h"
#include "PuzzelPlatformsGameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
	NumberOfPlayers++;

	if (NumberOfPlayers >= 3)
	{
		GetWorldTimerManager().SetTimer(StartSessionTimer, this, &ALobbyGameMode::StartSession, 10, false);
	}
}

void ALobbyGameMode::Logout(AController * Exiting)
{
	Super::Logout(Exiting);
	NumberOfPlayers--;
}

void ALobbyGameMode::StartSession()
{
	UPuzzelPlatformsGameInstance* GameInstance = Cast<UPuzzelPlatformsGameInstance>(GetGameInstance());
	if(!ensure(GameInstance != nullptr)) { return; }

	UWorld * World = GetWorld();
	if (!ensure(World != nullptr)) { return; }

	GameInstance->StartSession();

	bUseSeamlessTravel = true;

	FString InURL = FString::Printf(TEXT("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen"));
	World->ServerTravel(InURL);
}

