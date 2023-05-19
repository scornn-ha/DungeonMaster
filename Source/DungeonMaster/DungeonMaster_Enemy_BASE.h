// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonMaster_Enemy_BASE.generated.h"

UCLASS()
class DUNGEONMASTER_API ADungeonMaster_Enemy_BASE : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonMaster_Enemy_BASE();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
