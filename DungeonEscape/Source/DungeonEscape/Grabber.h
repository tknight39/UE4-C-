// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Components/ActorComponent.h"				// For Actor components
#include "PhysicsEngine/PhysicsHandleComponent.h"	// For physics 
#include "Components/InputComponent.h"				// For Input binding
#include "Grabber.generated.h"						// Keep Last


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGrabber();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	float Reach = 100.0f;	// How far away in cm player can reach	
	UPhysicsHandleComponent* PhysicsHandle = nullptr;	
	UInputComponent* InputComponent = nullptr;


private:
	void BindInputComponent(void);			// Binds the input keys for player
	void SearchForPhysicsHandle(void);		// Find out if a physics handle exists
	void PrintRayCastHit(FHitResult Hit);	// Print out if object is within reach
	void Grab(void);						// Raycast and grab whats in reach
	void Release(void);						// Release the grabbed object

	// Return hit for first physics body reach
	FHitResult GetFirstPhysicsBodyInReach(void);
	 
};
