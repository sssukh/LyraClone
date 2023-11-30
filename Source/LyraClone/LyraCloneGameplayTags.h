#pragma once

#include "Containers/UnrealString.h"
#include "Containers/Map.h"
#include "GameplayTagContainer.h"

// 전방선언
class UGameplayTagsManager;

/**
* FLyraCloneGameplayTags
* - singleton containing native gameplay tags
*/

struct FLyraCloneGameplayTags
{
	/**
	* static methods
	*/
	static const FLyraCloneGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeTags();

	/** 
	* member methods
	*/
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);
	void AddAllTags(UGameplayTagsManager& Manager);

	/**
	 * 아래의 GameplayTag는 초기화 과정 단계를 의미한다
	 * GameInstance의 초기화 과정에 UGameFrameworkComponentManager의 RegisterInitState로 등록된다
	 * 이 초기화 GameplayTag는 게임의 Actor사이에 공유되며, GameFrameworkInitStateInterface 상속
	 */

	FGameplayTag InitState_Spawned;
	FGameplayTag InitState_DataAvailable;
	FGameplayTag InitState_DataInitialized;
	FGameplayTag InitState_GameplayReady;

private:
	// static 변수 초기화는 .cpp에 해주는 것을 잊지 말기!
	static FLyraCloneGameplayTags GameplayTags;

};
