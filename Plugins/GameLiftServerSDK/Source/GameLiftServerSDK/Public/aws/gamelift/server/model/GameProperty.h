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

#ifndef GAMELIFT_USE_STD
    #ifndef MAX_KEY_LENGTH
        #define MAX_KEY_LENGTH 256
    #endif
    #ifndef MAX_VALUE_LENGTH
        #define MAX_VALUE_LENGTH 256
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
    * <p>Set of key-value pairs containing information your game server requires to
    * set up sessions. This object allows you to pass in any set of data needed for
    * your game. For more information, see the <a
    * href="http://docs.aws.amazon.com/gamelift/latest/developerguide/">Amazon
    * GameLift Developer Guide</a>.</p>
    */
class AWS_GAMELIFT_API GameProperty
{
#ifdef GAMELIFT_USE_STD
public:
    GameProperty(){}

    /**
    * <p>Destructor.</p>
    */
    ~GameProperty() {}

    /**
    * <p>Copy Constructor.</p>
    */
    GameProperty(const GameProperty& other) :
        m_key(other.m_key),
        m_value(other.m_value)
    { }

    /**
    * <p>Move Constructor.</p>
    */
    GameProperty(GameProperty&& other) 
    { 
		*this = std::move(other);
	}


    /**
    * <p>Copy assignment Constructor.</p>
    */
    GameProperty& operator= (const GameProperty& other) 
    {
        m_key = other.m_key;
        m_value = other.m_value;

        return *this;
    }

    /**
    * <p>Move assignment Constructor.</p>
    */
    GameProperty& operator= (GameProperty&& other) 
    {
        m_key = std::move(other.m_key);
        m_value = std::move(other.m_value);

        return *this;
    }

    inline const std::string& GetKey() const{ return m_key; }


    inline void SetKey(const std::string& value) { m_key = value; }


    inline void SetKey(std::string&& value) { m_key = value; }


    inline void SetKey(const char* value) { m_key.assign(value); }


    inline GameProperty& WithKey(const std::string& value) { SetKey(value); return *this; }


    inline GameProperty& WithKey(std::string&& value) { SetKey(value); return *this; }


    inline GameProperty& WithKey(const char* value) { SetKey(value); return *this; }


    inline const std::string& GetValue() const{ return m_value; }


    inline void SetValue(const std::string& value) { m_value = value; }


    inline void SetValue(std::string&& value) { m_value = value; }


    inline void SetValue(const char* value) { m_value.assign(value); }


    inline GameProperty& WithValue(const std::string& value) { SetValue(value); return *this; }


    inline GameProperty& WithValue(std::string&& value) { SetValue(value); return *this; }


    inline GameProperty& WithValue(const char* value) { SetValue(value); return *this; }

private:
    std::string m_key;
    std::string m_value;
#else
public:
    GameProperty()
    {
        memset(m_key, 0, MAX_KEY_LENGTH);
        memset(m_value, 0, MAX_VALUE_LENGTH);
    }

    /**
    * <p>Destructor.</p>
    */
    ~GameProperty() {}

    /**
    * <p>Copy Constructor.</p>
    */
    GameProperty(const GameProperty& other)
    {
        strncpy(m_key, other.m_key, sizeof(other.m_key));
        strncpy(m_value, other.m_value, sizeof(other.m_value));
    }

    /**
    * <p>Move Constructor.</p>
    */
    GameProperty(GameProperty&& other) 
    {
		*this = std::move(other);
    }

    /**
    * <p>Copy assignment Constructor.</p>
    */
    GameProperty& operator= (const GameProperty& other) 
    {
        strncpy(m_key, other.m_key, sizeof(other.m_key));
        strncpy(m_value, other.m_value, sizeof(other.m_value));

        return *this;
    }

    /**
    * <p>Move assignment Constructor.</p>
    */
    GameProperty& operator= (GameProperty&& other) 
    {
        strncpy(m_key, other.m_key, sizeof(other.m_key));
        strncpy(m_value, other.m_value, sizeof(other.m_value));

		memset(other.m_key, 0, MAX_KEY_LENGTH);
		memset(other.m_value, 0, MAX_VALUE_LENGTH);

        return *this;
    }

    inline const char* GetKey() const{ return m_key; }


    inline void SetKey(const char* value) { strncpy(m_key, value, sizeof(m_key)); m_key[sizeof(m_key)-1] = 0; }


    inline GameProperty& WithKey(const char* value) { SetKey(value); return *this; }


    inline const char* GetValue() const{ return m_value; }


    inline void SetValue(const char* value) { strncpy(m_value, value, sizeof(m_value)); m_value[sizeof(m_value)-1] = 0; }


    inline GameProperty& WithValue(const char* value) { SetValue(value); return *this; }

private:
    char m_key[MAX_KEY_LENGTH];
    char m_value[MAX_VALUE_LENGTH];
#endif
};

} // namespace Model
} // namespace Server
} // namespace GameLift
} // namespace Aws
