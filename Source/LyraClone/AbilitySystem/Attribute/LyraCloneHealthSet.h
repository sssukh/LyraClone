#pragma once

#include "LyraCloneAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "LyraCloneHealthSet.generated.h"

// 아래의 HealthSet은 의미 그대로, 체력에 대한 속성값을 관리한다

UCLASS(BlueprintType)
class ULyraCloneHealthSet : public ULyraCloneAttributeSet
{
	GENERATED_BODY()
public :
	ULyraCloneHealthSet();

	// 앞서 LyraCloneAttributeSet에서 정의했던, ATTRIBUTE_ACCESSORS를 통해, 아래 정의한 멤버변수와 똑같이 이름을 설정한다
	// - ATTRIBUTE_ACCESSORS의 Macro의 정의부분을 한번 살펴보자

	ATTRIBUTE_ACCESSORS(ULyraCloneHealthSet, Health);
	ATTRIBUTE_ACCESSORS(ULyraCloneHealthSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(ULyraCloneHealthSet, Healing);

	// 현재 체력
	UPROPERTY(BlueprintReadOnly, Category = "LyraClone|Health")
	FGameplayAttributeData Health;

	// 체력 최대치
	UPROPERTY(BlueprintReadOnly, Category = "LyraClone|Health")
	FGameplayAttributeData MaxHealth;

	// 체력 회복치
	UPROPERTY(BlueprintReadOnly, Category = "LyraClone|Health")
	FGameplayAttributeData Healing;
};