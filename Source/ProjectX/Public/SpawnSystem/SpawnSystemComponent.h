// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyActorPoll.h"
#include "Components/ActorComponent.h"
#include "Monster/MonsterBase.h"
#include "SpawnSystemComponent.generated.h"

USTRUCT()
struct FSpawnTask
{
	GENERATED_BODY()
	int32 RemainingSpawnCount; 
	int32 MonsterLevel; 

	TSubclassOf<AMonsterBase> MonsterClass; 
};



class ATargetPoint;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTX_API USpawnSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawnSystemComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	friend APlayerBase;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;



	/**
	 * 对象池相关
	 */
public:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	 //对象池初始的数量
	int32 InitCountOfPoll = 0; 

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	//需要构建对象池的怪物类
	TArray<TSubclassOf<AMonsterBase>> MonsterClasses;


	UFUNCTION(BlueprintCallable)
	//从对象池中获取一个对象
	AMonsterBase* GetActorFromPoll(TSubclassOf<AMonsterBase> MonsterClass); 
protected:

	//对象池
	TArray<MyActorPoll<AMonsterBase>*> ActorPolls; 



	/**
	 *  怪物死亡生成相关
	 */
public:
	UFUNCTION()
	//准备生成怪物
	void ReadyToSpawnNewMonsterByDyingMonster(TArray<AMonsterBase*> DyingMonsters); 

protected:

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	//准备生成时间
	int32 ReadySpawnMonsterByDyingMonsterTime = 1.0f; 

	FTimerHandle ReadySpawnNewMonsterByDyingMonsterTimer;
	//计算新生成怪物的位置
	FVector CalculateNewMonsterLocationByDyingMonsters(TArray<AMonsterBase*> DyingMonsters); 

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	//计算新怪物等级的划分数
	int32 MonsterLevelDistinctionCount = 10; 

	//计算新怪物的等级
	int32 CalculateNewMonsterLevelByDyingMonster(TArray<AMonsterBase*>& DyingMonsters); 

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	//生成时间
	int32 SpawnNewMonsterByDyingMonsterTime = 1.0f; 

	TArray<int32> NewMonsterLevels;
	TArray<FVector> NewMonsterLocations;

	//生成新怪物
	void SpawnNewMonsterByDyingMonster();
	TArray<FTimerHandle> SpawnNewMonsterByDyingMonsterTimers;

	/**
	 *	生成点生成相关
	 */
public:
	UFUNCTION()
	int32 CalculateNewMonsterLevel();

	int32 CalculateNewMonsterClassIndex();

	UFUNCTION()
	void SpawnMonsterAtTargetPoint();



	void ReadyToSpawnMonsterAtTargetPoint(TArray<ATargetPoint*> TargetPoints, TArray<TSubclassOf<AMonsterBase>> SpawnMonsterClasses, int32 MaxSpawnCount);
	TArray<FTimerHandle> SpawnAtTargetPointTimers;
	int32 CurrentShouldSpawnCount = 0;

	int32 CalculateNewMonsterMaxCount();

	


	


		
};
