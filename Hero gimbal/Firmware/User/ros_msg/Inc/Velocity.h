#ifndef _ROS_velocity_msgs_Velocity_h
#define _ROS_velocity_msgs_Velocity_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "msg.h"
#include "Header.h"

namespace velocity_msgs
{

  class Velocity : public ros::Msg
  {
    public:
      typedef std_msgs::Header _Header_type;
      _Header_type Header;
      float length_dot[6];

    Velocity():
      Header(),
      length_dot()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->Header.serialize(outbuffer + offset);
      for( uint32_t i = 0; i < 6; i++){
      union {
        float real;
        uint32_t base;
      } u_length_doti;
      u_length_doti.real = this->length_dot[i];
      *(outbuffer + offset + 0) = (u_length_doti.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_length_doti.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_length_doti.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_length_doti.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->length_dot[i]);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->Header.deserialize(inbuffer + offset);
      for( uint32_t i = 0; i < 6; i++){
      union {
        float real;
        uint32_t base;
      } u_length_doti;
      u_length_doti.base = 0;
      u_length_doti.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_length_doti.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_length_doti.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_length_doti.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->length_dot[i] = u_length_doti.real;
      offset += sizeof(this->length_dot[i]);
      }
     return offset;
    }

    virtual const char * getType() override { return "velocity_msgs/Velocity"; };
    virtual const char * getMD5() override { return "83d00c936b12ff642d3c23ac3a7bfeb0"; };

  };

}
#endif
