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

#include <aws/gamelift/common/GameLiftErrors.h>
#include <aws/gamelift/common/Outcome.h>

namespace Aws
{
namespace GameLift
{
namespace Internal
{
    enum class GAMELIFT_INTERNAL_STATE_TYPE {
        CLIENT,
        SERVER
    };

    class GameLiftCommonState;

    typedef Aws::GameLift::Outcome<GameLiftCommonState*, GameLiftError> GetInstanceOutcome;

    class GameLiftCommonState
    {
    public:

        static GetInstanceOutcome GetInstance();

        static GetInstanceOutcome GetInstance(GAMELIFT_INTERNAL_STATE_TYPE stateType);

        static GenericOutcome SetInstance(GameLiftCommonState* instance);

        static GenericOutcome DestroyInstance();

        virtual GAMELIFT_INTERNAL_STATE_TYPE GetStateType() = 0;

        //Normally this should not be public, but AwsMemory needs access to it
        virtual ~GameLiftCommonState();

    protected:

        GameLiftCommonState();

    private:

        static GameLiftCommonState* m_instance;
    
    };

} //namespace Internal
} //namespace GameLift
} //namespace Aws
