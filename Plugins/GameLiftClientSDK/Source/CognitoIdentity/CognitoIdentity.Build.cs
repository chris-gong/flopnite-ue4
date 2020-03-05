using UnrealBuildTool;
using System.IO;

public class CognitoIdentity : ModuleRules
{
	public CognitoIdentity(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));

		PublicDependencyModuleNames.AddRange(new string[] { "Engine", "Core", "CoreUObject", "InputCore", "Projects" });
		
		// This is required to fix a warning for Unreal Engine 4.21 and later
        	PrivatePCHHeaderFile = "Private/CognitoIdentityPrivatePCH.h";

		string BaseDirectory = System.IO.Path.GetFullPath(System.IO.Path.Combine(ModuleDirectory, "..", ".."));
        string ThirdPartyPath = System.IO.Path.Combine(BaseDirectory, "ThirdParty", "GameLiftClientSDK", Target.Platform.ToString());
        bool bIsThirdPartyPathValid = System.IO.Directory.Exists(ThirdPartyPath);

		if (bIsThirdPartyPathValid)
		{
			PublicLibraryPaths.Add(ThirdPartyPath);

			string CognitoLibFile = System.IO.Path.Combine(ThirdPartyPath, "aws-cpp-sdk-cognito-identity.lib");
			if (File.Exists(CognitoLibFile))
			{
				PublicAdditionalLibraries.Add(CognitoLibFile);
			}
			else
			{
				throw new BuildException("aws-cpp-sdk-cognito-identity.lib not found. Expected in this location: " + CognitoLibFile);
			}

			string CognitoDLLFile = System.IO.Path.Combine(ThirdPartyPath, "aws-cpp-sdk-cognito-identity.dll");
			if (File.Exists(CognitoDLLFile))
			{
                PublicDelayLoadDLLs.Add("aws-cpp-sdk-cognito-identity.dll");
                RuntimeDependencies.Add(CognitoDLLFile);
			}
			else
			{
				throw new BuildException("aws-cpp-sdk-cognito-identity.dll not found. Expected in this location: " + CognitoDLLFile);
			}
		}
	}
}
