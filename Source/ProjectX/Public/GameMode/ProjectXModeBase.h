// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectXModeBase.generated.h"

/**
 * 
 */


class USpawnTaskBase;
class USpawnManagerComponent;
class ASpawnPoint;
class UPlayerDataStatisticComponent;
class UMonsterDataStatisticComponent;

UCLASS()
class PROJECTX_API AProjectXModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AProjectXModeBase();
protected:
	virtual void BeginPlay() override;

public:
	
	void AddTaskToSpawnSystem(USpawnTaskBase* NewSpawnTask);
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	
	USpawnManagerComponent* SpawnManagerComponent;


public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<ASpawnPoint*> SpawnPoints;
	
protected:
	
};
