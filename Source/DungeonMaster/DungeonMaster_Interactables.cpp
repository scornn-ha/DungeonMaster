// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonMaster_Interactables.h"
#include "DungeonMaster_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"


// Sets default values
ADungeonMaster_Interactables::ADungeonMaster_Interactables()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CallForPC.AddDynamic(this, &ADungeonMaster_Interactables::GetPlayerController);

	AttackRange = CreateDefaultSubobject<USphereComponent>("AttackRange");
	AttackRange->SetupAttachment(RootComponent);
	AttackRange->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	AttackRange->SetSphereRadius(175.f);

	/* Selection Decal */
	SelectedComp = CreateDefaultSubobject<UDecalComponent>("SelectedComp");
	SelectedComp->SetupAttachment(RootComponent);
	/*static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}*/
	SelectedComp->DecalSize = FVector(100.0f, 100.0f, 100.0f);
	//SelectedComp->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	SelectedComp->SetVisibility(false);

	AreaOfEffect = CreateDefaultSubobject<UDecalComponent>("AreaOfEffect");
	AreaOfEffect->SetupAttachment(RootComponent);
	/*static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}*/
	AreaOfEffect->DecalSize = FVector(100.0f, 100.0f, 100.0f);
	//AreaOfEffect->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	AreaOfEffect->SetVisibility(false);

}

// Called when the game starts or when spawned
void ADungeonMaster_Interactables::BeginPlay()
{
	Super::BeginPlay();
	CallForPC.Broadcast();
	currHealth = maxHealth;

	AttackRange->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	SelectedComp->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	AreaOfEffect->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
}

// Called every frame
void ADungeonMaster_Interactables::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADungeonMaster_Interactables::GetPlayerController()
{
	while (PC == nullptr)
	{
		PC = Cast<ADungeonMaster_PlayerController>(UGameplayStatics::GetActorOfClass(GetWorld(), ADungeonMaster_PlayerController::StaticClass()));
	}
}