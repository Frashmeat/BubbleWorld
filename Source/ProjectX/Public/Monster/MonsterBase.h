// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterInterface.h"
#include "GameFramework/Character.h"
#include "MonsterBase.generated.h"


class APlayerBase;
class AProjectXModeBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMonsterDeadDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMonsterCorpseMoveToLocationDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNewMonsterSpawn,FVector,NewMonsterLocation,int32,NewMonsterLevel);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMonsterDyingDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMonsterShieldBrokenDelegate);


USTRUCT(BlueprintType)
struct FMonsterBaseProperty
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	float MaxHealth;
	UPROPERTY(EditAnywhere)
	float MaxShield;
	UPROPERTY(EditAnywhere)
	float MaxMoveSpeed;
	UPROPERTY(EditAnywhere)
	int32 Armour;
};


USTRUCT(BlueprintType)
struct FMonsterAttackProperty
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	float AttackValue;
	UPROPERTY(EditAnywhere)
	float AttackBuildPercentage;
	UPROPERTY(EditAnywhere)
	float AttackPlayerPercentage;
};


UENUM(BlueprintType)
enum class EMonsterLifeState :uint8
{
	ReadyToSpawn,
	Spawning,
	Active,
	Dying,
	Dead
};

class UMonsterSpawnSystemComponent;
UCLASS()
class PROJECTX_API AMonsterBase : public ACharacter , public IMonsterInterface
{
	GENERATED_BODY()


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	friend class UPOCSpawnComponent;
	// Sets default values for this character's properties
	AMonsterBase();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	/*
	 *  事件相关
	 */
public:
	//怪物死亡时调用
	UFUNCTION(BlueprintCallable)
	virtual void OnDead(); 

	//怪物濒死时调用
	UFUNCTION(BlueprintCallable)
	virtual void OnDying(); 

	//怪物生成时调用
	UFUNCTION(BlueprintCallable)
	virtual void OnSpawned(FVector NewLocation, int32 NewMonsterLevel);

	//怪物护盾破损后调用
	UFUNCTION(BlueprintCallable)
	virtual void OnShieldBroken();

	UPROPERTY(BlueprintAssignable)
	FMonsterCorpseMoveToLocationDelegate MonsterCorpseMoveToLocationDelegate;
	UPROPERTY(BlueprintAssignable)
	FMonsterDeadDelegate MonsterDeadDelegate;
	UPROPERTY(BlueprintAssignable)
	FNewMonsterSpawn NewMonsterSpawnDelegate;
	UPROPERTY(BlueprintAssignable)
	FMonsterDyingDelegate MonsterDyingDelegate;
	UPROPERTY(BlueprintAssignable)
	FMonsterShieldBrokenDelegate MonsterShieldBrokenDelegate;

	/*
	 *  生成相关
	 */
public:
	//计算怪物的属性
	virtual void CalculateMonsterProperty();

	//设置可实现
	virtual void SetUnHiddenInGame();
	virtual void SetHiddenInGame();
	
	//让怪物可以运行
	virtual void MakeMonsterSpawn(const FVector& NewLocation, int32 NewMonsterLevel); 
	

	
	/*
	 *  生成 属性相关
	 */
public:
	//获取怪物的生命状态
	UFUNCTION(BlueprintCallable)
	EMonsterLifeState GetMonsterLifeState() const;
	
	//获取怪物的等级
	UFUNCTION(BlueprintCallable)
	int32 GetMonsterLevel() const;
	
	//怪物生成时 生成的可攻击对象
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<AActor> MonsterEggClass;
	
	static constexpr int32 MaxMonsterLevel = 5;
protected:
	//怪物生命状态
	UPROPERTY(BlueprintReadOnly)
	EMonsterLifeState MonsterLifeState = EMonsterLifeState::Active;
	//怪物等级
	UPROPERTY(BlueprintReadOnly)
	int32 MonsterLevel = 0;

	/*
	 *	逻辑 属性相关 
	 */
public:
	
	//怪物的名称
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName MonsterName;
	
	//基本的 怪物最大基础属性
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FMonsterBaseProperty BasicMonsterBaseProperty;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FMonsterAttackProperty BasicMonsterAttackProperty;
	//当前血量
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Health = 0.f;
	//当前护盾
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Shield = 0.f;

	//获取当前我护甲等级
	UFUNCTION(BlueprintCallable)
	int GetArmor() const;
	
protected:
	//真实的 怪物最大基本属性
	FMonsterBaseProperty RealMonsterBaseProperty;

	FMonsterAttackProperty RealMonsterAttackProperty;

	
	/*
	 *	与其他模块交流
	 */
public:
	AProjectXModeBase * GameMode;
protected:
};
