// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DungeonMaster_Characters.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharGetPlayerController);

UDELEGATE() // when this is called attack first unit in InRange array
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttackEnemy);

UCLASS()
class DUNGEONMASTER_API ADungeonMaster_Characters : public ACharacter
{
	GENERATED_BODY()

public: // functions

	ADungeonMaster_Characters();

	/*Selection*/
	UFUNCTION()
	void OnCharacterClick(AActor* TouchedActor, FKey ButtonPressed);

public: // variables

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
	int Cost = 5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Info)
	float Damage = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Info)
	float AttackCD = 0.9f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
	UTexture2D* Thumbnail;
	

	/** A decal that shows if the unit is selected. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
	class UDecalComponent* SelectedComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
	class UDecalComponent* AreaOfEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
	class USphereComponent* AttackRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PC)
	class ADungeonMaster_PlayerController* PC;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PC)
	class ADungeonMaster_AIController* AIC;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PC)
	bool bIsActive = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PC)
	FVector currLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PC)
	FVector Destination;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PC)
	bool bIsMoving = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PC)
	float AttackTimer = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PC)
	float AttackFinalTimer = 0.f;

protected: // bp calls
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*Attacking*/
	UPROPERTY(VisibleAnywhere, Category = "Attacking")
	TArray<AActor*> InRange;
	UPROPERTY(BlueprintAssignable)
	FAttackEnemy AttackingEnemy;
	UFUNCTION()
	virtual void CharacterAttackUnit();
	UFUNCTION()
	void DamageEnemy(AActor* Obj);

private:

	UFUNCTION()
	void AttackRangeCheckIn(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void AttackRangeCheckOut(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	float TimerFunction(float SetTime, float DesiredTime);

	UPROPERTY(BlueprintAssignable)
	FCharGetPlayerController CallForPC;

	UFUNCTION()
	void GetPlayerController();

	UFUNCTION()
	void SetNewMove(FVector location);

};
