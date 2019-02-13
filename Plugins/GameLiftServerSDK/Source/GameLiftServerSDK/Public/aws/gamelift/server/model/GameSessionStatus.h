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
#include <cstring>

namespace Aws
{
namespace GameLift
{
namespace Server
{
namespace Model
{
  enum class GameSessionStatus
  {
    NOT_SET,
    ACTIVE,
    ACTIVATING,
    TERMINATED,
    TERMINATING
  };

namespace GameSessionStatusMapper
{
#ifdef GAMELIFT_USE_STD
    inline AWS_GAMELIFT_API GameSessionStatus GetGameSessionStatusForName(const std::string& s_name) {
      const char* name = s_name.c_str();
#else
    inline AWS_GAMELIFT_API GameSessionStatus GetGameSessionStatusForName(const char* name) {
#endif
      if (strcmp(name, "ACTIVE") == 0) {
          return GameSessionStatus::ACTIVE;
      }
      if (strcmp(name, "ACTIVATING") == 0) {
          return GameSessionStatus::ACTIVATING;
      }
      if (strcmp(name, "TERMINATING") == 0) {
          return GameSessionStatus::TERMINATING;
      }
      if (strcmp(name, "TERMINATED") == 0) {
          return GameSessionStatus::TERMINATED;
      }
      return GameSessionStatus::NOT_SET;
    }

#ifdef GAMELIFT_USE_STD
    inline AWS_GAMELIFT_API std::string GetNameForGameSessionStatus(GameSessionStatus value)
#else
    inline AWS_GAMELIFT_API const char* GetNameForGameSessionStatus(GameSessionStatus value)
#endif
    {
      switch (value) {
          case GameSessionStatus::ACTIVE:
              return "ACTIVE";
          case GameSessionStatus::ACTIVATING:
              return "ACTIVATING";
          case GameSessionStatus::TERMINATING:
              return "TERMINATING";
          case GameSessionStatus::TERMINATED:
              return "TERMINATED";
          default:
              return "NOT_SET";
      }
    }
} // namespace GameSessionStatusMapper
} // namespace Model
} // namespace Server
} // namespace GameLift
} // namespace Aws
