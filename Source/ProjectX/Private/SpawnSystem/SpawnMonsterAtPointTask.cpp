// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnSystem/SpawnMonsterAtPointTask.h"

bool USpawnMonsterAtPointTask::InitTask(ESpawnTaskType TaskType,
                                        TSubclassOf<AMonsterBase> MonsterClass, int32 MonsterLevel, int32 Count,
                                        ATargetPoint* Point)
{
	Super::InitTask();
	SpawnTaskType = TaskType;
	NewMonsterClass = MonsterClass;
	NewMonsterLevel = MonsterLevel;
	SpawnCount = Count;
	SpawnPoint = Point;
	return true;
}


void USpawnMonsterAtPointTask::Spawn()
{
	Super::Spawn();
	//每隔 1 秒生产怪物
	GetWorld()->GetTimerManager().SetTimer(SpawnMonsterAtPointTimer,[&]()
		{
			if(CurrentSpawnedCount < SpawnCount)
			{
				if(AMonsterBase* NewMonster = SpawnManager->GetNewMonsterFromPoll(NewMonsterClass))
				{
					NewMonster->MakeMonsterSpawn(SpawnPoint->GetActorLocation(), NewMonsterLevel);
					CurrentSpawnedCount++;
				}else
				{
					OnSpawnFail();
				}
			}else
			{
				OnSpawnSuccess();
			}

		},SpawnRate,true);
}

void USpawnMonsterAtPointTask::OnSpawnSuccess()
{
	Super::OnSpawnSuccess();
}

void USpawnMonsterAtPointTask::OnSpawnFail()
{
	Super::OnSpawnFail();
}

bool USpawnMonsterAtPointTask::IsValid()
{
	return Super::IsValid() && SpawnPoint;
}

void USpawnMonsterAtPointTask::SetSpawnRate(int32 NewSpawnRate)
{
	SpawnRate = NewSpawnRate;
}




