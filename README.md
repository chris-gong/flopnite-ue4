<img src="https://raw.githubusercontent.com/pivotman319-new/fortnite-clone-ue4/master/readme_images/T_Frontend_UI_Wordmark-512_black.png" width="512" height="256"></img>
## What is Weekly
Relatively new to game development, my friends and I wanted to challenge ourselves by recreating the popular triple A game, *Fortnite*, in a limited time frame while having fun doing so. We also documented the process and posted this documentary to our [YouTube channel](https://www.youtube.com/watch?v=UCyIgPcXKLQ). This recreation was recreated using Unreal Engine 4 and Amazon GameLift.

## How to build and run Weekly
### Note that we developed this game in Windows 10 using Visual Studio 2019, so these two will be required. Current Unreal version: 4.23 Preview
#### You can, however, compile this project using at least Visual Studio 2017.

### Step 1: Download the project
Simply clone or download the project from here, and make sure everything is extracted (no zip files!)

### Step 2: Download the necessary dll and lib files
This project relied on the amazon gamelift client sdk, which can be found [here](https://github.com/YetiTech-Studios/UE4GameLiftClientSDK). You can either download the whole sdk or preferably download just the dll and lib from the ThirdParty folder found [here](https://github.com/YetiTech-Studios/UE4GameLiftClientSDK/tree/master/GameLiftClientSDK/ThirdParty/GameLiftClientSDK/Win64). Now go to the `FortniteClone` project in File Explorer, and navigate to the folders Plugins -> GameLiftClientSDK, and create a new folder here. Title this folder, "ThirdParty". Open this folder, and create a new folder here. Title this folder "GameLiftClientSDK". Open this folder, and a create another new folder. Title this folder *Win64*. Open this folder, and copy and paste the dll and lib files downloaded from earlier into this folder.

#### Optional
This project also used the Amazon GameLift Server SDK. However, you only need these files if you plan on deploying this project to a GameLift server, either remotely or locally, since you will need to build server files. Note that to build server files, you will need the source version of Unreal Engine. Instructions on getting the dll and lib files for the server SDK can be found [here](https://docs.aws.amazon.com/gamelift/latest/developerguide/integration-engines-setup-unreal.html). Once you have those files, open the Fortnite Clone project in File Explorer and navigate to the folders Plugins -> GameLiftServerSDK -> ThirdParty -> GameLiftServerSDK -> Win64, and copy and paste the files there.

### Step 3: Generate the Visual Studio project files
For this step, navigate to the *FortniteClone* project folder, and right click the uproject file labelled *FortniteClone*, and select the option "Generate Visual Studio project files". 

### Step 4: Build the Visual Studio solution
Once the Visual Studio files have finished generating, you will notice an sln file in the project folder called "FortniteClone.sln". Open this file in Visual Studio. Make sure your solution configuration is set to *Development Editor* and that your solution platform is set to "Win64" in the dropdown menus at the top of Visual Studio. Then, right click the project, **not the whole solution**, called `FortniteClone` under the "Games" folder under the solution on the right side of Visual Studio, and select "Build".

### Step 5: Open the uproject file
Once the solution has finished building, return to the project folder, and double click the uproject file. The project should open up in Unreal Engine.

### Step 6: Fixing errors related to BuildData files
Once the editor has finished loading, you're going to see menu pop up with an error talking about how the builddata file for the main menu level can't be found. If you're not on the Level titled `Level_MainMenu`, then navigate to the *Content* folder below, then open the *Maps* folder and select that level. Then go to the toolbar at the top and select the ***Build*** option. You will need to apply the lighting when done (if this popup shows up). After that, go to File -> Save All. The same will have to be done for the level titled `Level_BattleRoyale_2`, so open that file and select the "Build" option in the toolbar and save when finished. Apply the lighting (if this popup shows up) then do File -> Save All.

### Step 7: Running the game
Now since this is a multiplayer game, you will need to set some multiplayer settings. At the top of the editor in the tool bar, click the downwards arrow to the right of the "Run" option and make sure ***Run Dedicated Server*** is enabled. Also, feel free to set the number of players to as many or as few as you want. Both of these options are under ***Multiplayer Options***. Personally, I also like to have the *New Editor Window* mode enabled too. Finally, click the "Run" button and have fun!

### Step 8: Hosting the game on GameLift (Optional)
If you chose to do the optional part of step 2, you can host the game on Amazon GameLift to generate your own codes for the main menu level and play the packaged game with other people. First, watch this [video](https://youtu.be/Iq2LpwXogTw) as well as this [video](https://youtu.be/2I8JDeMGkgc) to get yourself familiarized with GameLift. Then, go to the project's "Content" folder and create a new folder called "Credentials". Add two text files to this folder, one called `AWS_AccessKey` and the other called `AWS_SecretKey`. Note that you may not be able to add text files in the Unreal Engine Editor, so use File Explorer, a text editor, Visual Studio, or something else to do so. In the access key file, add the access key associated with the AWS user you created and added GameLift privileges to. In the secret key file, add the secret access key associated with that same user. The project should now be ready for the steps in those two videos linked in this step.

## Who was involved in Weekly
I am proud to say that this was truly a community effort. Although I was the main programmer, I had two friends, Ryan and Mike, who made all the models and designed the map. Here's a [link](https://github.com/rthomas24) to Ryan's github and a [link](https://www.instagram.com/mikedecaprio1/) to Mike's instagram. Also, a big thanks has to be given to all the people who helped me in the Unreal Slackers discord, all the people who answer questions on the Unreal Q&A forums, all the youtubers who post amazing Unreal tutorials, and all the people in our discord who came out to test the game.

## For those who want to test
We are currently looking for testers for a huge game session! If you're interested, then join our [Discord](https://discord.gg/2xbR5qT).
