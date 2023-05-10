// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DungeonMaster_Tiles_BASE.h"
#include "DungeonMaster_PlayerController.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateNotificationList, FString, TextInput);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGetCamera);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGetGameManager);

UCLASS()
class DUNGEONMASTER_API ADungeonMaster_PlayerController : public APlayerController
{
	GENERATED_BODY()

public: // functions
	ADungeonMaster_PlayerController();

	UFUNCTION()
	FVector FindTilePosition(FVector position);
	UFUNCTION()
	FVector GetMouseWorldLocation();
	UFUNCTION()
	void SetCurrentTile();


public: // variables

	/*info*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	int Money = 1000;

	/*Tiles*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tiles")
	bool bTilePlacement = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tiles")
	TArray<int> currentPlacements;

	/*UI*/
	UPROPERTY(BlueprintAssignable)
	FUpdateNotificationList NotifyList;

	UFUNCTION(BlueprintImplementableEvent)
	void TilePhaseUI();
	UFUNCTION(BlueprintImplementableEvent)
	void GamePhaseUI();

	/*Misc*/
	UPROPERTY(BlueprintAssignable)
	FGetCamera CallForCamera;
	UPROPERTY(BlueprintAssignable)
	FGetGameManager CallForGM;

protected: // bp calls

	virtual void Tick(float DeltaTime) override;
	//virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void SpawnTileActor(UClass* Build);


private:

	UPROPERTY(VisibleAnywhere, Category = "Tiles")
	AActor* CurrentTileActor;
	UPROPERTY(VisibleAnywhere, Category = "Player")
	class ADungeonMaster_Interactables* currentSelection;
	UPROPERTY(VisibleAnywhere, Category = "Misc")
	class ADungeonMaster_GameManager* GameManagerRef;


	UPROPERTY(VisibleAnywhere, Category = "Misc")
	class ADungeonMaster_Camera* mainCamera;
	
	UFUNCTION()
	void GetPlayerCamera();
	UFUNCTION()
	void GetGameManager();

};
