// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonMaster_GameManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DungeonMaster_Tiles_BASE.h"
#include "DungeonMaster_Towers.h"
#include "DungeonMaster_Characters.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADungeonMaster_GameManager::ADungeonMaster_GameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADungeonMaster_GameManager::BeginPlay()
{
	Super::BeginPlay();

	rowDiff = checkBoundingEdges(neBox.Y, swBox.Y);;
	UE_LOG(LogTemp, Warning, TEXT("ROW DIFF VALUE IS: %f"), rowDiff);
	rowNumber = rowDiff / cellSize;
	UE_LOG(LogTemp, Warning, TEXT("ROWNUMBER VALUE IS: %f"), rowNumber);

	columnDiff = checkBoundingEdges(neBox.X, swBox.X);
	UE_LOG(LogTemp, Warning, TEXT("COLUMN DIFF VALUE IS: %f"), columnDiff);
	columnNumber = columnDiff / cellSize;
	UE_LOG(LogTemp, Warning, TEXT("COLUMNNUMBER VALUE IS: %f"), columnNumber);

	createGrid();
}
// Called every frame
void ADungeonMaster_GameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADungeonMaster_GameManager::createGrid()
{
	UE_LOG(LogTemp, Warning, TEXT("STARTED CREATING GRID"));

	FVector startingPos = neBox;
	startingPos.X -= cellSize / 2;
	startingPos.Y += cellSize / 2;

	for (float i = 0; i < rowNumber + 1; i++)
	{
		fillDownColumns(startingPos);
		startingPos.Y += cellSize;

		if (startingPos.Y >= swBox.Y)
		{
			break;
		}
	}
}

void ADungeonMaster_GameManager::fillDownColumns(FVector startPos)
{
	FGridCell startCell;

	for (float i = 0; i < columnNumber + 1; i++)
	{
		startCell.midPoint.X = startPos.X;
		startCell.midPoint.Y = startPos.Y;

		//Setting collision types - add more later.
	//ProjectSettings->Engine->Collision->Object Channels
		TArray<TEnumAsByte<EObjectTypeQuery> > objectTypes;
		objectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1); //worldstatic
		objectTypes.Add(EObjectTypeQuery::ObjectTypeQuery2); //worlddynamic
		objectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3); //pawn

		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(this);

		FHitResult tHit;

		FVector endLoc = startCell.midPoint + FVector(0.0f, 0.0f, 500.f);
		//startCell.midPoint += FVector(0.0f, 0.0f, 500.f);
		FVector size = FVector(cellSize / 2.1, cellSize / 2.1, cellSize / 2.1);

		bool bIsHit = UKismetSystemLibrary::BoxTraceSingleForObjects(GetWorld(), startCell.midPoint, endLoc, size, FRotator(0.f, 0.f, 0.f),
			objectTypes, false, IgnoreActors, EDrawDebugTrace::Persistent, tHit, true);
		UE_LOG(LogTemp, Warning, TEXT("%s"), (bIsHit ? TEXT("true") : TEXT("false")));

		Grid.Add(startCell);
		startPos.X -= cellSize;

		if (startPos.X <= swBox.X)
		{
			break;
		}
	}
}

float ADungeonMaster_GameManager::checkBoundingEdges(float dx, float fx)
{
	float diff = 0;

	if (dx > fx)
	{
		diff = dx - fx;
	}
	else if (fx > dx)
	{
		diff = fx - dx;
	}
	else if (dx == fx)
	{
		diff = dx;
	}

	return diff;
}

FGridCell ADungeonMaster_GameManager::findSpecificTile(FVector startPos)
{
	FGridCell returnCell;

	for (int i = 0; i < Grid.Num(); i++)
	{
		FVector bbNW = FVector(Grid[i].midPoint.X + cellSize / 2, Grid[i].midPoint.Y - cellSize / 2, Grid[i].midPoint.Z);
		FVector bbNE = FVector(Grid[i].midPoint.X + cellSize / 2, Grid[i].midPoint.Y + cellSize / 2, Grid[i].midPoint.Z);
		FVector bbSW = FVector(Grid[i].midPoint.X - cellSize / 2, Grid[i].midPoint.Y - cellSize / 2, Grid[i].midPoint.Z);
		FVector bbSE = FVector(Grid[i].midPoint.X + cellSize / 2, Grid[i].midPoint.Y + cellSize / 2, Grid[i].midPoint.Z);


		if (checkBoundingEdges(bbNW.X, startPos.X) <= cellSize && checkBoundingEdges(bbNW.Y, startPos.Y) <= cellSize)
		{
			if (checkBoundingEdges(bbNE.X, startPos.X) <= cellSize && checkBoundingEdges(bbNE.Y, startPos.Y) <= cellSize)
			{
				if (checkBoundingEdges(bbSW.X, startPos.X) <= cellSize && checkBoundingEdges(bbSW.Y, startPos.Y) <= cellSize)
				{
					if (checkBoundingEdges(bbSE.X, startPos.X) <= cellSize && checkBoundingEdges(bbSE.Y, startPos.Y) <= cellSize)
					{
						//UE_LOG(LogTemp, Warning, TEXT("FOUND TILE %s"), *.midPoint.ToString());
						returnCell = Grid[i];
						break;
					}
				}
			}
		}
	}

	returnCell.midPoint.Z = 250.f;

	return returnCell;
}

void ADungeonMaster_GameManager::changeCellTile(FVector tMidPoint, ADungeonMaster_Tiles_BASE* newTile) 
{
	tMidPoint.Z = 0.f;
	for (int i = 0; i < Grid.Num(); i++) 
	{
		if (Grid[i].midPoint == tMidPoint) 
		{
			UE_LOG(LogTemp, Warning, TEXT("FOUND TILE"));
			Grid[i].TilePiece = newTile;
			Grid[i].TilePiece->GridSpace = i;
			break;
		}
	}
}

void ADungeonMaster_GameManager::switchCellTile(FVector tMidPoint, ADungeonMaster_Tiles_BASE* newTile)
{
	tMidPoint.Z = 0.f;
	for (int i = 0; i < Grid.Num(); i++)
	{
		if (Grid[i].midPoint == tMidPoint)
		{
			if (Grid[i].TilePiece != nullptr) 
			{
				Grid[i].TilePiece->UpdateTileAmount(1);
				Grid[i].TilePiece->Destroy();
				Grid[i].TilePiece = nullptr;

				Grid[i].TilePiece = newTile;
				Grid[i].TilePiece->bIsActive = true;
				Grid[i].TilePiece->UpdateTileAmount(-1);
				Grid[i].TilePiece->GridSpace = i;

				break;
			}
		}
	}
}

bool ADungeonMaster_GameManager::isValidCellTile(FVector tMidPoint)
{
	bool retVal = false;
	tMidPoint.Z = 0.f;

	for (int i = 0; i < Grid.Num(); i++)
	{
		if (Grid[i].midPoint == tMidPoint)
		{
			if (Grid[i].TilePiece != nullptr) 
			{
				retVal = true;
				break;
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Bool value is: %s"), (retVal ? TEXT("true") : TEXT("false")));
	return retVal;
}

void ADungeonMaster_GameManager::SetNumPoints(TArray<int> tileAmounts)
{
	int maxPoints = 0;

	maxPoints += (tileAmounts[0] * 2);
	maxPoints += (tileAmounts[1] * 2);
	maxPoints += (tileAmounts[2] * 4);
	maxPoints += tileAmounts[3];
	maxPoints += tileAmounts[4];

	neededConnectionPoints = maxPoints;

}

void ADungeonMaster_GameManager::GetCurrNumPoints() 
{
	currentConnectionPoints = 0;

	for (int i = 0; i < Grid.Num(); i++)
	{
		if (Grid[i].TilePiece != nullptr)
		{
			currentConnectionPoints += Grid[i].TilePiece->currConnections;
		}
	}
}

bool ADungeonMaster_GameManager::AttachToTile(FVector position, AActor* newAttachment)
{
	bool retVal = false;
	position.Z = 0.f;

	for (int i = 0; i < Grid.Num(); i++)
	{
		if (Grid[i].midPoint == position)
		{
			if (Grid[i].TilePiece != nullptr) // change this to tile type
			{
				if (Grid[i].TilePiece->Attachment == nullptr) 
				{
					bool canPlace = false;
					FString newString;
					AActor* AttachmentToSet = newAttachment;

					ADungeonMaster_Towers* tAttachment = Cast<ADungeonMaster_Towers>(newAttachment);
					if (tAttachment) 
					{
						newString = tAttachment->GetFName().ToString();
						AttachmentToSet = tAttachment;
						UE_LOG(LogTemp, Warning, TEXT("%s"), *newString);

					}

					ADungeonMaster_Characters* tCharacter = Cast<ADungeonMaster_Characters>(newAttachment);
					if (tCharacter)
					{
						newString = tCharacter->GetFName().ToString();
						AttachmentToSet = tCharacter;
						UE_LOG(LogTemp, Warning, TEXT("%s"), *newString);

					}

					for (int j = 0; j < Grid[i].TilePiece->attachmentType.Num(); j++)
					{
						if (newString.Contains(Grid[i].TilePiece->attachmentType[j]))
						{
							canPlace = true;
							break;
						}
					}

					if (canPlace == true)
					{
						Grid[i].TilePiece->SetAttachment(AttachmentToSet);
						UE_LOG(LogTemp, Warning, TEXT("SetNewAttachment"));
						retVal = true;
						break;
					}
				}
			}
		}
	}

	return retVal;
}

void ADungeonMaster_GameManager::removeTile(FVector pos) 
{
	pos.Z = 0.f;

	for (int i = 0; i < Grid.Num(); i++)
	{
		if (Grid[i].midPoint == pos)
		{
			if (Grid[i].TilePiece != nullptr)
			{
				Grid[i].TilePiece = nullptr;
				break;
			}
		}
	}
}

bool ADungeonMaster_GameManager::FindTileAttachment(FVector position)
{
	bool retVal = false;
	position.Z = 0.f;

	for (int i = 0; i < Grid.Num(); i++)
	{
		if (Grid[i].midPoint == position)
		{
			if (Grid[i].TilePiece != nullptr)
			{
				if (Grid[i].TilePiece->Attachment == nullptr)
				{
					retVal = true;
					break;
				}
			}
		}
	}
	return retVal;
}

void ADungeonMaster_GameManager::CharSwitchTileAttachment(FVector position, ADungeonMaster_Characters* newAttachment, FVector newPos)
{
	position.Z = 0.f;
	newPos.Z = 0.f;

	for (int i = 0; i < Grid.Num(); i++)
	{
		if (Grid[i].midPoint == position)
		{
			if (Grid[i].TilePiece != nullptr)
			{
				if (Grid[i].TilePiece->Attachment != nullptr) 
				{
					Grid[i].TilePiece->Attachment = nullptr;
					break;
				}
			}
		}
	}

	for (int j = 0; j < Grid.Num(); j++)
	{
		if (Grid[j].midPoint == newPos)
		{
			if (Grid[j].TilePiece != nullptr)
			{
				if (Grid[j].TilePiece->Attachment == nullptr)
				{
					Grid[j].TilePiece->SetAttachment(newAttachment);
					break;
				}
			}
		}
	}

}

void ADungeonMaster_GameManager::FillEmptyCells()
{
	for (int i = 0; i < Grid.Num(); i++)
	{
		if (Grid[i].TilePiece == nullptr) 
		{
			FVector actorSpawnLocation = Grid[i].midPoint;
			FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);
			FActorSpawnParameters Spawn;
			Spawn.Owner = this;
			Spawn.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			AActor* newTile = GetWorld()->SpawnActor(TilesToFillWith, &actorSpawnLocation, &SpawnRotation, Spawn);

			if (newTile)
			{
				ADungeonMaster_Tiles_BASE* tileUpdate = Cast<ADungeonMaster_Tiles_BASE>(newTile);
				if (tileUpdate) 
				{
					tileUpdate->bIsActive = true;
					tileUpdate->SetActorLocation(findSpecificTile(actorSpawnLocation).midPoint);
					Grid[i].TilePiece = tileUpdate;
				}
				
			}
		}
	}
}

FVector ADungeonMaster_GameManager::findAttachmentLoc(FVector startPos)
{
	FVector retVal;
	FGridCell newCell = findSpecificTile(startPos);

	ADungeonMaster_Tiles_BASE* newTile = Cast<ADungeonMaster_Tiles_BASE>(newCell.TilePiece);
	if (newTile) 
	{
		retVal = newCell.midPoint;
		retVal.Z = 0.f;
		retVal.Z = newTile->attachmentLocation.Z;
	}

	return retVal;
}

void ADungeonMaster_GameManager::FindNexusLocation() 
{
	for (int i = 0; i < Grid.Num(); i++) 
	{
		if (Grid[i].TilePiece != nullptr) 
		{
			if (Grid[i].TilePiece->GetFName().ToString().Contains("Nexus"))
			{
				NexusLocation = Grid[i].midPoint;
				NexusLocation.Z = 172.f;
				break;
			}
		}
	}
}

void ADungeonMaster_GameManager::FindStartLocation()
{
	for (int i = 0; i < Grid.Num(); i++)
	{
		if (Grid[i].TilePiece != nullptr)
		{
			if (Grid[i].TilePiece->GetFName().ToString().Contains("Start"))
			{
				StartLocation = Grid[i].midPoint;
				StartLocation.Z = 172.f;
				break;
			}
		}
	}
}

bool ADungeonMaster_GameManager::SpawnWave() 
{
	bool retVal = false;

	if (WaveSpawns.Num() > 0 && EnemiesNum == 0)
	{
		if (WaveNumber == WaveSpawns.Num()) 
		{
			if (newLevel.IsEmpty() == false)
				this->LoadNextLevel();
		}
		else 
		{
			currGruntSpawns = WaveSpawns[WaveNumber].waveSpawns[0]; // 0 - grunts
			currArcherSpawns = WaveSpawns[WaveNumber].waveSpawns[1]; // 1 - archers
			currMarauderSpawns = WaveSpawns[WaveNumber].waveSpawns[2]; // 2 - marauders

			EnemiesNum = (currGruntSpawns + currArcherSpawns + currMarauderSpawns);

			GetWorld()->GetTimerManager().SetTimer(GruntTimerHandle, this, &ADungeonMaster_GameManager::SpawnGrunts, 0.9f, true);
			GetWorld()->GetTimerManager().SetTimer(ArcherTimerHandle, this, &ADungeonMaster_GameManager::SpawnArchers, 1.5f, true);
			GetWorld()->GetTimerManager().SetTimer(MarauderTimerHandle, this, &ADungeonMaster_GameManager::SpawnMarauders, 4.f, true);
			retVal = true;
		}
	}

	return retVal;
}

void ADungeonMaster_GameManager::SpawnGrunts()
{
	if (currGruntSpawns == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(GruntTimerHandle);
	}
	else
	{
		SpawnEnemy(Grunts);
		currGruntSpawns -= 1;
	}
	
	
}

void ADungeonMaster_GameManager::SpawnArchers()
{
	if (currArcherSpawns == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(ArcherTimerHandle);
	}
	else
	{
		SpawnEnemy(Archers);
		currArcherSpawns -= 1;
	}
	
}

void ADungeonMaster_GameManager::SpawnMarauders()
{
	if (currMarauderSpawns == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(MarauderTimerHandle);
	}
	else
	{
		SpawnEnemy(Marauders);
		currMarauderSpawns -= 1;
	}
	
}

void ADungeonMaster_GameManager::SpawnEnemy(UClass* enemyType)
{
	FVector actorSpawnLocation = StartLocation;
	FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);
	FActorSpawnParameters Spawn;
	Spawn.Owner = this;
	Spawn.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	GetWorld()->SpawnActor(enemyType, &actorSpawnLocation, &SpawnRotation, Spawn);
}