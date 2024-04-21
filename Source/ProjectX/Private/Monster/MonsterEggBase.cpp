// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MonsterEggBase.h"

#include "PreHeader.h"
#include "SpawnSystem/SpawnMonsterByMonsterTask.h"


// Sets default values
AMonsterEggBase::AMonsterEggBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMonsterEggBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMonsterEggBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMonsterEggBase::Init(float MaxHealth, int32 ArmorLevel,USpawnMonsterByMonsterTask * SpawnTask)
{
	if(!SpawnTask)
	{
		DEBUG_LOG(__FUNCDNAME__ " : SpawnTask is Null");
	}else
	{
		SpawnMonsterByMonsterTask = SpawnTask;
	}
	Health = MaxHealth;
	Armor = ArmorLevel;
}

void AMonsterEggBase::GetDamage(float BaseDamage, int32 ArmorPiercingLevel)
{
	float RealDamage = BaseDamage;
	if(ArmorPiercingLevel < Armor)
	{
		RealDamage *= 0.1;
		Health = Health - RealDamage < 0 ? 0 : Health - RealDamage;
	}
	Health = Health - RealDamage < 0 ? 0 : Health - RealDamage;
	if(Health <= 0)
	{
		OnDestroy();
	}
	
}

void AMonsterEggBase::OnDestroy()
{
	//1. 通知结束生成任务
	SpawnMonsterByMonsterTask->OnSpawnBlocked();
}



