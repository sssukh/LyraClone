#include "LyraCloneUserFacingExperienceDefinition.h"
#include "../../../../../../Game/LyraClone/Plugins/CommonUser/Source/CommonUser/Public/CommonSessionSubsystem.h"

// 항상 generated.h 파일이 헤더에 포함되어 있다면, gen.cpp도 포함시키도록 한다!
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneUserFacingExperienceDefinition)

UCommonSession_HostSessionRequest* ULyraCloneUserFacingExperienceDefinition::CreateHostingRequest() const
{
	const FString ExperienceName = ExperienceID.PrimaryAssetName.ToString();
	UCommonSession_HostSessionRequest* Result = NewObject<UCommonSession_HostSessionRequest>();
	Result->MapID = MapID;
	Result->ExtraArgs.Add(TEXT("Experience"), ExperienceName);
	return Result;
}
