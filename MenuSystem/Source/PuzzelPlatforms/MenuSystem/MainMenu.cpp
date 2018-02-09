// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "MenuInterface.h"

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (Success)
	{
		HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
		JoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
		BackToMainButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);
		JoinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
		QuitGameButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);
		return true;
	}
	else
		return false;
}

void UMainMenu::HostServer()
{
	if (MenuInterface)
	{
		MenuInterface->Host();
	}
}

void UMainMenu::JoinServer()
{
	if (MenuInterface)
	{
		FString Address = IPTextBox->GetText().ToString();
		if(!Address.IsEmpty())
			MenuInterface->Join(Address);
	}
}

void UMainMenu::OpenJoinMenu()
{
	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::OpenMainMenu()
{
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::QuitGame()
{
	UWorld* World = GetWorld();
	if (!ensure(World)) { return; }

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!ensure(PlayerController)) { return; }

	PlayerController->ConsoleCommand("quit");
}