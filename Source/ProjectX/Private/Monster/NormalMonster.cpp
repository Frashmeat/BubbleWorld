// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/NormalMonster.h"
#include "PreHeader.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Monster/BossMonster.h"
#include "SpawnSystem/SpawnMonsterByMonsterTask.h"


// Sets default values
ANormalMonster::ANormalMonster()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ANormalMonster::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ANormalMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ANormalMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}



void ANormalMonster::OnDying()
{
	Super::OnDying();
	DetectDyingMonsterToSpawnNewMonster();
}

int32 ANormalMonster::CalculateMinNumOfGeneration()
{
	int32 Difference = MaxMonsterLevel - MonsterLevel;
	return Difference > 0 ? Difference : 1;
}

bool ANormalMonster::DetectDyingMonstersAround(TArray<ANormalMonster*> & DyingMonsters)
{
	//1. 进行盒体检测
	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> HitResults;
	UKismetSystemLibrary::BoxTraceMulti(this, GetActorLocation(), GetActorLocation(),
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

void ANormalMonster::GenerateNewMonster(TArray<ANormalMonster*>& DyingMonsters)
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
	GameMode->AddTaskToSpawnSystem(NewTask);
}

void ANormalMonster::DetectDyingMonsterToSpawnNewMonster()
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
		GetWorld()->GetTimerManager().SetTimer(ToDieTimer,this,&ANormalMonster::OnDead,DyingTime,false);
	}
	
}

