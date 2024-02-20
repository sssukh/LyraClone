#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "GameUIManagerSubsystem.generated.h"

/**
* GameUIManagerSubsystem은 GameInstanceSubsystem의 기반한다.
* - 여기서 주목해야 할 점은 UGameUIManagerSubsystem은 UCLASS 속성으로 Abstract라고 정의되어 있다
* - 해당 클래스는 단독으로 사용 불가하며, 누군가 상속받은 Concrete Class로서 활용되야 한다.
*/

class UGameUIPolicy;
class UCommonLocalPlayer;

UCLASS(Abstract, config=Game)
class COMMONGAME_API UGameUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UGameUIManagerSubsystem();
	const UGameUIPolicy* GetCurrentUIPolicy() const { return CurrentPolicy; }
	UGameUIPolicy* GetCurrentUIPolicy() { return CurrentPolicy; }

	void SwitchToPolicy(UGameUIPolicy* InPolicy);

	/**
	* UGameInstanceSubsystem's interface
	*/
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	/**
	* UGameInstanceSubsystem's interfaces
	*/
	void NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer);
	void NotifyPlayerRemoved(UCommonLocalPlayer* LocalPlayer);
	void NotifyPlayerDestroyed(UCommonLocalPlayer* LocalPlayer);

	UPROPERTY(Transient)
	TObjectPtr<UGameUIPolicy> CurrentPolicy = nullptr;

	/**
	* default UI Policy를 생성할 class
	* - 우리는 해당 클래스는 B_BttGameUIPolicy로 지정할 것이다
	*/
	UPROPERTY(Config, EditAnywhere)
	TSoftClassPtr<UGameUIPolicy> DefaultUIPolicyClass;
};


