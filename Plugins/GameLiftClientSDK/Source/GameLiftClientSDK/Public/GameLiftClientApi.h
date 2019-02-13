// Created by YetiTech Studios.

#pragma once

#include "CoreMinimal.h"
#include "GameLiftClientTypes.h"

// AWS includes
#include "aws/gamelift/GameLiftClient.h"
#include "aws/gamelift/model/CreateGameSessionResult.h"
#include "aws/core/auth/AWSCredentialsProvider.h"
//

#include "GameLiftClientApi.generated.h"

UENUM(BlueprintType)
enum class EActivateStatus : uint8
{
	/* Successfully activated. */
	ACTIVATE_Success				UMETA(DisplayName = "Success"),

	/* GameLiftClient was null. Make sure you called CreateGameLiftObject function. */
	ACTIVATE_NoGameLift				UMETA(DisplayName = "Null GameLift"),

	/* Delegate that was suppose to call when outcome is a success was not binded to any function. */
	ACTIVATE_NoSuccessCallback		UMETA(DisplayName = "Success Delegate not bound"),

	/* Delegate that was suppose to call when outcome is a failure was not binded to any function. */
	ACTIVATE_NoFailCallback			UMETA(DisplayName = "Failed Delegate not bound")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreateGameSessionSuccess, const FString&, GameSessionID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreateGameSessionFailed, const FString&, ErrorMessage);
UCLASS()
class GAMELIFTCLIENTSDK_API UGameLiftCreateGameSession : public UObject
{
	GENERATED_BODY()

	friend class UGameLiftClientObject;

public:
	UPROPERTY(BlueprintAssignable, Category = "GameLift CreateGameSession")
	FOnCreateGameSessionSuccess OnCreateGameSessionSuccess;

	UPROPERTY(BlueprintAssignable, Category = "GameLift CreateGameSession")
	FOnCreateGameSessionFailed OnCreateGameSessionFailed;

private:
	Aws::GameLift::GameLiftClient* GameLiftClient;
	FGameLiftGameSessionConfig SessionConfig;

	bool bIsUsingGameLiftLocal;

	static UGameLiftCreateGameSession* CreateGameSession(FGameLiftGameSessionConfig GameSessionProperties, bool bIsGameLiftLocal);

public:
	UFUNCTION(BlueprintCallable, Category = "GameLift CreateGameSession")
	EActivateStatus Activate();

private:
	void OnCreateGameSession(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::CreateGameSessionRequest& Request, const Aws::GameLift::Model::CreateGameSessionOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDescribeGameSessionStateSuccess, const FString&, SessionID, EGameLiftGameSessionStatus, SessionState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDescribeGameSessionStateFailed, const FString&, ErrorMessage);
UCLASS()
class GAMELIFTCLIENTSDK_API UGameLiftDescribeGameSession : public UObject
{
	GENERATED_BODY()

	friend class UGameLiftClientObject;

public:

	UPROPERTY(BlueprintAssignable, Category = "GameLift DescribeGameSession")
	FOnDescribeGameSessionStateSuccess OnDescribeGameSessionStateSuccess;
	
	UPROPERTY(BlueprintAssignable, Category = "GameLift DescribeGameSession")
	FOnDescribeGameSessionStateFailed OnDescribeGameSessionStateFailed;

private:
	Aws::GameLift::GameLiftClient* GameLiftClient;
	FString SessionID;

	static UGameLiftDescribeGameSession* DescribeGameSession(FString GameSessionID);

public:
	UFUNCTION(BlueprintCallable, Category = "GameLift DescribeGameSession")
	EActivateStatus Activate();	

private:
	void OnDescribeGameSessionState(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::DescribeGameSessionDetailsRequest& Request, const Aws::GameLift::Model::DescribeGameSessionDetailsOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
	EGameLiftGameSessionStatus GetSessionState(const Aws::GameLift::Model::GameSessionStatus& Status);
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreatePlayerSessionSuccess, const FString&, IPAddress, const FString&, Port, const FString&, PlayerSessionID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreatePlayerSessionFailed, const FString&, ErrorMessage);
UCLASS()
class GAMELIFTCLIENTSDK_API UGameLiftCreatePlayerSession : public UObject
{
	GENERATED_BODY()

	friend class UGameLiftClientObject;

public:

	UPROPERTY(BlueprintAssignable, Category = "GameLift CreatePlayerSession")
	FOnCreatePlayerSessionSuccess OnCreatePlayerSessionSuccess;

	UPROPERTY(BlueprintAssignable, Category = "GameLift CreatePlayerSession")
	FOnCreatePlayerSessionFailed OnCreatePlayerSessionFailed;

private:
	Aws::GameLift::GameLiftClient* GameLiftClient;
	FString GameSessionID;
	FString PlayerID;

	static UGameLiftCreatePlayerSession* CreatePlayerSession(FString GameSessionID, FString UniquePlayerID);

public:
	UFUNCTION(BlueprintCallable, Category = "GameLift CreatePlayerSession")
	EActivateStatus Activate();

private:
	void OnCreatePlayerSession(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::CreatePlayerSessionRequest& Request, const Aws::GameLift::Model::CreatePlayerSessionOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};
