// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonMaster_INT_Towers.h"
#include "DungeonMaster_PlayerController.h"
#include "DungeonMaster_Enemy_BASE.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "Components/StaticMeshComponent.h"

ADungeonMaster_INT_Towers::ADungeonMaster_INT_Towers()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	RootComponent = boxCollision;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	Mesh->bReceivesDecals = false;
	//Mesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	Mesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

}

// Called when the game starts or when spawned
void ADungeonMaster_INT_Towers::BeginPlay()
{
	Super::BeginPlay();

	OnClicked.AddDynamic(this, &ADungeonMaster_INT_Towers::OnClick);
	Attacking.AddDynamic(this, &ADungeonMaster_INT_Towers::TowerAttackUnit);

	AttackRange->OnComponentBeginOverlap.AddDynamic(this, &ADungeonMaster_INT_Towers::AttackRangeCheckIn);
	AttackRange->OnComponentEndOverlap.AddDynamic(this, &ADungeonMaster_INT_Towers::AttackRangeCheckOut);
}

// Called every frame
void ADungeonMaster_INT_Towers::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (bIsActive == false)
	{
		currLocation = PC->FindAttachmentPosition(GetActorLocation());
		FVector newLocation = PC->FindAttachmentPosition(PC->GetMouseWorldLocation());

		if (currLocation != newLocation)
		{
			SetActorLocation(newLocation);
			currLocation = newLocation;
		}
	}

	AttackTimer = TimerFunction(AttackTimer, AttackFinalTimer);



}

void ADungeonMaster_INT_Towers::OnClick(AActor* TouchedActor, FKey ButtonPressed)
{
	if (TouchedActor == Cast<ADungeonMaster_INT_Towers>(this))
	{
		if (ButtonPressed.GetFName() == "LeftMouseButton") // Interact
		{
			if (bIsActive == false)
			{
				PC->SetTileAttachment();
			}
			else if (bIsActive == true && PC->isBuilding == false) // select unit 
			{

			}
		}
	}
}

void ADungeonMaster_INT_Towers::AttackRangeCheckIn(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ADungeonMaster_Enemy_BASE* tUnit = Cast<ADungeonMaster_Enemy_BASE>(OtherActor);
		if (tUnit)
		{
			InRange.AddUnique(tUnit);
			UE_LOG(LogTemp, Warning, TEXT("AttackRange ADDUNIT"));
			Attacking.Broadcast();
		}
	}
}

void ADungeonMaster_INT_Towers::AttackRangeCheckOut(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (InRange.Num() > 0)
	{
		if (OtherActor)
		{
			ADungeonMaster_Enemy_BASE* tUnit = Cast<ADungeonMaster_Enemy_BASE>(OtherActor);
			if (tUnit)
			{
				InRange.RemoveSingle(tUnit);
				UE_LOG(LogTemp, Warning, TEXT("AttackRange REMOVEUNIT"));
				Attacking.Broadcast();
			}

		}
	}
}

float ADungeonMaster_INT_Towers::TimerFunction(float SetTime, float DesiredTime)
{
	float CurrentTime = GetWorld()->GetRealTimeSeconds();

	if (SetTime < DesiredTime)
		SetTime += (CurrentTime - SetTime);

	return SetTime;
}

void ADungeonMaster_INT_Towers::TowerAttackUnit() 
{
	if (InRange.Num() > 0) 
	{
		if (AttackTimer >= AttackFinalTimer) 
		{
			DealDamageToEnemy(InRange[0]);
			Attacking.Broadcast();
		}
		else 
		{
			Attacking.Broadcast();
		}
	}
}

void ADungeonMaster_INT_Towers::DealDamageToEnemy(AActor* Obj)
{
	if (Obj) 
	{
		ADungeonMaster_Enemy_BASE* tUnit = Cast<ADungeonMaster_Enemy_BASE>(Obj);
		if (tUnit)
		{
			//tUnit->currHealth -= this->Damage;
			AttackTimer = GetWorld()->GetRealTimeSeconds();
			AttackFinalTimer = (AttackTimer + AttackCD);
		}
	}
}