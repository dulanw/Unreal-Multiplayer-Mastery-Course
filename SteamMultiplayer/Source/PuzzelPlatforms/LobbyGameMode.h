// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzelPlatformsGameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PUZZELPLATFORMS_API ALobbyGameMode : public APuzzelPlatformsGameMode
{
	GENERATED_BODY()
public:
		virtual void PostLogin(APlayerController* NewPlayer) override;
		virtual void Logout(AController* Exiting) override;

private:
	void StartSession();

	uint32 NumberOfPlayers = 0;

	FTimerHandle StartSessionTimer;

};
