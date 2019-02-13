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
#include <aws/gamelift/server/model/StartMatchBackfillRequest.h>
#include <aws/gamelift/internal/modelmapper/PlayerMapper.h>
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

    class BackfillMatchmakingRequestMapper
    {
    public:
#ifdef GAMELIFT_USE_STD
        static pbuffer::BackfillMatchmakingRequest* ParseFromBackfillMatchmakingRequest(const StartMatchBackfillRequest &request)
        {
            pbuffer::BackfillMatchmakingRequest* event = new pbuffer::BackfillMatchmakingRequest();

            event->set_ticketid(request.GetTicketId());
            event->set_matchmakingconfigurationarn(request.GetMatchmakingConfigurationArn());
            event->set_gamesessionarn(request.GetGameSessionArn());
            for (auto& player : request.GetPlayers())
            {
                PlayerMapper::PopulateFromPlayer(*event->add_players(), player);
            }

            return event;
        }

        static void ParseFromBufferedBackfillMatchmakingResponse(const pbuffer::BackfillMatchmakingResponse &response,
                                                                 StartMatchBackfillResult& result)
        {
            result.WithTicketId(response.ticketid());
        }

        static pbuffer::StopMatchmakingRequest* ParseFromStopMatchmakingRequest(const StopMatchBackfillRequest &request)
        {
            pbuffer::StopMatchmakingRequest* event = new pbuffer::StopMatchmakingRequest();

            event->set_ticketid(request.GetTicketId());
            event->set_matchmakingconfigurationarn(request.GetMatchmakingConfigurationArn());
            event->set_gamesessionarn(request.GetGameSessionArn());

            return event;
        }

#else
        static pbuffer::BackfillMatchmakingRequest* ParseFromBackfillMatchmakingRequest(const StartMatchBackfillRequest &request)
        {
            pbuffer::BackfillMatchmakingRequest* event = new pbuffer::BackfillMatchmakingRequest();

            event->set_ticketid(request.GetTicketId());
            event->set_gamesessionarn(request.GetGameSessionArn());
            event->set_matchmakingconfigurationarn(request.GetMatchmakingConfigurationArn());

            int count;
            const Player* players = request.GetPlayers(count);
            for (int index = 0; index < count; ++index)
            {
                PlayerMapper::PopulateFromPlayer(*event->add_players(), players[index]);
            }

            return event;
        }

        static void ParseFromBufferedBackfillMatchmakingResponse(const pbuffer::BackfillMatchmakingResponse &response,
                                                                 StartMatchBackfillResult& result)
        {
            result.WithTicketId(response.ticketid().c_str());
        }

        static pbuffer::StopMatchmakingRequest* ParseFromStopMatchmakingRequest(const StopMatchBackfillRequest &request)
        {
            pbuffer::StopMatchmakingRequest* event = new pbuffer::StopMatchmakingRequest();

            event->set_ticketid(request.GetTicketId());
            event->set_gamesessionarn(request.GetGameSessionArn());
            event->set_matchmakingconfigurationarn(request.GetMatchmakingConfigurationArn());

            return event;
        }

#endif
    };
} //namespace ModelMapper
} //namespace Internal
} //namespace GameLift
} //namespace Aws
