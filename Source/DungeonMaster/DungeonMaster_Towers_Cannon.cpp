// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonMaster_Towers_Cannon.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "DungeonMaster_Enemy_BASE.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

ADungeonMaster_Towers_Cannon::ADungeonMaster_Towers_Cannon()
{
	maxHealth = 50.f;
	Name = "Cannon Tower";
	Description = "This tower can be placed alongside the path and will shoot an AOE attack at an enemy.";
	Cost = 600;
	Damage = 18.f;
	AttackCD = 1.8f;

	AttackRange->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	SelectedComp->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	AreaOfEffect->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
}

void ADungeonMaster_Towers_Cannon::DamageFunction()
{
	if (InRange.Num() > 0)
	{
		AActor* attackUnit = InRange[0];
		TArray<FHitResult> tHits;
		FCollisionShape tcheckArea = FCollisionShape::MakeSphere(250.f);
		//DrawDebugSphere(GetWorld(), attackUnit->GetActorLocation(), tcheckArea.GetSphereRadius(), 10, FColor::Red, true);

		bool isHit = GetWorld()->SweepMultiByChannel(tHits, attackUnit->GetActorLocation(), attackUnit->GetActorLocation(), FQuat::Identity, ECC_Pawn, tcheckArea);

		if (isHit == true) // we hit something
		{
			for (int i = 0; i < tHits.Num(); i++)
			{
				ADungeonMaster_Enemy_BASE* tUnit = Cast<ADungeonMaster_Enemy_BASE>(tHits[i].Actor);
				if (tUnit)
				{
					DealDamageToEnemy(tUnit);
				}
			}
		}
		FRotator NewRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), InRange[0]->GetActorLocation());
		SetActorRelativeRotation(FRotator(0.f, NewRot.Yaw, 0.f));
	}
}