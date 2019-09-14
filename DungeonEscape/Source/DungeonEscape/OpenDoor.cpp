// Fill out your copyright notice in the Description page of Project Settings.
#include "OpenDoor.h" // Keep First
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"			// For Trigger volumes(pressure plate)
#include "DungeonEscape.h"
#include "Engine/World.h"					// For getworld
#include "GameFramework/PlayerController.h" // For getplayercontroller



// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// Assign the player's pawn to the actor that opens
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	// The specific door
	Owner = GetOwner(); 
	
}

// Opens the door
void UOpenDoor::OpenDoor(void)
{
	Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
}

// Opens the door
void UOpenDoor::CloseDoor(void)
{
	Owner->SetActorRotation(FRotator(0.f, CloseAngle, 0.f));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get game time
	float CurrentTime = GetWorld()->GetTimeSeconds();

	// If a actor is inside the pressure plate
	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor();
		LastOpenDoorTime = GetWorld()->GetTimeSeconds();
	}

	// Check if it's time to close the door
	if ((CurrentTime - LastOpenDoorTime) >= DoorCloseDelay)
	{
		CloseDoor();
	}
}

