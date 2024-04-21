// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnTaskBase.h"
#include "../../../../../../../UE/UE_5.3/Engine/Source/Runtime/Core/Public/Containers/Array.h"
#include "../../../../../../../UE/UE_5.3/Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "../../../../../../../UE/UE_5.3/Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "../../../../../../../UE/UE_5.3/Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "SpawnMonsterByMonsterTask.generated.h"

class ANormalMonster;
/**
 * 
 */
UCLASS()
class PROJECTX_API USpawnMonsterByMonsterTask : public USpawnTaskBase
{
	GENERATED_BODY()

public:
	USpawnMonsterByMonsterTask()
	{
		
	}

	bool InitTask(ESpawnTaskType TaskType,
	                      TArray<ANormalMonster*>& Monsters);

	
	
	virtual void Spawn() override;
	virtual void OnSpawnSuccess() override;
	virtual void OnSpawnFail() override;
	virtual bool IsValid() override;
	void OnSpawnBlocked();
private:

	FVector CalculateNewMonsterLocation();
	void MoveDyingMonsterToSpawnLocation();
	int32 CalculateNewMonsterLevel();
	TSubclassOf<AMonsterBase> CalculateNewMonsterClass();
	void BeginToSpawnNewMonster();

	void ClearDyingMonsters();
	
	UPROPERTY()
	TArray<ANormalMonster*> DyingMonsters;
	
	FTimerHandle SpawnNewMonsterTimer;
	
	FVector NewMonsterLocation;
	
	AActor* MonsterEgg;
};
