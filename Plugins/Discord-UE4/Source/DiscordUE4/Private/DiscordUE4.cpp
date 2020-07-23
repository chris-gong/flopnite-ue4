// Created by Satheesh (ryanjon2040). Twitter: https://twitter.com/ryanjon2040. Discord: @ryanjon2040#5319

#include "DiscordUE4.h"
#include "Interfaces/IPluginManager.h"
#include "Runtime/Core/Public/Misc/MessageDialog.h"

#define LOCTEXT_NAMESPACE "FDiscordUE4Module"

void* FDiscordUE4Module::DiscordHandle = nullptr;

DEFINE_LOG_CATEGORY_STATIC(LogDiscordUE4, All, All)

#define LOG_NORMAL(StringParam)		UE_LOG(LogDiscordUE4, Display, TEXT("%s"), *FString(StringParam))
#define LOG_ERROR(StringParam)		UE_LOG(LogDiscordUE4, Error, TEXT("%s"), *FString(StringParam))

void FDiscordUE4Module::StartupModule()
{
	LOG_NORMAL("Starting DiscordUE4 module.");

	// Get the base directory of this plugin
	const FString BaseDir = IPluginManager::Get().FindPlugin("DiscordUE4")->GetBaseDir();

	// Add on the relative location of the third party dll and load it

#if PLATFORM_WINDOWS
	const FString DiscordDir = FPaths::Combine(*BaseDir, TEXT("Binaries"), TEXT("Win64"));
#endif
	LOG_NORMAL(FString::Printf(TEXT("Discord directory is %s"), *DiscordDir));

	static const FString DiscordDLLName = "discord_game_sdk";
	const bool bDependencyLoaded = Internal_LoadDependency(DiscordDir, DiscordDLLName, DiscordHandle);
	if (!bDependencyLoaded)
	{
		FFormatNamedArguments Arguments;
		Arguments.Add(TEXT("Name"), FText::FromString(DiscordDLLName));
		Arguments.Add(TEXT("Path"), FText::FromString(DiscordDir));
		FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("LoadDependencyError", "Failed to load {Name} from path {Path}."), Arguments));
		Internal_FreeDependency(DiscordHandle);
	}
}

void FDiscordUE4Module::ShutdownModule()
{
	Internal_FreeDependency(DiscordHandle);
	LOG_NORMAL("Shutting down Discord Module.");
}

bool FDiscordUE4Module::Internal_LoadDependency(const FString& Dir, const FString& Name, void*& Handle)
{
#if PLATFORM_WINDOWS || PLATFORM_MAC
	FString Lib = Name + TEXT(".") + FPlatformProcess::GetModuleExtension();
	FString Path = Dir.IsEmpty() ? *Lib : FPaths::Combine(*Dir, *Lib);

	Handle = FPlatformProcess::GetDllHandle(*Path);

	if (Handle == nullptr)
	{
		LOG_ERROR(FString::Printf(TEXT("Dependency %s failed to load in directory %s"), *Lib, *Dir));
		return false;
	}

	LOG_NORMAL(FString::Printf(TEXT("Dependency %s successfully loaded from directory %s"), *Lib, *Dir));
	return true;
#else
	return false;
#endif
}

void FDiscordUE4Module::Internal_FreeDependency(void*& Handle)
{
#if PLATFORM_WINDOWS || PLATFORM_MAC
	if (Handle != nullptr)
	{
		FPlatformProcess::FreeDllHandle(Handle);
		Handle = nullptr;
	}
#endif
}

#undef LOG_NORMAL
#undef LOG_ERROR
#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDiscordUE4Module, DiscordUE4)
