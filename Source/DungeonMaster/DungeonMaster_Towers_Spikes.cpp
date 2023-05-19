// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonMaster_Towers_Spikes.h"

ADungeonMaster_Towers_Spikes::ADungeonMaster_Towers_Spikes()
{
	maxHealth = 50.f;
	Name = "Spikes Trap";
	Description = "This tower can be placed on the path and will periodically attack all enemies in it's range.";
	Cost = 400;
	Damage = 10.f;
	AttackCD = 1.5f;
}

void ADungeonMaster_Towers_Spikes::TowerAttackUnit() 
{
	if (InRange.Num() > 0)
	{
		if (AttackTimer >= AttackFinalTimer)
		{
			for (int i = 0; i < InRange.Num(); i++) 
			{
				DealDamageToEnemy(InRange[i]);
			}
			Attacking.Broadcast();
		}
		else
		{
			Attacking.Broadcast();
		}
	}
}