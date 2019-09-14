// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h" // First

#include "Engine/World.h"					// For getworld
#include "GameFramework/PlayerController.h" // For getplayercontroller
#include "DrawDebugHelpers.h"				// For debug line
#include "Components/PrimitiveComponent.h"	// For Primitive Components

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	SearchForPhysicsHandle();
	BindInputComponent();

}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PlayerViewPointLocation;											// Player Location on map
	FVector LineTraceEnd;														// End of raycast line
	FRotator PlayerViewPointRotation;											// Player rotation in reference to map
	FHitResult Hit;																// Struct containing information about hit of a line trace
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());	// Setup query parameters for ray cast

	/// Set Loc and Rot
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	/// Draw debug laser
	LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);

	PrintRayCastHit(Hit);

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}



// For Debug Only
void UGrabber::PrintRayCastHit(FHitResult Hit)
{
	/// See what we hit & Log it to the screen
	AActor* ActorHit = Hit.GetActor();

	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is in reach"), *ActorHit->GetName());
	}
}



// Bind input keys for player
void UGrabber::BindInputComponent(void)
{
	/// Look for attached input component (only appears at runtime)
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		// Report status
		UE_LOG(LogTemp, Warning, TEXT("Input component found"));

		/// Bind the input key to grab and release
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No input component"));
	}
}



void UGrabber::Grab(void)
{
	UE_LOG(LogTemp, Warning, TEXT("Grabbing Object"));

	// Line trace to see if we are in reach
	FHitResult HitResult = GetFirstPhysicsBodyInReach();				// Struct for object in line trace
	AActor* ActorHit = HitResult.GetActor();							// Points to name of hit object
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();	// Points to the object we want to grab
	
	// If we hit something, attach a physics handle to the object
	if (ActorHit)
	{
		PhysicsHandle->GrabComponent(
			ComponentToGrab,								
			NAME_None,											// Skeletal Bones (Not used for this)
			ComponentToGrab->GetOwner()->GetActorLocation(),	// Location of object
			true												// Restrict rotation of object
		);
	}
	
}



void UGrabber::Release(void)
{
	UE_LOG(LogTemp, Warning, TEXT("Releasing Object"));
	PhysicsHandle->ReleaseComponent();
}


// Get info about object in reach
FHitResult UGrabber::GetFirstPhysicsBodyInReach(void)
{
	FVector PlayerViewPointLocation;											// Player Location on map
	FVector LineTraceEnd;														// End of raycast line
	FRotator PlayerViewPointRotation;											// Player rotation in reference to map
	FHitResult Hit;																// Struct containing information about hit of a line trace
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());	// Setup query parameters for ray cast

	/// Set Loc and Rot
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	/// Draw debug laser
	LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);	
	DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(255, 0, 0), false, 0, 0, 10.0f);

	/// Ray-cast out to reach distance and get the result of ray-cast and store it in the hit struct
	GetWorld()->LineTraceSingleByObjectType(OUT Hit, PlayerViewPointLocation, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters);

	return Hit;
}




void UGrabber::SearchForPhysicsHandle(void)
{
	/// Look for attached physics handle on the actor that has this component
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle)
	{
		// Physics handle is found
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName());
	}
}