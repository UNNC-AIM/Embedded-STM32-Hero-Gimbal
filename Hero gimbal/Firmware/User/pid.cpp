#include <pid.h>
#include "dbus_uart.h"

void PID::init (fp32 _kp, fp32 _ki, fp32 _kd, fp32 _limit, fp32 _i_limit) {
    pid[0] = _kp;
    pid[1] = _ki;
    pid[2] = _kd;

    limit = _limit;
    i_limit = _i_limit;

    output = i_output = error[0] = error[1] = 0.0f;
}

PID::~PID () {}

float PID::calc (fp32 _set, fp32 _ref) {
    error[0] = _set - _ref;
    i_output += pid[1] * error[0];
    saturation (i_output, i_limit);
    output = pid[0] * error[0] + i_output + pid[2] * (error[0] - error[1]);
    saturation (output, limit);
    error[1] = error[0];

    return output;
}
