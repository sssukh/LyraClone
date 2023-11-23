// Copyright Epic Games, Inc. All Rights Reserved.

#include "LyraClone.h"
#include "Modules/ModuleManager.h"
#include "System/LyraCloneAssetManager.h"

/**
 * FLyraGameModule
 */
class FLyraCloneGameModule : public FDefaultGameModuleImpl
{
	virtual void StartupModule() override;
	

	virtual void ShutdownModule() override;
	
};


void FLyraCloneGameModule::StartupModule()
{
	UE_LOG(LogTemp, Warning, TEXT("StartupModule"));
	ULyraCloneAssetManager::TestClone();
}

void FLyraCloneGameModule::ShutdownModule()
{
	UE_LOG(LogTemp, Warning, TEXT("ShutdownModule"));
}


IMPLEMENT_PRIMARY_GAME_MODULE(FLyraCloneGameModule, LyraClone, "LyraClone");
