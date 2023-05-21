// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonMaster_Enemy_BASE.h"
#include "DungeonMaster_Enemy_AIController.h"
#include "DungeonMaster_PlayerController.h"
#include "DungeonMaster_Characters.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"

// Sets default values
ADungeonMaster_Enemy_BASE::ADungeonMaster_Enemy_BASE()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttackRange = CreateDefaultSubobject<USphereComponent>("AttackRange");
	AttackRange->SetupAttachment(RootComponent);
	AttackRange->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	AttackRange->SetSphereRadius(175.f);

}

// Called when the game starts or when spawned
void ADungeonMaster_Enemy_BASE::BeginPlay()
{
	Super::BeginPlay();
	currHealth = maxHealth;

	AttackRange->OnComponentBeginOverlap.AddDynamic(this, &ADungeonMaster_Enemy_BASE::AttackRangeCheckIn);
	AttackRange->OnComponentEndOverlap.AddDynamic(this, &ADungeonMaster_Enemy_BASE::AttackRangeCheckOut);
	
}

// Called every frame
void ADungeonMaster_Enemy_BASE::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AIC == nullptr) // couldnt get delegate to work, think has something to do with spawn handling?
	{
		AIC = Cast<ADungeonMaster_Enemy_AIController>(GetController());

		if (AIC) 
		{
			UE_LOG(LogTemp, Warning, TEXT("GOT AIC"));
			AIC->MoveToSpecifiedLocation(AIC->Destination);
			bIsMoving = true;
		}
	}

	if (currHealth <= 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("0 Health"));
		if (this->IsPendingKill() == false)
		{
			if (this)
				AIC->PC->ManageMoney(this->DeathMoney);
				AIC->CallDeath();
				this->Destroy();
		}
	}

}

void ADungeonMaster_Enemy_BASE::AttackRangeCheckIn(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ADungeonMaster_Characters* tUnit = Cast<ADungeonMaster_Characters>(OtherActor);
		if (tUnit)
		{
			if (tUnit->bIsActive == true) 
			{
				InRange.AddUnique(tUnit);
				UE_LOG(LogTemp, Warning, TEXT("AttackRange ADDUNIT"));
				StartAttack();
			}
		}

		ADungeonMaster_Tiles_BASE* tTile = Cast<ADungeonMaster_Tiles_BASE>(OtherActor);
		if (tTile) 
		{
			if (tTile->tileType == ETileType::Nexus) 
			{
				InRange.AddUnique(tTile);
				UE_LOG(LogTemp, Warning, TEXT("AttackRange ADDUNIT"));
				StartAttack();
			}
		}
	}
}

void ADungeonMaster_Enemy_BASE::AttackRangeCheckOut(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (InRange.Num() > 0)
	{
		if (OtherActor)
		{
			ADungeonMaster_Characters* tUnit = Cast<ADungeonMaster_Characters>(OtherActor);
			if (tUnit)
			{
				InRange.RemoveSingle(tUnit);
				UE_LOG(LogTemp, Warning, TEXT("AttackRange REMOVEUNIT"));
			}

			ADungeonMaster_Tiles_BASE* tTile = Cast<ADungeonMaster_Tiles_BASE>(OtherActor);
			if (tTile)
			{
				if (tTile->tileType == ETileType::Nexus)
				{
					InRange.RemoveSingle(tTile);
					UE_LOG(LogTemp, Warning, TEXT("AttackRange REMOVEUNIT"));
				}
			}

		}

		if (InRange.Num() <= 0)
		{
			GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
			AIC->MoveToSpecifiedLocation(AIC->Destination);
			bIsMoving = true;
		}
	}

	
}

void ADungeonMaster_Enemy_BASE::StartAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("StartAttack"));
	if (InRange.Num() > 0)
	{
		if (bIsMoving == true)
		{
			bIsMoving = false;
			AIC->StopUnitMovement();
		}

		if (GetWorld()->GetTimerManager().IsTimerActive(AttackTimerHandle) == false)
		{
			DamageFunction();
			GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ADungeonMaster_Enemy_BASE::StillAttacking, AttackCD, true);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
		AIC->MoveToSpecifiedLocation(Destination);
		bIsMoving = true;
	}
}

void ADungeonMaster_Enemy_BASE::StillAttacking()
{
	UE_LOG(LogTemp, Warning, TEXT("StillAttacking"));
	if (InRange.Num() > 0)
	{
		DamageFunction();
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
		AIC->MoveToSpecifiedLocation(Destination);
		bIsMoving = true;
	}
}

void ADungeonMaster_Enemy_BASE::DamageFunction()
{
	if (InRange.Num() > 0)
	{
		DamageCharacter(InRange[0]);
		FRotator NewRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), InRange[0]->GetActorLocation());
		SetActorRelativeRotation(FRotator(0.f, NewRot.Yaw, 0.f));
	}
}

void ADungeonMaster_Enemy_BASE::DamageCharacter(AActor* Obj)
{
	if (Obj)
	{
		ADungeonMaster_Characters* tUnit = Cast<ADungeonMaster_Characters>(Obj);
		if (tUnit)
		{
			tUnit->currHealth -= this->Damage;
		}

		ADungeonMaster_Tiles_BASE* tTile = Cast<ADungeonMaster_Tiles_BASE>(Obj);
		if (tTile)
		{
			tTile->currHealth -= this->Damage;
		}
	}
}
