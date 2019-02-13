// Created by YetiTech Studios.

#include "GameLiftClientObject.h"
#include "aws/core/client/ClientConfiguration.h"



void UGameLiftClientObject::Internal_InitGameLiftClientSDK(const FString& AccessKey, const FString& Secret, const FString& Region, bool bUsingGameLiftLocal, int32 LocalPort)
{
#if WITH_GAMELIFTCLIENTSDK
	Aws::Client::ClientConfiguration ClientConfig;
	Aws::Auth::AWSCredentials Credentials;

	ClientConfig.connectTimeoutMs = 10000;
	ClientConfig.requestTimeoutMs = 10000;
	ClientConfig.region = TCHAR_TO_UTF8(*Region);
	
	bIsUsingGameLiftLocal = bUsingGameLiftLocal;

	// GameLiftLocal
	if (bUsingGameLiftLocal)
	{
		ClientConfig.scheme = Aws::Http::Scheme::HTTP;
		const FString HostAddress = FString::Printf(TEXT("localhost:%i"), LocalPort);
		ClientConfig.endpointOverride = TCHAR_TO_UTF8(*HostAddress);
		LOG_WARNING("GameLift is currently configured to use GameLift Local.");
	}

	Credentials = Aws::Auth::AWSCredentials(TCHAR_TO_UTF8(*AccessKey), TCHAR_TO_UTF8(*Secret));
	GameLiftClient = new Aws::GameLift::GameLiftClient(Credentials, ClientConfig);
#endif
}

UGameLiftClientObject* UGameLiftClientObject::CreateGameLiftObject(const FString& AccessKey, const FString& Secret, const FString& Region /*= "us-east-1"*/, bool bUsingGameLiftLocal /*= false*/, int32 LocalPort /*= 9080*/)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftClientObject* Proxy = NewObject<UGameLiftClientObject>();
	Proxy->Internal_InitGameLiftClientSDK(AccessKey, Secret, Region, bUsingGameLiftLocal, LocalPort);
	return Proxy;
#endif
	return nullptr;
}

UGameLiftCreateGameSession* UGameLiftClientObject::CreateGameSession(FGameLiftGameSessionConfig GameSessionProperties)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftCreateGameSession* Proxy = UGameLiftCreateGameSession::CreateGameSession(GameSessionProperties, bIsUsingGameLiftLocal);
	Proxy->GameLiftClient = GameLiftClient;
	return Proxy;
#endif
	return nullptr;
}

UGameLiftDescribeGameSession* UGameLiftClientObject::DescribeGameSession(FString GameSessionID)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftDescribeGameSession* Proxy = UGameLiftDescribeGameSession::DescribeGameSession(GameSessionID);
	Proxy->GameLiftClient = GameLiftClient;
	return Proxy;
#endif
	return nullptr;
}

UGameLiftCreatePlayerSession* UGameLiftClientObject::CreatePlayerSession(FString GameSessionID, FString UniquePlayerID)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftCreatePlayerSession* Proxy = UGameLiftCreatePlayerSession::CreatePlayerSession(GameSessionID, UniquePlayerID);
	Proxy->GameLiftClient = GameLiftClient;
	return Proxy;
#endif
	return nullptr;
}
