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
#include <aws/gamelift/server/model/PlayerSessionStatus.h>

#ifndef GAMELIFT_USE_STD
    #ifndef MAX_IP_ADDRESS_LENGTH
        #define MAX_IP_ADDRESS_LENGTH 45
    #endif
    #ifndef MAX_GAME_SESSION_ID_LENGTH
        #define MAX_GAME_SESSION_ID_LENGTH 256
    #endif
    #ifndef MAX_PLAYER_SESSION_ID_LENGTH
        #define MAX_PLAYER_SESSION_ID_LENGTH 256
    #endif
    #ifndef MAX_PLAYER_ID_LENGTH
        #define MAX_PLAYER_ID_LENGTH 1024
    #endif
    #ifndef MAX_FLEET_ID_LENGTH
        #define MAX_FLEET_ID_LENGTH 1024
    #endif
    #ifndef MAX_PLAYER_DATA_LENGTH
        #define MAX_PLAYER_DATA_LENGTH 2049
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
   * <p>Properties describing a player session.</p>
   */
  class AWS_GAMELIFT_API PlayerSession
  {
#ifdef GAMELIFT_USE_STD
  public:
    PlayerSession() :
      m_creationTime(0),
      m_terminationTime(0),
      m_port(0)
    { }

    /**
    * <p>Destructor.</p>
    */
    ~PlayerSession() {}

    /**
    * <p>Copy Constructor.</p>
    */
    PlayerSession(const PlayerSession& other) :
        m_playerSessionId(other.m_playerSessionId),
        m_playerId(other.m_playerId),
        m_gameSessionId(other.m_gameSessionId),
        m_creationTime(other.m_creationTime),
        m_terminationTime(other.m_terminationTime),
        m_ipAddress(other.m_ipAddress),
        m_port(other.m_port),
        m_playerData(other.m_playerData),
		m_dnsName(other.m_dnsName)
    { }

    /**
    * <p>Move Constructor.</p>
    */
    PlayerSession(PlayerSession&& other) 
    { 
		*this = std::move(other);
	}


    /**
    * <p>Copy assignment Constructor.</p>
    */
    PlayerSession& operator= (const PlayerSession& other) 
    {
        m_playerSessionId = other.m_playerSessionId;
        m_playerId = other.m_playerId;
        m_gameSessionId = other.m_gameSessionId;
        m_creationTime = other.m_creationTime;
        m_terminationTime = other.m_terminationTime;
        m_ipAddress = other.m_ipAddress;
        m_port = other.m_port;
        m_playerData = other.m_playerData;
		m_dnsName = other.m_dnsName;

        return *this;
    }

    /**
    * <p>Move assignment Constructor.</p>
    */
    PlayerSession& operator= (PlayerSession&& other) 
    {
        m_playerSessionId = std::move(other.m_playerSessionId);
        m_playerId = std::move(other.m_playerId);
        m_gameSessionId = std::move(other.m_gameSessionId);
        m_creationTime = std::move(other.m_creationTime);
        m_terminationTime = std::move(other.m_terminationTime);
        m_ipAddress = std::move(other.m_ipAddress);
        m_port = std::move(other.m_port);
        m_playerData = std::move(other.m_playerData);
		m_dnsName = std::move(other.m_dnsName);

        return *this;
    }

    /**
     * <p>Unique identifier for a player session.</p>
     */
    inline const std::string& GetPlayerSessionId() const{ return m_playerSessionId; }

    /**
     * <p>Unique identifier for a player session.</p>
     */
    inline void SetPlayerSessionId(const std::string& value) { m_playerSessionId = value; }

    /**
     * <p>Unique identifier for a player session.</p>
     */
    inline void SetPlayerSessionId(std::string&& value) { m_playerSessionId = value; }

    /**
     * <p>Unique identifier for a player session.</p>
     */
    inline void SetPlayerSessionId(const char* value) { m_playerSessionId.assign(value); }

    /**
     * <p>Unique identifier for a player session.</p>
     */
    inline PlayerSession& WithPlayerSessionId(const std::string& value) { SetPlayerSessionId(value); return *this;}

    /**
     * <p>Unique identifier for a player session.</p>
     */
    inline PlayerSession& WithPlayerSessionId(std::string&& value) { SetPlayerSessionId(value); return *this;}

    /**
     * <p>Unique identifier for a player session.</p>
     */
    inline PlayerSession& WithPlayerSessionId(const char* value) { SetPlayerSessionId(value); return *this;}

    /**
     * <p>Unique identifier for a player.</p>
     */
    inline const std::string& GetPlayerId() const{ return m_playerId; }

    /**
     * <p>Unique identifier for a player.</p>
     */
    inline void SetPlayerId(const std::string& value) { m_playerId = value; }

    /**
     * <p>Unique identifier for a player.</p>
     */
    inline void SetPlayerId(std::string&& value) { m_playerId = value; }

    /**
     * <p>Unique identifier for a player.</p>
     */
    inline void SetPlayerId(const char* value) { m_playerId.assign(value); }

    /**
     * <p>Unique identifier for a player.</p>
     */
    inline PlayerSession& WithPlayerId(const std::string& value) { SetPlayerId(value); return *this;}

    /**
     * <p>Unique identifier for a player.</p>
     */
    inline PlayerSession& WithPlayerId(std::string&& value) { SetPlayerId(value); return *this;}

    /**
     * <p>Unique identifier for a player.</p>
     */
    inline PlayerSession& WithPlayerId(const char* value) { SetPlayerId(value); return *this;}

    /**
     * <p>Unique identifier for the game session that the player session is connected
     * to.</p>
     */
    inline const std::string& GetGameSessionId() const{ return m_gameSessionId; }

    /**
     * <p>Unique identifier for the game session that the player session is connected
     * to.</p>
     */
    inline void SetGameSessionId(const std::string& value) { m_gameSessionId = value; }

    /**
     * <p>Unique identifier for the game session that the player session is connected
     * to.</p>
     */
    inline void SetGameSessionId(std::string&& value) { m_gameSessionId = value; }

    /**
     * <p>Unique identifier for the game session that the player session is connected
     * to.</p>
     */
    inline void SetGameSessionId(const char* value) { m_gameSessionId.assign(value); }

    /**
     * <p>Unique identifier for the game session that the player session is connected
     * to.</p>
     */
    inline PlayerSession& WithGameSessionId(const std::string& value) { SetGameSessionId(value); return *this;}

    /**
     * <p>Unique identifier for the game session that the player session is connected
     * to.</p>
     */
    inline PlayerSession& WithGameSessionId(std::string&& value) { SetGameSessionId(value); return *this;}

    /**
     * <p>Unique identifier for the game session that the player session is connected
     * to.</p>
     */
    inline PlayerSession& WithGameSessionId(const char* value) { SetGameSessionId(value); return *this;}

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
    inline PlayerSession& WithFleetId(const std::string& value) { SetFleetId(value); return *this;}

    /**
     * <p>Unique identifier for a fleet.</p>
     */
    inline PlayerSession& WithFleetId(std::string&& value) { SetFleetId(value); return *this;}

    /**
     * <p>Unique identifier for a fleet.</p>
     */
    inline PlayerSession& WithFleetId(const char* value) { SetFleetId(value); return *this;}

    /**
     * <p>Time stamp indicating when this data object was created. Format is a number
     * expressed in Unix time as milliseconds (ex: "1469498468.057".</p>
     */
    inline long GetCreationTime() const{ return m_creationTime; }

    /**
     * <p>Time stamp indicating when this data object was created. Format is a number
     * expressed in Unix time as milliseconds (ex: "1469498468.057".</p>
     */
    inline void SetCreationTime(long value) { m_creationTime = value; }

    /**
     * <p>Time stamp indicating when this data object was created. Format is a number
     * expressed in Unix time as milliseconds (ex: "1469498468.057".</p>
     */
    inline PlayerSession& WithCreationTime(long value) { SetCreationTime(value); return *this;}

    /**
     * <p>Time stamp indicating when this data object was terminated. Format is a
     * number expressed in Unix time as milliseconds (ex: "1469498468.057".</p>
     */
    inline long GetTerminationTime() const{ return m_terminationTime; }

    /**
     * <p>Time stamp indicating when this data object was terminated. Format is a
     * number expressed in Unix time as milliseconds (ex: "1469498468.057".</p>
     */
    inline void SetTerminationTime(long value) { m_terminationTime = value; }

    /**
     * <p>Time stamp indicating when this data object was terminated. Format is a
     * number expressed in Unix time as milliseconds (ex: "1469498468.057".</p>
     */
    inline PlayerSession& WithTerminationTime(long value) { SetTerminationTime(value); return *this;}

    /**
     * <p>Current status of the player session.</p> <p>Possible player session statuses
     * include the following:</p> <ul> <li> <p> <b>RESERVED</b> – The player session
     * request has been received, but the player has not yet connected to the server
     * process and/or been validated. </p> </li> <li> <p> <b>ACTIVE</b> – The player
     * has been validated by the server process and is currently connected.</p> </li>
     * <li> <p> <b>COMPLETED</b> – The player connection has been dropped.</p> </li>
     * <li> <p> <b>TIMEDOUT</b> – A player session request was received, but the player
     * did not connect and/or was not validated within the time-out limit (60
     * seconds).</p> </li> </ul>
     */
    inline const PlayerSessionStatus& GetStatus() const{ return m_status; }

    /**
     * <p>Current status of the player session.</p> <p>Possible player session statuses
     * include the following:</p> <ul> <li> <p> <b>RESERVED</b> – The player session
     * request has been received, but the player has not yet connected to the server
     * process and/or been validated. </p> </li> <li> <p> <b>ACTIVE</b> – The player
     * has been validated by the server process and is currently connected.</p> </li>
     * <li> <p> <b>COMPLETED</b> – The player connection has been dropped.</p> </li>
     * <li> <p> <b>TIMEDOUT</b> – A player session request was received, but the player
     * did not connect and/or was not validated within the time-out limit (60
     * seconds).</p> </li> </ul>
     */
    inline void SetStatus(const PlayerSessionStatus& value) { m_status = value; }

    /**
     * <p>Current status of the player session.</p> <p>Possible player session statuses
     * include the following:</p> <ul> <li> <p> <b>RESERVED</b> – The player session
     * request has been received, but the player has not yet connected to the server
     * process and/or been validated. </p> </li> <li> <p> <b>ACTIVE</b> – The player
     * has been validated by the server process and is currently connected.</p> </li>
     * <li> <p> <b>COMPLETED</b> – The player connection has been dropped.</p> </li>
     * <li> <p> <b>TIMEDOUT</b> – A player session request was received, but the player
     * did not connect and/or was not validated within the time-out limit (60
     * seconds).</p> </li> </ul>
     */
    inline void SetStatus(PlayerSessionStatus&& value) { m_status = value; }

    /**
     * <p>Current status of the player session.</p> <p>Possible player session statuses
     * include the following:</p> <ul> <li> <p> <b>RESERVED</b> – The player session
     * request has been received, but the player has not yet connected to the server
     * process and/or been validated. </p> </li> <li> <p> <b>ACTIVE</b> – The player
     * has been validated by the server process and is currently connected.</p> </li>
     * <li> <p> <b>COMPLETED</b> – The player connection has been dropped.</p> </li>
     * <li> <p> <b>TIMEDOUT</b> – A player session request was received, but the player
     * did not connect and/or was not validated within the time-out limit (60
     * seconds).</p> </li> </ul>
     */
    inline PlayerSession& WithStatus(const PlayerSessionStatus& value) { SetStatus(value); return *this;}

    /**
     * <p>Current status of the player session.</p> <p>Possible player session statuses
     * include the following:</p> <ul> <li> <p> <b>RESERVED</b> – The player session
     * request has been received, but the player has not yet connected to the server
     * process and/or been validated. </p> </li> <li> <p> <b>ACTIVE</b> – The player
     * has been validated by the server process and is currently connected.</p> </li>
     * <li> <p> <b>COMPLETED</b> – The player connection has been dropped.</p> </li>
     * <li> <p> <b>TIMEDOUT</b> – A player session request was received, but the player
     * did not connect and/or was not validated within the time-out limit (60
     * seconds).</p> </li> </ul>
     */
    inline PlayerSession& WithStatus(PlayerSessionStatus&& value) { SetStatus(value); return *this;}

    /**
     * <p>Game session IP address. All player sessions reference the game session
     * location.</p>
     */
    inline const std::string& GetIpAddress() const{ return m_ipAddress; }

    /**
     * <p>Game session IP address. All player sessions reference the game session
     * location.</p>
     */
    inline void SetIpAddress(const std::string& value) { m_ipAddress = value; }

    /**
     * <p>Game session IP address. All player sessions reference the game session
     * location.</p>
     */
    inline void SetIpAddress(std::string&& value) { m_ipAddress = value; }

    /**
     * <p>Game session IP address. All player sessions reference the game session
     * location.</p>
     */
    inline void SetIpAddress(const char* value) { m_ipAddress.assign(value); }

    /**
     * <p>Game session IP address. All player sessions reference the game session
     * location.</p>
     */
    inline PlayerSession& WithIpAddress(const std::string& value) { SetIpAddress(value); return *this;}

    /**
     * <p>Game session IP address. All player sessions reference the game session
     * location.</p>
     */
    inline PlayerSession& WithIpAddress(std::string&& value) { SetIpAddress(value); return *this;}

    /**
     * <p>Game session IP address. All player sessions reference the game session
     * location.</p>
     */
    inline PlayerSession& WithIpAddress(const char* value) { SetIpAddress(value); return *this;}

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
    inline PlayerSession& WithPort(int value) { SetPort(value); return *this;}

    /**
     * <p>Custom player data.</p>
     */
    inline const std::string& GetPlayerData() const{ return m_playerData; }

    /**
     * <p>Custom player data.</p>
     */
    inline void SetPlayerData(const std::string& value) { m_playerData = value; }

    /**
     * <p>Custom player data.</p>
     */
    inline void SetPlayerData(std::string&& value) { m_playerData = value; }

    /**
     * <p>Custom player data.</p>
     */
    inline void SetPlayerData(const char* value) { m_playerData.assign(value); }

    /**
     * <p>Custom player data.</p>
     */
    inline PlayerSession& WithPlayerData(const std::string& value) { SetPlayerData(value); return *this;}

    /**
     * <p>Custom player data.</p>
     */
    inline PlayerSession& WithPlayerData(std::string&& value) { SetPlayerData(value); return *this;}

    /**
     * <p>Custom player data.</p>
     */
    inline PlayerSession& WithPlayerData(const char* value) { SetPlayerData(value); return *this;}

	/**
	* <p>Game session DNS name. All player sessions reference the game session
	* location.</p>
	*/
	inline const std::string& GetDnsName() const { return m_dnsName; }

	/**
	* <p>Game session DNS name. All player sessions reference the game session
	* location.</p>
	*/
	inline void SetDnsName(const std::string& value) { m_dnsName = value; }

	/**
	* <p>Game session DNS name. All player sessions reference the game session
	* location.</p>
	*/
	inline void SetDnsName(std::string&& value) { m_dnsName = value; }

	/**
	* <p>Game session DNS name. All player sessions reference the game session
	* location.</p>
	*/
	inline void SetDnsName(const char* value) { m_dnsName.assign(value); }

	/**
	* <p>Game session DNS name. All player sessions reference the game session
	* location.</p>
	*/
	inline PlayerSession& WithDnsName(const std::string& value) { SetDnsName(value); return *this; }

	/**
	* <p>Game session DNS name. All player sessions reference the game session
	* location.</p>
	*/
	inline PlayerSession& WithDnsName(std::string&& value) { SetDnsName(value); return *this; }

	/**
	* <p>Game session DNS name. All player sessions reference the game session
	* location.</p>
	*/
	inline PlayerSession& WithDnsName(const char* value) { SetDnsName(value); return *this; }


  private:
    std::string m_playerSessionId;
    std::string m_playerId;
    std::string m_gameSessionId;
    std::string m_fleetId;
    long m_creationTime;
    long m_terminationTime;
    PlayerSessionStatus m_status;
    std::string m_ipAddress;
    int m_port;
    std::string m_playerData;
	std::string m_dnsName;
#else
  public:
    PlayerSession() :
      m_creationTime(0),
      m_terminationTime(0),
      m_port(0)
    {
      memset(m_playerSessionId, 0, sizeof(m_playerSessionId));
      memset(m_playerId, 0, sizeof(m_playerId));
      memset(m_gameSessionId, 0, sizeof(m_gameSessionId));
      memset(m_fleetId, 0, sizeof(m_fleetId));
      memset(m_ipAddress, 0, sizeof(m_ipAddress));
      memset(m_playerData, 0, sizeof(m_playerData));
	  memset(m_dnsName, 0, sizeof(m_dnsName));
     }

    /**
    * <p>Destructor.</p>
    */
    ~PlayerSession() {}

    /**
    * <p>Copy Constructor.</p>
    */
    PlayerSession(const PlayerSession& other) :
        m_creationTime(other.m_creationTime),
        m_terminationTime(other.m_terminationTime),
        m_status(other.m_status),
        m_port(other.m_port)
    {
        strncpy(m_playerSessionId, other.m_playerSessionId, sizeof(other.m_playerSessionId));
        strncpy(m_playerId, other.m_playerId, sizeof(other.m_playerId));
        strncpy(m_gameSessionId, other.m_gameSessionId, sizeof(other.m_gameSessionId));
        strncpy(m_fleetId, other.m_fleetId, sizeof(other.m_fleetId));
        strncpy(m_ipAddress, other.m_ipAddress, sizeof(other.m_ipAddress));
        strncpy(m_playerData, other.m_playerData, sizeof(other.m_playerData));
		strncpy(m_dnsName, other.m_dnsName, sizeof(other.m_dnsName));
    }

    /**
    * <p>Move Constructor.</p>
    */
    PlayerSession(PlayerSession&& other) 
    {
		*this = std::move(other);
    }

    /**
    * <p>Copy assignment Constructor.</p>
    */
    PlayerSession& operator= (const PlayerSession& other) 
    {
        m_creationTime = other.m_creationTime;
        m_terminationTime = other.m_terminationTime;
        m_status = other.m_status;
        m_port = other.m_port;

        strncpy(m_playerSessionId, other.m_playerSessionId, sizeof(other.m_playerSessionId));
        strncpy(m_playerId, other.m_playerId, sizeof(other.m_playerId));
        strncpy(m_gameSessionId, other.m_gameSessionId, sizeof(other.m_gameSessionId));
        strncpy(m_fleetId, other.m_fleetId, sizeof(other.m_fleetId));
        strncpy(m_ipAddress, other.m_ipAddress, sizeof(other.m_ipAddress));
        strncpy(m_playerData, other.m_playerData, sizeof(other.m_playerData));
		strncpy(m_dnsName, other.m_dnsName, sizeof(other.m_dnsName));

        return *this;
    }

    /**
    * <p>Move assignment Constructor.</p>
    */
    PlayerSession& operator= (PlayerSession&& other) 
    {
        m_creationTime = other.m_creationTime;
        m_terminationTime = other.m_terminationTime;
        m_status = other.m_status;
        m_port = other.m_port;

        strncpy(m_playerSessionId, other.m_playerSessionId, sizeof(other.m_playerSessionId));
        strncpy(m_playerId, other.m_playerId, sizeof(other.m_playerId));
        strncpy(m_gameSessionId, other.m_gameSessionId, sizeof(other.m_gameSessionId));
        strncpy(m_fleetId, other.m_fleetId, sizeof(other.m_fleetId));
        strncpy(m_ipAddress, other.m_ipAddress, sizeof(other.m_ipAddress));
        strncpy(m_playerData, other.m_playerData, sizeof(other.m_playerData));
		strncpy(m_dnsName, other.m_dnsName, sizeof(other.m_dnsName));

		other.m_creationTime = 0;
		other.m_terminationTime = 0;
		other.m_port = 0;

		memset(m_playerSessionId, 0, sizeof(m_playerSessionId));
		memset(m_playerId, 0, sizeof(m_playerId));
		memset(m_gameSessionId, 0, sizeof(m_gameSessionId));
		memset(m_fleetId, 0, sizeof(m_fleetId));
		memset(m_ipAddress, 0, sizeof(m_ipAddress));
		memset(m_playerData, 0, sizeof(m_playerData));
		memset(m_dnsName, 0, sizeof(m_dnsName));

        return *this;
    }

    /**
     * <p>Unique identifier for a player session.</p>
     */
    inline const char* GetPlayerSessionId() const{ return m_playerSessionId; }

    /**
     * <p>Unique identifier for a player session.</p>
     */
    inline void SetPlayerSessionId(const char* value) { strncpy(m_playerSessionId, value, sizeof(m_playerSessionId)); m_playerSessionId[sizeof(m_playerSessionId)-1] = 0; }

    /**
     * <p>Unique identifier for a player session.</p>
     */
    inline PlayerSession& WithPlayerSessionId(const char* value) { SetPlayerSessionId(value); return *this;}

    /**
     * <p>Unique identifier for a player.</p>
     */
    inline const char* GetPlayerId() const{ return m_playerId; }

    /**
     * <p>Unique identifier for a player.</p>
     */
    inline void SetPlayerId(const char* value) { strncpy(m_playerId, value, sizeof(m_playerId)); m_playerId[sizeof(m_playerId)-1] = 0; }

    /**
     * <p>Unique identifier for a player.</p>
     */
    inline PlayerSession& WithPlayerId(const char* value) { SetPlayerId(value); return *this;}

    /**
     * <p>Unique identifier for the game session that the player session is connected
     * to.</p>
     */
    inline const char* GetGameSessionId() const{ return m_gameSessionId; }

    /**
     * <p>Unique identifier for the game session that the player session is connected
     * to.</p>
     */
    inline void SetGameSessionId(const char* value) { strncpy(m_gameSessionId, value, sizeof(m_gameSessionId)); m_gameSessionId[sizeof(m_gameSessionId)-1] = 0; }

    /**
     * <p>Unique identifier for the game session that the player session is connected
     * to.</p>
     */
    inline PlayerSession& WithGameSessionId(const char* value) { SetGameSessionId(value); return *this;}

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
    inline PlayerSession& WithFleetId(const char* value) { SetFleetId(value); return *this;}

    /**
     * <p>Time stamp indicating when this data object was created. Format is a number
     * expressed in Unix time as milliseconds (ex: "1469498468.057".</p>
     */
    inline long GetCreationTime() const{ return m_creationTime; }

    /**
     * <p>Time stamp indicating when this data object was created. Format is a number
     * expressed in Unix time as milliseconds (ex: "1469498468.057".</p>
     */
    inline void SetCreationTime(long value) { m_creationTime = value; }

    /**
     * <p>Time stamp indicating when this data object was created. Format is a number
     * expressed in Unix time as milliseconds (ex: "1469498468.057".</p>
     */
    inline PlayerSession& WithCreationTime(long value) { SetCreationTime(value); return *this;}

    /**
     * <p>Time stamp indicating when this data object was terminated. Format is a
     * number expressed in Unix time as milliseconds (ex: "1469498468.057".</p>
     */
    inline long GetTerminationTime() const{ return m_terminationTime; }

    /**
     * <p>Time stamp indicating when this data object was terminated. Format is a
     * number expressed in Unix time as milliseconds (ex: "1469498468.057".</p>
     */
    inline void SetTerminationTime(long value) { m_terminationTime = value; }

    /**
     * <p>Time stamp indicating when this data object was terminated. Format is a
     * number expressed in Unix time as milliseconds (ex: "1469498468.057".</p>
     */
    inline PlayerSession& WithTerminationTime(long value) { SetTerminationTime(value); return *this;}

    /**
     * <p>Current status of the player session.</p> <p>Possible player session statuses
     * include the following:</p> <ul> <li> <p> <b>RESERVED</b> – The player session
     * request has been received, but the player has not yet connected to the server
     * process and/or been validated. </p> </li> <li> <p> <b>ACTIVE</b> – The player
     * has been validated by the server process and is currently connected.</p> </li>
     * <li> <p> <b>COMPLETED</b> – The player connection has been dropped.</p> </li>
     * <li> <p> <b>TIMEDOUT</b> – A player session request was received, but the player
     * did not connect and/or was not validated within the time-out limit (60
     * seconds).</p> </li> </ul>
     */
    inline const PlayerSessionStatus& GetStatus() const{ return m_status; }

    /**
     * <p>Current status of the player session.</p> <p>Possible player session statuses
     * include the following:</p> <ul> <li> <p> <b>RESERVED</b> – The player session
     * request has been received, but the player has not yet connected to the server
     * process and/or been validated. </p> </li> <li> <p> <b>ACTIVE</b> – The player
     * has been validated by the server process and is currently connected.</p> </li>
     * <li> <p> <b>COMPLETED</b> – The player connection has been dropped.</p> </li>
     * <li> <p> <b>TIMEDOUT</b> – A player session request was received, but the player
     * did not connect and/or was not validated within the time-out limit (60
     * seconds).</p> </li> </ul>
     */
    inline void SetStatus(const PlayerSessionStatus& value) { m_status = value; }

    /**
     * <p>Current status of the player session.</p> <p>Possible player session statuses
     * include the following:</p> <ul> <li> <p> <b>RESERVED</b> – The player session
     * request has been received, but the player has not yet connected to the server
     * process and/or been validated. </p> </li> <li> <p> <b>ACTIVE</b> – The player
     * has been validated by the server process and is currently connected.</p> </li>
     * <li> <p> <b>COMPLETED</b> – The player connection has been dropped.</p> </li>
     * <li> <p> <b>TIMEDOUT</b> – A player session request was received, but the player
     * did not connect and/or was not validated within the time-out limit (60
     * seconds).</p> </li> </ul>
     */
    inline void SetStatus(PlayerSessionStatus&& value) { m_status = value; }

    /**
     * <p>Current status of the player session.</p> <p>Possible player session statuses
     * include the following:</p> <ul> <li> <p> <b>RESERVED</b> – The player session
     * request has been received, but the player has not yet connected to the server
     * process and/or been validated. </p> </li> <li> <p> <b>ACTIVE</b> – The player
     * has been validated by the server process and is currently connected.</p> </li>
     * <li> <p> <b>COMPLETED</b> – The player connection has been dropped.</p> </li>
     * <li> <p> <b>TIMEDOUT</b> – A player session request was received, but the player
     * did not connect and/or was not validated within the time-out limit (60
     * seconds).</p> </li> </ul>
     */
    inline PlayerSession& WithStatus(const PlayerSessionStatus& value) { SetStatus(value); return *this;}

    /**
     * <p>Current status of the player session.</p> <p>Possible player session statuses
     * include the following:</p> <ul> <li> <p> <b>RESERVED</b> – The player session
     * request has been received, but the player has not yet connected to the server
     * process and/or been validated. </p> </li> <li> <p> <b>ACTIVE</b> – The player
     * has been validated by the server process and is currently connected.</p> </li>
     * <li> <p> <b>COMPLETED</b> – The player connection has been dropped.</p> </li>
     * <li> <p> <b>TIMEDOUT</b> – A player session request was received, but the player
     * did not connect and/or was not validated within the time-out limit (60
     * seconds).</p> </li> </ul>
     */
    inline PlayerSession& WithStatus(PlayerSessionStatus&& value) { SetStatus(value); return *this;}

    /**
     * <p>Game session IP address. All player sessions reference the game session
     * location.</p>
     */
    inline const char* GetIpAddress() const{ return m_ipAddress; }

    /**
     * <p>Game session IP address. All player sessions reference the game session
     * location.</p>
     */
    inline void SetIpAddress(const char* value) { strncpy(m_ipAddress, value, sizeof(m_ipAddress)); m_ipAddress[sizeof(m_ipAddress)-1] = 0; }

    /**
     * <p>Game session IP address. All player sessions reference the game session
     * location.</p>
     */
    inline PlayerSession& WithIpAddress(const char* value) { SetIpAddress(value); return *this;}

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
    inline PlayerSession& WithPort(int value) { SetPort(value); return *this;}

    /**
     * <p>Custom player data.</p>
     */
    inline const char* GetPlayerData() const{ return m_playerData; }

    /**
     * <p>Custom player data.</p>
     */
    inline void SetPlayerData(const char* value) { strncpy(m_playerData, value, sizeof(m_playerData)); m_playerData[sizeof(m_playerData)-1] = 0; }

    /**
     * <p>Custom player data.</p>
     */
    inline PlayerSession& WithPlayerData(const char* value) { SetPlayerData(value); return *this;}

	/**
	* <p>Game session DNS name. All player sessions reference the game session
	* location.</p>
	*/
	inline const char* GetDnsName() const { return m_dnsName; }

	/**
	* <p>Game session DNS name. All player sessions reference the game session
	* location.</p>
	*/
	inline void SetDnsName(const char* value) { strncpy(m_dnsName, value, sizeof(m_dnsName)); m_dnsName[sizeof(m_dnsName) - 1] = 0; }

	/**
	* <p>Game session DNS name. All player sessions reference the game session
	* location.</p>
	*/
	inline PlayerSession& WithDnsName(const char* value) { SetDnsName(value); return *this; }


  private:
    char m_playerSessionId[MAX_PLAYER_SESSION_ID_LENGTH];
    char m_playerId[MAX_PLAYER_ID_LENGTH];
    char m_gameSessionId[MAX_GAME_SESSION_ID_LENGTH];
    char m_fleetId[MAX_FLEET_ID_LENGTH];
    long m_creationTime;
    long m_terminationTime;
    PlayerSessionStatus m_status;
    char m_ipAddress[MAX_IP_ADDRESS_LENGTH];
    int m_port;
    char m_playerData[MAX_PLAYER_DATA_LENGTH];
	char m_dnsName[MAX_DNS_NAME_LENGTH];
#endif
  };

} // namespace Model
} // namespace Server
} // namespace GameLift
} // namespace Aws
