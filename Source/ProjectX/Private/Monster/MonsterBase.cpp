// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MonsterBase.h"
#include "GameMode/ProjectXModeBase.h"
#include "PreHeader.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerBase.h"


// Sets default values
AMonsterBase::AMonsterBase(): GameMode(nullptr)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	
	//获取GameMode对象
	GameMode = Cast<AProjectXModeBase> (UGameplayStatics::GetGameMode(this));
	if(!GameMode)
	{
		DEBUG_LOG(__FUNCDNAME__" : Fail to Get GameMode");
		return;
	}
	
	//属性初始化
	RealMonsterBaseProperty = BasicMonsterBaseProperty;
	Health = RealMonsterBaseProperty.MaxHealth;
	Shield = RealMonsterBaseProperty.MaxShield;
	RealMonsterAttackProperty = BasicMonsterAttackProperty;
	
}
// Called every frame
void AMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
// Called to bind functionality to input
void AMonsterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}



void AMonsterBase::OnDead()
{
	MonsterLifeState = EMonsterLifeState::Dead;
	this->SetHiddenInGame();
	
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetupAttachment(RootComponent);
	
	MonsterDeadDelegate.Broadcast();
}
void AMonsterBase::OnDying()
{
	MonsterLifeState = EMonsterLifeState::Dying;
	
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetSimulatePhysics(true);
	
	MonsterDyingDelegate.Broadcast();
}
void AMonsterBase::OnSpawned(FVector NewLocation,int32 NewMonsterLevel)
{
	//设置状态
	MonsterLifeState = EMonsterLifeState::Active;
	//设置怪物等级
	MonsterLevel = NewMonsterLevel;
	//设置可视性
	SetUnHiddenInGame();
	//计算属性
	CalculateMonsterProperty();
	//设定新位置
	SetActorLocation(NewLocation);
	//设置碰撞
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	NewMonsterSpawnDelegate.Broadcast(NewLocation,NewMonsterLevel);
}
void AMonsterBase::OnShieldBroken()
{
	MonsterShieldBrokenDelegate.Broadcast();
}



void AMonsterBase::MakeMonsterSpawn(const FVector& NewLocation, const int32 NewMonsterLevel)
{
	OnSpawned(NewLocation,NewMonsterLevel);
}
void AMonsterBase::CalculateMonsterProperty()
{
	
}
void AMonsterBase::SetUnHiddenInGame()
{
	SetHidden(false);
	GetMesh()->SetHiddenInGame(false);
}
void AMonsterBase::SetHiddenInGame()
{
	SetHidden(true);
	GetMesh()->SetHiddenInGame(true);
}

EMonsterLifeState AMonsterBase::GetMonsterLifeState() const
{
	return MonsterLifeState;
}

int32 AMonsterBase::GetMonsterLevel() const
{
	return MonsterLevel;
}

int AMonsterBase::GetArmor() const
{
	return RealMonsterBaseProperty.Armour;
}



