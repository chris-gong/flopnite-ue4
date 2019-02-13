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
#include <aws/gamelift/server/model/AttributeValue.h>
#include <aws/gamelift/server/protocols/sdk.pb.h>

namespace Aws
{
namespace GameLift
{
namespace Internal
{
namespace ModelMapper
{
    using namespace Aws::GameLift::Server::Model;
    using namespace com::amazon::whitewater::auxproxy;

    class AttributeValueMapper
    {
    public:
#ifdef GAMELIFT_USE_STD
        static pbuffer::AttributeValue ParseFromAttributeValue(const AttributeValue &attrValue)
        {
            AttributeValue::AttrType type = attrValue.GetType();
            pbuffer::AttributeValue event;
            event.set_type(static_cast<int>(type));

            switch (type)
            {
            case AttributeValue::AttrType::STRING:
            {
                event.set_s(attrValue.GetS());
                break;
            }

            case AttributeValue::AttrType::STRING_LIST:
            {
                for (auto& str : attrValue.GetSL())
                {
                    event.add_sl(str);
                }
                break;
            }

            case AttributeValue::AttrType::STRING_DOUBLE_MAP:
            {
                for (const auto& iter : attrValue.GetSDM())
                {
                    (*event.mutable_sdm())[iter.first] = iter.second;
                }
                break;
            }

            default:
            {
                event.set_n(attrValue.GetN());
                break;
            }
            }

            return event;
        }
#else
        static pbuffer::AttributeValue ParseFromAttributeValue(const AttributeValue &attrValue)
        {
            AttributeValue::AttrType type = attrValue.GetType();
            pbuffer::AttributeValue event;
            event.set_type(static_cast<int>(type));

            switch (type)
            {
            case AttributeValue::AttrType::STRING:
            {
                event.set_s(attrValue.GetS());
                break;
            }

            case AttributeValue::AttrType::STRING_LIST:
            {
                int count;
                const AttributeValue::AttributeStringType* strs = attrValue.GetSL(count);
                for (int index = 0; index < count; ++index)
                {
                    event.add_sl(strs[index]);
                }
                break;
            }

            case AttributeValue::AttrType::STRING_DOUBLE_MAP:
            {
                int count;
                const AttributeValue::KeyAndValue* entries = attrValue.GetSDM(count);
                for (int index = 0; index < count; ++index)
                {
                    (*event.mutable_sdm())[entries[index].GetKey()] = entries[index].GetValue();
                }
                break;
            }

            default:
            {
                event.set_n(attrValue.GetN());
                break;
            }
            }

            return event;
        }
#endif
    };
} //namespace ModelMapper
} //namespace Internal
} //namespace GameLift
} //namespace Aws
