// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnSystem/MonsterSpawnSystemComponent.h"
#include "PreHeader.h"
#include "GameMode/ProjectXModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Monster/NormalMonster.h"
#include "SpawnSystem/SpawnManagerComponent.h"
#include "SpawnSystem/SpawnMonsterByMonsterTask.h"
#include "SpawnSystem/SpawnTaskBase.h"

// Sets default values for this component's properties
UMonsterSpawnSystemComponent::UMonsterSpawnSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}
// Called when the game starts
void UMonsterSpawnSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	// ... 
	AProjectXModeBase* GameMode = Cast<AProjectXModeBase>(UGameplayStatics::GetGameMode(this));
	if (!GameMode) DEBUG_LOG("BeginPlay : Fail to Get GameMode");
	
	
	OwnedMonster = Cast<ANormalMonster>(GetOwner());
	if (!OwnedMonster)
	{
		DEBUG_LOG("OwnedMonster Fali to Get");
		return;
	}
	//OwnedMonster->MonsterDyingDelegate.AddUObject(this, &UMonsterSpawnSystemComponent::DetectDyingMonsterToSpawnNewMonster);

	
	SpawnManagerComponent = GameMode->SpawnManagerComponent;
	if(SpawnManagerComponent)
	{
		DEBUG_LOG(__FUNCDNAME__" : Fail to Get SpawnManagerComponent");
	}
	
}

void UMonsterSpawnSystemComponent::DetectDyingMonsterToSpawnNewMonster()
{
	bool CanSpawn = false;
	TArray<AMonsterBase*> DyingMonsters;
	CanSpawn = CanSpawnNewMonster(DyingMonsters);
	
	if(!CanSpawn)
	{
		GetWorld()->GetTimerManager().SetTimer(ToDieTimer, [&]()
			{
				DEBUG_LOG("ToDie Timer Active");
				OwnedMonster->OnDead();
			}, 2.0f, false);
		return;
	}
	
	BeginToSpawn(DyingMonsters);
}

// Called every frame
void UMonsterSpawnSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}




bool UMonsterSpawnSystemComponent::CanSpawnNewMonster(TArray<AMonsterBase*>& DyingMonsters)
{
	DetectDyingMonster(DyingMonsters);
	if (DyingMonsters.IsEmpty()) return false;
	if (DyingMonsters.Num() > ((MAX_MONSTER_LEVEL - OwnedMonster->GetMonsterLevel() ) * 2))
	{
		for (AMonsterBase* DyingMonster : DyingMonsters)
		{
			// GetWorld()->GetTimerManager().ClearTimer(DyingMonster);
		}
		return true;
	}
	return false;
}
void UMonsterSpawnSystemComponent::DetectDyingMonster(TArray<AMonsterBase*>& DyingMonsters)
{
	AActor* Owner = GetOwner();
	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> HitResults;

	UKismetSystemLibrary::BoxTraceMulti(this, Owner->GetActorLocation(), Owner->GetActorLocation(),
		DetectRange,{0,0,0},
		ETraceTypeQuery::TraceTypeQuery1,false, 
		ActorsToIgnore,EDrawDebugTrace::ForDuration,HitResults,false);

	UMonsterSpawnSystemComponent* MonsterSpawnSystemComponent = nullptr;
	for (FHitResult HitResult : HitResults)
	{
		if (AMonsterBase* Monster = Cast<AMonsterBase>(HitResult.GetActor()))
		{
			if(Monster->GetMonsterLifeState()==EMonsterLifeState::Dying)
			{
				DEBUG_LOG("DetectDyingMonster : %s", *Monster->GetName())
				DyingMonsters.Emplace(Monster);
			}
		}
	}

}
void UMonsterSpawnSystemComponent::BeginToSpawn(TArray<AMonsterBase*>& DyingMonsters)
{
	
	USpawnMonsterByMonsterTask* NewTask = NewObject<USpawnMonsterByMonsterTask>();
	if(NewTask)
	{
		// NewTask->InitTask(ESpawnTaskType::SpawnMonsterByMonster,DyingMonsters);
	}
	SpawnManagerComponent->AddSpawnTask(NewTask);
	
}



