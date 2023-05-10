// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonMaster_Tiles_BASE.h"
#include "DungeonMaster_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/BoxComponent.h"

// Sets default values
ADungeonMaster_Tiles_BASE::ADungeonMaster_Tiles_BASE()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* Events/Functionality */
	OnClicked.AddDynamic(this, &ADungeonMaster_Tiles_BASE::OnClick);
	CallForPC.AddDynamic(this, &ADungeonMaster_Tiles_BASE::GetPlayerController);

	boxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision Component");
	RootComponent = boxCollision;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	Mesh->bReceivesDecals = false;
	//Mesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	Mesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

}

// Called when the game starts or when spawned
void ADungeonMaster_Tiles_BASE::BeginPlay()
{
	Super::BeginPlay();

	CallForPC.Broadcast();
	
}

// Called every frame
void ADungeonMaster_Tiles_BASE::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsActive == false) 
	{
		currLocation = PC->FindTilePosition(GetActorLocation());
		FVector newLocation = PC->FindTilePosition(PC->GetMouseWorldLocation());

		if (currLocation != newLocation) 
		{
			SetActorLocation(newLocation);
			currLocation = newLocation;
		}
	}

}

void ADungeonMaster_Tiles_BASE::OnClick(AActor* TouchedActor, FKey ButtonPressed)
{
	if (TouchedActor == Cast<ADungeonMaster_Tiles_BASE>(this))
	{
		if (ButtonPressed.GetFName() == "LeftMouseButton") // Interact
		{
			if (bIsActive == false) 
			{
				
				bIsActive = true;
				PC->SetCurrentTile();
				// check if we have any amount of that tile type left
				// check if theres another tile on this grid cell
				// 
			}

		}
	}
}

void ADungeonMaster_Tiles_BASE::GetPlayerController() 
{
	PC = Cast<ADungeonMaster_PlayerController>(UGameplayStatics::GetActorOfClass(GetWorld(), ADungeonMaster_PlayerController::StaticClass()));
}