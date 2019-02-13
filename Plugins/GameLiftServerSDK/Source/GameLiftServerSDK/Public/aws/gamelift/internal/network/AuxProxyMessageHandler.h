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
#include <aws/gamelift/server/model/GameSession.h>
#include <aws/gamelift/server/model/UpdateGameSession.h>
#include <sio_client.h>

namespace Aws
{
namespace GameLift
{
namespace Internal
{
namespace Network
{
    using namespace Aws::GameLift::Server::Model;

    class AuxProxyMessageHandler
    {
    public:
        virtual void OnStartGameSession(GameSession& gameSession, sio::message::list& ack_resp) = 0;
        virtual void OnUpdateGameSession(UpdateGameSession& gameSession, sio::message::list& ack_resp) = 0;
        virtual void OnTerminateProcess(long terminationTime) = 0;
    };
} //namespace Network
} //namespace Internal
} //namespace GameLift
} //namespace Aws
