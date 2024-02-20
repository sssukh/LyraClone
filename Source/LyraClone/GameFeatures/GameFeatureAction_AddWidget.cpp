#include "GameFeatureAction_AddWidget.h"
#include "LyraClone/UI/LyraCloneHUD.h"
#include "CommonUIExtensions.h"
#include "UIExtensionSystem.h"
#include "Components/GameFrameworkComponentManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(GameFeatureAction_AddWidget)

void UGameFeatureAction_AddWidgets::AddWidget(AActor* Actor, FPerContextData& ActiveData)
{
	ALyraCloneHUD* HUD = CastChecked<ALyraCloneHUD>(Actor);

	// HUD를 통해, LocalPlayer을 가져오자
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(HUD->GetOwningPlayerController()->Player))
	{
		// Layout의 요청을 순회하자(보통 하나만 들어가 있긴하다)
		for (const FLyraCloneHUDLayoutRequest& Entry : Layout)
		{
			if (TSubclassOf<UCommonActivatableWidget> ConcreteWidgetClass = Entry.LayoutClass.Get())
			{
				ActiveData.LayoutsAdded.Add(UCommonUIExtensions::PushContentToLayer_ForPlayer(LocalPlayer, Entry.LayerID, ConcreteWidgetClass));
			}
		}

		// Widget을 순회하며, UIExtensionSubsystem의 Extension에 추가한다
		UUIExtensionSubsystem* ExtensionSubsystem = HUD->GetWorld()->GetSubsystem<UUIExtensionSubsystem>();
		for (const FLyraCloneHUDElementEntry& Entry : Widgets)
		{
			ActiveData.ExtensionHandles.Add(ExtensionSubsystem->RegisterExtensionAsWidgetForContext(Entry.SlotID, LocalPlayer, Entry.WidgetClass.Get(), -1));
		}
	}
}
void UGameFeatureAction_AddWidgets::RemoveWidgets(AActor* Actor, FPerContextData& ActiveData)
{
	ALyraCloneHUD* HUD = CastChecked<ALyraCloneHUD>(Actor);

	// LyraCloneHUD에 추가된 CommonActivatableWidget을 순회하며, Deactive시켜준다
	for (TWeakObjectPtr<UCommonActivatableWidget>& AddedLayout : ActiveData.LayoutsAdded)
	{
		if(AddedLayout.IsValid())
		{
			AddedLayout->DeactivateWidget();
		} 
	}
	ActiveData.LayoutsAdded.Reset();

	// UIExtension에 대해 순회하며, Unregister() 한다
	for (FUIExtensionHandle& Handle : ActiveData.ExtensionHandles)
	{
		//Unregister()는 UUIExtensionSystem에서 제거가 올바르게 되야 한다
		Handle.Unregister();
	}
	ActiveData.ExtensionHandles.Reset();
}

void UGameFeatureAction_AddWidgets::HandleActorExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext)
{
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);

	// Receiver인 ALyraCloneHUD의 Remove/Added에 따라 Widget을 추가/제거 해준다
	if ((EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved) || (EventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved))
	{
		RemoveWidgets(Actor, ActiveData);
	}
	else if ((EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded) || (EventName == UGameFrameworkComponentManager::NAME_GameActorReady))
	{
		AddWidget(Actor, ActiveData);
	}
}


void UGameFeatureAction_AddWidgets::AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext)
{
	UWorld* World = WorldContext.World();
	UGameInstance* GameInstance = WorldContext.OwningGameInstance;
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);

	// GameFrameworkComponentManager를 가져올 GameInstance와 World는 GameWorld여야 한다
	if ((GameInstance != nullptr) && (World != nullptr) && World->IsGameWorld())
	{
		//GameFrameworkComponentManager를 가져옴
		if (UGameFrameworkComponentManager* ComponentManager = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
		{
			// 기본적으로 Widget을 추가할 대상으로 ALyraCloneHUD로 고정한다
			TSoftClassPtr<AActor> HUDActorClass = ALyraCloneHUD::StaticClass();

			// GFA_WorldBase와 마찬가지로 HandleActorExtension을 콜백으로 받도록 하자
			TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle = ComponentManager->AddExtensionHandler(
				HUDActorClass,
				UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &ThisClass::HandleActorExtension, ChangeContext));

			ActiveData.ComponentRequests.Add(ExtensionRequestHandle);
		}
	}
}

void UGameFeatureAction_AddWidgets::Reset(FPerContextData& ActiveData)
{
	ActiveData.ComponentRequests.Empty();
	ActiveData.LayoutsAdded.Empty();

	for (FUIExtensionHandle& Handle : ActiveData.ExtensionHandles)
	{
		Handle.Unregister();
	}
	ActiveData.ExtensionHandles.Reset();
}

void UGameFeatureAction_AddWidgets::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);

	FPerContextData* ActiveData = ContextData.Find(Context);
	if (ensure(ActiveData))
	{
		Reset(*ActiveData);
	}
}