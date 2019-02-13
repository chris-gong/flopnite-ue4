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
#include <aws/gamelift/server/model/PlayerSession.h>

#ifndef GAMELIFT_USE_STD
    #ifndef MAX_PLAYER_SESSIONS
        #define MAX_PLAYER_SESSIONS 50
    #endif
    #ifndef MAX_NEXT_TOKEN_LENGTH
        #define MAX_NEXT_TOKEN_LENGTH 1024
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
   * <p>Represents the returned data in response to a request action.</p>
   */
  class AWS_GAMELIFT_API DescribePlayerSessionsResult
  {
#ifdef GAMELIFT_USE_STD
  public:
      DescribePlayerSessionsResult() {}

      /**
      * <p>Destructor.</p>
      */
      ~DescribePlayerSessionsResult() {}

      /**
      * <p>Copy Constructor.</p>
      */
      DescribePlayerSessionsResult(const DescribePlayerSessionsResult& other) :
          m_playerSessions(other.m_playerSessions),
          m_nextToken(other.m_nextToken)
      { }

      /**
      * <p>Move Constructor.</p>
      */
      DescribePlayerSessionsResult(DescribePlayerSessionsResult&& other) 
      {
          *this = std::move(other);
      }


      /**
      * <p>Copy assignment Constructor.</p>
      */
      DescribePlayerSessionsResult& operator= (const DescribePlayerSessionsResult& other) 
      {
          m_playerSessions = other.m_playerSessions;
          m_nextToken = other.m_nextToken;

          return *this;
      }

      /**
      * <p>Move assignment Constructor.</p>
      */
      DescribePlayerSessionsResult& operator= (DescribePlayerSessionsResult&& other) 
      {
          m_playerSessions = std::move(other.m_playerSessions);
          m_nextToken = std::move(other.m_nextToken);

          return *this;
      }


    /**
     * <p>Collection of objects containing properties for each player session that
     * matches the request.</p>
     */
    inline const std::vector<PlayerSession>& GetPlayerSessions() const{ return m_playerSessions; }

    /**
     * <p>Collection of objects containing properties for each player session that
     * matches the request.</p>
     */
    inline void SetPlayerSessions(const std::vector<PlayerSession>& value) { m_playerSessions = value; }

    /**
     * <p>Collection of objects containing properties for each player session that
     * matches the request.</p>
     */
    inline void SetPlayerSessions(std::vector<PlayerSession>&& value) { m_playerSessions = value; }

    /**
     * <p>Collection of objects containing properties for each player session that
     * matches the request.</p>
     */
    inline DescribePlayerSessionsResult& WithPlayerSessions(const std::vector<PlayerSession>& value) { SetPlayerSessions(value); return *this;}

    /**
     * <p>Collection of objects containing properties for each player session that
     * matches the request.</p>
     */
    inline DescribePlayerSessionsResult& WithPlayerSessions(std::vector<PlayerSession>&& value) { SetPlayerSessions(value); return *this;}

    /**
     * <p>Collection of objects containing properties for each player session that
     * matches the request.</p>
     */
    inline DescribePlayerSessionsResult& AddPlayerSessions(const PlayerSession& value) { m_playerSessions.push_back(value); return *this; }

    /**
     * <p>Collection of objects containing properties for each player session that
     * matches the request.</p>
     */
    inline DescribePlayerSessionsResult& AddPlayerSessions(PlayerSession&& value) { m_playerSessions.push_back(value); return *this; }

    /**
     * <p>Token indicating where to resume retrieving results on the next call to this
     * action. If no token is returned, these results represent the end of the
     * list.</p>
     */
    inline const std::string& GetNextToken() const{ return m_nextToken; }

    /**
     * <p>Token indicating where to resume retrieving results on the next call to this
     * action. If no token is returned, these results represent the end of the
     * list.</p>
     */
    inline void SetNextToken(const std::string& value) { m_nextToken = value; }

    /**
     * <p>Token indicating where to resume retrieving results on the next call to this
     * action. If no token is returned, these results represent the end of the
     * list.</p>
     */
    inline void SetNextToken(std::string&& value) { m_nextToken = value; }

    /**
     * <p>Token indicating where to resume retrieving results on the next call to this
     * action. If no token is returned, these results represent the end of the
     * list.</p>
     */
    inline void SetNextToken(const char* value) { m_nextToken.assign(value); }

    /**
     * <p>Token indicating where to resume retrieving results on the next call to this
     * action. If no token is returned, these results represent the end of the
     * list.</p>
     */
    inline DescribePlayerSessionsResult& WithNextToken(const std::string& value) { SetNextToken(value); return *this;}

    /**
     * <p>Token indicating where to resume retrieving results on the next call to this
     * action. If no token is returned, these results represent the end of the
     * list.</p>
     */
    inline DescribePlayerSessionsResult& WithNextToken(std::string&& value) { SetNextToken(value); return *this;}

    /**
     * <p>Token indicating where to resume retrieving results on the next call to this
     * action. If no token is returned, these results represent the end of the
     * list.</p>
     */
    inline DescribePlayerSessionsResult& WithNextToken(const char* value) { SetNextToken(value); return *this;}

  private:
    std::vector<PlayerSession> m_playerSessions;
    std::string m_nextToken;
#else
public:
    DescribePlayerSessionsResult()
    {
        memset(m_nextToken, 0, sizeof(m_nextToken));
        m_playerSessions_count = 0;
    }

    /**
    * <p>Destructor.</p>
    */
    ~DescribePlayerSessionsResult() {}

    /**
    * <p>Copy Constructor.</p>
    */
    DescribePlayerSessionsResult(const DescribePlayerSessionsResult& other) :
        m_playerSessions_count(other.m_playerSessions_count)
    {
        strncpy(m_nextToken, other.m_nextToken, sizeof(other.m_nextToken));

        std::copy(std::begin(other.m_playerSessions), std::end(other.m_playerSessions), std::begin(m_playerSessions));
    }

    /**
    * <p>Move Constructor.</p>
    */
    DescribePlayerSessionsResult(DescribePlayerSessionsResult&& other)
    {
		*this = std::move(other);
    }

    /**
    * <p>Copy assignment Constructor.</p>
    */
    DescribePlayerSessionsResult& operator= (const DescribePlayerSessionsResult& other) 
    {
        m_playerSessions_count = other.m_playerSessions_count;

        strncpy(m_nextToken, other.m_nextToken, sizeof(other.m_nextToken));

        std::copy(std::begin(other.m_playerSessions), std::end(other.m_playerSessions), std::begin(m_playerSessions));

        return *this;
    }

    /**
    * <p>Move assignment Constructor.</p>
    */
    DescribePlayerSessionsResult& operator= (DescribePlayerSessionsResult&& other) 
    {
        m_playerSessions_count = other.m_playerSessions_count;

        strncpy(m_nextToken, other.m_nextToken, sizeof(other.m_nextToken));

        std::copy(std::begin(other.m_playerSessions), std::end(other.m_playerSessions), std::begin(m_playerSessions));

		other.m_playerSessions_count = 0;

		memset(other.m_nextToken, 0, sizeof(other.m_nextToken));
		memset(other.m_playerSessions, 0, sizeof(other.m_playerSessions));

        return *this;
    }

    /**
     * <p>Collection of objects containing properties for each player session that
     * matches the request.</p>
     */
    inline const PlayerSession* GetPlayerSessions(int& count) const{ count = m_playerSessions_count; return m_playerSessions; }

    /**
    * <p>Set of player session for the request.</p>
    */
    inline void AddPlayerSession(PlayerSession playerSession) {
        if (m_playerSessions_count < MAX_PLAYER_SESSIONS) {
            m_playerSessions[m_playerSessions_count++] = playerSession;
        }
    };

    /**
     * <p>Collection of objects containing properties for each player session that
     * matches the request.</p>
     */
    inline DescribePlayerSessionsResult& WithPlayerSessions(PlayerSession playerSession) { AddPlayerSession(playerSession); return *this;}

    /**
     * <p>Token indicating where to resume retrieving results on the next call to this
     * action. If no token is returned, these results represent the end of the
     * list.</p>
     */
    inline const char* GetNextToken() const{ return m_nextToken; }

    /**
     * <p>Token indicating where to resume retrieving results on the next call to this
     * action. If no token is returned, these results represent the end of the
     * list.</p>
     */
    inline void SetNextToken(const char* value) { strncpy(m_nextToken, value, sizeof(m_nextToken)); m_nextToken[sizeof(m_nextToken)-1] = 0; }

    /**
     * <p>Token indicating where to resume retrieving results on the next call to this
     * action. If no token is returned, these results represent the end of the
     * list.</p>
     */
    inline DescribePlayerSessionsResult& WithNextToken(const char* value) { SetNextToken(value); return *this;}

  private:
    PlayerSession m_playerSessions[MAX_PLAYER_SESSIONS];
    int m_playerSessions_count;
    char m_nextToken[MAX_NEXT_TOKEN_LENGTH];
#endif
  };

} // namespace Model
} // namespace Server
} // namespace GameLift
} // namespace Aws
