// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionInterface.h"
#include "MenuSystem/MenuInterface.h"
#include "PuzzelPlatformsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZELPLATFORMS_API UPuzzelPlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UPuzzelPlatformsGameInstance(const FObjectInitializer & ObjectInitializer);
	
	virtual void Init();

	void StartSession();

private:
	UFUNCTION(BlueprintCallable)
		void LoadMenuWidget();

	UFUNCTION(BlueprintCallable)
		void LoadInGameMenu();

	UFUNCTION(Exec)
		virtual void Host(FString ServerName) override;

	UFUNCTION(Exec)
		 virtual void Join(uint8 index) override;
	
	//return from the game to the main menu
	virtual void QuitToMainMenu() override;

	//refresh the server list in the main menu
	virtual void RefreshServerList() override;

	TSubclassOf<class UUserWidget> MainMenuClass;
	class UMainMenu* MainMenuWidget = nullptr;

	TSubclassOf<class UUserWidget> InGameMenuClass;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	FString DesiredServerName;

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void CreateSession();
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void OnFindSessionComplete(bool Success);
};
