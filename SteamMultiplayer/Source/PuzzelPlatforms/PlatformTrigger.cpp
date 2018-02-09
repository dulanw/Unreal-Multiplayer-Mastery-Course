// Fill out your copyright notice in the Description page of Project Settings.

#include "PlatformTrigger.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MovingPlatform.h"


// Sets default values
APlatformTrigger::APlatformTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Volume"));
	RootComponent = TriggerVolume;

	TriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Trigger Mesh"));
	TriggerMesh->SetupAttachment(TriggerVolume);

	TriggerVolume->SetNotifyRigidBodyCollision(true);
}

// Called when the game starts or when spawned
void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APlatformTrigger::OnOverlapBegin);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &APlatformTrigger::OnOverlapEnd);

}

// Called every frame
void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlatformTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*UE_LOG(LogTemp, Warning, TEXT("Activate!"));
	UE_LOG(LogTemp, Warning, TEXT("Activate! by component %s, actor %s"), *OtherComp->GetName(),*OtherActor->GetName());*/
	//FVector CurrentLocation = TriggerMesh->GetComponentLocation();
	//TriggerMesh->SetWorldLocation(CurrentLocation + FVector(0, 0, -8));
	for (AMovingPlatform* MovingPlatform : PlatformsToTriggers)
	{
		MovingPlatform->AddActiveTrigger();
	}
}

void APlatformTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("Deactivate!"));
	//FVector CurrentLocation = TriggerMesh->GetComponentLocation();
	//TriggerMesh->SetWorldLocation(CurrentLocation + FVector(0, 0, 8));
	for (AMovingPlatform* MovingPlatform : PlatformsToTriggers)
	{
		MovingPlatform->RemoveActiveTrigger();
	}
}
