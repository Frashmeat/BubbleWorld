// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FactionComponent.generated.h"

UENUM(BlueprintType)
enum EFaction
{
	NoFaction,
	Player,
	Friendly,
	Neutral,
	Enemy
};
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTX_API UFactionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UFactionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	bool Init(EFaction Faction);
	
	EFaction getFaction() const;
	
protected:
	
	EFaction M_Faction = NoFaction;
	AController * OwnerController;
	bool PlayerInit();
	bool EnemyInit();
	bool FriendlyInit();
	bool NeutralInit();

	
};
