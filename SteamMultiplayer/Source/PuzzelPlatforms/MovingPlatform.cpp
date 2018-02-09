// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);

	TargetLocationBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Target Location Box"));
	TargetLocationBox->bHiddenInGame = true;
	TargetLocationBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}

	GlobalStartLocation = GetActorLocation();
	GlobalTargetLocation = TargetLocationBox->GetComponentLocation();
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority())
	{
		if (ActiveTriggers >= MinimumTriggers)
		{
			FVector CurrentLocation = GetActorLocation();
			FVector TargetDirection = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
			SetActorLocation(CurrentLocation + TargetDirection * MoveSpeed * DeltaTime);

			float DotProduct = FVector::DotProduct(TargetDirection, (GlobalTargetLocation - CurrentLocation));

			if (DotProduct < 0.f)
			{
				auto temp = GlobalTargetLocation;
				GlobalTargetLocation = GlobalStartLocation;
				GlobalStartLocation = temp;
			}
		}
	}
}

void AMovingPlatform::AddActiveTrigger()
{
	ActiveTriggers++;
}

void AMovingPlatform::RemoveActiveTrigger()
{
	ActiveTriggers--;
}

