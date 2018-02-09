// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;
	uint16 MaxPlayers;
	uint16 CurrentPlayers;
	FString HostUsername;
};

/**
 * 
 */
UCLASS()
class PUZZELPLATFORMS_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()
public:
	UMainMenu(const FObjectInitializer & ObjectInitializer);

	void SetServerList(TArray<FServerData> ServerDatas);

	void SelectIndex(uint8 Index);

	void UpdateChildren();

protected:
	virtual bool Initialize() override;

private:
	TSubclassOf<class UUserWidget> ServerRowClass;

	UPROPERTY(meta = (BindWidget))
		class UButton* HostMenuButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* JoinMenuButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* BackToMainButton;
	
	UPROPERTY(meta = (BindWidget))
		class UButton* BackToMainButton2;

	UPROPERTY(meta = (BindWidget))
		class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* ServerHostNameText;

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
		class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
		class UWidget* HostMenu;

	UPROPERTY(meta = (BindWidget))
		class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
		class UScrollBox* ServerList;

	UPROPERTY(meta = (BindWidget))
		class UButton* QuitGameButton;
	
	UFUNCTION()
		void HostServer();

	UFUNCTION()
		void JoinServer();

	UFUNCTION()
		void QuitGame();

	UFUNCTION()
		void OpenHostMenu();

	UFUNCTION()
		void OpenJoinMenu();

	UFUNCTION()
		void OpenMainMenu();

	TOptional<uint8> SelectedIndex;
};
