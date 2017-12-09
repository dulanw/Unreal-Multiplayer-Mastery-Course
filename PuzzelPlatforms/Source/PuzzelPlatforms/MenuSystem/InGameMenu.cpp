// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMenu.h"
#include "Components/Button.h"
#include "MenuInterface.h"

bool UInGameMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (Success)
	{
		BackToGameButton->OnClicked.AddDynamic(this, &UInGameMenu::CloseMenu);
		QuitToMainMenuButton->OnClicked.AddDynamic(this, &UInGameMenu::QuitMatch);
		return true;
	}
	else
		return false;
}

void UInGameMenu::CloseMenu()
{
	TearDown();
}

void UInGameMenu::QuitMatch()
{
	if (MenuInterface != nullptr)
	{
		TearDown();
		MenuInterface->QuitToMainMenu();
	}	
}




