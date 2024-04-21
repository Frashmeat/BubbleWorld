// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Monster/MonsterBase.h"
#include "MonsterSpawnSystemComponent.generated.h"

#define MAX_MONSTER_LEVEL 3
class ANormalMonster;
class USpawnManagerComponent;
class USpawnSystemComponent;



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTX_API UMonsterSpawnSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMonsterSpawnSystemComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;



public:

	UFUNCTION(BlueprintCallable)
	void DetectDyingMonsterToSpawnNewMonster();
	
	//Monster 设定怪物死亡Timer
	FTimerHandle ToDieTimer;
	
	//濒死状态持续时间
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float DurationOfDying = 2.0f; 

	//死亡检测范围
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector DetectRange = { 1000,1000,50 };



protected:
	//SpawnSystem 生成系统的顶层对象
	USpawnManagerComponent* SpawnManagerComponent;
	//判断是否可以生成
	bool CanSpawnNewMonster(TArray<AMonsterBase*>& DyingMonsters);
	//判定方式 盒体检测
	void DetectDyingMonster(TArray<AMonsterBase*>& DyingMonsters);
	//检测成功可以生成
	void BeginToSpawn(TArray<AMonsterBase*>& DyingMonsters);
	
	ANormalMonster* OwnedMonster;
		
};
