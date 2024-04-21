// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/FactionComponent.h"

#include "AIController.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
UFactionComponent::UFactionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
	OwnerController = Cast<ACharacter>(GetOwner())->Controller;
		
	
}
// Called when the game starts
void UFactionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}
// Called every frame
void UFactionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}


bool UFactionComponent::Init(EFaction Faction)
{
	M_Faction = Faction;
	switch (M_Faction)
	{
	case EFaction::Friendly:
		return FriendlyInit();
		break;
	case EFaction:: Enemy:
		return EnemyInit();
		break;
	case EFaction::Neutral:
		return NeutralInit();
		break;
	case EFaction::Player:
		return PlayerInit();
		break;
	default:
		return false;
		;
	}
}

EFaction UFactionComponent::getFaction() const
{
	return M_Faction;
}

bool UFactionComponent::PlayerInit()
{
	//1. 控制组件
	
	auto Component = GetOwner()->AddComponentByClass(UActorComponent::StaticClass(),false,UKismetMathLibrary::MakeTransform({0,0,0},{0,0,0}),false);
	Component->Rename(TEXT("Component"));
	//2. 摄像机控制组件
	//3. 属性管理组件
	//4. 属性词条组件
	//5. 武器组件
	//6. 武器词条组件
	return true;
}

bool UFactionComponent::EnemyInit()
{
	//1. AIController
	if(!OwnerController || !OwnerController()->GetClass()->IsChildOf(OwnerController))
	{
		OwnerController = NewObject<AAIController>(this);
	}
	
	return true;
}

bool UFactionComponent::FriendlyInit()
{
	return true;
}

bool UFactionComponent::NeutralInit()
{
	return true;
}
