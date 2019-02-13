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
#include <aws/gamelift/server/model/AttributeValue.h>

#ifndef GAMELIFT_USE_STD
    #ifndef MAX_ATTR_STRING_LENGTH
        #define MAX_ATTR_STRING_LENGTH 101
    #endif
    #ifndef MAX_TEAM_STRING_LENGTH
	#define MAX_TEAM_STRING_LENGTH 101
    #endif
    #ifndef MAX_PLAYER_ID_STRING_LENGTH
	#define MAX_PLAYER_ID_STRING_LENGTH 129
    #endif
    #ifndef MAX_PLAYER_ATTRIBUTES_SIZE
	#define MAX_PLAYER_ATTRIBUTES_SIZE 10
    #endif
    #ifndef MAX_LATENCY_SIZE
	#define MAX_LATENCY_SIZE 20
    #endif
    #ifndef REGION_LENGTH
    	#define REGION_LENGTH 65
    #endif
#else
#include <map>
#include <string>
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
    * <p>A player object to be used when making a StartMatchBackfill request.
    * For more information, see the <a
    * href="http://docs.aws.amazon.com/gamelift/latest/developerguide/">Amazon
    * GameLift Developer Guide</a>.</p>
    */
class AWS_GAMELIFT_API Player
{
#ifdef GAMELIFT_USE_STD
public:
    /**
    * <p>Constructor.</p>
    */
    Player()
    {
    }

    /**
    * <p>Destructor.</p>
    */
    ~Player() {}

    /**
    * <p>Copy Constructor.</p>
    */
    Player(const Player& other) :
        m_playerId(other.m_playerId),
        m_team(other.m_team),
        m_playerAttributes(other.m_playerAttributes),
        m_latencyInMs(other.m_latencyInMs)
    { }

    /**
    * <p>Move Constructor.</p>
    */
    Player(Player&& other) 
    { 
        *this = std::move(other);
	}

    /**
    * <p>Copy assignment Constructor.</p>
    */
    Player& operator= (const Player& other) 
    {
        m_playerId = other.m_playerId;
        m_team = other.m_team;
        m_playerAttributes = other.m_playerAttributes;
        m_latencyInMs = other.m_latencyInMs;

        return *this;
    }

    /**
    * <p>Move assignment Constructor.</p>
    */
    Player& operator= (Player&& other) 
    {
        m_playerId = std::move(other.m_playerId);
        m_team = std::move(other.m_team);
        m_playerAttributes = std::move(other.m_playerAttributes);
        m_latencyInMs = std::move(other.m_latencyInMs);

        return *this;
    }

    inline const std::string& GetPlayerId() const { return m_playerId; }


    inline void SetPlayerId(const std::string& value) { m_playerId = value; }


    inline void SetPlayerId(std::string&& value) { m_playerId = value; }


    inline void SetPlayerId(const char* value) { m_playerId.assign(value); }


    inline Player& WithPlayerId(const std::string& value) { SetPlayerId(value); return *this; }


    inline Player& WithPlayerId(std::string&& value) { SetPlayerId(value); return *this; }


    inline Player& WithPlayerId(const char* value) { SetPlayerId(value); return *this; }


    inline const std::string& GetTeam() const { return m_team; }


    inline void SetTeam(const std::string& value) { m_team = value; }


    inline void SetTeam(std::string&& value) { m_team = value; }


    inline void SetTeam(const char* value) { m_team.assign(value); }


    inline Player& WithTeam(const std::string& value) { SetTeam(value); return *this; }


    inline Player& WithTeam(std::string&& value) { SetTeam(value); return *this; }


    inline Player& WithTeam(const char* value) { SetTeam(value); return *this; }


    inline const std::map<std::string, AttributeValue>& GetPlayerAttributes() const { return m_playerAttributes; }


    inline void SetPlayerAttributes(const std::map<std::string, AttributeValue>& value) { m_playerAttributes = value; }


    inline void SetPlayerAttributes(const std::map<std::string, AttributeValue>&& value) { m_playerAttributes = std::move(value); }


    inline Player& WithPlayerAttributes(const std::map<std::string, AttributeValue>& value) { SetPlayerAttributes(value); return *this; }


    inline Player& WithPlayerAttributes(std::map<std::string, AttributeValue>&& value) { SetPlayerAttributes(value); return *this; }


    inline Player& AddPlayerAttribute(const std::string& attrName, const AttributeValue& attrValue)
    {
        m_playerAttributes.emplace(attrName, attrValue);
        return *this;
    }


    inline Player& AddPlayerAttribute(const std::string&& attrName, const AttributeValue&& attrValue)
    {
        m_playerAttributes.emplace(std::move(attrName), std::move(attrValue));
        return *this;
    }


    inline const std::map<std::string, int>& GetLatencyInMs() const { return m_latencyInMs; }


    inline void SetLatencyInMs(const std::map<std::string, int>& value) { m_latencyInMs = value; }


    inline void SetLatencyInMs(const std::map<std::string, int>&& value) { m_latencyInMs = std::move(value); }


    inline Player& WithLatencyInMs(const std::map<std::string, int>& value) { SetLatencyInMs(value); return *this; }


    inline Player& WithLatencyInMs(std::map<std::string, int>&& value) { SetLatencyInMs(value); return *this; }


    inline Player& AddLatencyInMs(const std::string& attrName, const int& attrValue) { m_latencyInMs[attrName] = attrValue; return *this; }


    inline Player& AddLatencyInMs(const std::string&& attrName, const int& attrValue)
    {
        m_latencyInMs.emplace(std::move(attrName), attrValue);
        return *this;
    }


private:
    std::string m_playerId;
    std::string m_team;
    std::map<std::string, AttributeValue> m_playerAttributes;
    std::map<std::string, int> m_latencyInMs;
#else
public:
    struct NamedAttribute;
    struct RegionAndLatency;
    typedef char AttributeStringType[MAX_ATTR_STRING_LENGTH];

    /**
    * <p>Constructor.</p>
    */
    Player() : m_attributeCount(0), m_latencyCount(0)
    {
        m_playerId[0] = 0;
        m_team[0] = 0;
    }

    /**
    * <p>Destructor.</p>
    */
    ~Player() {}

    /**
    * <p>Copy Constructor.</p>
    */
    Player(const Player& other)
    {
        *this = other;
    }

    /**
    * <p>Move Constructor.</p>
    */
    Player(Player&& other) 
    {
        *this = std::move(other);
    }

    /**
    * <p>Copy assignment Constructor.</p>
    */
    Player& operator= (const Player& other) 
    {
        SetPlayerId(other.m_playerId);
        SetTeam(other.m_team);

        m_attributeCount = other.m_attributeCount;
        m_latencyCount = other.m_latencyCount;

        for (int index = 0; index < m_attributeCount; ++index)
        {
            m_playerAttributes[index] = other.m_playerAttributes[index];
        }
        for (int index = 0; index < m_latencyCount; ++index)
        {
            m_latencyInMs[index] = other.m_latencyInMs[index];
        }

        return *this;
    }

    /**
    * <p>Move assignment Constructor.</p>
    */
    Player& operator= (Player&& other) 
    {
        *this = other;

        return *this;
    }


    inline const char* GetPlayerId() const { return m_playerId; }


    inline void SetPlayerId(const char* value) { strncpy(m_playerId, value, sizeof(m_playerId)); m_playerId[MAX_PLAYER_ID_STRING_LENGTH - 1] = '\0'; }


    inline Player& WithPlayerId(const char* value) { SetPlayerId(value); return *this; }


    inline const char* GetTeam() const { return m_team; }


    inline void SetTeam(const char* value) { strncpy(m_team, value, sizeof(m_team)); m_team[MAX_TEAM_STRING_LENGTH - 1] = '\0'; }


    inline Player& WithTeam(const char* value) { SetTeam(value); return *this; }


    inline const NamedAttribute* GetPlayerAttributes(int& count) const { count = m_attributeCount; return m_playerAttributes; }


    inline void AddPlayerAttribute(const char* attrName, const AttributeValue& attrValue)
    {
        if (m_attributeCount < MAX_PLAYER_ATTRIBUTES_SIZE)
        {
            m_playerAttributes[m_attributeCount++] = NamedAttribute(attrName, attrValue);
        }
    }


    inline Player& WithPlayerAttribute(const char* attrName, const AttributeValue& attrValue) { AddPlayerAttribute(attrName, attrValue); return *this; }


    inline const RegionAndLatency* GetLatencyMs(int& count) const { count = m_latencyCount; return m_latencyInMs; }


    inline void AddLatencyMs(const char* region, int latencyMs)
    {
        if (m_latencyCount < MAX_LATENCY_SIZE)
        {
            m_latencyInMs[m_latencyCount++] = RegionAndLatency(region, latencyMs);
        }
    };

    inline Player& WithLatencyMs(const char* region, int latencyMs) { AddLatencyMs(region, latencyMs); return *this; }


    struct NamedAttribute
    {
        NamedAttribute()
        {
            m_name[0] = 0;
        }

        NamedAttribute(const char* name, const AttributeValue& value) : m_value(value)
        {
            strncpy(m_name, name, sizeof(m_name));
            m_name[sizeof(m_name) - 1] = 0;
        }

        NamedAttribute(const NamedAttribute& other) : m_value(other.m_value)
        {
            strncpy(m_name, other.m_name, sizeof(m_name));
            m_name[sizeof(m_name) - 1] = 0;
        }

        NamedAttribute& operator=(const NamedAttribute& other)
        {
            strncpy(m_name, other.m_name, sizeof(m_name));
            m_name[sizeof(m_name) - 1] = 0;
            m_value = other.m_value;

            return *this;
        }

        NamedAttribute(const NamedAttribute&& other) : m_value(std::move(other.m_value))
        {
            strncpy(m_name, other.m_name, sizeof(m_name));
            m_name[sizeof(m_name) - 1] = 0;
        }

        NamedAttribute& operator=(const NamedAttribute&& other)
        {
            *this = other;

            return *this;
        }

        inline const char* GetName() const { return m_name; }

        inline AttributeValue GetValue() const { return m_value; }

    private:
        AttributeStringType m_name;
        AttributeValue m_value;
    };

    struct RegionAndLatency
    {
        RegionAndLatency() : m_latencyMs(0)
        {
            m_region[0] = 0;
        }

        RegionAndLatency(const char* region, int latencyMs)
        {
            strncpy(m_region, region, sizeof(m_region));
            m_region[sizeof(m_region) - 1] = 0;
            m_latencyMs = latencyMs;
        }

        RegionAndLatency(const RegionAndLatency& other)
        {
            *this = other;
        }

        RegionAndLatency& operator=(const RegionAndLatency& other)
        {
            strncpy(m_region, other.m_region, sizeof(m_region));
            m_region[sizeof(m_region) - 1] = 0;
            m_latencyMs = other.m_latencyMs;

            return *this;
        }

        RegionAndLatency(const RegionAndLatency&& other)
        {
            *this = std::move(other);
        }

        RegionAndLatency& operator=(const RegionAndLatency&& other)
        {
            *this = other;

            return *this;
        }

        inline const char* GetRegion() const { return m_region; }

        inline int GetLatencyMs() const { return m_latencyMs; }

    private:
        char m_region[REGION_LENGTH];
        int m_latencyMs;
    };

private:
    char m_playerId[MAX_PLAYER_ID_STRING_LENGTH];
    char m_team[MAX_TEAM_STRING_LENGTH];
    NamedAttribute m_playerAttributes[MAX_PLAYER_ATTRIBUTES_SIZE];
    RegionAndLatency m_latencyInMs[MAX_LATENCY_SIZE];
    int m_attributeCount;
    int m_latencyCount;
#endif
};

} // namespace Model
} // namespace Server
} // namespace GameLift
} // namespace Aws
