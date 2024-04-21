// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnSystem/SpawnSystemComponent.h"
#include "PreHeader.h"
#include "Engine/TargetPoint.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SpawnSystem/MonsterSpawnSystemComponent.h"


// Sets default values for this component's properties
USpawnSystemComponent::USpawnSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}
// Called when the game starts
void USpawnSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	if (MonsterClasses.IsEmpty())
	{
		DEBUG_LOG("BeginPlay : MonsterClasses is Empty");
	}
	for (TSubclassOf<AActor> MonsterClass : MonsterClasses)
	{
		ActorPolls.Emplace(new MyActorPoll<AMonsterBase>(GetWorld(), InitCountOfPoll, MonsterClass));
	}
}
// Called every frame
void USpawnSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}



AMonsterBase* USpawnSystemComponent::GetActorFromPoll(TSubclassOf<AMonsterBase> MonsterClass)
{
	for (int i = 0; i < MonsterClasses.Num(); ++i)
	{
		if (MonsterClass == MonsterClasses[i])
		{
			return ActorPolls[i]->GetActor();
		}
	}
	return nullptr;
}




void USpawnSystemComponent::ReadyToSpawnNewMonsterByDyingMonster(TArray<AMonsterBase*> DyingMonsters)
{
	//1. 准备新生成怪物的位置
	FVector NewLocation = CalculateNewMonsterLocationByDyingMonsters(DyingMonsters);
	NewMonsterLevels.Emplace(CalculateNewMonsterLevelByDyingMonster(DyingMonsters));
	NewMonsterLocations.Emplace(NewLocation);

	for (AMonsterBase* DyingMonster : DyingMonsters)
	{
		// //2. 设置怪物生命状态
		// DyingMonster->SetMonsterLifeState(EMonsterLifeState::ReadyToSpawn);
		// //3. 将尸体缓慢移动到指定位置
		// DyingMonster->SlowlyMoveToLocation(NewLocation);
	}
	//在移动完成后调用 准备结束 开始生成
	GetWorld()->GetTimerManager().SetTimer(ReadySpawnNewMonsterByDyingMonsterTimer, this, 
		&USpawnSystemComponent::SpawnNewMonsterByDyingMonster, 1.2f, false);
}
FVector USpawnSystemComponent::CalculateNewMonsterLocationByDyingMonsters(TArray<AMonsterBase*> DyingMonsters)
{
	TArray<FVector> DyingMonsterLocations;
	for (AMonsterBase* DyingMonster : DyingMonsters)
	{
		DyingMonsterLocations.Emplace(DyingMonster->GetActorLocation());
	}
	return UKismetMathLibrary::GetVectorArrayAverage(DyingMonsterLocations);
	
}
int32 USpawnSystemComponent::CalculateNewMonsterLevelByDyingMonster(TArray<AMonsterBase*>& DyingMonsters)
{
	int32 TotalDyingMonsterLevel = 0;
	int32 NewMonsterLevel = 0;
	for (AMonsterBase* DyingMonster : DyingMonsters)
	{
		// TotalDyingMonsterLevel += DyingMonster->MonsterLevel;
	}

	while(TotalDyingMonsterLevel > 0)
	{
		TotalDyingMonsterLevel -= MonsterLevelDistinctionCount;
		NewMonsterLevel++;
	}

	return NewMonsterLevel;
}
void USpawnSystemComponent::SpawnNewMonsterByDyingMonster()
{
	FTimerHandle SpawnNewMonsterByDyingMonsterTimer;
	GetWorld()->GetTimerManager().SetTimer(SpawnNewMonsterByDyingMonsterTimer, [&, SpawnNewMonsterByDyingMonsterTimer]()
	{
		int MonsterClassIndex = UKismetMathLibrary::RandomInteger(MonsterClasses.Num());
		if (AMonsterBase* NewMonster = GetActorFromPoll(MonsterClasses[MonsterClassIndex]))
		{
			if (!NewMonsterLevels.IsEmpty() && !NewMonsterLocations.IsEmpty())
			{
				// NewMonster->MakeMonsterActive(NewMonsterLocations.Pop(), NewMonsterLevels.Pop());
			}
		}
	}, 1.0f, false);

}





void USpawnSystemComponent::ReadyToSpawnMonsterAtTargetPoint(TArray<ATargetPoint*> TargetPoints,
	TArray<TSubclassOf<AMonsterBase>> SpawnMonsterClasses, int32 MaxSpawnCount)
{
	if(MaxSpawnCount <=0)
	{
		DEBUG_LOG("ReadyToSpawnMonsterAtTargetPoint : MaxSpawnCount <= 0");
		return;
	}
	if(TargetPoints.IsEmpty() || SpawnMonsterClasses.IsEmpty())
	{
		DEBUG_LOG("ReadyToSpawnMonsterAtTargetPoint : TargetPoints is Empty or SpawnMonsterClasses is Empty");
		return;
	}

	FTimerManager& TimerManager = GetWorld()->GetTimerManager(); 
	for (ATargetPoint* TargetPoint :TargetPoints)
	{
		
		FTimerHandle SpawnAtTargetPointTimer;
		TimerManager.SetTimer(SpawnAtTargetPointTimer, [&]()
			{
				if(MaxSpawnCount <= 0 )
				{
					TimerManager.ClearTimer(SpawnAtTargetPointTimer);
				}
				int32 ClassIndex = UKismetMathLibrary::RandomInteger(SpawnMonsterClasses.Num());
				FVector NewLocation = TargetPoint->GetActorLocation();
				if(TSubclassOf<AMonsterBase> MonsterClass = SpawnMonsterClasses[ClassIndex])
				{
					AMonsterBase* NewMonster = nullptr;
					NewMonster = GetActorFromPoll(MonsterClass);
					if(NewMonster)
					{
						// NewMonster->MakeMonsterActive(NewLocation, CalculateNewMonsterLevel());
						MaxSpawnCount--;
					}
				}
				
			}, 1.0f, true);
		SpawnAtTargetPointTimers.Emplace(SpawnAtTargetPointTimer);
	}



}
void USpawnSystemComponent::SpawnMonsterAtTargetPoint()
{
	//1. 计算需要生成的怪物数量
	int32 MaxNewMonsterCount = CalculateNewMonsterMaxCount();
	//2. 计算新怪物的种类
	int32 MonsterClassIndex = CalculateNewMonsterClassIndex();
	//3. 计算新怪物的等级

	//4. 分配生成任务

	//5. 开始生成
}
int32 USpawnSystemComponent::CalculateNewMonsterLevel()
{
	return 1;
}

int32 USpawnSystemComponent::CalculateNewMonsterClassIndex()
{
	return 0;
}

int32 USpawnSystemComponent::CalculateNewMonsterMaxCount()
{

	return 1;
}
