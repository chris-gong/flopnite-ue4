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
#pragma warning(disable:4996)
#include <aws/gamelift/common/GameLift_EXPORTS.h>

#include <aws/gamelift/server/model/GameSession.h>
#include <aws/gamelift/server/model/UpdateReason.h>

#ifndef GAMELIFT_USE_STD
    #ifndef MAX_BACKFILL_TICKET_ID_LENGTH
        #define MAX_BACKFILL_TICKET_ID_LENGTH 129
    #endif
#endif

namespace Aws
{
    namespace GameLift
    {
        namespace Server
        {
            namespace Model
            {
                /**
                * <p>Represents the data provided to the OnUpdateGameSession callback.
                * For more information, see the <a
                * href="http://docs.aws.amazon.com/gamelift/latest/developerguide/">Amazon
                * GameLift Developer Guide</a>.</p>
                */
                class AWS_GAMELIFT_API UpdateGameSession
                {
#ifdef GAMELIFT_USE_STD
                public:
                    UpdateGameSession(const GameSession& gameSession, UpdateReason updateReason, std::string backfillTicketId) :
                        m_backfillTicketId(backfillTicketId), m_gameSession(gameSession), m_updateReason(updateReason)
                    {
                    }


                    /**
                    * <p>Destructor.</p>
                    */
                    ~UpdateGameSession() {}

                    /**
                    * <p>Copy Constructor.</p>
                    */
                    UpdateGameSession(const UpdateGameSession& other) :
                        m_backfillTicketId(other.m_backfillTicketId), m_gameSession(other.m_gameSession),
                        m_updateReason(other.m_updateReason)
                    { }

                    /**
                    * <p>Move Constructor.</p>
                    */
                    UpdateGameSession(UpdateGameSession&& other)
                    {
                        *this = std::move(other);
                    }


                    /**
                    * <p>Copy assignment Constructor.</p>
                    */
                    UpdateGameSession& operator= (const UpdateGameSession& other)
                    {
                        m_backfillTicketId = other.m_backfillTicketId;
                        m_gameSession = other.m_gameSession;
                        m_updateReason = other.m_updateReason;

                        return *this;
                    }

                    /**
                    * <p>Move assignment Constructor.</p>
                    */
                    UpdateGameSession& operator= (UpdateGameSession&& other)
                    {
                        m_backfillTicketId = std::move(other.m_backfillTicketId);
                        m_gameSession = std::move(other.m_gameSession);
                        m_updateReason = std::move(other.m_updateReason);

                        return *this;
                    }

                    /**
                     * <p>The ticketId used for the submitted match backfill request that this update is in
                     *    response to.  Empty if this update was not in response to a backfill.</p>
                     */
                    inline std::string GetBackfillTicketId() const { return m_backfillTicketId; }

                private:
                    std::string m_backfillTicketId;
#else
                public:
                    UpdateGameSession(const GameSession& gameSession, UpdateReason updateReason, const char* backfillTicketId) :
                        m_gameSession(gameSession), m_updateReason(updateReason)
                    {
                        strncpy(m_backfillTicketId, backfillTicketId, MAX_BACKFILL_TICKET_ID_LENGTH - 1);
                        m_backfillTicketId[MAX_BACKFILL_TICKET_ID_LENGTH - 1] = '\0';
                    }

                    /**
                    * <p>Destructor.</p>
                    */
                    ~UpdateGameSession() {}

                    /**
                    * <p>Copy Constructor.</p>
                    */
                    UpdateGameSession(const UpdateGameSession& other) :
                        m_gameSession(other.m_gameSession), m_updateReason(other.m_updateReason)
                    {
                        strncpy(m_backfillTicketId, other.m_backfillTicketId, sizeof(m_backfillTicketId));
                    }

                    /**
                    * <p>Move Constructor.</p>
                    */
                    UpdateGameSession(UpdateGameSession&& other)
                    {
                        *this = std::move(other);
                    }

                    /**
                    * <p>Copy assignment Constructor.</p>
                    */
                    UpdateGameSession& operator= (const UpdateGameSession& other)
                    {
                        m_gameSession = other.m_gameSession;
                        m_updateReason = other.m_updateReason;
                        strncpy(m_backfillTicketId, other.m_backfillTicketId, sizeof(other.m_backfillTicketId));

                        return *this;
                    }

                    /**
                    * <p>Move assignment Constructor.</p>
                    */
                    UpdateGameSession& operator= (UpdateGameSession&& other)
                    {
                        m_gameSession = std::move(other.m_gameSession);
                        m_updateReason = std::move(other.m_updateReason);
                        strncpy(m_backfillTicketId, other.m_backfillTicketId, sizeof(other.m_backfillTicketId));

                        return *this;
                    }

                    /**
                     * <p>The ticketId used for the submitted match backfill request that this update is in
                     *    response to.  Empty if this update was not in response to a backfill.</p>
                     */
                    inline const char* GetBackfillTicketId() const { return m_backfillTicketId; }

                private:
                    char m_backfillTicketId[MAX_BACKFILL_TICKET_ID_LENGTH];
#endif
                public:
                    /**
                     * <p>The current state of the GameSession.</p>
                     */
                    inline GameSession GetGameSession() const { return m_gameSession; }

                    /**
                     * <p>The reason that this update is being posted to the game server.</p>
                     */
                    inline UpdateReason GetUpdateReason() const { return m_updateReason; }

                private:
                    GameSession m_gameSession;
                    UpdateReason m_updateReason;
                };

            } // namespace Model
        } // namespace Server
    } // namespace GameLift
} // namespace Aws
