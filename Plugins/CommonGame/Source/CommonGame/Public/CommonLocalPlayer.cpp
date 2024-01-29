#include "CommonLocalPlayer.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(CommonLocalPlayer)

UCommonLocalPlayer::UCommonLocalPlayer() 
	// 여기 Super()로 넘기는 과정에서 FObjectInitializer::Get() 통해 넘겼다
	// - 이는 우리가 UCommonLocalPlayer()를 상속받는 LocalPlayer는 굳이 FObjectInitializer를 신경 쓸 필요가 없다.
	// - 만약 상속받는 LocalPlayer에서 FObjectInitializer를 통해 뭔가 추가 SubObject를 정의한다면 이와 같은을 수정해야 한다
	: Super(FObjectInitializer::Get())
{}