// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnSystem/POCSpawnComponent.h"
#include "PreHeader.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Monster/MonsterBase.h"
#include "Monster/NormalMonster.h"
#include "SpawnSystem/SpawnMonsterByMonsterTask.h"


// Sets default values for this component's properties
UPOCSpawnComponent::UPOCSpawnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	if(ANormalMonster * TempOwner = Cast<ANormalMonster>(GetOwner()))	
	{
		MonsterOwner = TempOwner;
	}else
	{
		DEBUG_LOG("Fail to Get Onwer");
		return;
	}
	MonsterOwner->MonsterDyingDelegate.AddDynamic(this,&UPOCSpawnComponent::BeginPOCSpawn);
}


// Called when the game starts
void UPOCSpawnComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	SpawnManagerComponent = Cast<AProjectXModeBase>(UGameplayStatics::GetGameMode(this))->SpawnManagerComponent;
}


// Called every frame
void UPOCSpawnComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

int32 UPOCSpawnComponent::GetMaxMonsterLevel()
{
	return MaxMonsterLevel;
}

int32 UPOCSpawnComponent::CalculateMinNumOfGeneration()
{
	int32 Difference = MaxMonsterLevel - MonsterOwner->MonsterLevel;
	return Difference > 0 ? Difference : 1;
}

bool UPOCSpawnComponent::DetectDyingMonstersAround(TArray<ANormalMonster*> DyingMonsters)
{
	//1. 进行盒体检测
	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> HitResults;
	UKismetSystemLibrary::BoxTraceMulti(this,  MonsterOwner->GetActorLocation(), MonsterOwner->GetActorLocation(),
		DetectRange,{0,0,0},
		ETraceTypeQuery::TraceTypeQuery1,false, 
		ActorsToIgnore,EDrawDebugTrace::ForDuration,HitResults,false);
	//2. 对检测到的对象就行筛选
	for (FHitResult HitResult : HitResults)
	{	//2.1 筛选Dying状态的Monster
		if (ANormalMonster* Monster = Cast<ANormalMonster>(HitResult.GetActor()))
		{
			if(Monster->GetMonsterLifeState()==EMonsterLifeState::Dying)
			{
				DyingMonsters.Emplace(Monster);
			}
		}
	}
	//3. 判断符合条件的Monster是否足够
	if(DyingMonsters.Num() > CalculateMinNumOfGeneration())
	{
		return true;
	}
	
	return false;
}

void UPOCSpawnComponent::GenerateNewMonster(TArray<ANormalMonster*> DyingMonsters)
{
	//创建生成任务
	USpawnMonsterByMonsterTask* NewTask = NewObject<USpawnMonsterByMonsterTask>();
	if(NewTask)
	{
		NewTask->InitTask(ESpawnTaskType::SpawnMonsterByMonster,DyingMonsters);
	}else
	{
		DEBUG_LOG(__FUNCDNAME__" : Fail to Create NewTask");
	}
	//添加到生成系统中
	SpawnManagerComponent->AddSpawnTask(NewTask);
}

void UPOCSpawnComponent::BeginPOCSpawn()
{
	//1. 检测周围DyingMonster 是否符合条件
	TArray<ANormalMonster*> DyingMonsters;
	//2. 符合条件 则开始生成
	if(bool CanSpawnNewMonster = DetectDyingMonstersAround(DyingMonsters))
	{
		//2.1 对检测的到的怪物进行处理
		for (auto DyingMonster : DyingMonsters)
		{
			GetWorld()->GetTimerManager().ClearTimer(DyingMonster->ToDieTimer);
			DyingMonster->MonsterLifeState = EMonsterLifeState::Spawning;
		}
		//2.2 开始生成
		GenerateNewMonster(DyingMonsters);
	}else
	{
		//3. 不符合 则怪物死亡开始计时
		GetWorld()->GetTimerManager().SetTimer( MonsterOwner->ToDieTimer ,MonsterOwner,&ANormalMonster::OnDead,DyingTime,false);
	}
}

