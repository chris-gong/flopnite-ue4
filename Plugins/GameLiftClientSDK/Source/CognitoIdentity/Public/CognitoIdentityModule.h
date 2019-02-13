#pragma once

#include "ModuleManager.h"

class FCognitoIdentityModule : public IModuleInterface
{
public:
	void StartupModule();
	void ShutdownModule();

private:
	static void* CognitoIdentityLibraryHandle;
	static bool LoadDependency(const FString& Dir, const FString& Name, void*& Handle);
	static void FreeDependency(void*& Handle);
};