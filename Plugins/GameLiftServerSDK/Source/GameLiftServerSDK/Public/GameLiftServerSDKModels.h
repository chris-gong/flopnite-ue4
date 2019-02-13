// AMAZON CONFIDENTIAL

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

enum class GAMELIFTSERVERSDK_API EPlayerSessionCreationPolicy
{
    NOT_SET,
    ACCEPT_ALL,
    DENY_ALL
};

inline GAMELIFTSERVERSDK_API EPlayerSessionCreationPolicy GetPlayerSessionCreationPolicyForName(FString& name)
{
    if (name == "ACCEPT_ALL") {
        return EPlayerSessionCreationPolicy::ACCEPT_ALL;
    }
    if (name == "DENY_ALL") {
        return EPlayerSessionCreationPolicy::DENY_ALL;
    }
    return EPlayerSessionCreationPolicy::NOT_SET;
}
inline GAMELIFTSERVERSDK_API FString GetNameForPlayerSessionCreationPolicy(EPlayerSessionCreationPolicy value)
{
    switch (value) {
    case EPlayerSessionCreationPolicy::ACCEPT_ALL:
        return FString("ACCEPT_ALL");
    case EPlayerSessionCreationPolicy::DENY_ALL:
        return FString("DENY_ALL");
    default:
        return FString("NOT_SET");
    }
}


struct GAMELIFTSERVERSDK_API FGameLiftError {
    Aws::GameLift::GAMELIFT_ERROR_TYPE m_errorType;
    FString m_errorName;
    FString m_errorMessage;

    FGameLiftError(){}
    FGameLiftError(Aws::GameLift::GAMELIFT_ERROR_TYPE type, FString name, FString message) :
        m_errorType(type),
        m_errorName(name),
        m_errorMessage(message)
    {}

    FGameLiftError(Aws::GameLift::GameLiftError error)
    {
        m_errorMessage = FString(error.GetErrorMessage());
        m_errorName = FString(error.GetErrorName());
        m_errorType = error.GetErrorType();
    }
};

template <typename R, typename E>
class GAMELIFTSERVERSDK_API TGameLiftOutcome
{
public:

    TGameLiftOutcome() : success(false)
    {
    } // Default constructor
    TGameLiftOutcome(const R& r) : result(r), success(true)
    {
    } // Result copy constructor
    TGameLiftOutcome(const E& e) : error(e), success(false)
    {
    } // Error copy constructor
    TGameLiftOutcome(R&& r) : result(std::forward<R>(r)), success(true)
    {
    } // Result move constructor
    TGameLiftOutcome(E&& e) : error(std::forward<E>(e)), success(false)
    {
    } // Error move constructor
    TGameLiftOutcome(const TGameLiftOutcome& o) :
        result(o.result),
        error(o.error),
        success(o.success)
    {
    }

    TGameLiftOutcome& operator=(const TGameLiftOutcome& o)
    {
        if (this != &o)
        {
            result = o.result;
            error = o.error;
            success = o.success;
        }

        return *this;
    }

    TGameLiftOutcome(TGameLiftOutcome&& o) : // Required to force Move Constructor
        result(std::move(o.result)),
        error(std::move(o.error)),
        success(o.success)
    {
    }

    TGameLiftOutcome& operator=(TGameLiftOutcome&& o)
    {
        if (this != &o)
        {
            result = std::move(o.result);
            error = std::move(o.error);
            success = o.success;
        }

        return *this;
    }

    inline const R& GetResult() const
    {
        return result;
    }

    inline R& GetResult()
    {
        return result;
    }

    /**
    * casts the underlying result to an r-value so that caller can't take ownership of underlying resources.
    * this is necessary when streams are involved.
    */
    inline R&& GetResultWithOwnership()
    {
        return std::move(result);
    }

    inline const E& GetError() const
    {
        return error;
    }

    inline bool IsSuccess() const
    {
        return this->success;
    }

private:
    R result;
    E error;
    bool success;
};

typedef TGameLiftOutcome<void*, FGameLiftError> FGameLiftGenericOutcome;
typedef TGameLiftOutcome<FString, FGameLiftError> FGameLiftStringOutcome;
typedef TGameLiftOutcome<long, FGameLiftError> FGameLiftLongOutcome;