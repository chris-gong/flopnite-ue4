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
  enum class PlayerSessionStatus
  {
    NOT_SET,
    RESERVED,
    ACTIVE,
    COMPLETED,
    TIMEDOUT
  };

namespace PlayerSessionStatusMapper
{
#ifdef GAMELIFT_USE_STD
  inline AWS_GAMELIFT_API PlayerSessionStatus GetPlayerSessionStatusForName(const std::string& name)
  {
      if (name == "RESERVED") {
          return PlayerSessionStatus::RESERVED;
      }
      if (name == "ACTIVE") {
          return PlayerSessionStatus::ACTIVE;
      }
      if (name == "COMPLETED") {
          return PlayerSessionStatus::COMPLETED;
      }
      if (name == "TIMEDOUT") {
          return PlayerSessionStatus::TIMEDOUT;
      }
      return PlayerSessionStatus::NOT_SET;
  }
  inline AWS_GAMELIFT_API std::string GetNameForPlayerSessionStatus(PlayerSessionStatus value)
  {
      switch (value) {
          case PlayerSessionStatus::RESERVED:
              return "RESERVED";
          case PlayerSessionStatus::ACTIVE:
              return "ACTIVE";
          case PlayerSessionStatus::COMPLETED:
              return "COMPLETED";
          case PlayerSessionStatus::TIMEDOUT:
              return "TIMEDOUT";
          default:
              return "NOT_SET";
      }
  }
#else
  inline AWS_GAMELIFT_API PlayerSessionStatus GetPlayerSessionStatusForName(const char* name)
  {
      if (strcmp(name, "RESERVED") == 0) {
          return PlayerSessionStatus::RESERVED;
      }
      if (strcmp(name, "ACTIVE") == 0) {
          return PlayerSessionStatus::ACTIVE;
      }
      if (strcmp(name, "COMPLETED") == 0) {
          return PlayerSessionStatus::COMPLETED;
      }
      if (strcmp(name, "TIMEDOUT") == 0) {
          return PlayerSessionStatus::TIMEDOUT;
      }
      return PlayerSessionStatus::NOT_SET;
  }
  inline AWS_GAMELIFT_API const char* GetNameForPlayerSessionStatus(PlayerSessionStatus value)
  {
      switch (value) {
          case PlayerSessionStatus::RESERVED:
              return "RESERVED";
          case PlayerSessionStatus::ACTIVE:
              return "ACTIVE";
          case PlayerSessionStatus::COMPLETED:
              return "COMPLETED";
          case PlayerSessionStatus::TIMEDOUT:
              return "TIMEDOUT";
          default:
              return "NOT_SET";
      }
  }
#endif
} // namespace GameSessionStatusMapper
} // namespace Model
} // namespace Server
} // namespace GameLift
} // namespace Aws
