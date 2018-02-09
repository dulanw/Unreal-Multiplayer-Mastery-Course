// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

/**
 * 
 */
UCLASS()
class PUZZELPLATFORMS_API UServerRow : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Setup(class UMainMenu* parent, uint8 index);

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ServerName;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* UserName;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ConnectionFraction;

	void SetColorAndOpacity(float InR, float InG, float InB, float InA);

protected:
	virtual bool Initialize() override;
	
private:
	UPROPERTY(meta = (BindWidget))
		class UButton* SelectRowButton;

	class UMainMenu* parent = nullptr;
	uint8 index;

	UFUNCTION()
		void RowSelected();
};
