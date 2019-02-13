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
#include <aws/gamelift/internal/modelmapper/AttributeValueMapper.h>
#include <aws/gamelift/server/model/Player.h>
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

    class PlayerMapper
    {
    public:
#ifdef GAMELIFT_USE_STD
        static void PopulateFromPlayer(pbuffer::Player& event, const Player &player)
        {
            event.set_playerid(player.GetPlayerId());
            event.set_team(player.GetTeam());

            for (const auto& iter : player.GetLatencyInMs())
            {
                (*event.mutable_latencyinms())[iter.first] = iter.second;
            }

            for (const auto& iter : player.GetPlayerAttributes())
            {
                (*event.mutable_playerattributes())[iter.first] =
                    AttributeValueMapper::ParseFromAttributeValue(iter.second);
            }
        }
#else
        static void PopulateFromPlayer(pbuffer::Player& event, const Player &player)
        {
            event.set_playerid(player.GetPlayerId());
            event.set_team(player.GetTeam());

            int count;
            const Player::RegionAndLatency* entries = player.GetLatencyMs(count);
            for (int index = 0; index < count; ++index)
            {
                (*event.mutable_latencyinms())[entries[index].GetRegion()] = entries[index].GetLatencyMs();
            }

            const Player::NamedAttribute* attrs = player.GetPlayerAttributes(count);
            for (int index = 0; index < count; ++index)
            {
                (*event.mutable_playerattributes())[attrs[index].GetName()] =
                    AttributeValueMapper::ParseFromAttributeValue(attrs[index].GetValue());
            }
        }
#endif
    };
} //namespace ModelMapper
} //namespace Internal
} //namespace GameLift
} //namespace Aws
