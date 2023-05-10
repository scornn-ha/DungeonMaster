// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonMaster_GameManager.generated.h"

USTRUCT(BlueprintType)
struct FGridCell
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector midPoint;

	bool bMoveable = false;
	class ADungeonMaster_Tiles_BASE* TilePiece;

	bool operator==(const FGridCell& other) const
	{

		return (other.midPoint == midPoint);

	}

	bool operator!=(const FGridCell& other) const
	{

		return (other.midPoint != midPoint);

	}

	FGridCell()
	{
		midPoint = FVector(0.f, 0.f, 0.f);
	}

};

UCLASS()
class DUNGEONMASTER_API ADungeonMaster_GameManager : public AActor
{
	GENERATED_BODY()
	
public:	//Functions
	// Sets default values for this actor's properties
	ADungeonMaster_GameManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FVector neBox = FVector(2500.f, -2500.f, 0.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FVector swBox = FVector(-2500.f, 2500.f, 0.f);

	UFUNCTION()
	FGridCell findSpecificTile(FVector startPos);
	UFUNCTION()
	void changeCellTile(FVector MidPoint, class ADungeonMaster_Tiles_BASE* newTile);


public:	// variables


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(VisibleAnywhere, Category = "Grid")
	TArray<FGridCell> Grid;

	float rowDiff;
	float columnDiff;
	float rowNumber;
	float columnNumber;

	const float cellSize = 500.f;

	UFUNCTION()
	void createGrid();
	UFUNCTION()
	void fillDownColumns(FVector startCell);
	UFUNCTION()
	float checkBoundingEdges(float dx, float fx);

	

};
