/**********************************************************************************************
 * This is another PID test implementation based on the great work of
 * 
 * * Brett Beauregard <br3ttb@gmail.com> brettbeauregard.com
 * * Arduino PID Library - Version 1.2.1
 * * This Library is licensed under the MIT License
 * 
 * all credits go to him. (Tobias <medlor@web.de>)
 **********************************************************************************************/
 
#ifndef PIDBias_h
#define PIDBias_h
#define LIBRARY_VERSION	0.0.1

#include "rancilio-pid.h"

class PIDBias
{
  public:

  //Constants used in some of the functions below
  #define AUTOMATIC	1
  #define MANUAL	0

  #ifndef DEBUGMODE
  #define DEBUG_printLib(fmt, ...)
  #else
  #define DEBUG_printLib(fmt, ...) if ((*myDebug).isActive((*myDebug).DEBUG))   (*myDebug).printf("%0u " fmt, millis()/1000, ##__VA_ARGS__)
  #endif

  //commonly used functions **************************************************************************
    PIDBias(double*, double*, double*, double*,        // * constructor.  links the PIDBias to the Input, Output, and 
        double, double, double, RemoteDebug*);//   Setpoint.  Initial tuning parameters are also set here.
                                          //   (overload for specifying proportional mode)
	
    void SetMode(int Mode);               // * sets PIDBias to either Manual (0) or Auto (non-0)

    bool Compute();                       // * performs the PIDBias calculation.  it should be
                                          //   called every time loop() cycles. ON/OFF and
                                          //   calculation frequency can be set using SetMode
                                          //   SetSampleTime respectively

    void SetOutputLimits(double, double); // * clamps the output to a specific range. 0-255 by default, but
										                      //   it's likely the user will want to change this depending on
										                      //   the application
	


  //available but not commonly used functions ********************************************************
    void SetTunings(double, double,       // * While most users will set the tunings once in the 
                    double);         	    //   constructor, this function gives the user the option
                                          //   of changing tunings during runtime for Adaptive control      	  

    void SetSampleTime(int);              // * sets the frequency, in Milliseconds, with which 
                                          //   the PIDBias calculation is performed.  default is 100
										  
										  
										  
  //Display functions ****************************************************************
	double GetKp();						  // These functions query the pid for interal values.
	double GetKi();						  //  they were created mainly for the pid front-end,
	double GetKd();						  // where it's important to know what is actually 
	int GetMode();						  //  inside the PIDBias.

  double GetOutputP();
  double GetOutputI();
  double GetSumOutputI();
  double GetOutputD(); 
  double GetLastOutput();
  void SetBurst(double);
  void SetSumOutputI(double);
  double signnum_c(double);
  void SetFilterSumOutputI(double);
  void SetSteadyPowerOffset(double);
  void SetAutoTune(boolean);
  void SetSteadyPowerDefault(double);

  private:
	void Initialize();
	
	double dispKp;				// * we'll hold on to the tuning parameters in user-entered 
	double dispKi;				//   format for display purposes
	double dispKd;				//
    
	double kp;                  // * (P)roportional Tuning Parameter
  double ki;                  // * (I)ntegral Tuning Parameter
  double kd;                  // * (D)erivative Tuning Parameter

  double outputP;
  double outputI;
  double outputD;
  double sumOutputD;
  double sumOutputI;
  double burstOutput;
  double filterSumOutputI;
  double steadyPowerDefault;
  double steadyPowerOffset;
  boolean steadyPowerAutoTune;

  double *myInput;              // * Pointers to the Input, Output, and Setpoint variables
  double *myOutput;             //   This creates a hard link between the variables and the 
  double *mySetpoint;           //   PIDBias, freeing the user from having to constantly tell us
                                //   what these values are.  with pointers we'll just know.
  double *mySteadyPower;
  RemoteDebug *myDebug;
	unsigned long lastTime;
  unsigned long lastTrigger;
  double lastInput, lastLastInput, lastOutput, lastError;
	unsigned long SampleTime;
	double outMin, outMax;
	bool inAuto;
};
#endif
