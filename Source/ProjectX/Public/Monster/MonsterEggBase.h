// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterEggBase.generated.h"

class USpawnMonsterByMonsterTask;

UCLASS()
class PROJECTX_API AMonsterEggBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMonsterEggBase();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;



public:
	
	virtual void Init(float MaxHealth , int32 ArmorLevel , USpawnMonsterByMonsterTask * SpawnTask);
	
	virtual void GetDamage(float BaseDamage , int32 ArmorPiercingLevel);
	
	virtual void OnDestroy();
	
	UStaticMeshComponent * EggMesh;
protected:
	USpawnMonsterByMonsterTask * SpawnMonsterByMonsterTask;
	float Health = 0;
	int32 Armor = 0;
};
