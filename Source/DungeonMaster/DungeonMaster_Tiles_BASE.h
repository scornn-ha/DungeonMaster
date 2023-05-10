// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonMaster_Tiles_BASE.generated.h"

UENUM()
enum class ETileType : uint8
{
	Straight UMETA(DisplayName = "Straight"),
	Junction UMETA(DisplayName = "Junction"),
	Bend UMETA(DisplayName = "Bend"),
	Start UMETA(DisplayName = "Start"),
	Nexus UMETA(DisplayName =  "Nexus"),
};

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGetPlayerController);

UCLASS()
class DUNGEONMASTER_API ADungeonMaster_Tiles_BASE : public AActor
{
	GENERATED_BODY()
	
public:	// functions
	// Sets default values for this actor's properties
	ADungeonMaster_Tiles_BASE();

	/*Selection*/
	UFUNCTION()
	void OnClick(AActor* TouchedActor, FKey ButtonPressed);


public: // variables


	/*Info*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	UTexture2D* Thumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	TArray<TSubclassOf<class ADungeonMaster_Interactables> > Attachments;

	/*Placement*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Placement") // Structure is fully finished and operational
	bool bIsActive = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Placement")
	bool bCanPlace = false;

	/*Misc*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Misc")
	class ADungeonMaster_PlayerController* PC;

	/*Components*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	class UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	class UBoxComponent* boxCollision;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:	
	
	UPROPERTY(BlueprintAssignable)
	FGetPlayerController CallForPC;

	UFUNCTION()
	void GetPlayerController();

	UPROPERTY(VisibleAnywhere, Category = "Misc")
	FVector currLocation;

};
