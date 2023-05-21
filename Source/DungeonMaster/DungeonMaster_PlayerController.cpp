// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonMaster_PlayerController.h"
#include "DungeonMaster_Characters.h"
#include "DungeonMaster_Towers.h"
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

void ADungeonMaster_PlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("RotateTile", IE_Pressed, this, &ADungeonMaster_PlayerController::RotateTileActor);
	InputComponent->BindAction("MoveInteractable", IE_Pressed, this, &ADungeonMaster_PlayerController::MoveInteractableObject);
	InputComponent->BindAction("SelectInteractable", IE_Pressed, this, &ADungeonMaster_PlayerController::SelectInteractableObject);

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
	if (GameManagerRef)
	{
		GameManagerRef->SetNumPoints(currentPlacements);
	}
}

void ADungeonMaster_PlayerController::SetSelection(AActor* unit) 
{
	ClearSelection();
	currentSelection = unit;
	
	ADungeonMaster_Characters* tChar = Cast<ADungeonMaster_Characters>(unit);
	if (tChar) 
	{
		tChar->SetupUI();
	}

	ADungeonMaster_Towers* tTower = Cast<ADungeonMaster_Towers>(unit);
	if (tTower)
	{
		tTower->SetupUI();
	}

}

void ADungeonMaster_PlayerController::ClearSelection()
{
	if (currentSelection != nullptr) 
	{
		ADungeonMaster_Characters* tChar = Cast<ADungeonMaster_Characters>(currentSelection);
		if (tChar)
		{
			tChar->ClearUI();
			//change visibility
		}

		ADungeonMaster_Towers* tTower = Cast<ADungeonMaster_Towers>(currentSelection);
		if (tTower)
		{
			tTower->ClearUI();
		}

	}
	
	currentSelection = nullptr;
	//clear ui of unit
}

bool ADungeonMaster_PlayerController::FindSelection(AActor* unit)
{
	bool retVal = false;

	if (unit == currentSelection) 
	{
		retVal = true;
	}

	return retVal;
}

/*##########################################################################################*/
/********************************************TILES*******************************************/
/*##########################################################################################*/

void ADungeonMaster_PlayerController::SpawnCurrentActor(UClass* Build) 
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (CurrentSpawnedActor != nullptr)
		CurrentSpawnedActor->Destroy();

	FVector actorSpawnLocation = Hit.ImpactPoint;
	FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);
	FActorSpawnParameters Spawn;
	Spawn.Owner = this;
	Spawn.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	CurrentSpawnedActor = GetWorld()->SpawnActor(Build, &actorSpawnLocation, &SpawnRotation, Spawn);

	if (CurrentSpawnedActor) 
	{
		CurrentSpawnedActor->SetActorLocation(FindTilePosition(GetMouseWorldLocation()));
		isBuilding = true;
	}
}

FVector ADungeonMaster_PlayerController::FindTilePosition(FVector position)
{
	return GameManagerRef->findSpecificTile(position).midPoint;
}

FVector ADungeonMaster_PlayerController::FindAttachmentPosition(FVector position)
{
	return GameManagerRef->findAttachmentLoc(position);
}


FVector ADungeonMaster_PlayerController::GetMouseWorldLocation()
{
	FVector retVal;

	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	retVal = Hit.ImpactPoint;
	return retVal;

}

void ADungeonMaster_PlayerController::SetCurrentTile()
{
	ADungeonMaster_Tiles_BASE* newTile = Cast<ADungeonMaster_Tiles_BASE>(CurrentSpawnedActor);
	if (newTile) 
	{
		GameManagerRef->changeCellTile(CurrentSpawnedActor->GetActorLocation(), newTile);
		CurrentSpawnedActor = nullptr;
		isBuilding = false;
	}
}

bool ADungeonMaster_PlayerController::GetCurrentTile()
{
	bool retVal = false;
	retVal = GameManagerRef->isValidCellTile(CurrentSpawnedActor->GetActorLocation());

	return retVal;
}

void ADungeonMaster_PlayerController::SwitchCurrentTile()
{
	ADungeonMaster_Tiles_BASE* newTile = Cast<ADungeonMaster_Tiles_BASE>(CurrentSpawnedActor);
	if (newTile)
	{
		GameManagerRef->switchCellTile(CurrentSpawnedActor->GetActorLocation(), newTile);
		CurrentSpawnedActor = nullptr;
		isBuilding = false;
	}
}

void ADungeonMaster_PlayerController::RotateTileActor()
{
	ADungeonMaster_Tiles_BASE* currentTile = Cast<ADungeonMaster_Tiles_BASE>(CurrentSpawnedActor);
	if (currentTile)
	{
		FRotator oldRot = CurrentSpawnedActor->GetActorRotation();
		CurrentSpawnedActor->SetActorRotation(FRotator(oldRot.Pitch, oldRot.Yaw + 90.f, oldRot.Roll));
	}
}

void ADungeonMaster_PlayerController::CheckConnections()
{
	GameManagerRef->GetCurrNumPoints();
	isGameReady = false;

	if (GameManagerRef->neededConnectionPoints == GameManagerRef->currentConnectionPoints) 
	{
		isGameReady = true;
	}
}

void ADungeonMaster_PlayerController::removeCellTile(ADungeonMaster_Tiles_BASE* tile)
{
	GameManagerRef->removeTile(FindTilePosition(tile->GetActorLocation()));
	tile->Destroy();
}

void ADungeonMaster_PlayerController::NexusDeath()
{
	GameManagerRef->EndGame();
}

/*##########################################################################################*/
/*****************************************END OF TILES***************************************/
/*##########################################################################################*/

/*##########################################################################################*/
/********************************************Interactables***********************************/
/*##########################################################################################*/

bool ADungeonMaster_PlayerController::CheckMoney(int inputAmount)
{
	bool retVal = false;

	if (Money >= inputAmount) 
	{
		retVal = true;
	}

	return retVal;
}

void ADungeonMaster_PlayerController::MoveInteractableObject()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Camera, false, Hit);

	if (Hit.bBlockingHit == true) 
	{
		if (currentSelection != nullptr)
		{
			if (GameManagerRef->FindTileAttachment(FindTilePosition(Hit.ImpactPoint)) == true) // attachment of tile is empty
			{
				GameManagerRef->CharSwitchTileAttachment(FindTilePosition(currentSelection->GetActorLocation()), 
					Cast<ADungeonMaster_Characters>(currentSelection), FindTilePosition(Hit.ImpactPoint));
				OnMoveClick.Broadcast(FindTilePosition(Hit.ImpactPoint));
			}
			else  
			{
				NotifyList.Broadcast("This space is already occupied by another attachment!");
			}
		}
	}
}

void ADungeonMaster_PlayerController::SelectInteractableObject()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Camera, false, Hit);

	if (currentSelection) 
	{
		if (Hit.Actor != currentSelection)
		{
			ClearSelection();
		}
	}
}

void ADungeonMaster_PlayerController::StartGamePhase()
{
	this->GamePhaseUI();
	GameManagerRef->FillEmptyCells();
	GameManagerRef->FindNexusLocation();
	GameManagerRef->FindStartLocation();


}

void ADungeonMaster_PlayerController::SetTileAttachment() 
{
	if (CurrentSpawnedActor != nullptr) 
	{
		if (GameManagerRef->AttachToTile(CurrentSpawnedActor->GetActorLocation(), CurrentSpawnedActor) == true) // we've made the attachment complete
		{
			ADungeonMaster_Towers* newTower = Cast<ADungeonMaster_Towers>(CurrentSpawnedActor);
			if (newTower)
			{
				newTower->bIsActive = true;
				isBuilding = false;
				CurrentSpawnedActor = nullptr;
				ManageMoney(-newTower->Cost);
			}

			ADungeonMaster_Characters* newChar = Cast<ADungeonMaster_Characters>(CurrentSpawnedActor);
			if (newChar)
			{
				newChar->bIsActive = true;
				isBuilding = false;
				CurrentSpawnedActor = nullptr;
				ManageMoney(-newChar->Cost);
			}
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("UH OH"));
			NotifyList.Broadcast("Cannot place attachment here!");
		}
	}
}

/*##########################################################################################*/
/*****************************************END OF INTERACTABLES*******************************/
/*##########################################################################################*/

void ADungeonMaster_PlayerController::ManageMoney(int inputAmount) 
{
	// use negative numbers to take away money
	Money += inputAmount;
}