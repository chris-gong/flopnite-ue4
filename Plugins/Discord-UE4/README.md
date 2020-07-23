# Discord-UE4

Plugin for integrating Discord Rich Presence in Unreal Engine 4. Blueprints only example project is [available here](https://github.com/ryanjon2040/Discord-UE4-Example-Project).

[![Discord Link](https://img.shields.io/discord/685402622844600329?logo=discord&style=for-the-badge)](https://discord.gg/QnzVK5R) [![GitHub All Releases](https://img.shields.io/github/downloads/ryanjon2040/Discord-UE4/total?logo=github&style=for-the-badge)](https://github.com/ryanjon2040/Discord-UE4/releases) ![GitHub repo size](https://img.shields.io/github/repo-size/ryanjon2040/Discord-UE4?logo=github&style=for-the-badge) [![GitHub](https://img.shields.io/github/license/ryanjon2040/Discord-UE4?style=for-the-badge)](https://github.com/ryanjon2040/Discord-UE4/blob/master/LICENSE)

[![Twitter Follow](https://img.shields.io/twitter/follow/ryanjon2040?style=social)](https://twitter.com/ryanjon2040)

![Rich Presence Result](Result.png)

# How to
First you will need to download the binaries from Discord. Head over to [Discord Developer Portal](https://discord.com/developers/docs/game-sdk/sdk-starter-guide) and download **Discord Game SDK**.

![SDK_Download](Documentation/DownloadSDK.png)

After downloading, open the zip file and extract the `.dll` and `.lib` files from the `lib/x86_64` folder to `Binaries/Win64/` folder of this plugin. If you don't have `Binaries/Win64` folder, create it. Example of my setup:

![Extracted Example](Documentation/ExtractedExample.png)

After copying the binary files, open the `cpp` folder inside the zip file and extract all the contents to `Source/DiscordUE4/discord-files` folder. If the `discord-files` folder doesn't exist, create it.

Now you are good to go.

It is important to setup your game according to [Discord Startup Guide](https://discord.com/developers/docs/game-sdk/sdk-starter-guide). 

Example Setup:
![Example Setup in Blueprint](DiscordSetup.png)
