// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerRow.h"
#include "MainMenu.h"
#include "Components/Button.h"


void UServerRow::Setup(class UMainMenu* parent, uint8 index)
{
	this->parent = parent;
	this->index = index;
}

void UServerRow::SetColorAndOpacity(float InR, float InG, float InB, float InA)
{
	SelectRowButton->SetColorAndOpacity(FLinearColor(InR, InG, InB, InA));
}

bool UServerRow::Initialize()
{
	bool Success = Super::Initialize();
	if (Success)
	{
		SelectRowButton->OnClicked.AddDynamic(this, &UServerRow::RowSelected);
		return true;
	}
	else
		return false;
}

void UServerRow::RowSelected()
{
	if (parent != nullptr)
	{
		parent->SelectIndex(index);
		parent->UpdateChildren();
	}

}
