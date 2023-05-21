// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DungeonMaster_Characters.h"
#include "DungeonMaster_Characters_Wizard.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONMASTER_API ADungeonMaster_Characters_Wizard : public ADungeonMaster_Characters
{
	GENERATED_BODY()

public:

	ADungeonMaster_Characters_Wizard();

protected:

	virtual void DamageFunction() override;


private:

};
