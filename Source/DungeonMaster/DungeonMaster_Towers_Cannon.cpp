// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonMaster_Towers_Cannon.h"
#include "DungeonMaster_Enemy_BASE.h"
#include "DrawDebugHelpers.h"

ADungeonMaster_Towers_Cannon::ADungeonMaster_Towers_Cannon()
{
	maxHealth = 50.f;
	Name = "Cannon Tower";
	Description = "This tower can be placed alongside the path and will shoot an AOE attack at an enemy.";
	Cost = 600;
	Damage = 18.f;
	AttackCD = 3.f;
}

void ADungeonMaster_Towers_Cannon::TowerAttackUnit()
{
	if (InRange.Num() > 0)
	{
		if (AttackTimer >= AttackFinalTimer)
		{

			AActor* attackUnit = InRange[0];
			TArray<FHitResult> tHits;
			FCollisionShape tcheckArea = FCollisionShape::MakeSphere(150.f);
			DrawDebugSphere(GetWorld(), attackUnit->GetActorLocation(), tcheckArea.GetSphereRadius(), 50, FColor::Red, true);

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
			Attacking.Broadcast();
		}
		else
		{
			Attacking.Broadcast();
		}
	}
}