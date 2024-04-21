// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MonsterInterface.generated.h"

class APlayerBase;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMonsterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTX_API IMonsterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
	bool TakeDamage(float BaseDamage, float ShieldReduction, int ArmorPiercingLevel);
	
	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
	void Attack(AActor * Victim);
	
	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
	bool Init();
};
