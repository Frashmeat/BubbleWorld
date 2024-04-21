// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/XCharacterBase.h"

#include "PreHeader.h"
#include "Character/Component/FactionComponent.h"
#include "Runtime/AIModule/Classes/AIController.h"


// Sets default values
AXCharacterBase::AXCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FactionComponent = CreateDefaultSubobject<UFactionComponent>(FName("FactionComponent"));
	if(!FactionComponent) DEBUG_LOG("Fail to Create Faction Component");

	
	
}

// Called when the game starts or when spawned
void AXCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	Init();
	
}

// Called every frame
void AXCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AXCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}





