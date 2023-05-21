// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonMaster_Towers.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTowerGetPlayerController);

UCLASS()
class DUNGEONMASTER_API ADungeonMaster_Towers : public AActor
{
	GENERATED_BODY()

public: // functions
	// Sets default values for this actor's properties
	ADungeonMaster_Towers();
	/*Selection*/
	UFUNCTION()
	void OnClick(AActor* TouchedActor, FKey ButtonPressed);
	UFUNCTION(BlueprintImplementableEvent)
	void SetupUI();
	UFUNCTION(BlueprintImplementableEvent)
	void ClearUI();

	UFUNCTION(BlueprintCallable)
	void DestroySelf();
	UFUNCTION()
	void DecalVisible();
	
public:	// variables	

	/*Information*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	float currHealth;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	float maxHealth = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	FString Name = "Default Name";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	FString Description = "Default Desc";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	int Cost = 5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	float Damage = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	float AttackCD = 0.9f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	UTexture2D* Thumbnail;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UDecalComponent* SelectedComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UDecalComponent* AreaOfEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class USphereComponent* AttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* boxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PC")
	class ADungeonMaster_PlayerController* PC;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PC")
	bool bIsActive = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PC")
	FVector currLocation;

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
	void DealDamageToEnemy(AActor* Obj);


private:

	UFUNCTION()
	void AttackRangeCheckIn(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void AttackRangeCheckOut(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(BlueprintAssignable)
	FTowerGetPlayerController CallForPC;

	UFUNCTION()
	void GetPlayerController();
};
