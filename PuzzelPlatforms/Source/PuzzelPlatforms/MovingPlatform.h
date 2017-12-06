// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class PUZZELPLATFORMS_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()
		
public:
	AMovingPlatform();
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
		
	UPROPERTY(EditAnywhere)
		float MoveSpeed = 50.f;

	//use TargetLocationBox to set the location 
	//UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
	//	FVector TargetLocation;
	void AddActiveTrigger();
	void RemoveActiveTrigger();

private:

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* TargetLocationBox;

	FVector GlobalTargetLocation;
	FVector GlobalStartLocation;

	UPROPERTY(EditAnywhere)
		int8 ActiveTriggers = 0;

	UPROPERTY(EditAnywhere)
		int8 MinimumTriggers = 0;
};
