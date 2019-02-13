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
#pragma warning(disable:4996)
#include <aws/gamelift/common/GameLift_EXPORTS.h>

namespace Aws
{
namespace GameLift
{
namespace Server
{
namespace Model
{
#ifndef GAMELIFT_USE_STD
    #ifndef MAX_TICKET_ID_LENGTH
        #define MAX_TICKET_ID_LENGTH 129
    #endif
#endif

  /**
   * <p>Represents the returned data from StartMatchBackfill (currently only the ticketId)
   * in response to a request action.
   * For more information, see the <a
   * href="http://docs.aws.amazon.com/gamelift/latest/developerguide/">Amazon
   * GameLift Developer Guide</a>.</p>
   */
  class AWS_GAMELIFT_API StartMatchBackfillResult
  {
#ifdef GAMELIFT_USE_STD
  public:
      StartMatchBackfillResult() {}

      /**
      * <p>Destructor.</p>
      */
      ~StartMatchBackfillResult() {}

      /**
      * <p>Copy Constructor.</p>
      */
      StartMatchBackfillResult(const StartMatchBackfillResult& other) :
          m_ticketId(other.m_ticketId)
      { }

      /**
      * <p>Move Constructor.</p>
      */
      StartMatchBackfillResult(StartMatchBackfillResult&& other) 
      {
          *this = std::move(other);
      }


      /**
      * <p>Copy assignment Constructor.</p>
      */
      StartMatchBackfillResult& operator= (const StartMatchBackfillResult& other) 
      {
          m_ticketId = other.m_ticketId;

          return *this;
      }

      /**
      * <p>Move assignment Constructor.</p>
      */
      StartMatchBackfillResult& operator= (StartMatchBackfillResult&& other) 
      {
          m_ticketId = std::move(other.m_ticketId);

          return *this;
      }

    /**
     * <p>TicketId used for the submitted match backfill request.</p>
     */
    inline const std::string& GetTicketId() const{ return m_ticketId; }

    /**
     * <p>TicketId used for the submitted match backfill request.</p>
     */
    inline void SetTicketId(const std::string& value) { m_ticketId = value; }

    /**
     * <p>TicketId used for the submitted match backfill request.</p>
     */
    inline StartMatchBackfillResult& WithTicketId(std::string value) { SetTicketId(value); return *this;}

  private:
    std::string m_ticketId;
#else
public:
    StartMatchBackfillResult()
    {
		memset(m_ticketId, 0, sizeof(m_ticketId));
    }

    /**
    * <p>Destructor.</p>
    */
    ~StartMatchBackfillResult() {}

    /**
    * <p>Copy Constructor.</p>
    */
    StartMatchBackfillResult(const StartMatchBackfillResult& other)
    {
        strncpy(m_ticketId, other.m_ticketId, sizeof(other.m_ticketId));
    }

    /**
    * <p>Move Constructor.</p>
    */
    StartMatchBackfillResult(StartMatchBackfillResult&& other)
    {
        *this = std::move(other);
    }

    /**
    * <p>Copy assignment Constructor.</p>
    */
    StartMatchBackfillResult& operator= (const StartMatchBackfillResult& other) 
    {
        strncpy(m_ticketId, other.m_ticketId, sizeof(other.m_ticketId));

        return *this;
    }

    /**
    * <p>Move assignment Constructor.</p>
    */
    StartMatchBackfillResult& operator= (StartMatchBackfillResult&& other) 
    {
        strncpy(m_ticketId, other.m_ticketId, sizeof(other.m_ticketId));

        return *this;
    }

    /**
     * <p>TicketId used for the submitted match backfill request.</p>
     */
    inline const char* GetTicketId() const{ return m_ticketId; }

    /**
     * <p>TicketId used for the submitted match backfill request.</p>
     */
    inline void SetTicketId(const char* value) { strncpy(m_ticketId, value, sizeof(m_ticketId)); m_ticketId[sizeof(m_ticketId)-1] = '\0'; }

    /**
     * <p>TicketId used for the submitted match backfill request.</p>
     */
    inline StartMatchBackfillResult& WithTicketId(const char* value) { SetTicketId(value); return *this;}

  private:
    char m_ticketId[MAX_TICKET_ID_LENGTH];
#endif
  };

} // namespace Model
} // namespace Server
} // namespace GameLift
} // namespace Aws
