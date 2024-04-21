// Fill out your copyright notice in the Description page of Project Settings.


#include "Statistic/MonsterDataStatisticComponent.h"

#include "Monster/MonsterBase.h"

// Sets default values for this component's properties
UMonsterDataStatisticComponent::UMonsterDataStatisticComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMonsterDataStatisticComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMonsterDataStatisticComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMonsterDataStatisticComponent::CollectDyingMonsterData(AMonsterBase* Monster)
{
}

void UMonsterDataStatisticComponent::CollectDeadMonsterData(AMonsterBase* Monster)
{
}

void UMonsterDataStatisticComponent::CollectSpawnMonsterData(AMonsterBase* Monster)
{

}


