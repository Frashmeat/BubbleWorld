// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "POCSpawnComponent.generated.h"


class USpawnManagerComponent;
class USpawnSystemComponent;
class ANormalMonster;
class AMonsterBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTX_API UPOCSpawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPOCSpawnComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;



	
public:
	
	inline static int32 MaxMonsterLevel = 5;

	int32 GetMaxMonsterLevel();
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float DyingTime = 5.0f;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FVector DetectRange = {100 , 100 , 100};

	//濒死时调用
	UFUNCTION()
	void BeginPOCSpawn();

protected:
	ANormalMonster * MonsterOwner;

	USpawnManagerComponent * SpawnManagerComponent;
	
	int32 CalculateMinNumOfGeneration();
	
	bool DetectDyingMonstersAround(TArray<ANormalMonster*> Array);
	
	void GenerateNewMonster(TArray<ANormalMonster*> Array);
};
