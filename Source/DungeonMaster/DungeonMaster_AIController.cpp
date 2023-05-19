// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonMaster_AIController.h"
#include "DungeonMaster_Characters.h"
#include "Navigation/PathFollowingComponent.h"

void ADungeonMaster_AIController::BeginPlay() 
{
	Super::BeginPlay();

	//navArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
}

void ADungeonMaster_AIController::MoveToSpecifiedLocation(FVector location)
{
	this->MoveToLocation(location, 10.f, true);
}

void ADungeonMaster_AIController::StopUnitMovement()
{
	this->StopMovement();
}