// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
template<class T>
class PROJECTX_API MyTestClass
{
public:
	MyTestClass(): TestValue(nullptr)
	{

	}
	~MyTestClass(){};
	T* TestValue;
};
