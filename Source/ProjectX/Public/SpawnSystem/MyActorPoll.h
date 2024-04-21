// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PreHeader.h"
#include "Engine/World.h"
/**
 * 
 */
class AActor;
class UWorld;

template<class Object = AActor>
class PROJECTX_API MyActorPoll
{
public:
	MyActorPoll() {}
	MyActorPoll(UWorld* CurrentWorld, const int32 InitCount, const FString& ActorSourcePath)
		: M_CurrentWorld(CurrentWorld)
	{
		if(M_CurrentWorld == nullptr)
		{
			DEBUG_LOG("Construct : CurrentWorld is nullptr");
			return;
		}
		M_ActorSourcePath = ActorSourcePath + "_C";
		//创建对象池
		CreateNewActorToPoll(InitCount);
	}
	MyActorPoll(UWorld* CurrentWorld, const int32 InitCount, TSubclassOf<AActor> ActorClassInPoll)
		:M_CurrentWorld(CurrentWorld)
	{
		DEBUG_LOG("Construct : InitCount : %d",InitCount);
		if (M_CurrentWorld == nullptr)
		{
			DEBUG_LOG("Construct : CurrentWorld is nullptr");
			return;
		}
		if(!ActorClassInPoll)
		{
			DEBUG_LOG("Construct : ActorClassInPoll is nullptr")
		}
		M_CurrentLoadObject = ActorClassInPoll;

		CreateNewActorToPoll(InitCount);
		CurrentWorld->GetTimerManager().SetTimer(ClearTimerHandle, [&]()
			{
				ClearActor();
			}, 2.0f,true);
		if(!ClearTimerHandle.IsValid())
		{
			DEBUG_LOG("Construct : TimerHandle is UnVaild");
		}
	}
	~MyActorPoll()
	{
		M_GameObjectPool.~TArray();
		M_GameObjectPoolBuff.~TArray();
	}
	friend class AProjectXModeBase;
	friend class APlayerBase;
	void PrintCountOfActorPoll()
	{
		FString string = FString("Current the Count of Object which in Poll") + FString::FormatAsNumber(M_GameObjectPool.Num());
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, string);
		DEBUG_LOG("%s", *string);
	}
	//初始化对象池
	void InitActorPoll(UWorld* CurrentWorld, const int32 InitCount, const FString& ActorSourcePath)
	{
		if(CurrentWorld==nullptr)
		{
			DEBUG_LOG("Construct : CurrentWorld is nullptr");
			return;
		}
		M_CurrentWorld = CurrentWorld;
		M_MaxInitCount = InitCount;
		M_ActorSourcePath = ActorSourcePath;

		M_GameObjectPool.RemoveAll();
		M_GameObjectPoolBuff.RemoveAll();


		CreateNewActorToPoll(InitCount);
	};
	//从对象池中获取一个对象
	Object* GetActor()
	{
		if(!ClearTimerHandle.IsValid())
		{
			DEBUG_LOG("GetActor : Timer is Vaild");
			return nullptr;
		}
		M_CurrentWorld->GetTimerManager().PauseTimer(ClearTimerHandle);
		Object* Temp;
		//��������û�ж����Actorʱ
		if(M_GameObjectPool.Num()<=0)
		{
			int32 InitCount = M_GameObjectPoolBuff.Num() / 2 > 0 ? M_GameObjectPoolBuff.Num() / 2 : 1;
			CreateNewActorToPoll(InitCount);
			Temp = GetActorFromPoll();
			M_CurrentWorld->GetTimerManager().UnPauseTimer(ClearTimerHandle);
			return Temp;
		}

		//�����������㹻��Actorʱ
		Temp = GetActorFromPoll();
		if(!Temp)
		{
			DEBUG_LOG("CreateActor : GetActorFromObjectPoll : don't get vaild Object ptr");
			return nullptr;
		}
		M_CurrentWorld->GetTimerManager().UnPauseTimer(ClearTimerHandle);
		return Temp;
	}
	//回收被隐藏的对象 
	void ClearActor()
	{
		for (int i = 0; i < M_GameObjectPoolBuff.Num(); ++i)
		{
			Object* Temp = M_GameObjectPoolBuff[i];
			if(!Temp)
			{
				DEBUG_LOG("ClearActor : Temp is UnVaild");
				return;
			}
			if (!Temp->IsHidden())
			{
				continue;
			}
			if (M_GameObjectPool.Num() < M_MaxInitCount)
			{	
				M_GameObjectPool.Emplace(Temp);
				M_GameObjectPoolBuff.Remove(Temp);
				continue;
			}
		
			M_GameObjectPoolBuff.Remove(Temp);
			Temp->Destroy();
		}
	}
protected:
	Object* GetActorFromPoll()
	{
		if (M_GameObjectPool.Num()<=0) return nullptr;

		Object* Temp = M_GameObjectPool[0];
		M_GameObjectPool.Remove(Temp);
		Temp->SetHidden(false);
		M_GameObjectPoolBuff.Emplace(Temp);
		return Temp;
	}
	bool ActorPoolIsEmpty()
	{
		return M_GameObjectPool.IsEmpty();
	}
	Object* SpawnNewActor()
	{
		if(M_CurrentLoadObject)
		{  
			//DEBUG_LOG(LogTemp, Warning, TEXT("SpawnNewActor : F :%s"), *M_CurrentLoadObject->GetName());
			Object * Temp = M_CurrentWorld->SpawnActor<Object>(M_CurrentLoadObject);
			if(Temp)
			{
				Temp->SetHidden(true);
			}else
			{
				DEBUG_LOG("SpawnNewActor : Fail to SpawnActor");
			}
			return Temp;
		}
		DEBUG_LOG("SpawnNewActor : M_CurrentLoadObject is UnVaild");
		return nullptr;
	}
	void CreateNewActorToPoll(const int32 InitObjectCount)
	{
		M_MaxInitCount += InitObjectCount;
		for (int i = M_CurrentInitCount; i < M_MaxInitCount; ++i)
		{
			Object* Temp = this->SpawnNewActor();

			if(!Temp)
			{
				DEBUG_LOG("CreatNewObjectToObjectPoll : Spawn Error");
				continue;
			}
			
			M_GameObjectPool.Emplace(Temp);
			DEBUG_LOG("CreatNewObjectToObjectPoll : M_GameObjectPoll's Num : %d",M_GameObjectPool.Num());
			
		}
		M_CurrentInitCount += M_GameObjectPool.Num();
	}
	

private:
	
	UWorld* M_CurrentWorld = nullptr;
	
	FString M_ActorSourcePath;
	
	UClass* M_CurrentLoadObject = nullptr;
	
	int32 M_MaxInitCount = 0;
	int32 M_CurrentInitCount = 0;
	
	TArray<Object*> M_GameObjectPoolBuff;
	
	TArray<Object*> M_GameObjectPool;
	
	FTimerHandle ClearTimerHandle;
};


