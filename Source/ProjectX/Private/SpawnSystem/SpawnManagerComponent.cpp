// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnSystem/SpawnManagerComponent.h"

#include "SpawnSystem/SpawnMonsterAtPointTask.h"


// Sets default values for this component's properties
USpawnManagerComponent::USpawnManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
	
}
// Called when the game starts
void USpawnManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	LastTaskFinishTime = GetWorld()->GetTimeSeconds();
	if (MonsterClasses.IsEmpty())
	{
		DEBUG_LOG(__FUNCDNAME__" : MonsterClasses is Empty");
		return;
	}
	for (TSubclassOf<AMonsterBase> MonsterClass : MonsterClasses)
	{
		if(MonsterClass)
		{
			MonsterPolls.Emplace(new MyActorPoll<AMonsterBase>(GetWorld(), 0, MonsterClass));
		}else
		{
			DEBUG_LOG(__FUNCDNAME__" : Fail to Get MonsterClass");
		}
	}
	
	GetWorld()->GetTimerManager().SetTimer(CheckTaskToDoTimer,this,&USpawnManagerComponent::CheckTaskToDo,0.5f,true);

	
}
// Called every frame
void USpawnManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}


AMonsterBase* USpawnManagerComponent::GetNewMonsterFromPoll(TSubclassOf<AMonsterBase> MonsterClass)
{
	for (int i = 0; i < MonsterClasses.Num(); ++i)
	{
		if (MonsterClass == MonsterClasses[i])
		{
			return MonsterPolls[i]->GetActor();
		}
	}
	return nullptr;
}

void USpawnManagerComponent::OnTaskSuccess(USpawnTaskBase* SpawnTask)
{
	SpawnTasks.Remove(SpawnTask);
	RecordTask(SpawnTask);
	if(SpawnTasks.IsEmpty())
	{
		CalculateSpawnMonsterEfficiency();
		GenerateSpawnAtPointTask();
		LastTaskFinishTime = GetWorld()->GetTimeSeconds();
	}	
}
void USpawnManagerComponent::OnTaskFail(USpawnTaskBase* SpawnTask)
{
	SpawnTasks.Remove(SpawnTask);
}
void USpawnManagerComponent::CheckTaskToDo()
{
	for (auto SpawnTask : SpawnTasks)
	{
		if(SpawnTask->SpawnTaskState == Ready)
			SpawnTask->Spawn();
	}
}

void USpawnManagerComponent::AddSpawnTask(USpawnTaskBase* NewSpawnTask)
{
	if(NewSpawnTask)
	{
		if(NewSpawnTask->IsValid())
		{
			SpawnTasks.Emplace(NewSpawnTask);		
		}else
		{
			DEBUG_LOG(__FUNCDNAME__" : NewSpawnTask is UnVaild");
		}
	}
}

void USpawnManagerComponent::CheckCanSpawnNewPoint(USpawnTaskBase * SpawnTask)
{
	
}

void USpawnManagerComponent::RecordTask(USpawnTaskBase* SpawnTask)
{
	for (auto SpawnRecord : MonsterSpawnRecords)
	{
		if(SpawnRecord.MonsterClass == SpawnTask->NewMonsterClass)
		{
			SpawnRecord.MonsterCount += SpawnTask->SpawnCount;
			return;
		}
	}
	MonsterSpawnRecords.Emplace(FMonsterRecord{SpawnTask->NewMonsterClass,SpawnTask->NewMonsterLevel,SpawnTask->SpawnCount});
}

void USpawnManagerComponent::CalculateSpawnMonsterEfficiency()
{
	double CurrentTime = GetWorld()->GetTimeSeconds();
	double PassedTime = CurrentTime - LastTaskFinishTime;
	for (auto KillRecord : MonsterSpawnRecords)
	{
		int32 KillEfficiency = KillRecord.MonsterCount/PassedTime;
		MonsterSpawnEfficiencies.Emplace(FMonsterSpawnEfficiency{KillRecord.MonsterClass,KillRecord.MonsterLevel,KillEfficiency});
	}
}

void USpawnManagerComponent::GenerateSpawnAtPointTask()
{
	//1. 获取当前在场的怪物数量
	//2. 获取任务清空时玩家完成的对应等级怪物的击杀数量 计算击杀效率
	//3. 生成符合击杀效率的 怪物(对应等级和数量)
}

