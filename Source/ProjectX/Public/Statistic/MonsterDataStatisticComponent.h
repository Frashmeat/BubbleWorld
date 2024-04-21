// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Monster/MonsterBase.h"
#include "MonsterDataStatisticComponent.generated.h"

UENUM()
enum EMonsterStatisticDataType
{
	Dying,
	Dead,
	Spawn

};

USTRUCT()
struct FStatisticMonsterData
{
	GENERATED_BODY()
	
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTX_API UMonsterDataStatisticComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMonsterDataStatisticComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



public:
	void CollectDyingMonsterData(AMonsterBase* Monster);
	void CollectDeadMonsterData(AMonsterBase* Monster);
	void CollectSpawnMonsterData(AMonsterBase* Monster);

	
protected:

	FStatisticMonsterData StatisticMonsterData;
		
};
