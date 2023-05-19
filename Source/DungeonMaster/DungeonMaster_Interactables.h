// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonMaster_Interactables.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FIntGetPlayerController);

UCLASS()
class DUNGEONMASTER_API ADungeonMaster_Interactables : public AActor
{
	GENERATED_BODY()
	
public:	// functions
	// Sets default values for this actor's properties
	ADungeonMaster_Interactables();

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PC)
	float AttackTimer = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PC)
	float AttackFinalTimer = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Info)
	float AttackCD = 0.9f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
	UTexture2D* Thumbnail;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Info)
	bool bIsActive = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Info)
	FVector currLocation;

	/** A decal that shows if the unit is selected. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
	class UDecalComponent* SelectedComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
	class UDecalComponent* AreaOfEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
	class USphereComponent* AttackRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PC)
	class ADungeonMaster_PlayerController* PC;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PC)
	//bool bSpawnSetup = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(BlueprintAssignable)
	FIntGetPlayerController CallForPC;

	UFUNCTION()
	void GetPlayerController();


};
