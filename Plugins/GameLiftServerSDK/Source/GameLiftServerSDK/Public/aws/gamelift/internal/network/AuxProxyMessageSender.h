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

#include <aws/gamelift/common/Outcome.h>
#include <aws/gamelift/server/model/DescribePlayerSessionsRequest.h>
#include <aws/gamelift/server/model/StartMatchBackfillRequest.h>
#include <aws/gamelift/server/model/StopMatchBackfillRequest.h>
#include <aws/gamelift/server/LogParameters.h>
#include <google/protobuf/message_lite.h>
#include <sio_client.h>
#include <aws/gamelift/server/protocols/sdk.pb.h>
#ifdef _WIN32
    #include <windows.h>
    #undef ERROR
    #undef SetPort // See http://answers.perforce.com/articles/KB/2638
#endif

namespace Aws
{
namespace GameLift
{
namespace Internal
{
namespace Network
{
    using namespace Aws::GameLift::Server::Model;
    using namespace com::amazon::whitewater::auxproxy;

    class AuxProxyMessageSender
    {
    public:
        AuxProxyMessageSender(sio::client* sioClient);
        ~AuxProxyMessageSender();
        GenericOutcome ProcessReady(int port, const Aws::GameLift::Server::LogParameters &logParameters);
        GenericOutcome ProcessEnding();
        GenericOutcome ActivateGameSession(std::string gameSessionId);
        GenericOutcome TerminateGameSession(std::string gameSessionId);
        GenericOutcome UpdatePlayerSessionCreationPolicy(std::string gameSessionId, std::string newPlayerSessionPolicy);
        GenericOutcome AcceptPlayerSession(std::string playerSessionId, std::string gameSessionId);
        GenericOutcome RemovePlayerSession(std::string playerSessionId, std::string gameSessionId);
        DescribePlayerSessionsOutcome DescribePlayerSessions(const DescribePlayerSessionsRequest &describePlayerSessionsRequest);
        GenericOutcome ReportHealth(bool healthStatus);
        StartMatchBackfillOutcome BackfillMatchmaking(const StartMatchBackfillRequest &request);
        GenericOutcome StopMatchmaking(const StopMatchBackfillRequest &request);
    private:
        std::shared_ptr<std::string> ParseMessage(google::protobuf::MessageLite* message);
        GenericOutcome EmitEvent(google::protobuf::MessageLite* message);
        DescribePlayerSessionsOutcome Call(pbuffer::DescribePlayerSessionsRequest* message);
        StartMatchBackfillOutcome Call(pbuffer::BackfillMatchmakingRequest* message);
        void Send(google::protobuf::MessageLite* message, std::function<void(sio::message::list const&)> ackFunction);
        static GameLiftError From(pbuffer::GameLiftResponse& gameliftResponse);

        sio::client* m_sio_client;
        std::mutex m_lock;
    };
} //namespace Network
} //namespace Internal
} //namespace GameLift
} //namespace Aws
