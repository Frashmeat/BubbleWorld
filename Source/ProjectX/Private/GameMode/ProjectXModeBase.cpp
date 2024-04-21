// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ProjectXModeBase.h"
#include "PreHeader.h"
#include "SpawnSystem/SpawnManagerComponent.h"


AProjectXModeBase::AProjectXModeBase()
{
	
	SpawnManagerComponent = CreateDefaultSubobject<USpawnManagerComponent>(FName("SpawnManagerComponent"));
	if(!SpawnManagerComponent)
	{
		DEBUG_LOG(__FUNCDNAME__" : Fail to Create SpawnManagerComponent");
		return;
	}	
}

void AProjectXModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectXModeBase::AddTaskToSpawnSystem(USpawnTaskBase* NewSpawnTask)
{
	SpawnManagerComponent->AddSpawnTask(NewSpawnTask);
}

