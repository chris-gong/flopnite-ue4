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

#include <string>
#include <vector>

#ifdef _WIN32
//disable windows complaining about max template size.
#pragma warning (disable : 4503)
#endif

#if defined (_MSC_VER)
#pragma warning(disable : 4251)
  #ifdef USE_IMPORT_EXPORT
    #ifdef AWS_GAMELIFT_EXPORTS
      #define AWS_GAMELIFT_API __declspec(dllexport)
    #else
      #define AWS_GAMELIFT_API __declspec(dllimport)
    #endif /* AWS_GAMELIFT_EXPORTS */
  #else
     #define AWS_GAMELIFT_API
   #endif //
#else /* defined (_WIN32) */
#define AWS_GAMELIFT_API
#endif