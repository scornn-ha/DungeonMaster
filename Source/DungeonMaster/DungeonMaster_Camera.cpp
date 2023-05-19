// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonMaster_Camera.h"
#include "Components/SceneComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ADungeonMaster_Camera::ADungeonMaster_Camera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 3000.f;
	CameraBoom->SetRelativeRotation(FRotator(-90.f, 90.f, 0.f));
	CameraBoom->bDoCollisionTest = false;

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

}

// Called when the game starts or when spawned
void ADungeonMaster_Camera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADungeonMaster_Camera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

