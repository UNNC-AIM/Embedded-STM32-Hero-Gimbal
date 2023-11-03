//
// Created by ssyzz7 on 2/17/2023.
//

#ifndef STM32_PROJ_TWIST_H
#define STM32_PROJ_TWIST_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "msg.h"
#include "Vector3.h"

namespace geometry_msgs
{

    class Twist : public ros::Msg
    {
    public:
        typedef geometry_msgs::Vector3 _linear_type;
        _linear_type linear;
        typedef geometry_msgs::Vector3 _angular_type;
        _angular_type angular;

        Twist():
                linear(),
                angular()
        {
        }

        virtual int serialize(unsigned char *outbuffer) const override
        {
            int offset = 0;
            offset += this->linear.serialize(outbuffer + offset);
            offset += this->angular.serialize(outbuffer + offset);
            return offset;
        }

        virtual int deserialize(unsigned char *inbuffer) override
        {
            int offset = 0;
            offset += this->linear.deserialize(inbuffer + offset);
            offset += this->angular.deserialize(inbuffer + offset);
            return offset;
        }

        virtual const char * getType() override { return "geometry_msgs/Twist"; };
        virtual const char * getMD5() override { return "9f195f881246fdfa2798d1d3eebca84a"; };

    };

}

#endif //STM32_PROJ_TWIST_H
