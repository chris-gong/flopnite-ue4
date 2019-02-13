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

#ifndef MAX_STRING_LIST_LENGTH
    #define MAX_STRING_LIST_LENGTH 10
#endif
#ifndef MAX_STRING_MAP_SIZE 
    #define MAX_STRING_MAP_SIZE 10
#endif
#ifndef MAX_STRING_LENGTH
    #define MAX_STRING_LENGTH 101
#endif

#ifdef GAMELIFT_USE_STD
#include <map>
#include <vector>
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
    * <p>A single player attribute value to be used when making a StartMatchBackfill
    * request.  This could be a string, number, string list, or string/number map.
    * For more information, see the <a
    * href="http://docs.aws.amazon.com/gamelift/latest/developerguide/">Amazon
    * GameLift Developer Guide</a>.</p>
    */
class AWS_GAMELIFT_API AttributeValue
{
public:
    enum class AttrType
    {
        NONE,
        STRING,
        DOUBLE,
        STRING_LIST,
        STRING_DOUBLE_MAP
    };

    /**
    * <p>Construct an attribute value of type string list.</p>
    */
    static AttributeValue ConstructStringList()
    {
        return AttributeValue(AttrType::STRING_LIST);
    }


    /**
    * <p>Construct an attribute value of type string list.</p>
    */
    static AttributeValue ConstructStringDoubleMap()
    {
        return AttributeValue(AttrType::STRING_DOUBLE_MAP);
    }


    inline const AttrType GetType() const { return m_attrType; }


#ifdef GAMELIFT_USE_STD
    AttributeValue() : m_attrType(AttrType::NONE)
    {
    }

    /**
    * <p>Construct an attribute value of type double.</p>
    */
    explicit AttributeValue(double n) : m_N(n), m_attrType(AttrType::DOUBLE)
    {
    }

    /**
    * <p>Construct an attribute value of type string.</p>
    */
    AttributeValue(std::string s) : m_S(s), m_attrType(AttrType::STRING)
    {
    }

    /**
    * <p>Destructor.</p>
    */
    ~AttributeValue() {}

    /**
    * <p>Copy Constructor.</p>
    */
    AttributeValue(const AttributeValue& other) :
        m_S(other.m_S),
        m_N(other.m_N),
        m_SL(other.m_SL),
        m_SDM(other.m_SDM),
        m_attrType(other.m_attrType)
    { }

    /**
    * <p>Move Constructor.</p>
    */
    AttributeValue(AttributeValue&& other) 
    { 
        *this = std::move(other);
    }

    /**
    * <p>Copy assignment Constructor.</p>
    */
    AttributeValue& operator= (const AttributeValue& other) 
    {
        m_S = other.m_S;
        m_N = other.m_N;
        m_SL = other.m_SL;
        m_SDM = other.m_SDM;
        m_attrType = other.m_attrType;

        return *this;
    }

    /**
    * <p>Move assignment Constructor.</p>
    */
    AttributeValue& operator= (AttributeValue&& other) 
    {
        m_S = std::move(other.m_S);
        m_N = std::move(other.m_N);
        m_SL = std::move(other.m_SL);
        m_SDM = std::move(other.m_SDM);
        m_attrType = std::move(other.m_attrType);

        return *this;
    }

    inline const std::string& GetS() const { return m_S; }


    inline const double GetN() const { return m_N; }


    inline const std::vector<std::string>& GetSL() const { return m_SL; }


    inline const std::map<std::string, double>& GetSDM() const { return m_SDM; }


    inline void AddString(std::string value) {
        if (m_attrType == AttrType::STRING_LIST && m_SL.size() < MAX_STRING_LIST_LENGTH) {
            m_SL.emplace_back(std::move(value));
        }
    }


    inline AttributeValue& WithString(std::string value) { AddString(value); return *this; }


    inline void AddStringAndDouble(std::string key, double value) {
        if (m_attrType == AttrType::STRING_DOUBLE_MAP && m_SDM.size() < MAX_STRING_MAP_SIZE) {
            m_SDM.emplace(std::move(key), std::move(value));
        }
    }


    inline AttributeValue& WithStringAndDouble(std::string key, double value) { AddStringAndDouble(key, value); return *this; }

private:
    explicit AttributeValue(AttrType attrType) : m_attrType(attrType)
    {
    }
#else
public:
    explicit AttributeValue(AttrType attrType) : m_SLCount(0), m_SDMCount(0), m_attrType(attrType)
    {
    }

    AttributeValue() : m_SLCount(0), m_SDMCount(0), m_attrType(AttrType::NONE)
    {
    }

    /**
    * <p>Construct an attribute value of type double.</p>
    */
    explicit AttributeValue(double n) : m_N(n), m_SLCount(0), m_SDMCount(0), m_attrType(AttrType::DOUBLE)
    {
    }

    struct KeyAndValue;
    typedef char AttributeStringType[MAX_STRING_LENGTH];

    explicit AttributeValue(const char* s)
    {
        initValues();

        m_attrType = AttrType::STRING;
        strncpy(m_S, s, MAX_STRING_LENGTH);
        m_S[MAX_STRING_LENGTH - 1] = '\0';
    }

    /**
    * <p>Destructor.</p>
    */
    ~AttributeValue() {}

    /**
    * <p>Copy Constructor.</p>
    */
    AttributeValue(const AttributeValue& other)
    {
        *this = other;
    }

    /**
    * <p>Move Constructor.</p>
    */
    AttributeValue(AttributeValue&& other) 
    {
        *this = std::move(other);
    }

    /**
    * <p>Copy assignment Constructor.</p>
    */
    AttributeValue& operator= (const AttributeValue& other) 
    {
        m_attrType = other.m_attrType;
        strncpy(m_S, other.m_S, MAX_STRING_LENGTH);
        m_N = other.m_N;
        m_SLCount = other.m_SLCount;
        m_SDMCount = other.m_SDMCount;
        for (int index = 0; index < m_SLCount; ++index)
        {
            strncpy(m_SL[index], other.m_SL[index], MAX_STRING_LENGTH);
        }
        for (int index = 0; index < m_SDMCount; ++index)
        {
            m_SDM[index] = other.m_SDM[index];
        }

        return *this;
    }

    /**
    * <p>Move assignment Constructor.</p>
    */
    AttributeValue& operator= (AttributeValue&& other) 
    {
        *this = other;

        return *this;
    }

    inline const char* GetS() const{ return m_S; }


    inline double GetN() const { return m_N; }


    inline const AttributeStringType* const GetSL(int& count) const { count = m_SLCount; return m_SL; }


    inline const KeyAndValue* GetSDM(int& count) const { count = m_SDMCount; return m_SDM; }


    inline void AddString(const char* value) {
        if (m_attrType == AttrType::STRING_LIST && m_SLCount < MAX_STRING_LIST_LENGTH) {
            strncpy(m_SL[m_SLCount], value, MAX_STRING_LENGTH);
            m_SL[m_SLCount++][MAX_STRING_LENGTH - 1] = '\0';
        }
    };


    inline AttributeValue& WithString(const char* value) { AddString(value); return *this; }


    inline void AddStringAndDouble(const char* key, double value) {
        if (m_attrType == AttrType::STRING_DOUBLE_MAP && m_SDMCount < MAX_STRING_MAP_SIZE) {
            m_SDM[m_SDMCount++] = KeyAndValue(key, value);
        }
    };


    inline AttributeValue& WithStringAndDouble(const char* key, double value) { AddStringAndDouble(key, value); return *this; }


    struct KeyAndValue
    {
        KeyAndValue() : m_key(), m_value(0)
        {
        }

        KeyAndValue(const char* key, double value) : m_value(value)
        {
            strncpy(m_key, key, MAX_STRING_LENGTH);
            m_key[MAX_STRING_LENGTH - 1] = '\0';
        }

        KeyAndValue(const KeyAndValue& other)
        {
            *this = other;
        }

        KeyAndValue& operator=(const KeyAndValue& other)
        {
            strncpy(m_key, other.m_key, MAX_STRING_LENGTH);
            m_key[MAX_STRING_LENGTH - 1] = '\0';
            m_value = other.m_value;

            return *this;
        }

        KeyAndValue(const KeyAndValue&& other)
        {
            *this = std::move(other);
        }

        KeyAndValue& operator=(const KeyAndValue&& other)
        {
            *this = other;

            return *this;
        }

        inline const char* GetKey() const { return m_key; }

        inline double GetValue() const { return m_value; }

    private:
        AttributeStringType m_key;
        double m_value;
    };

private:
    void initValues()
    {
        m_S[0] = '\0';
        m_N = 0;
        memset(m_SL, 0, sizeof(AttributeStringType) * MAX_STRING_LIST_LENGTH);

        m_SLCount = 0;
        m_SDMCount = 0;
    }
#endif

private:
#ifdef GAMELIFT_USE_STD
    std::string m_S;
    double m_N;
    std::vector<std::string> m_SL;
    std::map<std::string, double> m_SDM;
    std::string m_value;
#else
    AttributeStringType m_S;
    double m_N;
    AttributeStringType m_SL[MAX_STRING_LIST_LENGTH];
    KeyAndValue m_SDM[MAX_STRING_MAP_SIZE];
    int m_SLCount;
    int m_SDMCount;
#endif
    AttrType m_attrType;
};

} // namespace Model
} // namespace Server
} // namespace GameLift
} // namespace Aws
