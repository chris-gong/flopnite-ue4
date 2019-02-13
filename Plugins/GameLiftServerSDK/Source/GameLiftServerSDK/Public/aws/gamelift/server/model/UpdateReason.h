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
                enum class UpdateReason
                {
                    MATCHMAKING_DATA_UPDATED,
                    BACKFILL_FAILED,
                    BACKFILL_TIMED_OUT,
                    BACKFILL_CANCELLED,
                    UNKNOWN
                };

                namespace UpdateReasonMapper
                {
#ifdef GAMELIFT_USE_STD
                    inline AWS_GAMELIFT_API UpdateReason GetUpdateReasonForName(std::string& s_name) {
                    const char* name = s_name.c_str();
#else
                    inline AWS_GAMELIFT_API UpdateReason GetUpdateReasonForName(const char* name) {
#endif
                        if (strcmp(name, "MATCHMAKING_DATA_UPDATED") == 0)
                        {
                            return UpdateReason::MATCHMAKING_DATA_UPDATED;
                        }
                        if (strcmp(name, "BACKFILL_FAILED") == 0)
                        {
                            return UpdateReason::BACKFILL_FAILED;
                        }
                        if (strcmp(name, "BACKFILL_TIMED_OUT") == 0)
                        {
                            return UpdateReason::BACKFILL_TIMED_OUT;
                        }
                        if (strcmp(name, "BACKFILL_CANCELLED") == 0)
                        {
                            return UpdateReason::BACKFILL_CANCELLED;
                        }

                        return UpdateReason::UNKNOWN;
                    }
#ifdef GAMELIFT_USE_STD
                    inline AWS_GAMELIFT_API std::string GetNameForUpdateReason(UpdateReason value)
#else
                    inline AWS_GAMELIFT_API const char* GetNameForUpdateReason(UpdateReason value)
#endif
                    {
                        switch (value)
                        {
                        case UpdateReason::MATCHMAKING_DATA_UPDATED:
                            return "MATCHMAKING_DATA_UPDATED";
                        case UpdateReason::BACKFILL_FAILED:
                            return "BACKFILL_FAILED";
                        case UpdateReason::BACKFILL_TIMED_OUT:
                            return "BACKFILL_TIMED_OUT";
                        case UpdateReason::BACKFILL_CANCELLED:
                            return "BACKFILL_CANCELLED";
                        default:
                            return "UNKNOWN";
                        }
                    }
                } // namespace GameSessionStatusMapper
            } // namespace Model
        } // namespace Server
    } // namespace GameLift
} // namespace Aws
