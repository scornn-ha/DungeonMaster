// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonMaster_Towers.h"
#include "DungeonMaster_PlayerController.h"
#include "DungeonMaster_Enemy_BASE.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ADungeonMaster_Towers::ADungeonMaster_Towers()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CallForPC.AddDynamic(this, &ADungeonMaster_Towers::GetPlayerController);

	boxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	RootComponent = boxCollision;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	Mesh->bReceivesDecals = false;
	Mesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	AttackRange = CreateDefaultSubobject<USphereComponent>("AttackRange");
	AttackRange->SetupAttachment(RootComponent);
	AttackRange->SetSphereRadius(175.f);

	SelectedComp = CreateDefaultSubobject<UDecalComponent>("SelectedComp");
	SelectedComp->SetupAttachment(RootComponent);
	SelectedComp->DecalSize = FVector(100.0f, 100.0f, 100.0f);
	SelectedComp->SetVisibility(false);

	AreaOfEffect = CreateDefaultSubobject<UDecalComponent>("AreaOfEffect");
	AreaOfEffect->SetupAttachment(RootComponent);
	AreaOfEffect->DecalSize = FVector(100.0f, 100.0f, 100.0f);
	AreaOfEffect->SetVisibility(false);


	AttackRange->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	SelectedComp->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	AreaOfEffect->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
}

// Called when the game starts or when spawned
void ADungeonMaster_Towers::BeginPlay()
{
	Super::BeginPlay();

	OnClicked.AddDynamic(this, &ADungeonMaster_Towers::OnClick);

	AttackRange->OnComponentBeginOverlap.AddDynamic(this, &ADungeonMaster_Towers::AttackRangeCheckIn);
	AttackRange->OnComponentEndOverlap.AddDynamic(this, &ADungeonMaster_Towers::AttackRangeCheckOut);

	CallForPC.Broadcast();
	currHealth = maxHealth;

	
	
}

// Called every frame
void ADungeonMaster_Towers::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsActive == false)
	{
		currLocation = PC->FindAttachmentPosition(GetActorLocation());
		FVector newLocation = PC->FindAttachmentPosition(PC->GetMouseWorldLocation());

		if (currLocation != newLocation)
		{
			SetActorLocation(newLocation);
			currLocation = newLocation;
		}
	}
}

void ADungeonMaster_Towers::DestroySelf()
{
	if (this->IsPendingKill() == false)
	{
		if (this)
		{
			this->ClearUI();
			PC->ClearSelection();
			PC->ManageMoney(Cost * 0.5f);
			PC->NotifyList.Broadcast("A character has died, you have been refunded some money!");
			this->Destroy();
		}
	}
}

void ADungeonMaster_Towers::OnClick(AActor* TouchedActor, FKey ButtonPressed)
{
	if (TouchedActor == Cast<ADungeonMaster_Towers>(this))
	{
		if (ButtonPressed.GetFName() == "LeftMouseButton") // Interact
		{
			if (bIsActive == false)
			{
				PC->SetTileAttachment();
			}
			else if (bIsActive == true && PC->isBuilding == false) // select unit 
			{
				PC->SetSelection(this);
				//change visibility
			}
		}
	}
}

void ADungeonMaster_Towers::AttackRangeCheckIn(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsActive == true) 
	{
		if (OtherActor)
		{
			ADungeonMaster_Enemy_BASE* tUnit = Cast<ADungeonMaster_Enemy_BASE>(OtherActor);
			if (tUnit)
			{
				InRange.AddUnique(tUnit);
				UE_LOG(LogTemp, Warning, TEXT("AttackRange ADDUNIT"));
				StartAttack();
			}
		}
	}
}

void ADungeonMaster_Towers::AttackRangeCheckOut(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (InRange.Num() > 0)
	{
		if (OtherActor)
		{
			ADungeonMaster_Enemy_BASE* tUnit = Cast<ADungeonMaster_Enemy_BASE>(OtherActor);
			if (tUnit)
			{
				InRange.RemoveSingle(tUnit);
				UE_LOG(LogTemp, Warning, TEXT("AttackRange REMOVEUNIT"));
			}

		}
	}
}

void ADungeonMaster_Towers::StartAttack()
{
	if (InRange.Num() > 0)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(AttackTimerHandle) == false) 
		{
			DamageFunction();
			GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ADungeonMaster_Towers::StillAttacking, AttackCD, true);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
	}
}

void ADungeonMaster_Towers::StillAttacking()
{
	if (InRange.Num() > 0) 
	{
		DamageFunction();
	}
	else 
	{
		GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
	}
}

void ADungeonMaster_Towers::DamageFunction() 
{
	if (InRange.Num() > 0) 
	{
		DealDamageToEnemy(InRange[0]);
		FRotator NewRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), InRange[0]->GetActorLocation());
		SetActorRelativeRotation(FRotator(0.f, NewRot.Yaw, 0.f));
	}
}

void ADungeonMaster_Towers::DealDamageToEnemy(AActor* Obj)
{
	if (Obj)
	{
		ADungeonMaster_Enemy_BASE* tUnit = Cast<ADungeonMaster_Enemy_BASE>(Obj);
		if (tUnit)
		{
			tUnit->currHealth -= this->Damage;
		}
	}
}

void ADungeonMaster_Towers::GetPlayerController()
{
	while (PC == nullptr)
	{
		PC = Cast<ADungeonMaster_PlayerController>(UGameplayStatics::GetActorOfClass(GetWorld(), ADungeonMaster_PlayerController::StaticClass()));
	}
}

void ADungeonMaster_Towers::DecalVisible()
{
	SelectedComp->SetVisibility(!SelectedComp->IsVisible());
}
