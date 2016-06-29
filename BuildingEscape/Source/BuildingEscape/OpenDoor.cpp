// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts,
	// and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	// ...
	
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, 
	FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll the trigger volume
	if (GetTotalMassOfActorsOnPlate() >= TriggerMass)
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	else if (GetWorld()->GetTimeSeconds() >= (LastDoorOpenTime + DoorCloseDelay))
	{
		CloseDoor();
	}
	
}

void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(FRotator(0., OpenAngle, 0.));
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0., CloseAngle, 0.));
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;
	if (PressurePlate)
	{
		TArray<AActor*> OverlappingActors;
		TArray<UPrimitiveComponent*> ActorComponents;
		PressurePlate->GetOverlappingActors(OverlappingActors);
		for (const auto& OverlappingActor : OverlappingActors)
		{
			TotalMass += OverlappingActor->
				FindComponentByClass<UPrimitiveComponent>()->GetMass();
		}
	}
	return TotalMass;
}