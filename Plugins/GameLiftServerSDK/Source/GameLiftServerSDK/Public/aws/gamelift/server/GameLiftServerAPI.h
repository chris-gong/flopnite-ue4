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
#pragma once

#include <aws/gamelift/common/GameLift_EXPORTS.h>
#include <aws/gamelift/common/GameLiftErrors.h>
#include <aws/gamelift/common/Outcome.h>

#include <aws/gamelift/server/model/DescribePlayerSessionsRequest.h>
#include <aws/gamelift/server/model/StartMatchBackfillRequest.h>
#include <aws/gamelift/server/model/StopMatchBackfillRequest.h>
#include <aws/gamelift/server/ProcessParameters.h>
#include <future>


/* The GameLiftServerAPI contains methods that should be used by the server executable you upload to GameLift. */


namespace Aws
{
namespace GameLift
{
namespace Internal
{
    class GameLiftServerState;
}
namespace Server
{
#ifdef GAMELIFT_USE_STD
    typedef Aws::GameLift::Outcome<Aws::GameLift::Internal::GameLiftServerState*, GameLiftError> InitSDKOutcome;
    typedef Aws::GameLift::Outcome<std::vector<std::string>, GameLiftError> GetExpectedPlayerSessionIDsOutcome;
    typedef Aws::GameLift::Outcome<std::vector<std::string>, GameLiftError> GetConnectedPlayerSessionIDsOutcome;

    /**
    @return The current SDK version.
    */
    AWS_GAMELIFT_API AwsStringOutcome GetSdkVersion();

    /**
    Initializes the GameLift server.
    Should be called when the server starts, before any GameLift-dependent initialization happens.
    @return Pointer to the internal server state for use with InitializeWithExisting().
    */
    AWS_GAMELIFT_API InitSDKOutcome InitSDK();

    /**
    Initializes the GameLift server with an existing server state. This function should only be used if you are invoking
    the GameLift SDK API across DLL boundaries.
    @param existingState A pointer to the internal server state obtained from a Server::Initialize() call.
    @return Pointer to the internal server state for use with InitializeWithExisting().
    */
    AWS_GAMELIFT_API InitSDKOutcome InitSDKWithExisting(Aws::GameLift::Internal::GameLiftServerState* existingState);

    /**
    Signals GameLift that the process is ready to receive GameSessions.
    The onStartGameSession callback will be invoked when the server is bound to a GameSession. Game-property-dependent initialization (such as loading a
    user-requested map) should take place at that time. The onHealthCheck callback is invoked asynchronously. There is no mechanism to
    to destroy the resulting thread. If it does not complete in a given time period the server status will be reported as unhealthy.
    @param processParameters The parameters required to successfully run the process.
    */
    AWS_GAMELIFT_API GenericOutcome ProcessReady(const Aws::GameLift::Server::ProcessParameters &processParameters);

    /**
    An asynchronous version of ProcessReady.
    */
    AWS_GAMELIFT_API GenericOutcomeCallable ProcessReadyAsync(const Aws::GameLift::Server::ProcessParameters &processParameters);

    /**
    Signals GameLift that the process is ending.
    GameLift will eventually terminate the process and recycle the host. Once the process is marked as Ending,
    */
    AWS_GAMELIFT_API GenericOutcome ProcessEnding();

    /**
    Reports to GameLift that the server process is now ready to receive player sessions.
    Should be called once all GameSession initialization has finished.
    */
    AWS_GAMELIFT_API GenericOutcome ActivateGameSession();

    /**
    Reports to GameLift that the GameSession has now ended.
    GameLift will now expect the server process to call either ProcessReady in order to launch a new GameSession
    or ProcessEnding which will trigger this process and host to be recycled.
    */
    AWS_GAMELIFT_API GenericOutcome TerminateGameSession();

    /**
    Reports to GameLift that we need to backfill a match using FlexMatch.
    When the match has been succeessfully backfilled updated matchmaker data will be sent to
    the OnUpdateGameSession callback.
    */
    AWS_GAMELIFT_API StartMatchBackfillOutcome StartMatchBackfill(const Aws::GameLift::Server::Model::StartMatchBackfillRequest &backfillMatchmakingRequest);

    /**
    Reports to GameLift that we need to stop a request to backfill a match using FlexMatch.
    */
    AWS_GAMELIFT_API GenericOutcome StopMatchBackfill(const Aws::GameLift::Server::Model::StopMatchBackfillRequest &request);

    /**
    update player session policy on the GameSession 
    */
    AWS_GAMELIFT_API GenericOutcome UpdatePlayerSessionCreationPolicy(Aws::GameLift::Server::Model::PlayerSessionCreationPolicy newPlayerSessionPolicy);

    /**
        @return The server's bound GameSession Id, if the server is Active.
     */
    AWS_GAMELIFT_API AwsStringOutcome GetGameSessionId();

    /**
    Gets the time remaining before Gamelift will shut down the server process. Use this method in your onProcessTerminate() callback implementation to learn when the process will be terminated.
    @return The server's terminationTime in epoch seconds. Value will be -1 if GameLift is not scheduled to terminate the server process.
    */
    AWS_GAMELIFT_API AwsLongOutcome GetTerminationTime();

    /**
        Processes and validates a player session connection. This method should be called when a client requests a
        connection to the server. The client should send the PlayerSessionID which it received from RequestPlayerSession
        or GameLift::CreatePlayerSession to be passed into this function.
        This method will return an UNEXPECTED_PLAYER_SESSION error if the player session ID is invalid.
        @param playerSessionId the ID of the joining player's session.
     */
    AWS_GAMELIFT_API GenericOutcome AcceptPlayerSession(const std::string& playerSessionId);

    /**
        Processes a player session disconnection. Should be called when a player leaves or otherwise disconnects from
        the server.
        @param playerSessionId the ID of the joining player's session.
     */
    AWS_GAMELIFT_API GenericOutcome RemovePlayerSession(const std::string& playerSessionId);

    /**
        <p>Retrieves properties for one or more player sessions. This action can be used
        in several ways: (1) provide a <code>PlayerSessionId</code> parameter to request
        properties for a specific player session; (2) provide a
        <code>GameSessionId</code> parameter to request properties for all player
        sessions in the specified game session; (3) provide a <code>PlayerId</code>
        parameter to request properties for all player sessions of a specified player.
        </p> <p>You can filter this request by player session status. Use the pagination
        parameters to retrieve results as a set of sequential pages. If successful, a 
        <a>PlayerSession</a> object is returned for each session matching the request.</p>
    */
    AWS_GAMELIFT_API DescribePlayerSessionsOutcome DescribePlayerSessions(const Aws::GameLift::Server::Model::DescribePlayerSessionsRequest &describePlayerSessionsRequest);

    /**
        Destroys allocated resources.
    */
    AWS_GAMELIFT_API GenericOutcome Destroy();
#else
    /**
    @return The current SDK version.
    */
    AWS_GAMELIFT_API AwsStringOutcome GetSdkVersion();

    /**
    Initializes the GameLift server.
    Should be called when the server starts, before any GameLift-dependent initialization happens.
    @return Pointer to the internal server state for use with InitializeWithExisting().
    */
    AWS_GAMELIFT_API GenericOutcome InitSDK();

    /**
    Signals GameLift that the process is ready to receive GameSessions.
    The onStartGameSession callback will be invoked when the server is bound to a GameSession. Game-property-dependent initialization (such as loading a
    user-requested map) should take place at that time. The onHealthCheck callback is invoked asynchronously. There is no mechanism to
    to destroy the resulting thread. If it does not complete in a given time period the server status will be reported as unhealthy.
    @param processParameters The parameters required to successfully run the process.
    */
    AWS_GAMELIFT_API GenericOutcome ProcessReady(const Aws::GameLift::Server::ProcessParameters &processParameters);

    /**
    Signals GameLift that the process is ending.
    GameLift will eventually terminate the process and recycle the host. Once the process is marked as Ending,
    */
    AWS_GAMELIFT_API GenericOutcome ProcessEnding();

    /**
    Reports to GameLift that the server process is now ready to receive player sessions.
    Should be called once all GameSession initialization has finished.
    */
    AWS_GAMELIFT_API GenericOutcome ActivateGameSession();

    /**
    Reports to GameLift that the GameSession has now ended.
    GameLift will now expect the server process to call either ProcessReady in order to launch a new GameSession
    or ProcessEnding which will trigger this process and host to be recycled.
    */
    AWS_GAMELIFT_API GenericOutcome TerminateGameSession();

	/**
	Reports to GameLift that we need to backfill a match using FlexMatch.
	When the match has been succeessfully backfilled updated matchmaker data will be sent to
	the OnUpdateGameSession callback.
	*/
	AWS_GAMELIFT_API StartMatchBackfillOutcome StartMatchBackfill(const Aws::GameLift::Server::Model::StartMatchBackfillRequest &backfillMatchmakingRequest);

    /**
    Reports to GameLift that we need to backfill a match using FlexMatch.
    When the match has been succeessfully backfilled updated matchmaker data will be sent to
    the OnUpdateGameSession callback.
    */
    AWS_GAMELIFT_API StartMatchBackfillOutcome StartMatchBackfill(const Aws::GameLift::Server::Model::StartMatchBackfillRequest &backfillMatchmakingRequest);

    /**
    Reports to GameLift that we need to stop a request to backfill a match using FlexMatch.
    */
    AWS_GAMELIFT_API GenericOutcome StopMatchBackfill(const Aws::GameLift::Server::Model::StopMatchBackfillRequest &request);

    /**
    Reports to GameLift that we need to backfill a match using FlexMatch.
    When the match has been succeessfully backfilled updated matchmaker data will be sent to
    the OnUpdateGameSession callback.
    */
    AWS_GAMELIFT_API StartMatchBackfillOutcome StartMatchBackfill(const Aws::GameLift::Server::Model::StartMatchBackfillRequest &backfillMatchmakingRequest);

    /**
    Reports to GameLift that we need to stop a request to backfill a match using FlexMatch.
    */
    AWS_GAMELIFT_API GenericOutcome StopMatchBackfill(const Aws::GameLift::Server::Model::StopMatchBackfillRequest &request);

    /**
    update player session policy on the GameSession
    */
    AWS_GAMELIFT_API GenericOutcome UpdatePlayerSessionCreationPolicy(Aws::GameLift::Server::Model::PlayerSessionCreationPolicy newPlayerSessionPolicy);

    /**
    @return The server's bound GameSession Id, if the server is Active.
    */
    AWS_GAMELIFT_API AwsStringOutcome GetGameSessionId();

    /**
    Gets the time remaining before Gamelift will shut down the server process. Use this method in your onProcessTerminate() callback implementation to learn when the process will be terminated.
    @return The server's terminationTime in epoch seconds. Value will be -1 if GameLift is not scheduled to terminate the server process.
    */
    AWS_GAMELIFT_API AwsLongOutcome GetTerminationTime();

    /**
    Processes and validates a player session connection. This method should be called when a client requests a
    connection to the server. The client should send the PlayerSessionID which it received from RequestPlayerSession
    or GameLift::CreatePlayerSession to be passed into this function.
    This method will return an UNEXPECTED_PLAYER_SESSION error if the player session ID is invalid.
    @param playerSessionId the ID of the joining player's session.
    */
    AWS_GAMELIFT_API GenericOutcome AcceptPlayerSession(const char* playerSessionId);

    /**
    Processes a player session disconnection. Should be called when a player leaves or otherwise disconnects from
    the server.
    @param playerSessionId the ID of the joining player's session.
    */
    AWS_GAMELIFT_API GenericOutcome RemovePlayerSession(const char* playerSessionId);

    /**
        <p>Retrieves properties for one or more player sessions. This action can be used
        in several ways: (1) provide a <code>PlayerSessionId</code> parameter to request
        properties for a specific player session; (2) provide a
        <code>GameSessionId</code> parameter to request properties for all player
        sessions in the specified game session; (3) provide a <code>PlayerId</code>
        parameter to request properties for all player sessions of a specified player.
        </p> <p>You can filter this request by player session status. Use the pagination
        parameters to retrieve results as a set of sequential pages. If successful, a 
        <a>PlayerSession</a> object is returned for each session matching the request.</p>
    */
    AWS_GAMELIFT_API DescribePlayerSessionsOutcome DescribePlayerSessions(const Aws::GameLift::Server::Model::DescribePlayerSessionsRequest &describePlayerSessionsRequest);

    /**
    Destroys allocated resources.
    */
    AWS_GAMELIFT_API GenericOutcome Destroy();
#endif
} //namespace Server
} //namespace GameLift
} //namespace Aws
