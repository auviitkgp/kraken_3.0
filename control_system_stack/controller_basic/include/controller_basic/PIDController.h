#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H

namespace kraken_controller
{
  
  class PIDController
  {
    private :
      float _kp;
      float _kd;
      float _ki;
      void resetErrors();
    public:
      PIDController(float kp=0,float kd=0,float ki=0);
      /*
       * Must implement how to calculate next output
       */
      virtual float getNextOutput(float feedback)=0;
      /*
       * Must implement how the setpoins to be changed. Force is used to resetErrors
       */
      virtual void  changeSetPoint(float setpoint,bool force=false)=0;
      /*
       * Force is used to call resetErrors while changin parameters
       */
      void  changeKP(float kp,bool force=false);
      void  changeKD(float kd,bool force=false);
      void  changeKI(float kI,bool force=false);
      /*
       * To revert the controller gains to 0
       */
      void  resetController();
      inline float getError()
      {
        return _error;
      }
      inline float getDError()
      {
        return _D_error;
      }
      inline float getIError()
      {
        return _I_error;
      }
      inline float getSetPoint()
      {
        return _setpoint;
      }
      virtual ~PIDController();
    protected:
      float _setpoint;
      float _error;
      float _D_error;
      float _I_error;
    
  };
  
}

#endif // PIDCONTROLLER_H
