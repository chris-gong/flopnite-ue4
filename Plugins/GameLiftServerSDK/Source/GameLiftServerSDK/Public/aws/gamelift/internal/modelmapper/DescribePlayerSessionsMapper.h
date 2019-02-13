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
#include <aws/gamelift/server/model/DescribePlayerSessionsResult.h>
#include <aws/gamelift/server/protocols/sdk.pb.h>

namespace Aws
{
namespace GameLift
{
namespace Internal
{
namespace ModelMapper
{
    using namespace Aws::GameLift::Server::Model;
    using namespace com::amazon::whitewater::auxproxy;

    class DescribePlayerSessionsMapper
    {
    public:
#ifdef GAMELIFT_USE_STD
        static void ParseFromBufferedDescribePlayerSessionsResponse(const pbuffer::DescribePlayerSessionsResponse &describePlayerSessionsResponse, DescribePlayerSessionsResult& result)
        {
            result.SetNextToken(describePlayerSessionsResponse.nexttoken().c_str());

            std::vector<PlayerSession> playerSessions;
            for (int i = 0; i < describePlayerSessionsResponse.playersessions_size(); i++)
            {
                PlayerSession playerSession = ParseFromBufferedPlayerSession(describePlayerSessionsResponse.playersessions(i));
                playerSessions.push_back(playerSession);
            }

            result.SetPlayerSessions(playerSessions);
        }

        static pbuffer::DescribePlayerSessionsRequest* ParseFromDescribePlayerSessionsRequest(const DescribePlayerSessionsRequest &describePlayerSessionsRequest)
        {
            pbuffer::DescribePlayerSessionsRequest* event = new pbuffer::DescribePlayerSessionsRequest;

            if (!describePlayerSessionsRequest.GetGameSessionId().empty()) {
                event->set_gamesessionid(describePlayerSessionsRequest.GetGameSessionId());
            }
            if (!describePlayerSessionsRequest.GetPlayerId().empty()) {
                event->set_playerid(describePlayerSessionsRequest.GetPlayerId());
            }
            if (!describePlayerSessionsRequest.GetPlayerSessionId().empty()) {
                event->set_playersessionid(describePlayerSessionsRequest.GetPlayerSessionId());
            }
            if (!describePlayerSessionsRequest.GetPlayerSessionStatusFilter().empty()) {
                event->set_playersessionstatusfilter(describePlayerSessionsRequest.GetPlayerSessionStatusFilter());
            }
            if (!describePlayerSessionsRequest.GetNextToken().empty()) {
                event->set_nexttoken(describePlayerSessionsRequest.GetNextToken());
            }
            if (describePlayerSessionsRequest.GetLimit() != 0) {
                event->set_limit(describePlayerSessionsRequest.GetLimit());
            }

            return event;
        }
#else
        static void ParseFromBufferedDescribePlayerSessionsResponse(const pbuffer::DescribePlayerSessionsResponse &describePlayerSessionsResponse, DescribePlayerSessionsResult& result)
        {
            result.SetNextToken(describePlayerSessionsResponse.nexttoken().c_str());

            for (int i = 0; i < describePlayerSessionsResponse.playersessions_size(); i++)
            {
                PlayerSession playerSession = ParseFromBufferedPlayerSession(describePlayerSessionsResponse.playersessions(i));
                result.AddPlayerSession(playerSession);
            }
        }

        static pbuffer::DescribePlayerSessionsRequest* ParseFromDescribePlayerSessionsRequest(const DescribePlayerSessionsRequest &describePlayerSessionsRequest)
        {
            pbuffer::DescribePlayerSessionsRequest* event = new pbuffer::DescribePlayerSessionsRequest;

            if (strlen(describePlayerSessionsRequest.GetGameSessionId()) != 0) {
                event->set_gamesessionid(describePlayerSessionsRequest.GetGameSessionId());
            }
            if (strlen(describePlayerSessionsRequest.GetPlayerId()) != 0) {
                event->set_playerid(describePlayerSessionsRequest.GetPlayerId());
            }
            if (strlen(describePlayerSessionsRequest.GetPlayerSessionId()) != 0) {
                event->set_playersessionid(describePlayerSessionsRequest.GetPlayerSessionId());
            }
            if (strlen(describePlayerSessionsRequest.GetPlayerSessionStatusFilter()) != 0) {
                event->set_playersessionstatusfilter(describePlayerSessionsRequest.GetPlayerSessionStatusFilter());
            }
            if (strlen(describePlayerSessionsRequest.GetNextToken()) != 0) {
                event->set_nexttoken(describePlayerSessionsRequest.GetNextToken());
            }
            if (describePlayerSessionsRequest.GetLimit() != 0) {
                event->set_limit(describePlayerSessionsRequest.GetLimit());
            }

            return event;
        }
#endif

    private:
        static PlayerSession ParseFromBufferedPlayerSession(const pbuffer::PlayerSession &bPlayerSession)
        {
            PlayerSession result;

            result.SetPlayerSessionId(bPlayerSession.playersessionid().c_str());
            result.SetPlayerId(bPlayerSession.playerid().c_str());
            result.SetGameSessionId(bPlayerSession.gamesessionid().c_str());
            result.SetFleetId(bPlayerSession.fleetid().c_str());
            result.SetIpAddress(bPlayerSession.ipaddress().c_str());
            PlayerSessionStatus status = PlayerSessionStatusMapper::GetPlayerSessionStatusForName(bPlayerSession.status().c_str());
            result.SetStatus(status);
            result.SetCreationTime(bPlayerSession.creationtime());
            result.SetTerminationTime(bPlayerSession.terminationtime());
            result.SetPort(bPlayerSession.port());
            result.SetPlayerData(bPlayerSession.playerdata().c_str());
            result.SetDnsName(bPlayerSession.dnsname().c_str());

            return result;
        }
    };
} //namespace ModelMapper
} //namespace Internal
} //namespace GameLift
} //namespace Aws
