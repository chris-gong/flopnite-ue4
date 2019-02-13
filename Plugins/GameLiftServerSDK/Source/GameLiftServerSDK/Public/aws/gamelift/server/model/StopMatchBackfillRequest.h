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
#include <aws/gamelift/server/model/StartMatchBackfillRequest.h>
#include <aws/gamelift/server/model/Player.h>

namespace Aws
{
    namespace GameLift
    {
        namespace Server
        {
            namespace Model
            {

                /**
                * <p>A request to be used for making a StopMatchBackfill call.
                * For more information, see the <a
                * href="http://docs.aws.amazon.com/gamelift/latest/developerguide/">Amazon
                * GameLift Developer Guide</a>.</p>
                */
                class AWS_GAMELIFT_API StopMatchBackfillRequest
                {
#ifdef GAMELIFT_USE_STD
                public:
                    /**
                    * <p>Constructor.</p>
                    */
                    StopMatchBackfillRequest()
                    {
                    }

                    /**
                    * <p>Destructor.</p>
                    */
                    ~StopMatchBackfillRequest() {}

                    /**
                    * <p>Copy Constructor.</p>
                    */
                    StopMatchBackfillRequest(const StopMatchBackfillRequest& other) :
                        m_ticketId(other.m_ticketId),
                        m_matchmakingConfigurationArn(other.m_matchmakingConfigurationArn),
                        m_gameSessionArn(other.m_gameSessionArn)
                    { }

                    /**
                    * <p>Move Constructor.</p>
                    */
                    StopMatchBackfillRequest(StopMatchBackfillRequest&& other)
                    {
                        *this = std::move(other);
                    }

                    /**
                    * <p>Copy assignment Constructor.</p>
                    */
                    StopMatchBackfillRequest& operator= (const StopMatchBackfillRequest& other)
                    {
                        m_ticketId = other.m_ticketId;
                        m_matchmakingConfigurationArn = other.m_matchmakingConfigurationArn;
                        m_gameSessionArn = other.m_gameSessionArn;

                        return *this;
                    }

                    /**
                    * <p>Move assignment Constructor.</p>
                    */
                    StopMatchBackfillRequest& operator= (StopMatchBackfillRequest&& other)
                    {
                        m_ticketId = std::move(other.m_ticketId);
                        m_gameSessionArn = std::move(other.m_gameSessionArn);
                        m_matchmakingConfigurationArn = std::move(other.m_matchmakingConfigurationArn);

                        return *this;
                    }

                    inline const std::string& GetTicketId() const { return m_ticketId; }


                    inline void SetTicketId(const std::string& value) { m_ticketId = value; }


                    inline void SetTicketId(std::string&& value) { m_ticketId = value; }


                    inline void SetTicketId(const char* value) { m_ticketId.assign(value); }


                    inline StopMatchBackfillRequest& WithTicketId(const std::string& value) { SetTicketId(value); return *this; }


                    inline StopMatchBackfillRequest& WithTicketId(std::string&& value) { SetTicketId(value); return *this; }


                    inline StopMatchBackfillRequest& WithTicketId(const char* value) { SetTicketId(value); return *this; }


                    inline const std::string& GetMatchmakingConfigurationArn() const { return m_matchmakingConfigurationArn; }


                    inline void SetMatchmakingConfigurationArn(const std::string& value) { m_matchmakingConfigurationArn = value; }


                    inline void SetMatchmakingConfigurationArn(std::string&& value) { m_matchmakingConfigurationArn = value; }


                    inline void SetMatchmakingConfigurationArn(const char* value) { m_matchmakingConfigurationArn.assign(value); }


                    inline StopMatchBackfillRequest& WithMatchmakingConfigurationArn(const std::string& value) { SetMatchmakingConfigurationArn(value); return *this; }


                    inline StopMatchBackfillRequest& WithMatchmakingConfigurationArn(std::string&& value) { SetMatchmakingConfigurationArn(value); return *this; }


                    inline StopMatchBackfillRequest& WithMatchmakingConfigurationArn(const char* value) { SetMatchmakingConfigurationArn(value); return *this; }


                    inline const std::string& GetGameSessionArn() const { return m_gameSessionArn; }


                    inline void SetGameSessionArn(const std::string& value) { m_gameSessionArn = value; }


                    inline void SetGameSessionArn(std::string&& value) { m_gameSessionArn = value; }


                    inline void SetGameSessionArn(const char* value) { m_gameSessionArn.assign(value); }


                    inline StopMatchBackfillRequest& WithGameSessionArn(const std::string& value) { SetGameSessionArn(value); return *this; }


                    inline StopMatchBackfillRequest& WithGameSessionArn(std::string&& value) { SetGameSessionArn(value); return *this; }


                    inline StopMatchBackfillRequest& WithGameSessionArn(const char* value) { SetGameSessionArn(value); return *this; }

                private:
                    std::string m_ticketId;
                    std::string m_matchmakingConfigurationArn;
                    std::string m_gameSessionArn;
#else
                public:
                    StopMatchBackfillRequest()
                    {
                        m_ticketId[0] = '\0';
                        m_matchmakingConfigurationArn[0] = '\0';
                        m_gameSessionArn[0] = '\0';
                    }

                    /**
                    * <p>Destructor.</p>
                    */
                    ~StopMatchBackfillRequest() {}

                    /**
                    * <p>Copy Constructor.</p>
                    */
                    StopMatchBackfillRequest(const StopMatchBackfillRequest& other)
                    {
                        *this = other;
                    }

                    /**
                    * <p>Move Constructor.</p>
                    */
                    StopMatchBackfillRequest(StopMatchBackfillRequest&& other)
                    {
                        *this = std::move(other);
                    }

                    /**
                    * <p>Copy assignment Constructor.</p>
                    */
                    StopMatchBackfillRequest& operator= (const StopMatchBackfillRequest& other)
                    {
                        SetTicketId(other.m_ticketId);
                        SetGameSessionArn(other.m_gameSessionArn);
                        SetMatchmakingConfigurationArn(other.m_matchmakingConfigurationArn);

                        return *this;
                    }

                    /**
                    * <p>Move assignment Constructor.</p>
                    */
                    StopMatchBackfillRequest& operator= (StopMatchBackfillRequest&& other)
                    {
                        *this = other;

                        return *this;
                    }


                    inline const char* GetTicketId() const { return m_ticketId; }


                    inline void SetTicketId(const char* value) { strncpy(m_ticketId, value, sizeof(m_ticketId)); m_ticketId[MAX_TICKET_LENGTH - 1] = '\0'; }


                    inline StopMatchBackfillRequest& WithTicketId(const char* value) { SetTicketId(value); return *this; }


                    inline const char* GetMatchmakingConfigurationArn() const { return m_matchmakingConfigurationArn; }


                    inline void SetMatchmakingConfigurationArn(const char* value)
                    {
                        strncpy(m_matchmakingConfigurationArn, value, sizeof(m_matchmakingConfigurationArn));
                        m_matchmakingConfigurationArn[MAX_ARN_LENGTH - 1] = '\0';
                    }


                    inline StopMatchBackfillRequest& WithMatchmakingConfigurationArn(const char* value) { SetMatchmakingConfigurationArn(value); return *this; }


                    inline const char* GetGameSessionArn() const { return m_gameSessionArn; }


                    inline void SetGameSessionArn(const char* value)
                    {
                        strncpy(m_gameSessionArn, value, sizeof(m_gameSessionArn));
                        m_gameSessionArn[MAX_ARN_LENGTH - 1] = '\0';
                    }


                    inline StopMatchBackfillRequest& WithGameSessionArn(const char* value) { SetGameSessionArn(value); return *this; }

                private:
                    char m_ticketId[MAX_TICKET_LENGTH];
                    char m_matchmakingConfigurationArn[MAX_ARN_LENGTH];
                    char m_gameSessionArn[MAX_ARN_LENGTH];
#endif
                };
            } // namespace Model
        } // namespace Server
    } // namespace GameLift
} // namespace Aws
