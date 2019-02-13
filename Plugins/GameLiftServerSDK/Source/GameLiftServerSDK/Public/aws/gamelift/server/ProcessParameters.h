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
#include <aws/gamelift/server/model/UpdateReason.h>
#include <aws/gamelift/server/LogParameters.h>
#include <functional>

namespace Aws
{
namespace GameLift
{
namespace Server
{
#ifndef GAMELIFT_USE_STD
    typedef void(*StartGameSessionFn)(Aws::GameLift::Server::Model::GameSession, void*);
    typedef void(*UpdateGameSessionFn)(Aws::GameLift::Server::Model::UpdateGameSession, void*);
    typedef void(*ProcessTerminateFn)(void*);
    typedef bool(*HealthCheckFn)(void*);
#endif

    class ProcessParameters
    {
#ifdef GAMELIFT_USE_STD
    public:
        ProcessParameters() :
        m_onStartGameSession(nullptr),
        m_onUpdateGameSession(nullptr),
        m_onProcessTerminate(nullptr),
        m_onHealthCheck(nullptr),
        m_port(-1),
        m_logParameters(LogParameters())
        {}

        ProcessParameters(const std::function<void(Aws::GameLift::Server::Model::GameSession)> onStartGameSession,
            const std::function<void()> onProcessTerminate,
            const std::function<bool()> onHealthCheck,
            int port,
            const Aws::GameLift::Server::LogParameters logParameters) :
            m_onStartGameSession(onStartGameSession),
            m_onUpdateGameSession([](Aws::GameLift::Server::Model::UpdateGameSession) { }),
            m_onProcessTerminate(onProcessTerminate),
            m_onHealthCheck(onHealthCheck),
            m_port(port),
            m_logParameters(logParameters) { }

        ProcessParameters(const std::function<void(Aws::GameLift::Server::Model::GameSession)> onStartGameSession,
            const std::function<void(Aws::GameLift::Server::Model::UpdateGameSession)> onUpdateGameSession,
            const std::function<void()> onProcessTerminate,
            const std::function<bool()> onHealthCheck,
            int port,
            const Aws::GameLift::Server::LogParameters logParameters) :
            m_onStartGameSession(onStartGameSession),
            m_onUpdateGameSession(onUpdateGameSession),
            m_onProcessTerminate(onProcessTerminate),
            m_onHealthCheck(onHealthCheck),
            m_port(port),
            m_logParameters(logParameters) { }

        AWS_GAMELIFT_API std::function<void(Aws::GameLift::Server::Model::GameSession)> getOnStartGameSession() const { return m_onStartGameSession; }
        AWS_GAMELIFT_API std::function<void(Aws::GameLift::Server::Model::UpdateGameSession)> getOnUpdateGameSession() const { return m_onUpdateGameSession; }
        AWS_GAMELIFT_API std::function<void()> getOnProcessTerminate() const { return m_onProcessTerminate; }
        AWS_GAMELIFT_API std::function<bool()> getOnHealthCheck() const { return m_onHealthCheck; }
        AWS_GAMELIFT_API int getPort() const { return m_port; }
        AWS_GAMELIFT_API Aws::GameLift::Server::LogParameters getLogParameters() const { return m_logParameters; }

    private:
        std::function<void(Aws::GameLift::Server::Model::GameSession)> m_onStartGameSession;
        std::function<void(Aws::GameLift::Server::Model::UpdateGameSession)> m_onUpdateGameSession;
        std::function<void()> m_onProcessTerminate;
        std::function<bool()> m_onHealthCheck;
        int m_port;
        Aws::GameLift::Server::LogParameters m_logParameters;
#else
    public:
        ProcessParameters() :
        m_onStartGameSession(nullptr),
        m_onUpdateGameSession(nullptr),
        m_onProcessTerminate(nullptr),
        m_onHealthCheck(nullptr),
        m_startGameSessionState(nullptr),
        m_processTerminateState(nullptr),
        m_healthCheckState(nullptr),
        m_port(-1),
        m_logParameters(LogParameters())
        { }

        ProcessParameters(
            StartGameSessionFn onStartGameSession,
            void* startGameSessionState,
            UpdateGameSessionFn onUpdateGameSession,
            void* updateGameSessionState,
            ProcessTerminateFn onProcessTerminate,
            void* processTerminateState,
            HealthCheckFn onHealthCheck,
            void* healthCheckState,
            int port,
            const Aws::GameLift::Server::LogParameters logParameters) :

            m_onStartGameSession(onStartGameSession),
            m_onUpdateGameSession(onUpdateGameSession),
            m_onProcessTerminate(onProcessTerminate),
            m_onHealthCheck(onHealthCheck),
            m_startGameSessionState(startGameSessionState),
            m_updateGameSessionState(updateGameSessionState),
            m_processTerminateState(processTerminateState),
            m_healthCheckState(healthCheckState),
            m_port(port),
            m_logParameters(logParameters){}

        ProcessParameters(
            StartGameSessionFn onStartGameSession,
            void* startGameSessionState,
            ProcessTerminateFn onProcessTerminate,
            void* processTerminateState,
            HealthCheckFn onHealthCheck,
            void* healthCheckState,
            int port,
            const Aws::GameLift::Server::LogParameters logParameters) :

            m_onStartGameSession(onStartGameSession),
            m_onUpdateGameSession([](Aws::GameLift::Server::Model::UpdateGameSession, void*) { }),
            m_onProcessTerminate(onProcessTerminate),
            m_onHealthCheck(onHealthCheck),
            m_startGameSessionState(startGameSessionState),
            m_updateGameSessionState(nullptr),
            m_processTerminateState(processTerminateState),
            m_healthCheckState(healthCheckState),
            m_port(port),
            m_logParameters(logParameters){}

        AWS_GAMELIFT_API StartGameSessionFn getOnStartGameSession() const { return m_onStartGameSession; }
        AWS_GAMELIFT_API void* getStartGameSessionState() const { return m_startGameSessionState; }
        AWS_GAMELIFT_API UpdateGameSessionFn getOnUpdateGameSession() const { return m_onUpdateGameSession; }
        AWS_GAMELIFT_API void* getUpdateGameSessionState() const { return m_updateGameSessionState; }
        AWS_GAMELIFT_API ProcessTerminateFn getOnProcessTerminate() const { return m_onProcessTerminate; }
        AWS_GAMELIFT_API void* getProcessTerminateState() const { return m_processTerminateState; }
        AWS_GAMELIFT_API HealthCheckFn getOnHealthCheck() const { return m_onHealthCheck; }
        AWS_GAMELIFT_API void* getHealthCheckState() const { return m_healthCheckState; }
        AWS_GAMELIFT_API int getPort() const { return m_port; }
        AWS_GAMELIFT_API Aws::GameLift::Server::LogParameters getLogParameters() const { return m_logParameters; }

    private:
        StartGameSessionFn m_onStartGameSession;
        UpdateGameSessionFn m_onUpdateGameSession;
        ProcessTerminateFn m_onProcessTerminate;
        HealthCheckFn m_onHealthCheck;
        void* m_startGameSessionState;
        void* m_updateGameSessionState;
        void* m_processTerminateState;
        void* m_healthCheckState;
        int m_port;
        Aws::GameLift::Server::LogParameters m_logParameters;
#endif
    };
} // namespace Server
} // namespace GameLift
} // namespace Aws

