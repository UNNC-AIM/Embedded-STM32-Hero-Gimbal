//
// Created by ssyzz7 on 2/17/2023.
//

#ifndef STM32_PROJ_PID_H
#define STM32_PROJ_PID_H

#include "typedefs.h"

#define saturation(input, max) \
    {                          \
        if (input > max)       \
        {                      \
            input = max;       \
        }                      \
        else if (input < -max) \
        {                      \
            input = -max;      \
        }                      \
    }


class PID {
private:
    fp32 pid[3];
    fp32 limit;
    fp32 i_limit;
    fp32 error[2];
    fp32 i_output;
    fp32 output;
public:

    ~PID ();
    void init (fp32 _kp, fp32 _ki, fp32 _kd, fp32 _limit, fp32 _i_limit);
    float calc (fp32 _set, fp32 _ref);
};




#endif //STM32_PROJ_PID_H
