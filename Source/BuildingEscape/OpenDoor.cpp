// Copyright: Andy Hotzler, 2021


#include "Engine/World.h"
#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component on it, but no pressure plate"), *GetOwner()->GetName());
	}

	ActorThatOpensDoor = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	float CloseDoorTime = GetWorld()->GetTimeSeconds();

	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpensDoor))
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	} 
	else 
	{	
		
		if (CloseDoorTime - DoorLastOpened > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}

	// UE_LOG(LogTemp, Warning, TEXT("DLO: %f"), DoorLastOpened);
	// UE_LOG(LogTemp, Error, TEXT("Close Door Time: %f"), CloseDoorTime);
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	// roll = x; pitch = y; yaw = z
	// Set Actor Rotation
	CurrentYaw = FMath::Lerp(CurrentYaw, OpenAngle, DeltaTime * DoorOpenSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * DoorCloseSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}

/*
	Grabbing System

	If (player is in radius of interactable object)
	press e to grab (if not too heavy)

	if (object weighs as much as player)
	then grab actor
	walk to pressure plate
	drop on pressure plate
	door opens

	if (object does not weigh as much as player)
	door does not open
	more complicated version could show pressure plate slightly lowering, but not entirely
*/
