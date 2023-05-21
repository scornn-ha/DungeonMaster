// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DungeonMaster_Tiles_BASE.h"
#include "DungeonMaster_PlayerController.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetNewMove, FVector, location);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateNotificationList, FString, TextInput);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateTilePhaseAmounts, int, Index, int, Amount);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGetCamera);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateTileConnections);

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
	FVector FindAttachmentPosition(FVector position);
	UFUNCTION()
	FVector GetMouseWorldLocation();

	/*Tiles*/
	UFUNCTION()
	void SetCurrentTile();
	UFUNCTION()
	bool GetCurrentTile();
	UFUNCTION()
	void SwitchCurrentTile();
	UFUNCTION()
	void CheckConnections();
	UFUNCTION()
	void SetTileAttachment();
	UFUNCTION()
	void removeCellTile(class ADungeonMaster_Tiles_BASE* tile);

	/*Selection*/
	UFUNCTION()
	void SetSelection(AActor* unit);
	UFUNCTION()
	void ClearSelection();
	UFUNCTION()
	bool FindSelection(AActor* unit);

	UFUNCTION(BlueprintCallable)
	void ManageMoney(int inputAmount);

public: // variables

	/*info*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	int Money = 1000;

	/*Tiles*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tiles")
	TArray<int> currentPlacements;
	UPROPERTY(BlueprintAssignable)
	FUpdateTileConnections CallTileUpdates;
	UFUNCTION(BlueprintCallable)
	void NexusDeath();

	/*UI*/
	UPROPERTY(BlueprintAssignable)
	FUpdateNotificationList NotifyList;
	UPROPERTY(BlueprintAssignable)
	FUpdateTilePhaseAmounts TilePhaseUIUpdate;
	UFUNCTION(BlueprintImplementableEvent)
	void TilePhaseUI();
	UFUNCTION(BlueprintImplementableEvent)
	void GamePhaseUI();


	/*Misc*/
	UPROPERTY(BlueprintAssignable)
	FGetCamera CallForCamera;
	UPROPERTY(BlueprintAssignable)
	FGetGameManager CallForGM;
	UPROPERTY(BlueprintAssignable)
	FGetNewMove OnMoveClick;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Misc")
	bool isBuilding = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Misc")
	bool isGameReady = false;

protected: // bp calls

	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

	/*Tiles*/
	UFUNCTION(BlueprintCallable)
	void SpawnCurrentActor(UClass* Build);

	/*Input*/
	UFUNCTION(BlueprintCallable)
	void RotateTileActor();
	UFUNCTION(BlueprintCallable)
	void MoveInteractableObject();
	UFUNCTION(BlueprintCallable)
	void SelectInteractableObject();

	/*GamePhase*/
	UFUNCTION(BlueprintCallable)
	void StartGamePhase();
	UFUNCTION(BlueprintCallable)
	bool CheckMoney(int inputAmount);

private:

	UPROPERTY(VisibleAnywhere, Category = "Misc")
	AActor* CurrentSpawnedActor;
	UPROPERTY(VisibleAnywhere, Category = "Misc")
	AActor* currentSelection;
	UPROPERTY(VisibleAnywhere, Category = "Misc")
	class ADungeonMaster_GameManager* GameManagerRef;

	UPROPERTY(VisibleAnywhere, Category = "Misc")
	class ADungeonMaster_Camera* mainCamera;
	
	UFUNCTION()
	void GetPlayerCamera();
	UFUNCTION()
	void GetGameManager();


};
