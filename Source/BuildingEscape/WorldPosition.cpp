// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldPosition.h"
#include "GameFramework/Actor.h"
#include "Math/Vector.h"


// Sets default values for this component's properties
UWorldPosition::UWorldPosition()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWorldPosition::BeginPlay()
{
	Super::BeginPlay();

	// FString Log = TEXT("Testing!");
	// FString* PrtLog = &Log; // PrtLog is NOT an FString; it's a pointer to a FString

	// Use a dot operator when you're dealing with the specific variable
	// Use arrow function when dealing with a pointer
	
	// UE_LOG(LogTemp, Warning, TEXT("%s"), **PrtLog); // dereferencing and overloading the function as required

	FString ObjectName = GetOwner()->GetName();
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetName());
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *ObjectName);
	FString ObjectPosition = GetOwner()->GetActorLocation().ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s's world position is: %s"), *ObjectName, *ObjectPosition);

}


// Called every frame
void UWorldPosition::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

