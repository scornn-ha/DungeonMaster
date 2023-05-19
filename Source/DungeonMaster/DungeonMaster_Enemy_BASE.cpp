// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonMaster_Enemy_BASE.h"

// Sets default values
ADungeonMaster_Enemy_BASE::ADungeonMaster_Enemy_BASE()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADungeonMaster_Enemy_BASE::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADungeonMaster_Enemy_BASE::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

