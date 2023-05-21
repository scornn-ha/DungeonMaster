// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonMaster_Characters_Wizard.h"
#include "DungeonMaster_Enemy_BASE.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

ADungeonMaster_Characters_Wizard::ADungeonMaster_Characters_Wizard()
{
	maxHealth = 350.f;
	Name = "Wizard";
	Description = "This character can be moved at will, and will shoot a large AOE damaging all enemies.";
	Cost = 2500;
	Damage = 30.f;
	AttackCD = 2.5f;
}


void ADungeonMaster_Characters_Wizard::DamageFunction()
{
	if (InRange.Num() > 0)
	{
		AActor* attackUnit = InRange[0];
		TArray<FHitResult> tHits;
		FCollisionShape tcheckArea = FCollisionShape::MakeSphere(200.f);
		DrawDebugSphere(GetWorld(), attackUnit->GetActorLocation(), tcheckArea.GetSphereRadius(), 10, FColor::Red, true);

		bool isHit = GetWorld()->SweepMultiByChannel(tHits, attackUnit->GetActorLocation(), attackUnit->GetActorLocation(), FQuat::Identity, ECC_Pawn, tcheckArea);

		if (isHit == true) // we hit something
		{
			for (int i = 0; i < tHits.Num(); i++)
			{
				ADungeonMaster_Enemy_BASE* tUnit = Cast<ADungeonMaster_Enemy_BASE>(tHits[i].Actor);
				if (tUnit)
				{
					DamageEnemy(tUnit);
				}
			}
		}

		FRotator NewRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), InRange[0]->GetActorLocation());
		SetActorRelativeRotation(FRotator(0.f, NewRot.Yaw, 0.f));
	}
}