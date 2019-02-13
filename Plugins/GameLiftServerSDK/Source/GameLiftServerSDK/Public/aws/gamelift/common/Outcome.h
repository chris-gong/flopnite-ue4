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
#include <aws/gamelift/server/model/DescribePlayerSessionsResult.h>
#include <aws/gamelift/server/model/StartMatchBackfillResult.h>
#include <future>

namespace Aws
{
    namespace GameLift
    {
        /**
        * Template class representing the outcome of making a request.  It will contain
        * either a successful result or the failure error.  The caller must check
        * whether the outcome of the request was a success before attempting to access
        *  the result or the error.
        */
        template<typename R, typename E> // Result, Error
        class Outcome
        {
#ifdef GAMELIFT_USE_STD
        public:

            Outcome() : success(false)
            {
            } // Default constructor
            Outcome(const R& r) : result(r), success(true)
            {
            } // Result copy constructor
            Outcome(const E& e) : error(e), success(false)
            {
            } // Error copy constructor
            Outcome(R&& r) : result(std::forward<R>(r)), success(true)
            {
            } // Result move constructor
            Outcome(E&& e) : error(std::forward<E>(e)), success(false)
            {
            } // Error move constructor
            Outcome(const Outcome& o) :
                result(o.result),
                error(o.error),
                success(o.success)
            {
            }

            Outcome& operator=(const Outcome& o)
            {
                if (this != &o)
                {
                    result = o.result;
                    error = o.error;
                    success = o.success;
                }

                return *this;
            }

            Outcome(Outcome&& o) : // Required to force Move Constructor
                result(std::move(o.result)),
                error(std::move(o.error)),
                success(o.success)
            {
            }

            Outcome& operator=(Outcome&& o)
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

        typedef Outcome<void*, GameLiftError> GenericOutcome;
        typedef std::future<GenericOutcome> GenericOutcomeCallable;
        typedef Outcome<std::string, GameLiftError> AwsStringOutcome;
        typedef Outcome<long, GameLiftError> AwsLongOutcome;
        typedef Outcome<Aws::GameLift::Server::Model::DescribePlayerSessionsResult, GameLiftError> DescribePlayerSessionsOutcome;
        typedef Outcome<Aws::GameLift::Server::Model::StartMatchBackfillResult, GameLiftError> StartMatchBackfillOutcome;
#else
        public:

            Outcome() : success(false)
            {
            } // Default constructor
            Outcome(const R& r) : result(r), success(true)
            {
            } // Result copy constructor
            Outcome(const E& e) : error(e), success(false)
            {
            } // Error copy constructor

            Outcome(const Outcome& o) :
                result(o.result),
                error(o.error),
                success(o.success)
            {
            }

            Outcome& operator=(const Outcome& o)
            {
                if (this != &o)
                {
                    result = o.result;
                    error = o.error;
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

        typedef Outcome<void*, GameLiftError> GenericOutcome;
        typedef Outcome<const char*, GameLiftError> AwsStringOutcome;
        typedef Outcome<long, GameLiftError> AwsLongOutcome;
        typedef Outcome<Aws::GameLift::Server::Model::DescribePlayerSessionsResult, GameLiftError> DescribePlayerSessionsOutcome;
        typedef Outcome<Aws::GameLift::Server::Model::StartMatchBackfillResult, GameLiftError> StartMatchBackfillOutcome;
#endif
    } // namespace GameLift
} // namespace Aws
