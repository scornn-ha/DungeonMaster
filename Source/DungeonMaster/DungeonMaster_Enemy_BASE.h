// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DungeonMaster_Enemy_BASE.generated.h"

UCLASS()
class DUNGEONMASTER_API ADungeonMaster_Enemy_BASE : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADungeonMaster_Enemy_BASE();

	/*Information*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Info)
	float currHealth;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Info)
	float maxHealth = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Info)
	FString Name = "Default Name";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Info)
	FString Description = "Default Desc";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Info)
	float Damage = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Info)
	int DeathMoney = 5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Info)
	float AttackCD = 0.9f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
	class USphereComponent* AttackRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PC)
	class ADungeonMaster_Enemy_AIController* AIC;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PC)
	FVector currLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PC)
	FVector Destination;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PC)
	bool bIsMoving = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*Attacking*/
	UPROPERTY(VisibleAnywhere, Category = "PC")
	TArray<AActor*> InRange;
	UPROPERTY(VisibleAnywhere, Category = "PC")
	FTimerHandle AttackTimerHandle;
	UFUNCTION()
	void StartAttack();
	UFUNCTION()
	void StillAttacking();
	UFUNCTION()
	virtual void DamageFunction();
	UFUNCTION()
	void DamageCharacter(AActor* Obj);

private:

	UFUNCTION()
	void AttackRangeCheckIn(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void AttackRangeCheckOut(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
