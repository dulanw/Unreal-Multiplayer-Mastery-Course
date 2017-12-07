// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PuzzelPlatformsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZELPLATFORMS_API UPuzzelPlatformsGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPuzzelPlatformsGameInstance(const FObjectInitializer & ObjectInitializer);
	
	virtual void Init();

private:
	UFUNCTION(BlueprintCallable)
		void LoadMenu();

	UFUNCTION(Exec)
		void Host();

	UFUNCTION(Exec)
		void Join(const FString& Address);

	TSubclassOf<class UUserWidget> MainMenuClass;
};
