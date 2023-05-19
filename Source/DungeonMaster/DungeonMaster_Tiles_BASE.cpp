// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonMaster_Tiles_BASE.h"
#include "DungeonMaster_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"
#include "DungeonMaster_Interactables.h"

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
	this->SetConnectionPoints();
	
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
				if (PC->GetCurrentTile() == false)
				{
					bIsActive = true;
					PC->SetCurrentTile();
					UpdateTileAmount(-1);
					PC->CallTileUpdates.Broadcast();
				}
			}
			else if (bIsActive == true && PC->isBuilding == true && PC->isGameReady == false)
			{
				PC->SwitchCurrentTile();
				PC->CallTileUpdates.Broadcast();
			}

		}
	}
}

void ADungeonMaster_Tiles_BASE::GetPlayerController() 
{
	while (PC == nullptr) 
	{
		PC = Cast<ADungeonMaster_PlayerController>(UGameplayStatics::GetActorOfClass(GetWorld(), ADungeonMaster_PlayerController::StaticClass()));
		PC->CallTileUpdates.AddDynamic(this, &ADungeonMaster_Tiles_BASE::UpdateConnections);
	}
		
}

void ADungeonMaster_Tiles_BASE::UpdateTileAmount(int newAmount) 
{
	switch (tileType)
	{
	case ETileType::Straight:
		PC->TilePhaseUIUpdate.Broadcast(0, newAmount);
		break;
	case ETileType::Junction:
		PC->TilePhaseUIUpdate.Broadcast(1, newAmount);
		break;
	case ETileType::Bend:
		PC->TilePhaseUIUpdate.Broadcast(2, newAmount);
		break;
	case ETileType::Start:
		PC->TilePhaseUIUpdate.Broadcast(3, newAmount);
		break;
	case ETileType::Nexus:
		PC->TilePhaseUIUpdate.Broadcast(4, newAmount);
		break;
	default:
		break;
	}
}

void ADungeonMaster_Tiles_BASE::UpdateConnections()
{
	this->currConnections = 0;

	TArray<FHitResult> tHits;
	FCollisionShape tcheckArea = FCollisionShape::MakeBox(FVector(35.f, 15.f, 100.f));
	FRotator currentRot = GetActorRotation();

	for (int i = 0; i < ConnectionPoints.Num(); i++)
	{
		FVector newRot;
		if (currentRot.Yaw > -1.f && currentRot.Yaw < 1.f)  // 0 rotations /4 
		{
			newRot = FVector(0.f, ConnectionPoints[i].Y, 0.f);
		}
		else if (currentRot.Yaw > 89.f && currentRot.Yaw < 91.f)  // 1 rotation
		{
			newRot = FVector((ConnectionPoints[i].Y * -1.f), 0.f, 0.f);
		}
		else if (currentRot.Yaw > 179.f && currentRot.Yaw < 181.f) // 2 rotations
		{
			newRot = FVector(0.f, (ConnectionPoints[i].Y * -1.f), 0.f);
		}
		else if (currentRot.Yaw > -91.f && currentRot.Yaw < -89.f) // 3 rotations
		{
			newRot = FVector(ConnectionPoints[i].Y, 0.f, 0.f);
		}

		// 0 rotations - Y axis
		// 1 rotation - x axis
		// 2 rotations - y axis but flipped
		// 3 rotations - x axis but flipped

		FVector connectPointLoc = (GetActorLocation() + (newRot * 5));
		bool isHit = GetWorld()->SweepMultiByChannel(tHits, connectPointLoc, connectPointLoc, FQuat(GetActorRotation()),
			ECC_GameTraceChannel1, tcheckArea); // connections

		DrawDebugBox(GetWorld(), connectPointLoc, FVector(35.f, 15.f, 100.f), FQuat(GetActorRotation()), FColor::Cyan, true);

		if (isHit == true)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit value is: %s"), (isHit ? TEXT("true") : TEXT("false")));
			for (int j = 0; j < tHits.Num(); j++)
			{
				FHitResult tempHit = tHits[j];
				UBoxComponent* tBox = Cast<UBoxComponent>(tempHit.Component);
				if (tBox)
				{
					if (this != tBox->GetOwner() && tBox->GetFName().ToString().Contains("ConnectionPoint")) 
					{
						UE_LOG(LogTemp, Warning, TEXT("Adding Connection"));
						this->currConnections += 1;
					}
				}
			}
		}

	}
	PC->CheckConnections();
}

void ADungeonMaster_Tiles_BASE::SetAttachment(AActor* newAttachment)
{
	if (Attachment == nullptr) 
	{
		Attachment = newAttachment;
		UE_LOG(LogTemp, Warning, TEXT("SETTILEATTACHMENT"));
	}
	else 
	{
		//notify list
	}
}