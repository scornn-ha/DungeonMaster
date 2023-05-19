#include "DungeonMaster_Characters.h"
#include "DungeonMaster_PlayerController.h"
#include "DungeonMaster_AIController.h"
#include "DungeonMaster_Enemy_BASE.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"

ADungeonMaster_Characters::ADungeonMaster_Characters()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OnClicked.AddDynamic(this, &ADungeonMaster_Characters::OnCharacterClick);
	AttackingEnemy.AddDynamic(this, &ADungeonMaster_Characters::CharacterAttackUnit);
	CallForPC.AddDynamic(this, &ADungeonMaster_Characters::GetPlayerController);

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

void ADungeonMaster_Characters::BeginPlay()
{
	Super::BeginPlay();
	currHealth = maxHealth;

	CallForPC.Broadcast();

	AttackRange->OnComponentBeginOverlap.AddDynamic(this, &ADungeonMaster_Characters::AttackRangeCheckIn);
	AttackRange->OnComponentEndOverlap.AddDynamic(this, &ADungeonMaster_Characters::AttackRangeCheckOut);
}

// Called every frame
void ADungeonMaster_Characters::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (bIsActive == false)
	{
		currLocation = PC->FindAttachmentPosition(GetActorLocation());
		FVector newLocation = PC->FindAttachmentPosition(PC->GetMouseWorldLocation());

		if (currLocation != newLocation)
		{
			this->SetActorLocation(newLocation);
			currLocation = newLocation;
		}
	}

	AttackTimer = TimerFunction(AttackTimer, AttackFinalTimer);
}

void ADungeonMaster_Characters::OnCharacterClick(AActor* TouchedActor, FKey ButtonPressed)
{
	if (TouchedActor == Cast<ADungeonMaster_Characters>(this))
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
			}
		}
	}
}

void ADungeonMaster_Characters::SetNewMove(FVector location) 
{
	if (PC->FindSelection(this)) 
	{
		if (AIC)
		{
			Destination = location;
			AIC->MoveToSpecifiedLocation(location);
			bIsMoving = true;
		}
	}
}

void ADungeonMaster_Characters::AttackRangeCheckIn(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ADungeonMaster_Enemy_BASE* tUnit = Cast<ADungeonMaster_Enemy_BASE>(OtherActor);
		if (tUnit)
		{
			InRange.AddUnique(tUnit);
			UE_LOG(LogTemp, Warning, TEXT("AttackRange ADDUNIT"));
			AttackingEnemy.Broadcast();
		}
	}
}

void ADungeonMaster_Characters::AttackRangeCheckOut(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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
				AttackingEnemy.Broadcast();
			}

		}
	}
}

float ADungeonMaster_Characters::TimerFunction(float SetTime, float DesiredTime)
{
	float CurrentTime = GetWorld()->GetRealTimeSeconds();

	if (SetTime < DesiredTime)
		SetTime += (CurrentTime - SetTime);

	return SetTime;
}

void ADungeonMaster_Characters::CharacterAttackUnit()
{
	if (InRange.Num() > 0)
	{
		if (bIsMoving == true) 
		{
			bIsMoving = false;
			AIC->StopUnitMovement();
		}

		if (AttackTimer >= AttackFinalTimer)
		{
			DamageEnemy(InRange[0]);
			AttackingEnemy.Broadcast();
		}
		else
		{
			AttackingEnemy.Broadcast();
		}
	}
	else 
	{
		AIC->MoveToSpecifiedLocation(Destination);
		bIsMoving = true;
	}
}

void ADungeonMaster_Characters::DamageEnemy(AActor* Obj)
{
	if (Obj)
	{
		ADungeonMaster_Enemy_BASE* tUnit = Cast<ADungeonMaster_Enemy_BASE>(Obj);
		if (tUnit)
		{
			//tUnit->currHealth -= this->Damage;
			AttackTimer = GetWorld()->GetRealTimeSeconds();
			AttackFinalTimer = (AttackTimer + AttackCD);
		}
	}
}

void ADungeonMaster_Characters::GetPlayerController()
{
	while (PC == nullptr)
	{
		PC = Cast<ADungeonMaster_PlayerController>(UGameplayStatics::GetActorOfClass(GetWorld(), ADungeonMaster_PlayerController::StaticClass()));
		PC->OnMoveClick.AddDynamic(this, &ADungeonMaster_Characters::SetNewMove);
	}

	while (AIC == nullptr) 
	{
		AIC = Cast<ADungeonMaster_AIController>(GetController());
	}
}