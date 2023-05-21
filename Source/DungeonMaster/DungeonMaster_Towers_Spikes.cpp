// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonMaster_Towers_Spikes.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"

ADungeonMaster_Towers_Spikes::ADungeonMaster_Towers_Spikes()
{
	maxHealth = 50.f;
	Name = "Spikes Trap";
	Description = "This tower can be placed on the path and will periodically attack all enemies in it's range.";
	Cost = 400;
	Damage = 10.f;
	AttackCD = 1.5f;

	AttackRange->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	SelectedComp->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	AreaOfEffect->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
}

void ADungeonMaster_Towers_Spikes::DamageFunction() 
{
	if (InRange.Num() > 0)
	{
		for (int i = 0; i < InRange.Num(); i++) 
		{
			DealDamageToEnemy(InRange[i]);
		}

	}
}