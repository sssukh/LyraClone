#pragma once

#include "Engine/LocalPlayer.h"
#include "Delegates/Delegate.h"
#include "CommonLocalPlayer.generated.h"

UCLASS()
class COMMONGAME_API UCommonLocalPlayer : public ULocalPlayer
{
	GENERATED_BODY()
public:

	// 여기서 주목해보면 좋을 점들은 패턴이다 : 우리는 여기서 FObjectInitializer::Get()과 같이 넘기지 않았다.
	UCommonLocalPlayer();

	// Player controller가 local player에 할당(assign)되었을 경우 실행할 Delegate
	DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerControllerSetDelegate, UCommonLocalPlayer* LocalPlayer, APlayerController* PlayerController)
	FPlayerControllerSetDelegate OnPlayerControllerSet;
};