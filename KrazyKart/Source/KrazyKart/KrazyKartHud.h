// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/HUD.h"
#include "KrazyKartHud.generated.h"


UCLASS(config = Game)
class AKrazyKartHud : public AHUD
{
	GENERATED_BODY()

public:
	AKrazyKartHud();

	/** Font used to render the vehicle info */
	UPROPERTY()
	UFont* HUDFont;

	// Begin AHUD interface
	virtual void DrawHUD() override;
	// End AHUD interface
};
