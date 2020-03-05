// Created by YetiTech Studios.

#include "GameLiftClientApi.h"
#include "GameLiftClientGlobals.h"

#if WITH_GAMELIFTCLIENTSDK
#include "aws/gamelift/model/DescribeGameSessionDetailsRequest.h"
#include "aws/gamelift/GameLiftClient.h"
#include "aws/core/utils/Outcome.h"
#include "aws/core/auth/AWSCredentialsProvider.h"
#include "aws/gamelift/model/GameProperty.h"
#include "aws/gamelift/model/SearchGameSessionsRequest.h"
#include "aws/gamelift/model/CreatePlayerSessionRequest.h"
#include "aws/gamelift/model/CreateGameSessionRequest.h"
#include "aws/gamelift/model/DescribeGameSessionQueuesRequest.h"
#include "aws/gamelift/model/SearchGameSessionsRequest.h"
#include "aws/gamelift/model/StartGameSessionPlacementRequest.h"
#include "aws/gamelift/model/DescribeGameSessionPlacementRequest.h"
#include <aws/core/http/HttpRequest.h>
#endif

UGameLiftCreateGameSession* UGameLiftCreateGameSession::CreateGameSession(FGameLiftGameSessionConfig GameSessionProperties, bool bIsGameLiftLocal)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftCreateGameSession* Proxy = NewObject<UGameLiftCreateGameSession>();
	Proxy->SessionConfig = GameSessionProperties;
	Proxy->bIsUsingGameLiftLocal = bIsGameLiftLocal;
	return Proxy;
#endif
	return nullptr;
}

EActivateStatus UGameLiftCreateGameSession::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to create game session...");

		if (OnCreateGameSessionSuccess.IsBound() == false)
		{
			LOG_ERROR("No functions were bound to OnCreateGameSessionSuccess multi-cast delegate! Aborting Activate.");
			return EActivateStatus::ACTIVATE_NoSuccessCallback;
		}

		if (OnCreateGameSessionFailed.IsBound() == false)
		{
			LOG_ERROR("No functions were bound to OnCreateGameSessionFailed multi-cast delegate! Aborting Activate.");
			return EActivateStatus::ACTIVATE_NoFailCallback;
		}

		Aws::GameLift::Model::CreateGameSessionRequest GameSessionRequest;

		GameSessionRequest.SetMaximumPlayerSessionCount(SessionConfig.GetMaxPlayers());
		
		if (bIsUsingGameLiftLocal)
		{
			GameSessionRequest.SetFleetId(TCHAR_TO_UTF8(*SessionConfig.GetGameLiftLocalFleetID()));
		}
		else
		{
			GameSessionRequest.SetAliasId(TCHAR_TO_UTF8(*SessionConfig.GetAliasID()));
		}

		LOG_NORMAL("Setting Alias ID: " + SessionConfig.GetAliasID());
		for (FGameLiftGameSessionServerProperties ServerSetting : SessionConfig.GetGameSessionProperties())
		{
			LOG_NORMAL("********************************************************************");
			Aws::GameLift::Model::GameProperty MapProperty;
			MapProperty.SetKey(TCHAR_TO_UTF8(*ServerSetting.Key));
			MapProperty.SetValue(TCHAR_TO_UTF8(*ServerSetting.Value));
			GameSessionRequest.AddGameProperties(MapProperty);
			LOG_NORMAL(FString::Printf(TEXT("New GameProperty added. Key: (%s) Value: (%s)"), *ServerSetting.Key, *ServerSetting.Value));
		}

		Aws::GameLift::CreateGameSessionResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftCreateGameSession::OnCreateGameSession, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Starting game session...");
		GameLiftClient->CreateGameSessionAsync(GameSessionRequest, Handler);
		return EActivateStatus::ACTIVATE_Success;
	}
	LOG_ERROR("GameLiftClient is null. Did you call CreateGameLiftObject first?");	
#endif
	return EActivateStatus::ACTIVATE_NoGameLift;
}

void UGameLiftCreateGameSession::OnCreateGameSession(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::CreateGameSessionRequest& Request, const Aws::GameLift::Model::CreateGameSessionOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	if (Outcome.IsSuccess())
	{
		LOG_NORMAL("Received OnCreateGameSession with Success outcome.");
		const FString GameSessionID = FString(Outcome.GetResult().GetGameSession().GetGameSessionId().c_str());
		OnCreateGameSessionSuccess.Broadcast(GameSessionID);
	}
	else
	{
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnCreateGameSession with failed outcome. Error: " + MyErrorMessage);
		OnCreateGameSessionFailed.Broadcast(MyErrorMessage);
	}
#endif
}

UGameLiftDescribeGameSession* UGameLiftDescribeGameSession::DescribeGameSession(FString GameSessionID)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftDescribeGameSession* Proxy = NewObject<UGameLiftDescribeGameSession>();
	Proxy->SessionID = GameSessionID;
	return Proxy;
#endif
	return nullptr;
}

EActivateStatus UGameLiftDescribeGameSession::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to describe game session...");

		if (OnDescribeGameSessionStateSuccess.IsBound() == false)
		{
			LOG_ERROR("No functions were bound to OnDescribeGameSessionStateSuccess multi-cast delegate! Aborting Activate.");
			return EActivateStatus::ACTIVATE_NoSuccessCallback;
		}

		if (OnDescribeGameSessionStateFailed.IsBound() == false)
		{
			LOG_ERROR("No functions were bound to OnDescribeGameSessionStateFailed multi-cast delegate! Aborting Activate.");
			return EActivateStatus::ACTIVATE_NoFailCallback;
		}

		Aws::GameLift::Model::DescribeGameSessionDetailsRequest Request;
		Request.SetGameSessionId(TCHAR_TO_UTF8(*SessionID));

		Aws::GameLift::DescribeGameSessionDetailsResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftDescribeGameSession::OnDescribeGameSessionState, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Requesting to describe game session with ID: " + SessionID);
		GameLiftClient->DescribeGameSessionDetailsAsync(Request, Handler);
		return EActivateStatus::ACTIVATE_Success;
	}
	LOG_ERROR("GameLiftClient is null. Did you call CreateGameLiftObject first?");
#endif
	return EActivateStatus::ACTIVATE_NoGameLift;
}

void UGameLiftDescribeGameSession::OnDescribeGameSessionState(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::DescribeGameSessionDetailsRequest& Request, const Aws::GameLift::Model::DescribeGameSessionDetailsOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	if (Outcome.IsSuccess())
	{
		LOG_NORMAL("Received OnDescribeGameSessionState with Success outcome.");
		const FString MySessionID = FString(Outcome.GetResult().GetGameSessionDetails().data()->GetGameSession().GetGameSessionId().c_str());
		EGameLiftGameSessionStatus MySessionStatus = GetSessionState(Outcome.GetResult().GetGameSessionDetails().data()->GetGameSession().GetStatus());
		OnDescribeGameSessionStateSuccess.Broadcast(MySessionID, MySessionStatus);
	}
	else
	{
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnDescribeGameSessionState with failed outcome. Error: " + MyErrorMessage);
		OnDescribeGameSessionStateFailed.Broadcast(MyErrorMessage);
	}
#endif
}

EGameLiftGameSessionStatus UGameLiftDescribeGameSession::GetSessionState(const Aws::GameLift::Model::GameSessionStatus& Status)
{
#if WITH_GAMELIFTCLIENTSDK
	switch (Status)
	{
		case Aws::GameLift::Model::GameSessionStatus::ACTIVATING:
			return EGameLiftGameSessionStatus::STATUS_Activating;
		case Aws::GameLift::Model::GameSessionStatus::ACTIVE:
			return EGameLiftGameSessionStatus::STATUS_Active;
		case Aws::GameLift::Model::GameSessionStatus::ERROR_:
			return EGameLiftGameSessionStatus::STATUS_Error;
		case Aws::GameLift::Model::GameSessionStatus::NOT_SET:
			return EGameLiftGameSessionStatus::STATUS_NotSet;
		case Aws::GameLift::Model::GameSessionStatus::TERMINATED:
			return EGameLiftGameSessionStatus::STATUS_Terminated;
		case Aws::GameLift::Model::GameSessionStatus::TERMINATING:
			return EGameLiftGameSessionStatus::STATUS_Terminating;
		default:
			break;
	}
	checkNoEntry(); // This code block should never reach!
#endif
	return EGameLiftGameSessionStatus::STATUS_NoStatus; // Just a dummy return
}

UGameLiftCreatePlayerSession* UGameLiftCreatePlayerSession::CreatePlayerSession(FString GameSessionID, FString UniquePlayerID)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftCreatePlayerSession* Proxy = NewObject<UGameLiftCreatePlayerSession>();
	Proxy->GameSessionID = GameSessionID;
	Proxy->PlayerID = UniquePlayerID;
	return Proxy;
#endif
	return nullptr;
}

EActivateStatus UGameLiftCreatePlayerSession::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{		
		LOG_NORMAL("Preparing to create player session...");

		if (OnCreatePlayerSessionSuccess.IsBound() == false)
		{
			LOG_ERROR("No functions were bound to OnCreatePlayerSessionSuccess multi-cast delegate! Aborting Activate.");
			return EActivateStatus::ACTIVATE_NoSuccessCallback;
		}

		if (OnCreatePlayerSessionFailed.IsBound() == false)
		{
			LOG_ERROR("No functions were bound to OnCreatePlayerSessionFailed multi-cast delegate! Aborting Activate.");
			return EActivateStatus::ACTIVATE_NoFailCallback;
		}

		Aws::GameLift::Model::CreatePlayerSessionRequest Request;
		LOG_NORMAL("Create player session game session ID: " + GameSessionID);
		if (GameSessionID.Len() > 0) {
			LOG_NORMAL("Setting game session ID: " + GameSessionID);
			Request.SetGameSessionId(TCHAR_TO_UTF8(*GameSessionID));
		}
		if (PlayerID.Len() > 0) {
			LOG_NORMAL("Setting player ID: " + PlayerID);
			Request.SetPlayerId(TCHAR_TO_UTF8(*PlayerID));
		}
		
		Aws::GameLift::CreatePlayerSessionResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftCreatePlayerSession::OnCreatePlayerSession, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Creating new player session...");
		GameLiftClient->CreatePlayerSessionAsync(Request, Handler);
		return EActivateStatus::ACTIVATE_Success;
	}
	LOG_ERROR("GameLiftClient is null. Did you call CreateGameLiftObject first?");
#endif
	return EActivateStatus::ACTIVATE_NoGameLift;
}

void UGameLiftCreatePlayerSession::OnCreatePlayerSession(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::CreatePlayerSessionRequest& Request, const Aws::GameLift::Model::CreatePlayerSessionOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	if (Outcome.IsSuccess())
	{
		LOG_NORMAL("Received OnCreatePlayerSession with Success outcome.");
		const FString ServerIpAddress = FString(Outcome.GetResult().GetPlayerSession().GetIpAddress().c_str());
		const FString ServerPort = FString::FromInt(Outcome.GetResult().GetPlayerSession().GetPort());
		const FString MyPlayerSessionID = FString(Outcome.GetResult().GetPlayerSession().GetPlayerSessionId().c_str());
		Aws::GameLift::Model::PlayerSessionStatus Status = Outcome.GetResult().GetPlayerSession().GetStatus();
		int PlayerSessionStatus = 0;
		if (Status == Aws::GameLift::Model::PlayerSessionStatus::ACTIVE) {
			PlayerSessionStatus = 2;
		}
		else if (Status == Aws::GameLift::Model::PlayerSessionStatus::RESERVED) {
			PlayerSessionStatus = 1;
		}
		else if (Status == Aws::GameLift::Model::PlayerSessionStatus::COMPLETED) {
			PlayerSessionStatus = -1;
		}
		else if(Status == Aws::GameLift::Model::PlayerSessionStatus::TIMEDOUT) {
			PlayerSessionStatus = 0;
		}
		const int MyPlayerSessionStatus = PlayerSessionStatus;
		OnCreatePlayerSessionSuccess.Broadcast(ServerIpAddress, ServerPort, MyPlayerSessionID, MyPlayerSessionStatus);
	}
	else
	{
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnCreatePlayerSession with failed outcome. Error: " + MyErrorMessage);
		OnCreatePlayerSessionFailed.Broadcast(MyErrorMessage);
	}
#endif
}

UGameLiftDescribeGameSessionQueues* UGameLiftDescribeGameSessionQueues::DescribeGameSessionQueues(FString QueueName)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftDescribeGameSessionQueues* Proxy = NewObject<UGameLiftDescribeGameSessionQueues>();
	Proxy->QueueName = QueueName;
	return Proxy;
#endif
	return nullptr;
}

EActivateStatus UGameLiftDescribeGameSessionQueues::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to search for fleets associated with a queue...");

		if (OnDescribeGameSessionQueuesSuccess.IsBound() == false)
		{
			LOG_ERROR("No functions were bound to OnDescribeGameSessionQueuesSuccess multi-cast delegate! Aborting Activate.");
			return EActivateStatus::ACTIVATE_NoSuccessCallback;
		}

		if (OnDescribeGameSessionQueuesFailed.IsBound() == false)
		{
			LOG_ERROR("No functions were bound to OnDescribeGameSessionQueuesFailed multi-cast delegate! Aborting Activate.");
			return EActivateStatus::ACTIVATE_NoFailCallback;
		}

		Aws::GameLift::Model::DescribeGameSessionQueuesRequest Request;
		LOG_NORMAL("Setting queue name: " + QueueName);
		std::vector<Aws::String, Aws::Allocator<Aws::String>> QueueNames;
		Aws::String QueueNameStr(TCHAR_TO_UTF8(*QueueName), QueueName.Len());
		QueueNames.push_back(QueueNameStr);
		Request.SetNames(QueueNames);

		Aws::GameLift::DescribeGameSessionQueuesResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftDescribeGameSessionQueues::OnDescribeGameSessionQueues, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Searching...");
		GameLiftClient->DescribeGameSessionQueuesAsync(Request, Handler);
		return EActivateStatus::ACTIVATE_Success;
	}
	LOG_ERROR("GameLiftClient is null. Did you call CreateGameLiftObject first?");
#endif
	return EActivateStatus::ACTIVATE_NoGameLift;
}

void UGameLiftDescribeGameSessionQueues::OnDescribeGameSessionQueues(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::DescribeGameSessionQueuesRequest& Request, const Aws::GameLift::Model::DescribeGameSessionQueuesOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	if (Outcome.IsSuccess())
	{
		LOG_NORMAL("Received OnDescribeGameSessionQueues with Success outcome.");
		const std::vector<Aws::GameLift::Model::GameSessionQueue, Aws::Allocator<Aws::GameLift::Model::GameSessionQueue>> Queues = Outcome.GetResult().GetGameSessionQueues();
		LOG_NORMAL("Received OnDescribeGameSessionQueues after getting destinations");
		TArray<FString> FleetARNs; // NOTE: STILL HAVE TO SPLIT THE STRINGS IN HERE BY REGION AND FLEET ID/ALIAS
		for (int i = 0; i < Queues.size(); i++) {
			const std::vector<Aws::GameLift::Model::GameSessionQueueDestination, Aws::Allocator<Aws::GameLift::Model::GameSessionQueueDestination>> Destinations = Queues[i].GetDestinations();
			for (int j = 0; j < Destinations.size(); j++) {
				FleetARNs.Add(Destinations[j].GetDestinationArn().c_str());
			}
		}
		LOG_NORMAL("Received OnDescribeGameSessionQueues after loading fleet arns");
		const TArray<FString> FleetARNsCopy = TArray<FString>(FleetARNs);
		OnDescribeGameSessionQueuesSuccess.Broadcast(FleetARNsCopy);
	}
	else
	{
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnDescribeGameSessionQueues with failed outcome. Error: " + MyErrorMessage);
		OnDescribeGameSessionQueuesFailed.Broadcast(MyErrorMessage);
	}
#endif
}

UGameLiftSearchGameSessions* UGameLiftSearchGameSessions::SearchGameSessions(FString FleetId, FString AliasId, FString FilterExpression, FString SortExpression)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftSearchGameSessions* Proxy = NewObject<UGameLiftSearchGameSessions>();
	Proxy->FleetId = FleetId;
	Proxy->AliasId = AliasId;
	Proxy->FilterExpression = FilterExpression;
	Proxy->SortExpression = SortExpression;
	return Proxy;
#endif
	return nullptr;
}

EActivateStatus UGameLiftSearchGameSessions::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to search for game sessions...");

		if (OnSearchGameSessionsSuccess.IsBound() == false)
		{
			LOG_ERROR("No functions were bound to OnSearchGameSessionsSuccess multi-cast delegate! Aborting Activate.");
			return EActivateStatus::ACTIVATE_NoSuccessCallback;
		}

		if (OnSearchGameSessionsFailed.IsBound() == false)
		{
			LOG_ERROR("No functions were bound to OnSearchGameSessionsFailed multi-cast delegate! Aborting Activate.");
			return EActivateStatus::ACTIVATE_NoFailCallback;
		}

		Aws::GameLift::Model::SearchGameSessionsRequest Request;
		
		if (FleetId.Len() > 0) {
			LOG_NORMAL("Setting fleet id: " + FleetId);
			Request.SetFleetId(TCHAR_TO_UTF8(*FleetId));
		}
		if (AliasId.Len() > 0) {
			LOG_NORMAL("Setting alias id: " + AliasId);
			Request.SetAliasId(TCHAR_TO_UTF8(*AliasId));
		}
		if (FilterExpression.Len() > 0) {
			LOG_NORMAL("Setting filter expression: " + FilterExpression);
			Request.SetFilterExpression(TCHAR_TO_UTF8(*FilterExpression));
		}
		if (SortExpression.Len() > 0) {
			LOG_NORMAL("Setting sort expression: " + SortExpression);
			Request.SetSortExpression(TCHAR_TO_UTF8(*SortExpression));
		}

		Aws::GameLift::SearchGameSessionsResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftSearchGameSessions::OnSearchGameSessions, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Searching...");
		GameLiftClient->SearchGameSessionsAsync(Request, Handler);
		return EActivateStatus::ACTIVATE_Success;
	}
	LOG_ERROR("GameLiftClient is null. Did you call CreateGameLiftObject first?");
#endif
	return EActivateStatus::ACTIVATE_NoGameLift;
}

void UGameLiftSearchGameSessions::OnSearchGameSessions(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::SearchGameSessionsRequest& Request, const Aws::GameLift::Model::SearchGameSessionsOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	if (Outcome.IsSuccess())
	{
		LOG_NORMAL("Received OnSearchGameSessions with Success outcome.");
		const std::vector<Aws::GameLift::Model::GameSession, Aws::Allocator<Aws::GameLift::Model::GameSession>> GameSessions = Outcome.GetResult().GetGameSessions();
		TArray<FString> GameSessionIds;
		for (int i = 0; i < GameSessions.size(); i++) {
			LOG_NORMAL("OnSearchGameSessionsSuccess");
			GameSessionIds.Add(GameSessions[i].GetGameSessionId().c_str());
		}
		const TArray<FString> GameSessionIdsCopy = TArray<FString>(GameSessionIds);
		OnSearchGameSessionsSuccess.Broadcast(GameSessionIdsCopy);
	}
	else
	{
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received SearchGameSessions with failed outcome. Error: " + MyErrorMessage);
		OnSearchGameSessionsFailed.Broadcast(MyErrorMessage);
	}
#endif
}

UGameLiftStartGameSessionPlacement* UGameLiftStartGameSessionPlacement::StartGameSessionPlacement(FString QueueName, int MaxPlayerCount, FString PlacementId)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftStartGameSessionPlacement* Proxy = NewObject<UGameLiftStartGameSessionPlacement>();
	Proxy->QueueName = QueueName;
	Proxy->MaxPlayerCount = MaxPlayerCount;
	Proxy->PlacementId = PlacementId;
	return Proxy;
#endif
	return nullptr;
}

EActivateStatus UGameLiftStartGameSessionPlacement::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to make a game session if available...");

		if (OnStartGameSessionPlacementSuccess.IsBound() == false)
		{
			LOG_ERROR("No functions were bound to OnStartGameSessionPlacement multi-cast delegate! Aborting Activate.");
			return EActivateStatus::ACTIVATE_NoSuccessCallback;
		}

		if (OnStartGameSessionPlacementFailed.IsBound() == false)
		{
			LOG_ERROR("No functions were bound to OnStartGameSessionPlacement multi-cast delegate! Aborting Activate.");
			return EActivateStatus::ACTIVATE_NoFailCallback;
		}

		Aws::GameLift::Model::StartGameSessionPlacementRequest Request;
		LOG_NORMAL("Setting queue name: " + QueueName);
		Request.SetGameSessionQueueName(TCHAR_TO_UTF8(*QueueName));
		LOG_NORMAL("Setting max player count: " + FString(std::to_string(MaxPlayerCount).c_str()));
		Request.SetMaximumPlayerSessionCount(MaxPlayerCount);
		LOG_NORMAL("PlacementId " + PlacementId);
		Request.SetPlacementId(TCHAR_TO_UTF8(*PlacementId));

		Aws::GameLift::StartGameSessionPlacementResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftStartGameSessionPlacement::OnStartGameSessionPlacement, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Searching...");
		GameLiftClient->StartGameSessionPlacementAsync(Request, Handler);
		return EActivateStatus::ACTIVATE_Success;
	}
	LOG_ERROR("GameLiftClient is null. Did you call CreateGameLiftObject first?");
#endif
	return EActivateStatus::ACTIVATE_NoGameLift;
}

void UGameLiftStartGameSessionPlacement::OnStartGameSessionPlacement(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::StartGameSessionPlacementRequest& Request, const Aws::GameLift::Model::StartGameSessionPlacementOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	if (Outcome.IsSuccess())
	{
		LOG_NORMAL("Received OnStartGameSessionPlacement with Success outcome.");
		const FString GameSessionId = Outcome.GetResult().GetGameSessionPlacement().GetGameSessionId().c_str();
		const FString GameSessionPlacementId = Outcome.GetResult().GetGameSessionPlacement().GetPlacementId().c_str();
		Aws::GameLift::Model::GameSessionPlacementState Status = Outcome.GetResult().GetGameSessionPlacement().GetStatus();
		int GameSessionPlacementStatus = 0;
		if (Status == Aws::GameLift::Model::GameSessionPlacementState::FULFILLED) {
			GameSessionPlacementStatus = 1;
		}
		else if (Status == Aws::GameLift::Model::GameSessionPlacementState::PENDING) {
			GameSessionPlacementStatus = 0;
		}
		else {
			GameSessionPlacementStatus = -1;
		}

		const int GameSessionPlacementStatusCopy = GameSessionPlacementStatus;
		OnStartGameSessionPlacementSuccess.Broadcast(GameSessionId, GameSessionPlacementId, GameSessionPlacementStatusCopy);
	}
	else
	{
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnStartGameSessionPlacement with failed outcome. Error: " + MyErrorMessage);
		OnStartGameSessionPlacementFailed.Broadcast(MyErrorMessage);
	}
#endif
}

UGameLiftDescribeGameSessionPlacement* UGameLiftDescribeGameSessionPlacement::DescribeGameSessionPlacement(FString GameSessionPlacementId)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftDescribeGameSessionPlacement* Proxy = NewObject<UGameLiftDescribeGameSessionPlacement>();
	Proxy->GameSessionPlacementId = GameSessionPlacementId;
	return Proxy;
#endif
	return nullptr;
}

EActivateStatus UGameLiftDescribeGameSessionPlacement::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to describe a game session placement if available...");

		if (OnDescribeGameSessionPlacementSuccess.IsBound() == false)
		{
			LOG_ERROR("No functions were bound to OnDescribeGameSessionPlacement multi-cast delegate! Aborting Activate.");
			return EActivateStatus::ACTIVATE_NoSuccessCallback;
		}

		if (OnDescribeGameSessionPlacementFailed.IsBound() == false)
		{
			LOG_ERROR("No functions were bound to OnDescribeGameSessionPlacement multi-cast delegate! Aborting Activate.");
			return EActivateStatus::ACTIVATE_NoFailCallback;
		}

		Aws::GameLift::Model::DescribeGameSessionPlacementRequest Request;
		LOG_NORMAL("PlacementId " + GameSessionPlacementId);
		Request.SetPlacementId(TCHAR_TO_UTF8(*GameSessionPlacementId));

		Aws::GameLift::DescribeGameSessionPlacementResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftDescribeGameSessionPlacement::OnDescribeGameSessionPlacement, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Searching...");
		GameLiftClient->DescribeGameSessionPlacementAsync(Request, Handler);
		return EActivateStatus::ACTIVATE_Success;
	}
	LOG_ERROR("GameLiftClient is null. Did you call CreateGameLiftObject first?");
#endif
	return EActivateStatus::ACTIVATE_NoGameLift;
}

void UGameLiftDescribeGameSessionPlacement::OnDescribeGameSessionPlacement(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::DescribeGameSessionPlacementRequest& Request, const Aws::GameLift::Model::DescribeGameSessionPlacementOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	if (Outcome.IsSuccess())
	{
		LOG_NORMAL("Received OnDescribeGameSessionPlacement with Success outcome.");
		const FString GameSessionId = Outcome.GetResult().GetGameSessionPlacement().GetGameSessionId().c_str();
		const FString PlacementId = Outcome.GetResult().GetGameSessionPlacement().GetPlacementId().c_str();
		Aws::GameLift::Model::GameSessionPlacementState Status = Outcome.GetResult().GetGameSessionPlacement().GetStatus();
		int GameSessionPlacementStatus = 0;
		if (Status == Aws::GameLift::Model::GameSessionPlacementState::FULFILLED) {
			GameSessionPlacementStatus = 1;
		}
		else if (Status == Aws::GameLift::Model::GameSessionPlacementState::PENDING) {
			GameSessionPlacementStatus = 0;
		}
		else {
			GameSessionPlacementStatus = -1;
		}

		const int GameSessionPlacementStatusCopy = GameSessionPlacementStatus;
		OnDescribeGameSessionPlacementSuccess.Broadcast(GameSessionId, PlacementId, GameSessionPlacementStatusCopy);
	}
	else
	{
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnDescribeGameSessionPlacement with failed outcome. Error: " + MyErrorMessage);
		OnDescribeGameSessionPlacementFailed.Broadcast(MyErrorMessage);
	}
#endif
}
