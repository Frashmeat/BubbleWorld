// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyActorPoll.h"
#include "Components/ActorComponent.h"
#include "SpawnManagerComponent.generated.h"


enum class ESpawnTaskType : UINT8;
class USpawnTaskBase;
class ATargetPoint;
class USpawnWorkerBase;
class AMonsterBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTX_API USpawnManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawnManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;




	/*
	 * 对象池相关
	 */
public:
	//可以生成的怪物类型
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<TSubclassOf<AMonsterBase>> MonsterClasses;  

	//从池子中获取新的Monster
	AMonsterBase* GetNewMonsterFromPoll(TSubclassOf<AMonsterBase> MonsterClass); 
	
protected:
	//对象池
	TArray<MyActorPoll<AMonsterBase>*>  MonsterPolls;


	

	/*
	 * 生成相关
	 */
public:
	
	//生成点
	TArray<ATargetPoint*> SpawnPoints;


	struct FMonsterRecord
	{
		TSubclassOf<AMonsterBase> MonsterClass; //怪物种类
		int32 MonsterLevel; //怪物等级
		int32 MonsterCount; //总共的数量
	};

	struct FMonsterSpawnEfficiency
	{
		TSubclassOf<AMonsterBase> MonsterClass; //生成的怪物种类
		int32 MonsterLevel; //生成的怪物等级
		int32 SpawnEfficiency; //总共生成的效率
	};

	//当一个任务成功完成时
	void OnTaskSuccess(USpawnTaskBase* SpawnTask);
	//当一个任务失败时
	void OnTaskFail(USpawnTaskBase* SpawnTask);
	//检测是否有任务需要完成
	void CheckTaskToDo();
	//添加任务
	void AddSpawnTask(USpawnTaskBase* NewSpawnTask);
	//检测是否可以生成新的生成点
	void CheckCanSpawnNewPoint(USpawnTaskBase* SpawnTask);
protected:
	
	TArray<USpawnTaskBase*> SpawnTasks;
	FTimerHandle CheckTaskToDoTimer;

	void RecordTask(USpawnTaskBase * SpawnTask);
	TArray<FMonsterRecord> MonsterKillRecords;
	TArray<FMonsterRecord> MonsterSpawnRecords;
	TArray<FMonsterSpawnEfficiency> MonsterSpawnEfficiencies;
	void CalculateSpawnMonsterEfficiency();
	void GenerateSpawnAtPointTask();
	
	double LastTaskFinishTime;
	
};
