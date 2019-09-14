// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "DungeonEscape.h"

// Keep Last
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void OpenDoor(void);
	void CloseDoor(void);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(VisibleAnywhere)
	float OpenAngle = -90.0f;

	UPROPERTY(VisibleAnywhere)
	float CloseAngle = 0.0f;
		
	// Allows us to add a trigger to the door
	UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlate = nullptr;

	// Makes an actor 
	AActor* ActorThatOpens; // The Actor That Can Open The Door
	AActor* Owner;			// The Current Door Instance

	// Timer for door close
	UPROPERTY(EditAnywhere)
		float DoorCloseDelay = 0.5f;

	float LastOpenDoorTime;
};
