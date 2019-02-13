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
#include <aws/gamelift/server/model/GameSessionStatus.h>
#include <aws/gamelift/server/model/PlayerSessionCreationPolicy.h>
#include <aws/gamelift/server/model/GameProperty.h>


#ifndef GAMELIFT_USE_STD
    #ifndef MAX_GAME_PROPERTIES
        #define MAX_GAME_PROPERTIES 32
    #endif
    #ifndef MAX_IP_LENGTH
        #define MAX_IP_LENGTH 45
    #endif
    #ifndef MAX_SESSION_ID_LENGTH
        #define MAX_SESSION_ID_LENGTH 256
    #endif
    #ifndef MAX_SESSION_NAME_LENGTH
        #define MAX_SESSION_NAME_LENGTH 256
    #endif
    #ifndef MAX_FLEET_ID_LENGTH
        #define MAX_FLEET_ID_LENGTH 1024
    #endif
    #ifndef MAX_GAME_SESSION_DATA_LENGTH
        #define MAX_GAME_SESSION_DATA_LENGTH 4097
    #endif
    #ifndef MAX_MATCHMAKER_DATA_LENGTH
        #define MAX_MATCHMAKER_DATA_LENGTH 4097
    #endif
    #ifndef MAX_DNS_NAME_LENGTH
        #define MAX_DNS_NAME_LENGTH 64
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
    * <p>Properties describing a game session.</p>
    */
    class AWS_GAMELIFT_API GameSession
    {
#ifdef GAMELIFT_USE_STD
    public:
        GameSession() :
            m_maximumPlayerSessionCount(0),
            m_port(0)
        { }

        /**
        * <p>Destructor.</p>
        */
        ~GameSession() {}

        /**
        * <p>Copy Constructor.</p>
        */
        GameSession(const GameSession& other) :
            m_maximumPlayerSessionCount(other.m_maximumPlayerSessionCount),
            m_port(other.m_port),
            m_ipAddress(other.m_ipAddress),
            m_gameSessionId(other.m_gameSessionId),
            m_name(other.m_name),
            m_fleetId(other.m_fleetId),
            m_status(other.m_status),
            m_gameProperties(other.m_gameProperties),
            m_gameSessionData(other.m_gameSessionData),
            m_matchmakerData(other.m_matchmakerData),
            m_dnsName(other.m_dnsName)
        { }

        /**
        * <p>Move Constructor.</p>
        */
        GameSession(GameSession&& other)  :
        m_maximumPlayerSessionCount(other.m_maximumPlayerSessionCount),
            m_port(other.m_port),
            m_ipAddress(std::move(other.m_ipAddress)),
            m_gameSessionId(std::move(other.m_gameSessionId)),
            m_name(std::move(other.m_name)),
            m_fleetId(std::move(other.m_fleetId)),
            m_status(std::move(other.m_status)),
            m_gameProperties(std::move(other.m_gameProperties)),
            m_gameSessionData(std::move(other.m_gameSessionData)),
            m_matchmakerData(std::move(other.m_matchmakerData)),
            m_dnsName(other.m_dnsName)
        {
        }


        /**
        * <p>Copy assignment Constructor.</p>
        */
        GameSession& operator= (const GameSession& other) 
        {
            m_maximumPlayerSessionCount = other.m_maximumPlayerSessionCount;
            m_port = other.m_port;
            m_ipAddress = other.m_ipAddress;
            m_gameSessionId = other.m_gameSessionId;
            m_name = other.m_name;
            m_fleetId = other.m_fleetId;
            m_status = other.m_status;
            m_gameProperties = other.m_gameProperties;
            m_gameSessionData = other.m_gameSessionData;
            m_matchmakerData = other.m_matchmakerData;
            m_dnsName = other.m_dnsName;

            return *this;
        }

        /**
        * <p>Move assignment Constructor.</p>
        */
        GameSession& operator= (GameSession&& other) 
        {
            m_maximumPlayerSessionCount = other.m_maximumPlayerSessionCount;
            other.m_maximumPlayerSessionCount = 0;

            m_port = other.m_port;
            other.m_port = 0;

            m_ipAddress = std::move(other.m_ipAddress);
            m_gameSessionId = std::move(other.m_gameSessionId);
            m_name = std::move(other.m_name);
            m_fleetId = std::move(other.m_fleetId);
            m_status = std::move(other.m_status);
            m_gameProperties = std::move(other.m_gameProperties);
            m_gameSessionData = std::move(other.m_gameSessionData);
            m_matchmakerData = std::move(other.m_matchmakerData);
            m_dnsName = std::move(other.m_dnsName);
            return *this;
        }


        /**
        * <p>Unique identifier for a game session.</p>
        */
        inline const std::string& GetGameSessionId() const{ return m_gameSessionId; }

        /**
        * <p>Unique identifier for a game session.</p>
        */
        inline void SetGameSessionId(const std::string& value) { m_gameSessionId = value; }

        /**
        * <p>Unique identifier for a game session.</p>
        */
        inline void SetGameSessionId(std::string&& value) { m_gameSessionId = value; }

        /**
        * <p>Unique identifier for a game session.</p>
        */
        inline void SetGameSessionId(const char* value) { m_gameSessionId.assign(value); }

        /**
        * <p>Unique identifier for a game session.</p>
        */
        inline GameSession& WithGameSessionId(const std::string& value) { SetGameSessionId(value); return *this; }

        /**
        * <p>Unique identifier for a game session.</p>
        */
        inline GameSession& WithGameSessionId(std::string&& value) { SetGameSessionId(value); return *this; }

        /**
        * <p>Unique identifier for a game session.</p>
        */
        inline GameSession& WithGameSessionId(const char* value) { SetGameSessionId(value); return *this; }

        /**
        * <p>Descriptive label associated with a game session. Session names do not need
        * to be unique.</p>
        */
        inline const std::string& GetName() const{ return m_name; }

        /**
        * <p>Descriptive label associated with a game session. Session names do not need
        * to be unique.</p>
        */
        inline void SetName(const std::string& value) { m_name = value; }

        /**
        * <p>Descriptive label associated with a game session. Session names do not need
        * to be unique.</p>
        */
        inline void SetName(std::string&& value) { m_name = value; }

        /**
        * <p>Descriptive label associated with a game session. Session names do not need
        * to be unique.</p>
        */
        inline void SetName(const char* value) { m_name.assign(value); }

        /**
        * <p>Descriptive label associated with a game session. Session names do not need
        * to be unique.</p>
        */
        inline GameSession& WithName(const std::string& value) { SetName(value); return *this; }

        /**
        * <p>Descriptive label associated with a game session. Session names do not need
        * to be unique.</p>
        */
        inline GameSession& WithName(std::string&& value) { SetName(value); return *this; }

        /**
        * <p>Descriptive label associated with a game session. Session names do not need
        * to be unique.</p>
        */
        inline GameSession& WithName(const char* value) { SetName(value); return *this; }

        /**
        * <p>Unique identifier for a fleet.</p>
        */
        inline const std::string& GetFleetId() const{ return m_fleetId; }

        /**
        * <p>Unique identifier for a fleet.</p>
        */
        inline void SetFleetId(const std::string& value) { m_fleetId = value; }

        /**
        * <p>Unique identifier for a fleet.</p>
        */
        inline void SetFleetId(std::string&& value) { m_fleetId = value; }

        /**
        * <p>Unique identifier for a fleet.</p>
        */
        inline void SetFleetId(const char* value) { m_fleetId.assign(value); }

        /**
        * <p>Unique identifier for a fleet.</p>
        */
        inline GameSession& WithFleetId(const std::string& value) { SetFleetId(value); return *this; }

        /**
        * <p>Unique identifier for a fleet.</p>
        */
        inline GameSession& WithFleetId(std::string&& value) { SetFleetId(value); return *this; }

        /**
        * <p>Unique identifier for a fleet.</p>
        */
        inline GameSession& WithFleetId(const char* value) { SetFleetId(value); return *this; }

        /**
        * <p>Maximum number of players allowed in the game session.</p>
        */
        inline int GetMaximumPlayerSessionCount() const{ return m_maximumPlayerSessionCount; }

        /**
        * <p>Maximum number of players allowed in the game session.</p>
        */
        inline void SetMaximumPlayerSessionCount(int value) { m_maximumPlayerSessionCount = value; }

        /**
        * <p>Maximum number of players allowed in the game session.</p>
        */
        inline GameSession& WithMaximumPlayerSessionCount(int value) { SetMaximumPlayerSessionCount(value); return *this; }

        /**
        * <p>Current status of the game session. A game session must be in an
        * <code>ACTIVE</code> state to have player sessions.</p>
        */
        inline const GameSessionStatus& GetStatus() const{ return m_status; }

        /**
        * <p>Current status of the game session. A game session must be in an
        * <code>ACTIVE</code> state to have player sessions.</p>
        */
        inline void SetStatus(const GameSessionStatus& value) { m_status = value; }

        /**
        * <p>Current status of the game session. A game session must be in an
        * <code>ACTIVE</code> state to have player sessions.</p>
        */
        inline void SetStatus(GameSessionStatus&& value) { m_status = value; }

        /**
        * <p>Current status of the game session. A game session must be in an
        * <code>ACTIVE</code> state to have player sessions.</p>
        */
        inline GameSession& WithStatus(const GameSessionStatus& value) { SetStatus(value); return *this; }

        /**
        * <p>Current status of the game session. A game session must be in an
        * <code>ACTIVE</code> state to have player sessions.</p>
        */
        inline GameSession& WithStatus(GameSessionStatus&& value) { SetStatus(value); return *this; }

        /**
        * <p>Set of custom properties for the game session.</p>
        */
        inline const std::vector<GameProperty>& GetGameProperties() const{ return m_gameProperties; }

        /**
        * <p>Set of custom properties for the game session.</p>
        */
        inline void SetGameProperties(const std::vector<GameProperty>& value) { m_gameProperties = value; }

        /**
        * <p>Set of custom properties for the game session.</p>
        */
        inline void SetGameProperties(std::vector<GameProperty>&& value) { m_gameProperties = value; }

        /**
        * <p>Set of custom properties for the game session.</p>
        */
        inline GameSession& WithGameProperties(const std::vector<GameProperty>& value) { SetGameProperties(value); return *this; }

        /**
        * <p>Set of custom properties for the game session.</p>
        */
        inline GameSession& WithGameProperties(std::vector<GameProperty>&& value) { SetGameProperties(value); return *this; }

        /**
        * <p>Set of custom properties for the game session.</p>
        */
        inline GameSession& AddGameProperties(const GameProperty& value) { m_gameProperties.push_back(value); return *this; }

        /**
        * <p>Set of custom properties for the game session.</p>
        */
        inline GameSession& AddGameProperties(GameProperty&& value) { m_gameProperties.push_back(value); return *this; }

        /**
        * <p>IP address of the game session. To connect to a GameLift server process, an
        * app needs both the IP address and port number.</p>
        */
        inline const std::string& GetIpAddress() const{ return m_ipAddress; }

        /**
        * <p>IP address of the game session. To connect to a GameLift server process, an
        * app needs both the IP address and port number.</p>
        */
        inline void SetIpAddress(const std::string& value) { m_ipAddress = value; }

        /**
        * <p>IP address of the game session. To connect to a GameLift server process, an
        * app needs both the IP address and port number.</p>
        */
        inline void SetIpAddress(std::string&& value) { m_ipAddress = value; }

        /**
        * <p>IP address of the game session. To connect to a GameLift server process, an
        * app needs both the IP address and port number.</p>
        */
        inline void SetIpAddress(const char* value) { m_ipAddress.assign(value); }

        /**
        * <p>IP address of the game session. To connect to a GameLift server process, an
        * app needs both the IP address and port number.</p>
        */
        inline GameSession& WithIpAddress(const std::string& value) { SetIpAddress(value); return *this; }

        /**
        * <p>IP address of the game session. To connect to a GameLift server process, an
        * app needs both the IP address and port number.</p>
        */
        inline GameSession& WithIpAddress(std::string&& value) { SetIpAddress(value); return *this; }

        /**
        * <p>IP address of the game session. To connect to a GameLift server process, an
        * app needs both the IP address and port number.</p>
        */
        inline GameSession& WithIpAddress(const char* value) { SetIpAddress(value); return *this; }

        /**
        * <p>Port number for the game session. To connect to a GameLift server process, an
        * app needs both the IP address and port number.</p>
        */
        inline int GetPort() const{ return m_port; }

        /**
        * <p>Port number for the game session. To connect to a GameLift server process, an
        * app needs both the IP address and port number.</p>
        */
        inline void SetPort(int value) { m_port = value; }

        /**
        * <p>Port number for the game session. To connect to a GameLift server process, an
        * app needs both the IP address and port number.</p>
        */
        inline GameSession& WithPort(int value) { SetPort(value); return *this; }

        /**
        * <p>Custom data for the game session.</p>
        */
        inline const std::string& GetGameSessionData() const{ return m_gameSessionData; }

        /**
        * <p>Custom data for the game session.</p>
        */
        inline void SetGameSessionData(const std::string& value) { m_gameSessionData = value; }

        /**
        * <p>Custom data for the game session.</p>
        */
        inline void SetGameSessionData(std::string&& value) { m_gameSessionData = value; }

        /**
        * <p>Custom data for the game session.</p>
        */
        inline void SetGameSessionData(const char* value) { m_gameSessionData.assign(value); }

        /**
        * <p>Custom data for the game session.</p>
        */
        inline GameSession& WithGameSessionData(const std::string& value) { SetGameSessionData(value); return *this; }

        /**
        * <p>Custom data for the game session.</p>
        */
        inline GameSession& WithGameSessionData(std::string&& value) { SetGameSessionData(value); return *this; }

        /**
        * <p>Custom data for the game session.</p>
        */
        inline GameSession& WithGameSessionData(const char* value) { SetGameSessionData(value); return *this; }

        /**
        * <p>Data generated from GameLift Matchmaking.</p>
        */
        inline const std::string& GetMatchmakerData() const{ return m_matchmakerData; }

        /**
        * <p>Data generated from GameLift Matchmaking.</p>
        */
        inline void SetMatchmakerData(const std::string& value) { m_matchmakerData = value; }

        /**
        * <p>Data generated from GameLift Matchmaking.</p>
        */
        inline void SetMatchmakerData(std::string&& value) { m_matchmakerData = value; }

        /**
        * <p>Data generated from GameLift Matchmaking.</p>
        */
        inline void SetMatchmakerData(const char* value) { m_matchmakerData.assign(value); }

        /**
        * <p>Data generated from GameLift Matchmaking.</p>
        */
        inline GameSession& WithMatchmakerData(const std::string& value) { SetMatchmakerData(value); return *this; }

        /**
        * <p>Data generated from GameLift Matchmaking.</p>
        */
        inline GameSession& WithMatchmakerData(std::string&& value) { SetMatchmakerData(value); return *this; }

        /**
        * <p>Data generated from GameLift Matchmaking.</p>
        */
        inline GameSession& WithMatchmakerData(const char* value) { SetMatchmakerData(value); return *this; }
        
        /**
        * <p>The DNS name of the host running a GameLift server process, used for establishing a TLS connection for a game session.</p>
        */
        inline const std::string& GetDnsName() const{ return m_dnsName; }

        /**
        * <p>The DNS name of the host running a GameLift server process, used for establishing a TLS connection for a game session.</p>
        */
        inline void SetDnsName(const std::string& value) { m_dnsName = value; }

        /**
        * <p>The DNS name of the host running a GameLift server process, used for establishing a TLS connection for a game session.</p>
        */
        inline void SetDnsName(std::string&& value) { m_dnsName = value; }

        /**
        * <p>The DNS name of the host running a GameLift server process, used for establishing a TLS connection for a game session.</p>
        */
        inline void SetDnsName(const char* value) { m_dnsName.assign(value); }

        /**
        * <p>The DNS name of the host running a GameLift server process, used for establishing a TLS connection for a game session.</p>
        */
        inline GameSession& WithDnsName(const std::string& value) { SetDnsName(value); return *this; }

        /**
        * <p>The DNS name of the host running a GameLift server process, used for establishing a TLS connection for a game session.</p>
        */
        inline GameSession& WithDnsName(std::string&& value) { SetDnsName(value); return *this; }

        /**
        * <p>The DNS name of the host running a GameLift server process, used for establishing a TLS connection for a game session.</p>
        */
        inline GameSession& WithDnsName(const char* value) { SetDnsName(value); return *this; }

    private:
        std::string m_gameSessionId;
        std::string m_name;
        std::string m_fleetId;
        int m_maximumPlayerSessionCount;
        GameSessionStatus m_status;
        std::vector<GameProperty> m_gameProperties;
        std::string m_ipAddress;
        int m_port;
        std::string m_gameSessionData;
        std::string m_matchmakerData;
        std::string m_dnsName;
#else
    public:
        GameSession() :
            m_maximumPlayerSessionCount(0),
            m_gameProperties_count(0),
            m_port(0) 
        { 
            memset(m_gameSessionId, 0, MAX_SESSION_ID_LENGTH);
            memset(m_name, 0, MAX_SESSION_NAME_LENGTH);
            memset(m_fleetId, 0, MAX_FLEET_ID_LENGTH);
            memset(m_gameProperties, 0, MAX_GAME_PROPERTIES);
            memset(m_ipAddress, 0, MAX_IP_LENGTH);
            memset(m_gameSessionData, 0, MAX_GAME_SESSION_DATA_LENGTH);
            memset(m_matchmakerData, 0, MAX_MATCHMAKER_DATA_LENGTH);
            memset(m_dnsName, 0, MAX_DNS_NAME_LENGTH);
        }

        /**
        * <p>Destructor.</p>
        */
        ~GameSession() {}

        /**
        * <p>Copy Constructor.</p>
        */
        GameSession(const GameSession& other) :
            m_maximumPlayerSessionCount(other.m_maximumPlayerSessionCount),
			m_status(other.m_status),
            m_gameProperties_count(other.m_gameProperties_count),
            m_port(other.m_port)
        {
            strncpy(m_ipAddress, other.m_ipAddress, sizeof(other.m_ipAddress));
            strncpy(m_gameSessionId, other.m_gameSessionId, sizeof(other.m_gameSessionId));
            strncpy(m_name, other.m_name, sizeof(other.m_name));
            strncpy(m_fleetId, other.m_fleetId, sizeof(other.m_fleetId));
            strncpy(m_gameSessionData, other.m_gameSessionData, sizeof(other.m_gameSessionData));
            strncpy(m_matchmakerData, other.m_matchmakerData, sizeof(other.m_matchmakerData));
            strncpy(m_dnsName, other.m_dnsName, sizeof(other.m_dnsName));

            std::copy(std::begin(other.m_gameProperties), std::end(other.m_gameProperties), std::begin(m_gameProperties));
        }

        /**
        * <p>Move Constructor.</p>
        */
        GameSession(GameSession&& other) 
        {
            *this = std::move(other);
        }

        /**
        * <p>Copy assignment Constructor.</p>
        */
        GameSession& operator= (const GameSession& other) 
        {
            m_maximumPlayerSessionCount = other.m_maximumPlayerSessionCount;
            m_gameProperties_count = other.m_gameProperties_count;
            m_port = other.m_port;
            m_status = other.m_status;

            strncpy(m_ipAddress, other.m_ipAddress, sizeof(other.m_ipAddress));
            strncpy(m_gameSessionId, other.m_gameSessionId, sizeof(other.m_gameSessionId));
            strncpy(m_name, other.m_name, sizeof(other.m_name));
            strncpy(m_fleetId, other.m_fleetId, sizeof(other.m_fleetId));
            strncpy(m_gameSessionData, other.m_gameSessionData, sizeof(other.m_gameSessionData));
            strncpy(m_matchmakerData, other.m_matchmakerData, sizeof(other.m_matchmakerData));
            strncpy(m_dnsName, other.m_dnsName, sizeof(other.m_dnsName));

            std::copy(std::begin(other.m_gameProperties), std::end(other.m_gameProperties), std::begin(m_gameProperties));

            return *this;
        }

        /**
        * <p>Move assignment Constructor.</p>
        */
        GameSession& operator= (GameSession&& other) 
        {
            m_maximumPlayerSessionCount = other.m_maximumPlayerSessionCount;
            m_gameProperties_count = other.m_gameProperties_count;
            m_port = other.m_port;
            m_status = other.m_status;

            strncpy(m_gameSessionId, other.m_gameSessionId, sizeof(other.m_gameSessionId));
            strncpy(m_ipAddress, other.m_ipAddress, sizeof(other.m_ipAddress));
            strncpy(m_name, other.m_name, sizeof(other.m_name));
            strncpy(m_fleetId, other.m_fleetId, sizeof(other.m_fleetId));
            strncpy(m_gameSessionData, other.m_gameSessionData, sizeof(other.m_gameSessionData));
            strncpy(m_matchmakerData, other.m_matchmakerData, sizeof(other.m_matchmakerData));
            strncpy(m_dnsName, other.m_dnsName, sizeof(other.m_dnsName));

            std::copy(std::begin(other.m_gameProperties), std::end(other.m_gameProperties), std::begin(m_gameProperties));

            other.m_maximumPlayerSessionCount = 0;
            other.m_gameProperties_count = 0;
            other.m_port = 0;

            memset(other.m_gameSessionId, 0, MAX_SESSION_ID_LENGTH);
            memset(other.m_ipAddress, 0, MAX_IP_LENGTH);
            memset(other.m_name, 0, MAX_SESSION_NAME_LENGTH);
            memset(other.m_fleetId, 0, MAX_FLEET_ID_LENGTH);
            memset(other.m_gameProperties, 0, MAX_GAME_PROPERTIES);
            memset(other.m_gameSessionData, 0, MAX_GAME_SESSION_DATA_LENGTH);
            memset(other.m_matchmakerData, 0, MAX_MATCHMAKER_DATA_LENGTH);
            memset(other.m_dnsName, 0, MAX_DNS_NAME_LENGTH);

            return *this;
        }


        /**
        * <p>Unique identifier for a game session.</p>
        */
        inline const char* GetGameSessionId() const{ return m_gameSessionId; }

        /**
        * <p>Unique identifier for a game session.</p>
        */
        inline void SetGameSessionId(const char* value) { strncpy(m_gameSessionId, value, sizeof(m_gameSessionId)); m_gameSessionId[sizeof(m_gameSessionId)-1] = 0; }

        /**
        * <p>Unique identifier for a game session.</p>
        */
        inline GameSession& WithGameSessionId(const char* value) { SetGameSessionId(value); return *this; }

        /**
        * <p>Descriptive label associated with a game session. Session names do not need
        * to be unique.</p>
        */
        inline const char* GetName() const{ return m_name; }

        /**
        * <p>Descriptive label associated with a game session. Session names do not need
        * to be unique.</p>
        */
        inline void SetName(const char* value) { strncpy(m_name, value, sizeof(m_name)); m_name[sizeof(m_name)-1] = 0; }

        /**
        * <p>Descriptive label associated with a game session. Session names do not need
        * to be unique.</p>
        */
        inline GameSession& WithName(const char* value) { SetName(value); return *this; }

        /**
        * <p>Unique identifier for a fleet.</p>
        */
        inline const char* GetFleetId() const{ return m_fleetId; }

        /**
        * <p>Unique identifier for a fleet.</p>
        */
        inline void SetFleetId(const char* value) { strncpy(m_fleetId, value, sizeof(m_fleetId)); m_fleetId[sizeof(m_fleetId)-1] = 0; }

        /**
        * <p>Unique identifier for a fleet.</p>
        */
        inline GameSession& WithFleetId(const char* value) { SetFleetId(value); return *this; }

        /**
        * <p>Maximum number of players allowed in the game session.</p>
        */
        inline int GetMaximumPlayerSessionCount() const{ return m_maximumPlayerSessionCount; }

        /**
        * <p>Maximum number of players allowed in the game session.</p>
        */
        inline void SetMaximumPlayerSessionCount(int value) { m_maximumPlayerSessionCount = value; }

        /**
        * <p>Maximum number of players allowed in the game session.</p>
        */
        inline GameSession& WithMaximumPlayerSessionCount(int value) { SetMaximumPlayerSessionCount(value); return *this; }

        /**
        * <p>Current status of the game session. A game session must be in an
        * <code>ACTIVE</code> state to have player sessions.</p>
        */
        inline const GameSessionStatus& GetStatus() const{ return m_status; }

        /**
        * <p>Current status of the game session. A game session must be in an
        * <code>ACTIVE</code> state to have player sessions.</p>
        */
        inline void SetStatus(const GameSessionStatus& value) { m_status = value; }

        /**
        * <p>Current status of the game session. A game session must be in an
        * <code>ACTIVE</code> state to have player sessions.</p>
        */
        inline void SetStatus(GameSessionStatus&& value) { m_status = value; }

        /**
        * <p>Current status of the game session. A game session must be in an
        * <code>ACTIVE</code> state to have player sessions.</p>
        */
        inline GameSession& WithStatus(const GameSessionStatus& value) { SetStatus(value); return *this; }

        /**
        * <p>Current status of the game session. A game session must be in an
        * <code>ACTIVE</code> state to have player sessions.</p>
        */
        inline GameSession& WithStatus(GameSessionStatus&& value) { SetStatus(value); return *this; }

        /**
        * <p>Get the custom properties for the game session.</p>
        */
        inline const GameProperty* GetGameProperties(int& count) const{ count = m_gameProperties_count; return m_gameProperties; }

        /**
        * <p>Set of custom property for the game session.</p>
        */
        inline void AddGameProperty(GameProperty gameProperty) {
            if (m_gameProperties_count < MAX_GAME_PROPERTIES) {
                m_gameProperties[m_gameProperties_count++] = gameProperty;
            }
        };

        /**
        * <p>Set of custom properties for the game session.</p>
        */
        inline GameSession& WithGameProperty(GameProperty gameProperty) { AddGameProperty(gameProperty); return *this; }

        /**
        * <p>IP address of the game session. To connect to a GameLift server process, an
        * app needs both the IP address and port number.</p>
        */
        inline const char* GetIpAddress() const{ return m_ipAddress; }

        /**
        * <p>IP address of the game session. To connect to a GameLift server process, an
        * app needs both the IP address and port number.</p>
        */
        inline void SetIpAddress(const char* value) { strncpy(m_ipAddress, value, sizeof(m_ipAddress)); m_ipAddress[sizeof(m_ipAddress)-1] = 0; }

        /**
        * <p>IP address of the game session. To connect to a GameLift server process, an
        * app needs both the IP address and port number.</p>
        */
        inline GameSession& WithIpAddress(const char* value) { SetIpAddress(value); return *this; }

        /**
        * <p>Port number for the game session. To connect to a GameLift server process, an
        * app needs both the IP address and port number.</p>
        */
        inline int GetPort() const{ return m_port; }

        /**
        * <p>Port number for the game session. To connect to a GameLift server process, an
        * app needs both the IP address and port number.</p>
        */
        inline void SetPort(int value) { m_port = value; }

        /**
        * <p>Port number for the game session. To connect to a GameLift server process, an
        * app needs both the IP address and port number.</p>
        */
        inline GameSession& WithPort(int value) { SetPort(value); return *this; }

        /**
        * <p>Custom data for the game session.</p>
        */
        inline const char* GetGameSessionData() const{ return m_gameSessionData; }

        /**
        * <p>Custom data for the game session.</p>
        */
        inline void SetGameSessionData(const char* value) { strncpy(m_gameSessionData, value, sizeof(m_gameSessionData)); m_gameSessionData[sizeof(m_gameSessionData)-1] = 0; }

        /**
        * <p>Custom data for the game session.</p>
        */
        inline GameSession& WithGameSessionData(const char* value) { SetGameSessionData(value); return *this; }

        /**
        * <p>Data generated from GameLift Matchmaking.</p>
        */
        inline const char* GetMatchmakerData() const{ return m_matchmakerData; }

        /**
        * <p>Data generated from GameLift Matchmaking.</p>
        */
        inline void SetMatchmakerData(const char* value) { strncpy(m_matchmakerData, value, sizeof(m_matchmakerData)); m_matchmakerData[sizeof(m_matchmakerData)-1] = 0; }

        /**
        * <p>Data generated from GameLift Matchmaking.</p>
        */
        inline GameSession& WithMatchmakerData(const char* value) { SetMatchmakerData(value); return *this; }

        /**
        * The DNS name of the host running a GameLift server process, used for establishing a TLS connection for a game session.
        */
        inline const char* GetDnsName() const{ return m_dnsName; }

        /**
        * The DNS name of the host running a GameLift server process, used for establishing a TLS connection for a game session.
        */
        inline void SetDnsName(const char* value) { strncpy(m_dnsName, value, sizeof(m_dnsName)); m_dnsName[sizeof(m_dnsName)-1] = 0; }

        /**
        * The DNS name of the host running a GameLift server process, used for establishing a TLS connection for a game session.
        */
        inline GameSession& WithDnsName(const char* value) { SetDnsName(value); return *this; }

    private:
        char m_gameSessionId[MAX_SESSION_ID_LENGTH];
        char m_name[MAX_SESSION_NAME_LENGTH];
        char m_fleetId[MAX_FLEET_ID_LENGTH];
        int m_maximumPlayerSessionCount;
        GameSessionStatus m_status;
        GameProperty m_gameProperties[MAX_GAME_PROPERTIES];
        int m_gameProperties_count;
        char m_ipAddress[MAX_IP_LENGTH];
        int m_port;
        char m_gameSessionData[MAX_GAME_SESSION_DATA_LENGTH];
        char m_matchmakerData[MAX_MATCHMAKER_DATA_LENGTH];
        char m_dnsName[MAX_DNS_NAME_LENGTH];
#endif
    };

} // namespace Model
} // namespace Server
} // namespace GameLift
} // namespace Aws
