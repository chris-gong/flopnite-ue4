using UnrealBuildTool;
using System.IO;

public class AWSCore : ModuleRules
{
	public AWSCore(ReadOnlyTargetRules Target ): base(Target)
	{
		PrivateIncludePaths.AddRange(new string[] { "AWSCore/Private" });
		PublicIncludePaths.AddRange(new string[] { "AWSCore/Public" });

		PublicDependencyModuleNames.AddRange(new string[] { "Engine", "Core", "CoreUObject", "InputCore", "Projects"});
		PrivateDependencyModuleNames.AddRange(new string[] { });

		string BaseDirectory = System.IO.Path.GetFullPath(System.IO.Path.Combine(ModuleDirectory, "..", ".."));
        string ThirdPartyPath = System.IO.Path.Combine(BaseDirectory, "ThirdParty", "GameLiftClientSDK", Target.Platform.ToString());
        bool bIsThirdPartyPathValid = System.IO.Directory.Exists(ThirdPartyPath);

		if (bIsThirdPartyPathValid)
		{
			PublicLibraryPaths.Add(ThirdPartyPath);
			string AWSCoreLibFile = System.IO.Path.Combine(ThirdPartyPath, "aws-cpp-sdk-core.lib");
			if (File.Exists(AWSCoreLibFile))
			{
				PublicAdditionalLibraries.Add(AWSCoreLibFile);
			}
			else
			{
				throw new BuildException("aws-cpp-sdk-core.lib not found. Expected in this location: " + AWSCoreLibFile);
			}

			string AWSCoreDLLFile = System.IO.Path.Combine(ThirdPartyPath, "aws-cpp-sdk-core.dll");
			if (File.Exists(AWSCoreDLLFile))
			{
                PublicDelayLoadDLLs.Add("aws-cpp-sdk-core.dll");
                RuntimeDependencies.Add(new RuntimeDependency(AWSCoreDLLFile));
			}
			else
			{
				throw new BuildException("aws-cpp-sdk-core.dll not found. Expected in this location: " + AWSCoreDLLFile);
			}

			string BinariesDirectory = System.IO.Path.Combine(BaseDirectory, "Binaries", Target.Platform.ToString());
			if (!Directory.Exists(BinariesDirectory))
			{
				Directory.CreateDirectory(BinariesDirectory);
			}

			if (File.Exists(System.IO.Path.Combine(BinariesDirectory, "aws-cpp-sdk-core.dll")) == false)
			{
				File.Copy(System.IO.Path.Combine(ThirdPartyPath, "aws-cpp-sdk-core.dll"), System.IO.Path.Combine(BinariesDirectory, "aws-cpp-sdk-core.dll"));
			}
		}
	}
}
