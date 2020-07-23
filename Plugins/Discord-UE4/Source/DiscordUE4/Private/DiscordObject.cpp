// Created by Satheesh (ryanjon2040). Twitter: https://twitter.com/ryanjon2040. Discord: @ryanjon2040#5319


#include "DiscordObject.h"
#include "../discord-files/discord.h"
#include "UObject/Class.h"

DEFINE_LOG_CATEGORY_STATIC(LogDiscord, Log, All)

#define LogDisplay(Param1)		UE_LOG(LogDiscord, Display, TEXT("%s"), *FString(Param1))
#define LogError(Param1)		UE_LOG(LogDiscord, Error, TEXT("%s"), *FString(Param1))

discord::Core* core{};
discord::Activity activity{};

UDiscordObject* UDiscordObject::DiscordObjectInstance = nullptr;


UDiscordObject::UDiscordObject()
{
	bCanTick = bTimerStarted = false;
}

UDiscordObject* UDiscordObject::GetOrCreateDiscordObject(FString InClientID, const bool bRequireDiscordRunning /*= false*/, const bool bStartElapsedTimer /*= true*/)
{
	if (DiscordObjectInstance == nullptr)
	{
		DiscordObjectInstance = NewObject<UDiscordObject>();
		DiscordObjectInstance->AddToRoot();
		DiscordObjectInstance->Internal_CreateDiscordObject(InClientID, bRequireDiscordRunning, bStartElapsedTimer);
	}
	
	return DiscordObjectInstance;
}

void UDiscordObject::DestroyDiscordObject()
{
	if (DiscordObjectInstance)
	{
		DiscordObjectInstance->StopDiscordTimer();
		DiscordObjectInstance->RemoveFromRoot();
		DiscordObjectInstance->ConditionalBeginDestroy();
		DiscordObjectInstance = nullptr;
		LogDisplay("Discord object destroyed.");
	}
}

const FString UDiscordObject::GetDiscordResultString(EDiscordReturnResult InDiscordResult)
{
	return UEnum::GetDisplayValueAsText(InDiscordResult).ToString();
}

void UDiscordObject::SetState(FString InNewState)
{
	activity.SetState(TCHAR_TO_UTF8(*InNewState));
	core->ActivityManager().UpdateActivity(activity, [](discord::Result result)
	{
		uint8 ResultByte = (uint8)result;
		DiscordObjectInstance->OnStateSet.Broadcast(static_cast<EDiscordReturnResult>(ResultByte));
		LogDisplay(FString::Printf(TEXT("State Set Result: %s"), *GetDiscordResultString(static_cast<EDiscordReturnResult>(ResultByte))));
	});
}

void UDiscordObject::SetDetails(FString InNewDetails)
{
	activity.SetDetails(TCHAR_TO_UTF8(*InNewDetails));
	core->ActivityManager().UpdateActivity(activity, [](discord::Result result)
	{
		uint8 ResultByte = (uint8)result;
		DiscordObjectInstance->OnDetailsSet.Broadcast(static_cast<EDiscordReturnResult>(ResultByte));
		LogDisplay(FString::Printf(TEXT("Details Set Result: %s"), *GetDiscordResultString(static_cast<EDiscordReturnResult>(ResultByte))));
	});
}

void UDiscordObject::SetLargeImage(const FString InKeyName)
{
	activity.GetAssets().SetLargeImage(TCHAR_TO_UTF8(*InKeyName));
	core->ActivityManager().UpdateActivity(activity, [](discord::Result result)	
	{
		uint8 ResultByte = (uint8)result;
		DiscordObjectInstance->OnLargeImageSet.Broadcast(static_cast<EDiscordReturnResult>(ResultByte));
		LogDisplay(FString::Printf(TEXT("Large Image Set Result: %s"), *GetDiscordResultString(static_cast<EDiscordReturnResult>(ResultByte))));
	});
}

void UDiscordObject::StartDiscordTimer()
{
	activity.GetTimestamps().SetStart(FDateTime::UtcNow().ToUnixTimestamp());
	core->ActivityManager().UpdateActivity(activity, [](discord::Result result)
	{
		uint8 ResultByte = (uint8)result;
		DiscordObjectInstance->OnTimerStart.Broadcast(static_cast<EDiscordReturnResult>(ResultByte));
		LogDisplay(FString::Printf(TEXT("Timer Start Result: %s"), *GetDiscordResultString(static_cast<EDiscordReturnResult>(ResultByte))));
	});
}

void UDiscordObject::StopDiscordTimer()
{
	DiscordObjectInstance->bTimerStarted = false;
	activity.GetTimestamps().SetEnd(FDateTime::UtcNow().ToUnixTimestamp());
	core->ActivityManager().UpdateActivity(activity, [](discord::Result result)
	{
		uint8 ResultByte = (uint8)result;
		DiscordObjectInstance->OnTimerEnd.Broadcast(static_cast<EDiscordReturnResult>(ResultByte));
		LogDisplay(FString::Printf(TEXT("Timer End Result: %s"), *GetDiscordResultString(static_cast<EDiscordReturnResult>(ResultByte))));
	});
}

void UDiscordObject::Internal_CreateDiscordObject(const FString& InClientID, const bool bRequireDiscordRunning, const bool bStartElapsedTimer)
{	
	discord::Result result = discord::Core::Create(FCString::Atoi64(*InClientID), bRequireDiscordRunning ? DiscordCreateFlags_Default : DiscordCreateFlags_NoRequireDiscord, &core);
	DiscordObjectInstance->bCanTick = true;
	LogDisplay("Discord object created.");

	if (bStartElapsedTimer)
	{
		DiscordObjectInstance->StartDiscordTimer();
	}
}

void UDiscordObject::Tick(float DeltaTime)
{
	if (bCanTick)
	{
		::core->RunCallbacks();
	}
}


#undef LogDisplay
#undef LogError
