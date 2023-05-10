// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonMaster_Interactables.generated.h"

UENUM()
enum class ECurrentState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Moving UMETA(DisplayName = "Moving"),
	Attacking UMETA(DisplayName = "Attacking"),
	Stopped UMETA(DisplayName = "Stopped"),
};

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Info)
	float AttackTimer = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Info)
	float AttackFinalTimer = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Info)
	float AttackCD = 0.9f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
	UTexture2D* unitThumbnail;

	/** A decal that shows if the unit is selected. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	class UDecalComponent* HighlightedComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
	class UBoxComponent* AttackRange;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PC)
	//class ARTSTest1_MainController* PlayerController;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PC)
	bool bSpawnSetup = false;

	/*Movement*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	FVector TilePosition;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	FVector Destination;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	ECurrentState UnitState = ECurrentState::Idle;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
