// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZELPLATFORMS_API UInGameMenu : public UMenuWidget
{
	GENERATED_BODY()
protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* BackToGameButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* QuitToMainMenuButton;

	UFUNCTION()
		void QuitMatch();


	UFUNCTION()
		void CloseMenu();

};
