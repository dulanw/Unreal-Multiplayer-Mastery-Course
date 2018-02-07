// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuWidget.h"
#include "MenuInterface.h"


void UMenuWidget::Setup()
{
	this->AddToViewport();

	UWorld* World = GetWorld();
	if (!ensure(World)) { return; }

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!ensure(PlayerController)) { return; }

	FInputModeUIOnly InputModeUIOnly;
	PlayerController->SetInputMode(InputModeUIOnly);
	PlayerController->bShowMouseCursor = true;
}

void UMenuWidget::TearDown()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!ensure(PlayerController)) { return; }

	FInputModeGameOnly InputModeGameOnly;
	PlayerController->SetInputMode(InputModeGameOnly);
	PlayerController->bShowMouseCursor = false;
	this->RemoveFromViewport();
}

void UMenuWidget::SetMenuInterface(IMenuInterface * MenuInterface)
{
	this->MenuInterface = MenuInterface;
}

