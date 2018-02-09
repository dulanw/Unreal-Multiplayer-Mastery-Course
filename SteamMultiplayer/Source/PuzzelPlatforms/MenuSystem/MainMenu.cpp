// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "MenuInterface.h"
#include "ServerRow.h"
#include "Components/TextBlock.h"

UMainMenu::UMainMenu(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));

	if (ensure(ServerRowBPClass.Class != NULL))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Class found: %s"), *PlatformTriggerBPClass.Class->GetName());
		ServerRowClass = ServerRowBPClass.Class;
	}
}

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (Success)
	{
		HostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);
		JoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
		BackToMainButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);
		BackToMainButton2->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);
		JoinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
		QuitGameButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);
		HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
		return true;
	}
	else
		return false;
}

void UMainMenu::HostServer()
{
	if (MenuInterface)
	{
		FString ServerName = ServerHostNameText->GetText().ToString();
		MenuInterface->Host(ServerName);
	}
}

void UMainMenu::JoinServer()
{
	if (SelectedIndex.IsSet())
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Index: %i"), SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Please Select a Server"));
	}

	if (MenuInterface && SelectedIndex.IsSet())
	{
		MenuInterface->Join(SelectedIndex.GetValue());
	}
}

void UMainMenu::SelectIndex(uint8 Index)
{
	SelectedIndex = Index;
}

void UMainMenu::SetServerList(TArray<FServerData> ServerDatas)
{
	if (ensure(ServerRowClass != NULL))
	{
		UWorld* World = GetWorld();
		if (!ensure(World)) { return; }

		ServerList->ClearChildren();

		uint8 index = 0;
		for (FServerData& ServerData : ServerDatas)
		{
			UServerRow* ServerRow = CreateWidget<UServerRow>(World, ServerRowClass);
			ServerRow->Setup(this, index);
			ServerRow->ServerName->SetText(FText::FromString(ServerData.Name));
			ServerRow->UserName->SetText(FText::FromString(ServerData.HostUsername));
			FString ConnectionFractionTEXT = FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayers);
			ServerRow->ConnectionFraction->SetText(FText::FromString(ConnectionFractionTEXT));
			index++;

			ServerList->AddChild(ServerRow);
		}
	}
}

void UMainMenu::OpenJoinMenu()
{
	MenuSwitcher->SetActiveWidget(JoinMenu);
	MenuInterface->RefreshServerList();
}

void UMainMenu::OpenHostMenu()
{
	MenuSwitcher->SetActiveWidget(HostMenu);
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

void UMainMenu::UpdateChildren()
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); i++)
	{
		UServerRow* ServerRow = Cast<UServerRow>(ServerList->GetChildAt(i));
		if (ServerRow)
		{
			if (SelectedIndex.IsSet() && SelectedIndex == i)
				ServerRow->SetColorAndOpacity(1, 0, 0, 1);
			
			else
				ServerRow->SetColorAndOpacity(1, 1, 1, 1);
			
		}
	}
}