#include <controller_basic/PIDController.h>
#include <dynamic_reconfigure/server.h>
#include <controller_basic/controllerConfig.h>

namespace kraken_controller
{
void callback(controller_basic::controllerConfig&config, uint32_t level)
{
    ROS_INFO("Reconfigure Request: %f %f %f",
             config.Kp,
             config.Kd,
             config.Ki);
}
PIDController::PIDController(float kp, float kd, float ki):_kp(kp),_kd(kd),_ki(ki)
{
    _error=0;
    _D_error=0;
    _I_error=0;
    _setpoint=0;
}

PIDController::~PIDController()
{

}

void PIDController::changeKD(float kd, bool force)
{
    _kd = kd;

    if(force)
    {
        resetErrors();
    }
}

void PIDController::changeKI(float kI, bool force)
{
    _ki = kI;

    if(force)
    {
        resetErrors();
    }
}

void PIDController::changeKP(float kp, bool force)
{
    _kp = kp;

    if(force)
    {
        resetErrors();
    }
}

void PIDController::resetController()
{
    _kp = _ki = _kd = 0;
}

void PIDController::resetErrors()
{
    _error=0;
    _D_error=0;
    _I_error=0;
    _setpoint=0;
}

}
