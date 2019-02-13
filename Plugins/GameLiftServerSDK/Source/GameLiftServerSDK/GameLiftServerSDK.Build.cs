// AMAZON CONFIDENTIAL

/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/

using UnrealBuildTool;

public class GameLiftServerSDK : ModuleRules
{
    public GameLiftServerSDK(ReadOnlyTargetRules Target) : base (Target)
    {
        PublicIncludePaths.AddRange(
            new string[] {
            "GameLiftServerSDK/Public"
            }
            );


        PrivateIncludePaths.AddRange(
            new string[] {
            "GameLiftServerSDK/Private",
            }
            );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
            "Core",
            "Projects"
            }
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
            }
            );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
            }
            );

        // This is required to fix a warning for Unreal Engine 4.21 and later
        PrivatePCHHeaderFile = "Private/GameLiftServerSDKPrivatePCH.h";

        bEnableExceptions = true;

        string BaseDirectory = System.IO.Path.GetFullPath(System.IO.Path.Combine(ModuleDirectory, "..", ".."));
        string SDKDirectory = System.IO.Path.Combine(BaseDirectory, "ThirdParty", "GameLiftServerSDK", Target.Platform.ToString());

        bool bHasGameLiftSDK = System.IO.Directory.Exists(SDKDirectory);

        if (bHasGameLiftSDK)
        {
            if (Target.Type == TargetRules.TargetType.Server)
            {
                PublicDefinitions.Add("WITH_GAMELIFT=1");    
                if (Target.Platform == UnrealTargetPlatform.Linux)
                {
                    SDKDirectory = System.IO.Path.Combine(SDKDirectory, "x86_64-unknown-linux-gnu");
                    string SDKLib = System.IO.Path.Combine(SDKDirectory, "libaws-cpp-sdk-gamelift-server.so");
                
                    PublicLibraryPaths.Add(SDKDirectory);
                    PublicAdditionalLibraries.Add(SDKLib);    
                    RuntimeDependencies.Add(SDKLib);
                }
                else if (Target.Platform == UnrealTargetPlatform.Win64)
                {
                    PublicLibraryPaths.Add(SDKDirectory);
                    PublicAdditionalLibraries.Add(System.IO.Path.Combine(SDKDirectory, "aws-cpp-sdk-gamelift-server.lib"));
                    PublicDelayLoadDLLs.Add("aws-cpp-sdk-gamelift-server.dll");
                    string SDKLibWindows = System.IO.Path.Combine(SDKDirectory, "aws-cpp-sdk-gamelift-server.dll");
                    RuntimeDependencies.Add(SDKLibWindows);
                }
            }
            else
            {
                PublicDefinitions.Add("WITH_GAMELIFT=0");
            }
        }
        else
        {
            PublicDefinitions.Add("WITH_GAMELIFT=0");
        }
    }
}
