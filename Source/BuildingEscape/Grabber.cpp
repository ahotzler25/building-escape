// Copyright: Andy Hotzler, 2021

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h" // This should be bottom #include at all times (why?)

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  
	// You can turn these features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
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

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	// Without == nullptr, possibility for UE4 to crash due to PhysicsHandle equaling a null pointer
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No physics handle component found on %s."), *GetOwner()->GetActorLabel());
	}
}

void UGrabber::Grab()
{
	// When right-mouse button is pressed, this function is called
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UE_LOG(LogTemp, Warning, TEXT("Grab button was pressed."));

	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	// Attempt to grab any actors with a physics body collision channel set
	if (HitResult.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			GetPlayerReach()
		);
	}
}

void UGrabber::Release()
{
	// remove/release physics handle
	PhysicsHandle->ReleaseComponent();
	
	UE_LOG(LogTemp, Warning, TEXT("Grab button was released."));
}

// Called every frame
// This is a 'hot loop'
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// If (physicshandle is attached)
	if (PhysicsHandle->GrabbedComponent)
	{
		// Move the object we are holding
		PhysicsHandle->SetTargetLocation(GetPlayerReach());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FHitResult Hit;
	
	// Ray-cast out to a certain distance
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		Hit,
		GetPlayerPosition(),
		GetPlayerReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	// DEBUG: Log out to test
	AActor* ActorHit = Hit.GetActor();

	if (ActorHit) UE_LOG(LogTemp, Warning, TEXT("Object %s was hit!"), *ActorHit->GetActorLabel());

	return Hit;
}

FVector UGrabber::GetPlayerPosition() const
{
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpointLocation,
		OUT PlayerViewpointRotation
	);

	return PlayerViewpointLocation;
}

FVector UGrabber::GetPlayerReach() const
{
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpointLocation, 
		OUT PlayerViewpointRotation
	);

	return PlayerViewpointLocation + PlayerViewpointRotation.Vector() * Reach;
}