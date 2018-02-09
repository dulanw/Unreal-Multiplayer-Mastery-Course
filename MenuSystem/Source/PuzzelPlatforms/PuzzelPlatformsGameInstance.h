// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
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

private:
	UFUNCTION(BlueprintCallable)
		void LoadMenu();

	UFUNCTION(BlueprintCallable)
		void LoadInGameMenu();

	UFUNCTION(Exec)
		virtual void Host() override;

	UFUNCTION(Exec)
		 virtual void Join(const FString& Address) override;

	virtual void QuitToMainMenu() override;

	TSubclassOf<class UUserWidget> MainMenuClass;
	class UMenuWidget* MainMenuWidget = nullptr;

	TSubclassOf<class UUserWidget> InGameMenuClass;
	class UMenuWidget* InGameMenuWidget = nullptr;
};
