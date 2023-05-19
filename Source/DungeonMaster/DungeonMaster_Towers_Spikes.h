// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DungeonMaster_INT_Towers.h"
#include "DungeonMaster_Towers_Spikes.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONMASTER_API ADungeonMaster_Towers_Spikes : public ADungeonMaster_INT_Towers
{
	GENERATED_BODY()

public:

	ADungeonMaster_Towers_Spikes();

protected:

	virtual void TowerAttackUnit() override;

private:
	
};
