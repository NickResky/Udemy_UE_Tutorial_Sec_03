// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Runtime/Core/Public/Math/Rotator.h"
#include "Classes/Engine/World.h"
#include "Classes/GameFramework/PlayerController.h"
#include "Classes/GameFramework/Pawn.h"
#include "Classes/Components/PrimitiveComponent.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();

	if (!Owner) {
		UE_LOG(LogTemp, Error, TEXT("Owner not found on %s"), *(GetOwner()->GetName()));
	}

	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("Pressure plate not found on %s"), *(GetOwner()->GetName()));
	}
}

void UOpenDoor::OpenDoor()
{
	FRotator NewRotation = FRotator(0.f, -OpenAngle, 0.f);
	Owner->SetActorRotation(NewRotation);
}

void UOpenDoor::CloseDoor()
{
	FRotator NewRotation = FRotator(0.f, 0.f, 0.f);
	Owner->SetActorRotation(NewRotation);
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) {
		return 0.f;
	}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	float TotalMass = 0.f;
	for (const auto& Actor : OverlappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on plate"), *Actor->GetName());
	}
	return TotalMass;
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Owner) {
		return;
	}

	if (GetTotalMassOfActorsOnPlate() >= 50.f) {
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay) {
		CloseDoor();
	}

}

