// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GoKart.generated.h"

USTRUCT()
struct FGoKartMove
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		float Throttle;

	UPROPERTY()
		float SteeringThrow;

	UPROPERTY()
		float DeltaTime;

	UPROPERTY()
		float Time;	
};

USTRUCT()
struct FGoKartState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		FTransform Transform;

	UPROPERTY()
		FVector Velocity;

	UPROPERTY()
		FGoKartMove LastMove;
};


UCLASS()
class KRAZYKART_API AGoKart : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGoKart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ApplyRotation(float DeltaTime, float SteeringThrow);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void SimulateMove(FGoKartMove Move);

	FGoKartMove CreateMove(float DeltaTime);
	void ClearUnacknowledgedMoves(FGoKartMove LastMove);

	void MoveForward(float Val);
	void MoveRight(float Val);

	UPROPERTY(EditAnywhere)
		float Mass = 1000.f;

	UPROPERTY(EditAnywhere)
		float MaxDrivingForce = 10000.f;

	UPROPERTY(EditAnywhere)
		float DragCoefficiant = 16.f;

	//highers means more rolling resistance
	UPROPERTY(EditAnywhere)
		float RollingDragCoefficiant = 0.015f;

	UPROPERTY(EditAnywhere)
		float TurningRadius = 11.f;


	void UpdateLocationFromVelocity(float DeltaTime);
	FVector GetAirResistance();
	FVector GetRollingResistance();

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SendMove(FGoKartMove Move);

	float Throttle;
	float SteeringThrow;

	FString GetRoleEnumString(ENetRole Role);

	FVector Velocity;

	UFUNCTION()
		void OnRep_ServerState();

	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
		FGoKartState ServerState;

	TArray<FGoKartMove> UnacknowledgedMoves;
};
