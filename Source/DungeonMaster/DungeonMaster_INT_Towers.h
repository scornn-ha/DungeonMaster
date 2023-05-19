// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DungeonMaster_Interactables.h"
#include "DungeonMaster_INT_Towers.generated.h"

UDELEGATE() // when this is called attack first unit in InRange array
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttackUnit);

UCLASS()
class DUNGEONMASTER_API ADungeonMaster_INT_Towers : public ADungeonMaster_Interactables
{
	GENERATED_BODY()

public: // functions

	ADungeonMaster_INT_Towers();

	/*Selection*/
	UFUNCTION()
	void OnClick(AActor* TouchedActor, FKey ButtonPressed);

public: // variables

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* boxCollision;

protected: // bp calls
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	/*Attacking*/
	UPROPERTY(VisibleAnywhere, Category = "Attacking")
	TArray<AActor*> InRange;
	UPROPERTY(BlueprintAssignable)
	FAttackUnit Attacking;
	UFUNCTION()
	virtual void TowerAttackUnit();
	UFUNCTION()
	void DealDamageToEnemy(AActor* Obj);

private:

	UFUNCTION()
	void AttackRangeCheckIn(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void AttackRangeCheckOut(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
	UFUNCTION()
	float TimerFunction(float SetTime, float DesiredTime);

	
};
