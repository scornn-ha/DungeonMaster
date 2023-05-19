// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DungeonMaster_AIController.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONMASTER_API ADungeonMaster_AIController : public AAIController
{
	GENERATED_BODY()

public: // functions

	UFUNCTION()
	void MoveToSpecifiedLocation(FVector location);

	UFUNCTION()
	void StopUnitMovement();

public: // variables

protected:
	virtual void BeginPlay() override;


private: 

	//class UNavigationSystemV1* navArea;
	
};
