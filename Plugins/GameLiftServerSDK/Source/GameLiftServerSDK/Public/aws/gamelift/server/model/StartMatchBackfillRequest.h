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
#include <aws/gamelift/server/model/Player.h>

#ifndef GAMELIFT_USE_STD
    #ifndef MAX_TICKET_LENGTH
        #define MAX_TICKET_LENGTH 129
    #endif
    #ifndef MAX_ARN_LENGTH
		#define MAX_ARN_LENGTH 257
    #endif
    #ifndef MAX_PLAYERS
		#define MAX_PLAYERS 40
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
    * <p>A request to be used for making a StartMatchBackfill call.
    * For more information, see the <a
    * href="http://docs.aws.amazon.com/gamelift/latest/developerguide/">Amazon
    * GameLift Developer Guide</a>.</p>
    */
class AWS_GAMELIFT_API StartMatchBackfillRequest
{
#ifdef GAMELIFT_USE_STD
public:
    /**
    * <p>Constructor.</p>
    */
    StartMatchBackfillRequest()
    {
    }

    /**
    * <p>Destructor.</p>
    */
    ~StartMatchBackfillRequest() {}

    /**
    * <p>Copy Constructor.</p>
    */
    StartMatchBackfillRequest(const StartMatchBackfillRequest& other) :
        m_ticketId(other.m_ticketId),
        m_matchmakingConfigurationArn(other.m_matchmakingConfigurationArn),
        m_gameSessionArn(other.m_gameSessionArn),
        m_players(other.m_players)
    { }

    /**
    * <p>Move Constructor.</p>
    */
    StartMatchBackfillRequest(StartMatchBackfillRequest&& other) 
    { 
        *this = std::move(other);
    }

    /**
    * <p>Copy assignment Constructor.</p>
    */
    StartMatchBackfillRequest& operator= (const StartMatchBackfillRequest& other) 
    {
        m_ticketId = other.m_ticketId;
        m_matchmakingConfigurationArn = other.m_matchmakingConfigurationArn;
        m_gameSessionArn = other.m_gameSessionArn;
        m_players = other.m_players;

        return *this;
    }

    /**
    * <p>Move assignment Constructor.</p>
    */
    StartMatchBackfillRequest& operator= (StartMatchBackfillRequest&& other) 
    {
        m_ticketId = std::move(other.m_ticketId);
        m_matchmakingConfigurationArn = std::move(other.m_matchmakingConfigurationArn);
        m_gameSessionArn = std::move(other.m_gameSessionArn);
        m_players = std::move(other.m_players);

        return *this;
    }

    inline const std::string& GetTicketId() const { return m_ticketId; }


    inline void SetTicketId(const std::string& value) { m_ticketId = value; }


    inline void SetTicketId(std::string&& value) { m_ticketId = value; }


    inline void SetTicketId(const char* value) { m_ticketId.assign(value); }


    inline StartMatchBackfillRequest& WithTicketId(const std::string& value) { SetTicketId(value); return *this; }


    inline StartMatchBackfillRequest& WithTicketId(std::string&& value) { SetTicketId(value); return *this; }


    inline StartMatchBackfillRequest& WithTicketId(const char* value) { SetTicketId(value); return *this; }


    inline const std::string& GetMatchmakingConfigurationArn() const { return m_matchmakingConfigurationArn; }


    inline void SetMatchmakingConfigurationArn(const std::string& value) { m_matchmakingConfigurationArn = value; }


    inline void SetMatchmakingConfigurationArn(std::string&& value) { m_matchmakingConfigurationArn = value; }


    inline void SetMatchmakingConfigurationArn(const char* value) { m_matchmakingConfigurationArn.assign(value); }


    inline StartMatchBackfillRequest& WithMatchmakingConfigurationArn(const std::string& value) { SetMatchmakingConfigurationArn(value); return *this; }


    inline StartMatchBackfillRequest& WithMatchmakingConfigurationArn(std::string&& value) { SetMatchmakingConfigurationArn(value); return *this; }


    inline StartMatchBackfillRequest& WithMatchmakingConfigurationArn(const char* value) { SetMatchmakingConfigurationArn(value); return *this; }


    inline const std::string& GetGameSessionArn() const { return m_gameSessionArn; }


    inline void SetGameSessionArn(const std::string& value) { m_gameSessionArn = value; }


    inline void SetGameSessionArn(std::string&& value) { m_gameSessionArn = value; }


    inline void SetGameSessionArn(const char* value) { m_gameSessionArn.assign(value); }


    inline StartMatchBackfillRequest& WithGameSessionArn(const std::string& value) { SetGameSessionArn(value); return *this; }


    inline StartMatchBackfillRequest& WithGameSessionArn(std::string&& value) { SetGameSessionArn(value); return *this; }


    inline StartMatchBackfillRequest& WithGameSessionArn(const char* value) { SetGameSessionArn(value); return *this; }


    inline const std::vector<Player>& GetPlayers() const { return m_players; }


    inline void SetPlayers(const std::vector<Player>& value) { m_players = value; }


    inline void SetPlayers(std::vector<Player>&& value) { m_players = value; }


    inline StartMatchBackfillRequest& WithPlayer(const std::vector<Player>& value) { SetPlayers(value); return *this; }


    inline StartMatchBackfillRequest& WithPlayer(std::vector<Player>&& value) { SetPlayers(value); return *this; }


    inline StartMatchBackfillRequest& AddPlayer(const Player& value) { m_players.push_back(value); return *this; }


    inline StartMatchBackfillRequest& AddPlayer(Player&& value) { m_players.push_back(value); return *this; }


private:
    std::string m_ticketId;
    std::string m_matchmakingConfigurationArn;
    std::string m_gameSessionArn;
    std::vector<Player> m_players;
#else
public:
    StartMatchBackfillRequest() : 
		m_playerCount(0)
    {
		memset(m_ticketId, 0, sizeof(m_ticketId));
		memset(m_matchmakingConfigurationArn, 0, sizeof(m_matchmakingConfigurationArn));
		memset(m_gameSessionArn, 0, sizeof(m_gameSessionArn));
    }

    /**
    * <p>Destructor.</p>
    */
    ~StartMatchBackfillRequest() {}

    /**
    * <p>Copy Constructor.</p>
    */
    StartMatchBackfillRequest(const StartMatchBackfillRequest& other)
    {
        *this = other;
    }

    /**
    * <p>Move Constructor.</p>
    */
    StartMatchBackfillRequest(StartMatchBackfillRequest&& other) 
    {
        *this = std::move(other);
    }

    /**
    * <p>Copy assignment Constructor.</p>
    */
    StartMatchBackfillRequest& operator= (const StartMatchBackfillRequest& other) 
    {
        SetTicketId(other.m_ticketId);
        SetMatchmakingConfigurationArn(other.m_matchmakingConfigurationArn);
        SetGameSessionArn(other.m_gameSessionArn);

        m_playerCount = 0;
        for (int index = 0; index < other.m_playerCount; ++index)
        {
            AddPlayer(other.m_players[index]);
        }

        return *this;
    }

    /**
    * <p>Move assignment Constructor.</p>
    */
    StartMatchBackfillRequest& operator= (StartMatchBackfillRequest&& other) 
    {
        *this = other;

        return *this;
    }


    inline const char* GetTicketId() const { return m_ticketId; }


    inline void SetTicketId(const char* value) { strncpy(m_ticketId, value, sizeof(m_ticketId)); m_ticketId[MAX_TICKET_LENGTH - 1] = '\0'; }


    inline StartMatchBackfillRequest& WithTicketId(const char* value) { SetTicketId(value); return *this; }


    inline const char* GetMatchmakingConfigurationArn() const { return m_matchmakingConfigurationArn; }


    inline void SetMatchmakingConfigurationArn(const char* value)
    {
        strncpy(m_matchmakingConfigurationArn, value, sizeof(m_matchmakingConfigurationArn));
        m_matchmakingConfigurationArn[MAX_ARN_LENGTH - 1] = '\0';
    }


    inline StartMatchBackfillRequest& WithMatchmakingConfigurationArn(const char* value) { SetMatchmakingConfigurationArn(value); return *this; }


    inline const char* GetGameSessionArn() const { return m_gameSessionArn; }


    inline void SetGameSessionArn(const char* value)
    {
        strncpy(m_gameSessionArn, value, sizeof(m_gameSessionArn));
        m_gameSessionArn[MAX_ARN_LENGTH - 1] = '\0';
    }


    inline StartMatchBackfillRequest& WithGameSessionArn(const char* value) { SetGameSessionArn(value); return *this; }


    inline const Player* GetPlayers(int& count) const { count = m_playerCount; return m_players; }


    inline void AddPlayer(const Player& player) {
        if (m_playerCount < MAX_PLAYERS) {
            m_players[m_playerCount++] = player;
        }
    };


    inline StartMatchBackfillRequest& WithPlayer(Player player) { AddPlayer(player); return *this; }


private:
    char m_ticketId[MAX_TICKET_LENGTH];
    char m_matchmakingConfigurationArn[MAX_ARN_LENGTH];
    char m_gameSessionArn[MAX_ARN_LENGTH];
    Player m_players[MAX_PLAYERS];
    int m_playerCount;
#endif
};

} // namespace Model
} // namespace Server
} // namespace GameLift
} // namespace Aws
