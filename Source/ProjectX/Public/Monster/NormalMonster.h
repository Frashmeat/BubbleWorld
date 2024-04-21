// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterBase.h"
#include "NormalMonster.generated.h"

UCLASS()
class PROJECTX_API ANormalMonster : public AMonsterBase
{
	GENERATED_BODY()

public:
	friend class UPOCSpawnComponent;
	// Sets default values for this character's properties
	ANormalMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
public:
	
	virtual void OnDying() override;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float DyingTime = 1.0f;
	
public:
	
	FVector DetectRange = {100,100,50}; 
private:
	int32 CalculateMinNumOfGeneration();
	bool DetectDyingMonstersAround(TArray<ANormalMonster*>& DyingMonsters);
	void GenerateNewMonster(TArray<ANormalMonster*> &DyingMonsters);
	void DetectDyingMonsterToSpawnNewMonster();
	FTimerHandle ToDieTimer;
};
