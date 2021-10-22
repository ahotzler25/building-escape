// Copyright: Andy Hotzler, 2021

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h" // This should be bottom #include at all times (why?)

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("Physics handle component found on %s."), *GetOwner()->GetActorLabel());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No physics handle component found on %s."), *GetOwner()->GetActorLabel());
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

// Called every frame
// This is a 'hot loop'
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UGrabber::Grab()
{
	// When E key pressed, this function is called
	GetFirstPhysicsBodyInReach();
	UE_LOG(LogTemp, Warning, TEXT("Grab button was pressed."));

	// Attempt to grab any actors with a physics body collision channel set

	// If (we hit something then attach physics handle)
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab button was released."));

	// remove/release physics handle
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	// Get player's viewpoint
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpointLocation, 
		OUT PlayerViewpointRotation
	);

	FVector LineTraceEnd = PlayerViewpointLocation + PlayerViewpointRotation.Vector() * Reach;

	FHitResult Hit;
	
	// Ray-cast out to a certain distance
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		Hit,
		PlayerViewpointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	// Log out to test
	AActor* ActorHit = Hit.GetActor();

	if (ActorHit) UE_LOG(LogTemp, Warning, TEXT("Object %s was hit!"), *ActorHit->GetName());

	return Hit;
}