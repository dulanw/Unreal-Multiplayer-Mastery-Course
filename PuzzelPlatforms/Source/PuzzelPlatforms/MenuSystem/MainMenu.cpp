// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"


bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (Success)
	{
		HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
		return true;
	}
	else
		return false;
}

void UMainMenu::HostServer()
{
	UE_LOG(LogTemp, Warning, TEXT("Starting Server!"));
}