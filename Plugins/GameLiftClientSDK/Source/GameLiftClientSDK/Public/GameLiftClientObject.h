// Created by YetiTech Studios.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameLiftClientTypes.h"
#if WITH_GAMELIFTCLIENTSDK
#include "aws/gamelift/GameLiftClient.h"
#endif
#include "GameLiftClientObject.generated.h"


UCLASS(BlueprintType)
class GAMELIFTCLIENTSDK_API UGameLiftClientObject : public UObject
{
	GENERATED_BODY()

private:

#if WITH_GAMELIFTCLIENTSDK	
	Aws::GameLift::GameLiftClient* GameLiftClient;
	bool bIsUsingGameLiftLocal;
#endif

	void Internal_InitGameLiftClientSDK(const FString& AccessKey, const FString& Secret, const FString& Region, bool bUsingGameLiftLocal, int32 LocalPort);
	
public:
	
	/**
	* public static UGameLiftClientObject::CreateGameLiftObject
	* Creates a GameLiftClientObject. This function must be called first before accessing any GameLift client functions.
	* @param AccessKey [const FString&] AccessKey of your AWS user. @See http://docs.aws.amazon.com/general/latest/gr/managing-aws-access-keys.html
	* @param Secret [const FString&] SecretKey of your AWS user. @See http://docs.aws.amazon.com/general/latest/gr/managing-aws-access-keys.html
	* @param Region [const FString&] Default is set to us-east-1 (North Virginia).
	* @param bUsingGameLiftLocal [bool] If true, then it is assumed you are using GameLift Local. @See http://docs.aws.amazon.com/gamelift/latest/developerguide/integration-testing-local.html#integration-testing-local-server
	* @param LocalPort [int32] port to use if bUsingGameLiftLocal is true.
	* @return [UGameLiftClientObject*] Returns UGameLiftClientObject*. Use this to create game sessions, player sessions etc.
	**/
	UFUNCTION(BlueprintCallable, Category = "GameLift Client Object")
	static UGameLiftClientObject* CreateGameLiftObject(const FString& AccessKey, const FString& Secret, const FString& Region = "us-east-1", bool bUsingGameLiftLocal = false, int32 LocalPort = 9080);
	
	/**
	* public UGameLiftClientObject::CreateGameSession
	* Creates a multi player game session for players.
	* This action creates a game session record and assigns an available server process in the specified fleet to host the game session. 
	* A fleet must have an ACTIVE status before a game session can be created in it.
	* From the return value first bind both success and fail events and then call Activate to create game session.
	* @param GameSessionProperties [FGameLiftGameSessionConfig] Settings for the game session you want to create.
	* @return [UGameLiftCreateGameSession*] Returns UGameLiftCreateGameSession* Object.
	**/
	UFUNCTION(BlueprintCallable, Category = "GameLift Client Object")	
	UGameLiftCreateGameSession* CreateGameSession(FGameLiftGameSessionConfig GameSessionProperties);

	/**
	* public UGameLiftClientObject::DescribeGameSession
	* Retrieves the given game session.
	* From the return value first bind both success and fail events and then call Activate to describe game session.
	* @param GameSessionID [FString] Game Session ID. This can be obtained after a successful create game session.
	* @return [UGameLiftDescribeGameSession*] Returns UGameLiftDescribeGameSession* Object.
	**/
	UFUNCTION(BlueprintCallable, Category = "GameLift Client Object")	
	UGameLiftDescribeGameSession* DescribeGameSession(FString GameSessionID);

	/**
	* public UGameLiftClientObject::CreatePlayerSession
	* Adds a player to a game session and creates a player session record. 
	* Before a player can be added, a game session must have an ACTIVE status, have a creation policy of ALLOW_ALL, and have an open player slot.	
	* From the return value first bind both success and fail events and then call Activate to create player session.
	* @param GameSessionID [FString] Unique identifier for the game session to add a player to.
	* @param UniquePlayerID [FString] Unique identifier for a player. Player IDs are developer-defined.
	* @return [UGameLiftCreatePlayerSession*] Returns UGameLiftCreatePlayerSession* Object.
	**/
	UFUNCTION(BlueprintCallable, Category = "GameLift Client Object")	
	UGameLiftCreatePlayerSession* CreatePlayerSession(FString GameSessionID, FString UniquePlayerID);
};
