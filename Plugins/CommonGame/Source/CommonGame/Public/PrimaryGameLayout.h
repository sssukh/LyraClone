#pragma once

#include "CommonUserWidget.h"
#include "PrimaryGameLayout.generated.h"

/**
* 인게임에서 메인 UI의 레이아웃을 담당하는 UMG이다 (Slate vs UMG를 이해하자 - 간단하게 UObject을 기반하는가 아닌가 차이)
* - PrimaryGameLayout은 플레이어당 하나씩 가지는 최상위 UI 레이아웃으로 이해하면 된다
* - PrimaryGameLayout는 레이어 기반으로 UI를 관리한다
*/
UCLASS(Abstract)
class COMMONGAME_API UPrimaryGameLayout : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	// 해당 클래스는 Abstract로 설정되어있으므로 굳이 FObjectInitializer::Get()할 필요는 없다
	UPrimaryGameLayout(const FObjectInitializer& ObjectInitializer);
};