#include "LyraCloneHealthSet.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneHealthSet)

ULyraCloneHealthSet::ULyraCloneHealthSet()	
	:Super()
	// 우리는 초기값으로 50.0f의 체력으로 맞추어 놓는다
	, Health(50.0f)
	// 최대 체력으로 100으로 설정:
	// - 참고로 버프나, 장비로 인해 최대 체력을 변경 가능하다
	, MaxHealth(100.0f)
{}