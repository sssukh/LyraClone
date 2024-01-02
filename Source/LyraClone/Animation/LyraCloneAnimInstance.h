#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LyraCloneAnimInstance.generated.h"

UCLASS()
class ULyraCloneAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	ULyraCloneAnimInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// 해당 속성값은 Lyra의 AnimBP에서  사용되는 값이므로 정의해주자
	UPROPERTY(BlueprintReadOnly, Category = "Character State Data")
	float GroundDistance = -1.0f;
};
