// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonMaster_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DungeonMaster_GameManager.h"
#include "DungeonMaster_Tiles_BASE.h"
#include "DungeonMaster_Camera.h"

ADungeonMaster_PlayerController::ADungeonMaster_PlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	CallForCamera.AddDynamic(this, &ADungeonMaster_PlayerController::GetPlayerCamera);
	CallForGM.AddDynamic(this, &ADungeonMaster_PlayerController::GetGameManager);
}

void ADungeonMaster_PlayerController::Tick(float deltaTime) 
{
	Super::Tick(deltaTime);
}

void ADungeonMaster_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	this->TilePhaseUI();
	CallForCamera.Broadcast();
	CallForGM.Broadcast();
}

/*void ADungeonMaster_PlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

}*/

void ADungeonMaster_PlayerController::SpawnTileActor(UClass* Build) 
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (CurrentTileActor)
		CurrentTileActor->Destroy();

	FVector actorSpawnLocation = Hit.ImpactPoint;
	FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);
	FActorSpawnParameters Spawn;
	Spawn.Owner = this;
	Spawn.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	CurrentTileActor = GetWorld()->SpawnActor(Build, &actorSpawnLocation, &SpawnRotation, Spawn);

	ADungeonMaster_Tiles_BASE* currentTile = Cast<ADungeonMaster_Tiles_BASE>(CurrentTileActor);
	if (currentTile)
	{
		currentTile->SetActorLocation(FindTilePosition(GetMouseWorldLocation()));
	}
}

FVector ADungeonMaster_PlayerController::FindTilePosition(FVector position)
{
	return GameManagerRef->findSpecificTile(position).midPoint;
}

FVector ADungeonMaster_PlayerController::GetMouseWorldLocation()
{
	FVector retVal;

	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	retVal = Hit.ImpactPoint;
	return retVal;

}

void ADungeonMaster_PlayerController::GetPlayerCamera() 
{
	while (mainCamera == nullptr) 
	{
		mainCamera = Cast<ADungeonMaster_Camera>(UGameplayStatics::GetActorOfClass(GetWorld(), ADungeonMaster_Camera::StaticClass()));
		SetViewTargetWithBlend(mainCamera);
	}
}

void ADungeonMaster_PlayerController::GetGameManager()
{
	while (GameManagerRef == nullptr)
	{
		GameManagerRef = Cast<ADungeonMaster_GameManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADungeonMaster_GameManager::StaticClass()));
	}
}

void ADungeonMaster_PlayerController::SetCurrentTile()
{
	ADungeonMaster_Tiles_BASE* newTile = Cast<ADungeonMaster_Tiles_BASE>(CurrentTileActor);
	if (newTile) 
	{
		GameManagerRef->changeCellTile(CurrentTileActor->GetActorLocation(), newTile);
		CurrentTileActor = nullptr;
	}
}