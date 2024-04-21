// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnSystem/SpawnMonsterByMonsterTask.h"

#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Monster/NormalMonster.h"



bool USpawnMonsterByMonsterTask::InitTask( ESpawnTaskType TaskType,
	TArray<ANormalMonster*>& Monsters)
{
	Super::InitTask();
	if(!DyingMonsters.IsEmpty())
	{
		// ReSharper disable once CppExpressionWithoutSideEffects
		for (int32 i = 0; i < DyingMonsters.Num() , DyingMonsters.Emplace(Monsters[i]);++i);
		
	}else
	{
		DEBUG_LOG(__FUNCDNAME__ " : DyingMonsters Is Empty");
		return false;
	}
	SpawnTaskType = TaskType;
	SpawnCount = 1;
	//计算新怪物的等级
	NewMonsterLevel = CalculateNewMonsterLevel();
	//计算新怪物的位置
	NewMonsterLocation = CalculateNewMonsterLocation();
	//计算新怪物的种类
	NewMonsterClass = CalculateNewMonsterClass();
	return true;
}

void USpawnMonsterByMonsterTask::Spawn()
{
	Super::Spawn();
	//1. 尸体移动 , 不可攻击阶段
	//MoveDyingMonsterToSpawnLocation();
	//2. 开始生成 , 可以攻击阶段
	BeginToSpawnNewMonster();
	//3. 生成完成
	
}

void USpawnMonsterByMonsterTask::OnSpawnSuccess()
{
	Super::OnSpawnSuccess();
	
	ClearDyingMonsters();
}

void USpawnMonsterByMonsterTask::OnSpawnFail()
{
	Super::OnSpawnFail();
	ClearDyingMonsters();
}



void USpawnMonsterByMonsterTask::OnSpawnBlocked()
{
	//当生成被阻断
	SpawnTaskState = Fail;
	//清除生成计时器
	if(SpawnNewMonsterTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnNewMonsterTimer);
	}
	//检测是否可以生成 新的生成点
	SpawnManager->CheckCanSpawnNewPoint(this);
	//通知Manager 生成失败
	SpawnManager->OnTaskFail(this);
	//清理尸体	
	ClearDyingMonsters();
}

bool USpawnMonsterByMonsterTask::IsValid()
{
	return Super::IsValid();
}


void USpawnMonsterByMonsterTask::MoveDyingMonsterToSpawnLocation()
{
	
	
}

FVector USpawnMonsterByMonsterTask::CalculateNewMonsterLocation()
{
	//计算传入Monsters的位置平均值
	if(DyingMonsters.IsEmpty())
	{
		DEBUG_LOG(__FUNCDNAME__" : DyingMonster isEmpty");
		return {0,0,0};
	}
	TArray<FVector> DyingMonsterLocations;
	for (auto DyingMonster : DyingMonsters)
	{
		DyingMonsterLocations.Emplace(DyingMonster->GetActorLocation());		
	}
	return UKismetMathLibrary::GetVectorArrayAverage(DyingMonsterLocations);	
}
int32 USpawnMonsterByMonsterTask::CalculateNewMonsterLevel()
{
	int32 TotalMonsterLevel = 0;
	for (auto DyingMonster : DyingMonsters)
	{
		TotalMonsterLevel += DyingMonster->GetMonsterLevel();
	}
	int32 AvgMonsterLevel = TotalMonsterLevel / DyingMonsters.Num();
	return AvgMonsterLevel + 1 > AMonsterBase::MaxMonsterLevel ? AMonsterBase::MaxMonsterLevel : AvgMonsterLevel + 1;
}
TSubclassOf<AMonsterBase> USpawnMonsterByMonsterTask::CalculateNewMonsterClass()
{
	//随机选取
	int32 Index = UKismetMathLibrary::RandomInteger(DyingMonsters.Num());
	if(DyingMonsters[Index])
	{
		return DyingMonsters[Index]->GetClass();
	}else
	{
		DEBUG_LOG(__FUNCDNAME__" : Fail to Get DyingMonster Class");
	}
	return nullptr;
}

void USpawnMonsterByMonsterTask::BeginToSpawnNewMonster()
{
	AMonsterBase* NewMonster = SpawnManager->GetNewMonsterFromPoll(NewMonsterClass);
	
	//生成怪物卵
	if(NewMonster)
	{
		MonsterEgg = GetWorld()->SpawnActor(NewMonster->MonsterEggClass,&NewMonsterLocation);
		if(!MonsterEgg)
		{
			DEBUG_LOG(__FUNCDNAME__" : Fail to Spawn MonsterEgg");
		}
	}else
	{
		DEBUG_LOG(__FUNCDNAME__ " : Fail to Get MonsterEggClass");
		return;
	}
	
	//准备生成怪物本体
	GetWorld()->GetTimerManager().SetTimer(SpawnNewMonsterTimer,[&]()
	{
		if(NewMonster)
		{
			NewMonster->MakeMonsterSpawn(NewMonsterLocation , NewMonsterLevel);
			OnSpawnSuccess();
		}else
		{
			OnSpawnFail();
		}
		
	},1.0f,false);
}

void USpawnMonsterByMonsterTask::ClearDyingMonsters()
{
	for (auto DyingMonster : DyingMonsters)
	{
		DyingMonster->OnDead();
	}
}
