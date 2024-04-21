// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "SpawnSystem/SpawnTaskBase.h"
#include "SpawnMonsterAtPointTask.generated.h"

class ATargetPoint;
/**
 * 
 */
UCLASS()
class PROJECTX_API USpawnMonsterAtPointTask : public USpawnTaskBase
{
	GENERATED_BODY()
public:
	
	bool InitTask(ESpawnTaskType TaskType,
	                      TSubclassOf<AMonsterBase> MonsterClass, int32 MonsterLevel, int32 Count,
	                      ATargetPoint* Point);
	virtual void Spawn() override;

	virtual void OnSpawnSuccess() override;

	virtual void OnSpawnFail() override;

	virtual bool IsValid() override;

	
	USpawnMonsterAtPointTask()
	{
		
	}

	void SetSpawnRate(int32 NewSpawnRate);
	
	ATargetPoint* SpawnPoint = nullptr;
private:
	
	FTimerHandle SpawnMonsterAtPointTimer;
	int32 SpawnRate = 1.0f;
	int32 CurrentSpawnedCount = 0;
};
