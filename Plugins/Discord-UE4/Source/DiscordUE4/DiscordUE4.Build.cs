// Created by Satheesh (ryanjon2040). Twitter: https://twitter.com/ryanjon2040. Discord: @ryanjon2040#5319

using UnrealBuildTool;
using System.IO;

public class DiscordUE4 : ModuleRules
{
	public DiscordUE4(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		if (Directory.Exists(DiscordPath) == false)
		{
			throw new BuildException("discord files not found. Check it exists here: " + DiscordPath);
		}

		if (File.Exists(DiscordLibFile) == false)
		{
			throw new BuildException(DiscordLibFileName + " was not found. Check it exists here: " + DiscordLibFile);
		}

		if (IsWin64() && File.Exists(DiscordDllFile) == false)
		{
			throw new BuildException("discord_game_sdk.dll was not found. Check it exists here: " + DiscordDllFile);
		}

		PublicIncludePaths.Add(DiscordPath);
		
		PublicDependencyModuleNames.AddRange( new string[] { "Core", "Projects" } );		
		PrivateDependencyModuleNames.AddRange( new string[] { "CoreUObject", "Engine" } );

		PublicAdditionalLibraries.Add(DiscordLibFile);
		if (IsWin64())
		{
			PublicDelayLoadDLLs.Add("discord_game_sdk.dll");
		}

		RuntimeDependencies.Add(DiscordDllFile);
	}

	private string DiscordPath
	{
		get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "discord-files")); }
	}

	private string DiscordLibFile
	{
		get 
		{
			return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../Binaries/", GetPlatformName, DiscordLibFileName));
		}
	}

	private string DiscordLibFileName
	{
		get
		{
			if (IsWin64())
			{
				return "discord_game_sdk.dll.lib";
			}
			else if (IsMac())
			{
				return "discord_game_sdk.dylib";
			}

			return null;
		}
	}

	private string DiscordDllFile
	{
		get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../Binaries/Win64/discord_game_sdk.dll")); }
	}

	private bool IsWin64()
	{
		return Target.Platform == UnrealTargetPlatform.Win64;
	}

	private bool IsMac()
	{
		return Target.Platform == UnrealTargetPlatform.Mac;
	}

	private string GetPlatformName
	{
		get 
		{
			if (IsWin64())
			{
				return "Win64";
			}
			else if (IsMac())
			{
				return "Mac";
			}

			return null;
		}
	}
}
