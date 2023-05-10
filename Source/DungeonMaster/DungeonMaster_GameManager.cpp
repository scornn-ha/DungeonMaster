// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonMaster_GameManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DungeonMaster_Tiles_BASE.h"
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
	startPos.Z = 125.f;

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

	returnCell.midPoint.Z = 125.f;

	return returnCell;
}

void ADungeonMaster_GameManager::changeCellTile(FVector tMidPoint, ADungeonMaster_Tiles_BASE* newTile) 
{
	for (int i = 0; i < Grid.Num(); i++) 
	{
		if (Grid[i].midPoint == tMidPoint) 
		{
			Grid[i].TilePiece = newTile;
			break;
		}
	}
}