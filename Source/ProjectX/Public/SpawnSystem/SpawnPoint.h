// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "SpawnPoint.generated.h"


UENUM(BlueprintType)
enum class ESpawnPointType : uint8
{
	Default,
	CorpsePile
};

/**
 * 
 */
UCLASS()
class PROJECTX_API ASpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ESpawnPointType SpawnPointType = ESpawnPointType::Default;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStaticMeshComponent * SpawnPointMesh;
private:
};
