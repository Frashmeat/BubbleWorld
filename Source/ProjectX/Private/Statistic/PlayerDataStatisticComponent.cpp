// Fill out your copyright notice in the Description page of Project Settings.


#include "Statistic/PlayerDataStatisticComponent.h"

// Sets default values for this component's properties
UPlayerDataStatisticComponent::UPlayerDataStatisticComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerDataStatisticComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UPlayerDataStatisticComponent::CollectPlayerData(APlayerBase* Player)
{

}


// Called every frame
void UPlayerDataStatisticComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

