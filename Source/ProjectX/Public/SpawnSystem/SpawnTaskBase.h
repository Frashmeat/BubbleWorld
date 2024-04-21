// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnManagerComponent.h"
#include "GameMode/ProjectXModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Monster/MonsterBase.h"
#include "UObject/NoExportTypes.h"

#include "SpawnTaskBase.generated.h"

class USpawnManagerComponent;
class AMonsterBase;

UENUM(BlueprintType)
enum class ESpawnTaskType :UINT8
{
	SpawnMonsterAtPoint,
	SpawnPoint,
	SpawnMonsterByMonster,
	None
};

UENUM()
enum ESpawnTaskState
{
	Ready,
	Spawning,
	Success,
	Fail
};
/**
 * 
 */
UCLASS()
class PROJECTX_API USpawnTaskBase : public UObject
{
	GENERATED_BODY()
public:
	USpawnTaskBase()
	{
	}
	virtual void InitTask()
	{
		SpawnManager = Cast<AProjectXModeBase>(UGameplayStatics::GetGameMode(this))->SpawnManagerComponent;
		if(!SpawnManager)
		{
			DEBUG_LOG(__FUNCDNAME__" : Fail to Get SpawnManager");
		}
	}
	virtual void Spawn()
	{
		SpawnTaskState = Spawning;
	}
	virtual void OnSpawnSuccess()
	{
		SpawnTaskState = Success;
		SpawnManager->OnTaskSuccess(this);
		
	}
	virtual void OnSpawnFail()
	{
		SpawnTaskState = Fail;
		SpawnManager->OnTaskFail(this);
	}

	virtual bool IsValid()
	{
		return SpawnTaskState == Ready && NewMonsterClass && NewMonsterLevel >= 0 && SpawnCount > 0;
	}

	

	
	USpawnManagerComponent* SpawnManager = nullptr;

	TSubclassOf<AMonsterBase> NewMonsterClass = nullptr;
	int32 NewMonsterLevel = 0;
	int32 SpawnCount = 0;
	ESpawnTaskType SpawnTaskType = ESpawnTaskType::None;
	ESpawnTaskState SpawnTaskState = ESpawnTaskState::Ready;
};
