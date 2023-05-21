// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonMaster_Enemy_AIController.h"
#include "DungeonMaster_PlayerController.h"
#include "DungeonMaster_GameManager.h"
#include "DungeonMaster_Enemy_BASE.h"
#include "Navigation/PathFollowingComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


void ADungeonMaster_Enemy_AIController::BeginPlay()
{
	Super::BeginPlay();

	CallForGM.AddDynamic(this, &ADungeonMaster_Enemy_AIController::GetGM);
	CallForGM.Broadcast();

	//MoveToSpecifiedLocation(Destination);

	//navArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
}

void ADungeonMaster_Enemy_AIController::MoveToSpecifiedLocation(FVector location)
{
	UE_LOG(LogTemp, Warning, TEXT("Moving Unit Called"));
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Destination.ToString());
	this->MoveToLocation(location, 5.f, true);
}

void ADungeonMaster_Enemy_AIController::StopUnitMovement()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop Moving called"));
	this->StopMovement();
}

void ADungeonMaster_Enemy_AIController::CallDeath()
{
	GameManagerRef->EnemiesNum -= 1;

	if (GameManagerRef->EnemiesNum == 0) 
	{
		GameManagerRef->WaveNumber++;
		PC->ManageMoney(100.f);
		PC->NotifyList.Broadcast("Round ended!");
	}
}

void ADungeonMaster_Enemy_AIController::GetGM()
{
	while (GameManagerRef == nullptr)
	{
		GameManagerRef = Cast<ADungeonMaster_GameManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADungeonMaster_GameManager::StaticClass()));

		if (GameManagerRef)
			Destination = GameManagerRef->NexusLocation;
	}

	while (PC == nullptr)
	{
		PC = Cast<ADungeonMaster_PlayerController>(UGameplayStatics::GetActorOfClass(GetWorld(), ADungeonMaster_PlayerController::StaticClass()));
	}

}
