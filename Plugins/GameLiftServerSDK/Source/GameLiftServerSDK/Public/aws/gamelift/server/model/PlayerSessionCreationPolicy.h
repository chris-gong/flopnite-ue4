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

namespace Aws
{
namespace GameLift
{
namespace Server
{
namespace Model
{
    enum class PlayerSessionCreationPolicy
    {
        NOT_SET,
        ACCEPT_ALL,
        DENY_ALL
    };

namespace PlayerSessionCreationPolicyMapper {
#ifdef GAMELIFT_USE_STD
    inline AWS_GAMELIFT_API PlayerSessionCreationPolicy GetPlayerSessionCreationPolicyForName(const std::string& name)
    {
        if (name == "ACCEPT_ALL") {
           return PlayerSessionCreationPolicy::ACCEPT_ALL;
        }
        if (name == "DENY_ALL") {
            return PlayerSessionCreationPolicy::DENY_ALL;
        }
        return PlayerSessionCreationPolicy::NOT_SET;
    }

    inline AWS_GAMELIFT_API std::string GetNameForPlayerSessionCreationPolicy(PlayerSessionCreationPolicy value)
    {
        switch (value) {
            case PlayerSessionCreationPolicy::ACCEPT_ALL:
        return "ACCEPT_ALL";
            case PlayerSessionCreationPolicy::DENY_ALL:
        return "DENY_ALL";
            default:
        return "NOT_SET";
    }
    }
#else
    inline AWS_GAMELIFT_API PlayerSessionCreationPolicy GetPlayerSessionCreationPolicyForName(const char* name)
    {
        if (strcmp(name, "ACCEPT_ALL") == 0) {
            return PlayerSessionCreationPolicy::ACCEPT_ALL;
        }
        if (strcmp(name, "DENY_ALL") == 0) {
            return PlayerSessionCreationPolicy::DENY_ALL;
        }
        return PlayerSessionCreationPolicy::NOT_SET;
    }

    inline AWS_GAMELIFT_API const char* GetNameForPlayerSessionCreationPolicy(PlayerSessionCreationPolicy value)
    {
        switch (value) {
        case PlayerSessionCreationPolicy::ACCEPT_ALL:
            return "ACCEPT_ALL";
        case PlayerSessionCreationPolicy::DENY_ALL:
            return "DENY_ALL";
        default:
            return "NOT_SET";
        }
    }
#endif
} // namespace PlayerSessionCreationPolicyMapper
} // namespace Model
} // namespace Server
} // namespace GameLift
} // namespace Aws
