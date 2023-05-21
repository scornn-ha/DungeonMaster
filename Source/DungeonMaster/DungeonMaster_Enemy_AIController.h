// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DungeonMaster_Enemy_AIController.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyGetGameManager);

UCLASS()
class DUNGEONMASTER_API ADungeonMaster_Enemy_AIController : public AAIController
{
	GENERATED_BODY()

public: // functions

	UFUNCTION()
	void MoveToSpecifiedLocation(FVector location);
	UFUNCTION()
	void StopUnitMovement();
	UFUNCTION()
	void CallDeath();

public: // variables

	UPROPERTY(VisibleAnywhere, Category = "Misc")
	FVector Destination;
	UPROPERTY(VisibleAnywhere, Category = "Misc")
	class ADungeonMaster_PlayerController* PC;

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(BlueprintAssignable)
	FEnemyGetGameManager CallForGM;
	UFUNCTION()
	void GetGM();
	UPROPERTY(VisibleAnywhere, Category = "Misc")
	class ADungeonMaster_GameManager* GameManagerRef;

	//class UNavigationSystemV1* navArea;

	
};
