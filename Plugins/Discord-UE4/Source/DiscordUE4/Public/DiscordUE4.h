// Created by Satheesh (ryanjon2040). Twitter: https://twitter.com/ryanjon2040. Discord: @ryanjon2040#5319

#pragma once


#include "Modules/ModuleManager.h"

class FDiscordUE4Module : public IModuleInterface
{

private:

	static void* DiscordHandle;

public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
		
	static bool Internal_LoadDependency(const FString& Dir, const FString& Name, void*& Handle);
	static void Internal_FreeDependency(void*& Handle);
};
