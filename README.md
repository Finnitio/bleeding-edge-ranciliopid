# ranciliopid
Rancilio-Silvia PID für Arduino http://rancilio-pid.de

BLEEDING EDGE BETA VERSION

Version 2.0.1_beta6

# Most important features in comparison to original rancilio master:
1. New PID Controller "Multi-state PID with steadyPower (Bias)"
   - Distinct PID settings dependend on the current "state" of the maschine. Most of the settings are either static or semi-automatically tuned.
   - Currently 5 states are implemented:
     - Coldstart (maschine is cold)
     - Coldstart stabilisation (directly after coldstart)
     - Inner Zone (temperature near setPoint)
     - Outer Zone (temperature outside of "inner zone")
     - Brewing
   - steadyPower is introduced which compensates the constant temperature loss due to environment
   - steadyPowerOffset is introduced which compensates the increased temperature loss when the maschine (brew head etc.) are still very cold.
   - PidController offers feature like filtering, special handling of setPoint crossings and more (hard-coded)
   - PID Controller is now integral part of the software and not an external library.
1. MQTT support to integrate maschine in smart-home solutions and to easier extract details for graphing/alerting.
1. Added RemoteDebug over telnet so that we dont need USB to debug/tune pid anymore (https://github.com/JoaoLopesF/RemoteDebug)
   - Just "$ telnet rancilio_ip 23"
1. "Brew Ready" Detection implemented, which detects when the temperature has stabilized at setPoint. It can send an
   MQTT event or have hardware pin 15 triggered (which can be used to turn a LED on).
1. All heater power relevant settings are now set and given in percent (and not absolute output) and therefore better to understand
1. Many useful functions to be used internally  getAverageTemperature(), pastTemperatureChange() + updateTemperatureHistory())
1. Safetly toogle added to shutdown heater on sensor malfunction (TEMPSENSORRECOVERY)

# ATTENTION:
- EEPROM has changed. Therefore you have to connect to blynk at least once after flashing, and manually set correct settings in blynk app (see screenshots for default values).
- I only own the pid-only hardware solution, so I am greatful for feedback(bug reports) from persons using the full version. Please monitor our maschine's temperature closely the first few times. The muti-state pid controller should never lead to temperatures greater than 5 degress above setpoint!

# Additional important information
- Installation is as defined on http://rancilio-pid.de/ but with following adapations.
- Copy file userConfig.h.SAMPLE to userConfig.h and edit this file accordingly.
- Additional Arduino dependency on PubSubClient (tested with Version 2.7.0). 
  Please install this lib by using Arduino->Sketch->Include Library->"Library Manager".
  ![Library Manager](https://github.com/medlor/ranciliopid/blob/master/PubSubClient_Dep.jpg)
- Additional Arduino dependency on RemoteDebug (tested with Version 3.0.5). 
  Please install this lib by using Arduino->Sketch->Include Library->"Library Manager".
  ![Library Manager](https://github.com/medlor/ranciliopid/blob/master/RemoteDebug_Dep.jpg)
- If you see the following error during compile "Height incorrect, please fix Adafruit_SSD1306.h!", then open the file  ./rancilio-pid/libraries/Adafruit_SSD1306/Adafruit_SSD1306.h and adapt Line 72ff to match following code:
  ```
  #define SSD1306_128_64
  //#define SSD1306_128_32
  //#define SSD1306_96_16
  ```
  Also check and fix all files matching "Adafruit_SSD1306.h" in your Documents/ subfolder!!
- If you want to increase the inactivity timeout you have to manually edit the file "RemoteDebugCfg.h" which is somewhere in a subpath below $USER/Documents/. Just search for the line "#define MAX_TIME_INACTIVE 600000" and replace the number. Example :
  ```
  #define MAX_TIME_INACTIVE 18000000
  ```
  Additionally just adapt the value MAX_TIME_INACTIVE in rancilio-pid.ino to your liking (new default is this code is 30min).


# Blynk App Dashboard
Unfortunately you have to manually build your dashboard (config does not fit in QR code).
Please stick to the following screenshots and use the "virtual pin mapping" as described:

## Blynk application screenshots
<p align="center">
<img src="https://github.com/medlor/ranciliopid/blob/master/blynk_app_status.jpg" height="500">
<img src="https://github.com/medlor/ranciliopid/blob/master/blynk_app_controller.jpg" height="500">
<img src="https://github.com/medlor/ranciliopid/blob/master/blynk_app_preinfusion.jpg" height="500">
</p>

## Blynk Pin Mapping
- Tab "Status":  
  On/OFF (Type: Styled Button) := V13  
  EspressoReady (Type: Led) := V14  
  TargetTemp (Type: Numeric Input) := V7  
  CurrentTemp (Type: Labeled Value) := V2  
  HeaterPower := V23  
  Temperature Error := V11  
  Temperature Change (last 10 sec) := V35  
  CurrentTemp/TargetTemp (Type: SuperChart) := V2/V3  
  HeaterPower := V23  
  Water Temp changes :=V35  

- Tab "PID Controller":  
  Interzone P := V4  
  Interzone I := V5  
  Interzone D := V6  
  Outerzone P := V30  
  Outerzone I := V31  
  Outerzone D := V32  
  SteadyPower := V41  
  SteadyPower Offset Time := V43  
  SteadyPower Offset Power := V42  
  StartTemp := V12  
  Brew Detection Temperaturdrop := V34  
  BurstShot (Type: Button) := V40  
  BurstPower := V44  

- Tab "Preinfusion":  
  Brew Time := V8  
  Preinfusion Time := V9  
  Preinfusion Pause := V10  

## Alternative way to clone blynk app dashboard (experimental)
- Login to your server on which blynk is installed
- install the programm jq (should be included in any distribution)
- Shutdown blynk Service
- Search for file in blynk/data which looks like <email>.Blynk.user. This is the config file. Make a backup of this file.
- Download this project's config [rancilio_v2.json](https://github.com/medlor/ranciliopid/blob/master/rancilio_v2.json) and put it in the same folder as the config file.
- Execute following command in this folder:
  ```
  jq --argjson blynkInfo "$(<rancilio_v2.json)" '.profile.dashBoards += [$blynkInfo]' YOUR_EMAIL.Blynk.user
  ```
- Startup blynk service
- Open App, Search newly create project, Open the "Project Settings" then devices-> Master -> Master and Press "Refresh Token". Use this token in the rancilio software as auth token.


# Tunings instructions
1. Maschine has to be cold (<40 C): Adjust coldstart step 1 and step 2 to cleanly reach setPoint without any PID controls.
   - tbd
1. After this is configured correctly, you have to determine your steadyPower value. 
   - tbd
1. After this is configured correctly, you should configure steadyPowerOffset and steadyPowerOffsetTime.
   - tbd (maschine has to be cold)
1. After this is configured correctly, you should configure the PID values for inner-zone and outer-zone.
   - tbd

# Debugging Howto
## Explanation of the PID log line
```
[0m(D p:^5000ms) 435 Input= 93.46 | error= 0.54 delta= 2.45 | Output= 27.88 = b:52.10 + k: 0.86 + i: 0.00( 0.00) + d:-25.09

```
- 435 := Time since power-on of the arduino (in seconds)
- Input= 93.46 := Current temperature
- error= 0.54  := Temperature difference calculated by (target_temp - current_temp)
- delta= 2.45  := Change in temperature in the last 10 seconds
- Output= 27.88 := Heater Power (in percent)
  which is calcucated by the sum of:
   - b: 5.10        := steadyPower (in percent)
   - k: 0.86        := PID Kp (in percent)
   - i: 0.00( 0.00) := PID KiSum (KiLast) (in percent)
   - d:-25.09       := PID Kd (in percent)

# How to use a simple LED as brewReady signal
- The easiest way is to use arduino GPIO15. For this to work you have to connect a resistor and in parallel another resistor with the led in series. This has to be connected between GPIO Pin 15 and GROUND.  
- A technical description can be found here https://www.forward.com.au/pfod/ESP8266/GPIOpins/index.html
- Required configuration in config.h:
  - #define BREW_READY_LED 1
  - #define BREW_READY_DETECTION 0.2  # or any other value

# Changelog
- 2.0.1_beta6:
  - Fix: Reducing "temperature sensor reading" errors.
  - Fix: steadyPowerOffset_Time is now correctly configurable via blynk.
  - Fix: Hardware switch sensitivity ("brewswitch") is increased from 1000 to 700.
  - Improved: mySteadyPower failure detections.
  - Code Cleanup + Refactorings.
- 2.0.1_beta5:
  - RemoteDebug inactivity time requires a version not yet in arduino library manager. Therefore a workaround is document in the meantime.
  - Pre-Infusion variables are now configurable in config.h: PREINFUSION, PREINFUSION_PAUSE, BREWTIME
  - Emergency temperature theshold is now configurable in config.h: EMERGENCY_TEMP
  - BrewDetection sensitivity is now configurable in config.h: BREWDETECTION_SENSITIVITY (renamed from brewboarder)
- 2.0.1_beta4:
  - Limit auto-tuning of steadyPower to reduce overly increased values.
  - Fix bug in brew().bezugsZeit calculation.
  - Renamed outputK to outputI.
  - Explanation of PID log line added.
- 2.0.1_beta3:
  - Pre-Infusion Values are now in seconds (previously milliseconds).  
    !! ATTENTION: Therefore you HAVE to change following values via blynk: brewtime, preinfusion, preinfusionpause !!
  - Reduce overhead of preinfusion functionality by having it run just every 50ms (and not every ms).
  - Add support for custom emergency text in display when temp >120 degree. (see EMERGENCY_TEXT in config.h.SAMPLE)
  - Emergency temperature threshold can now be set by variable emergency_temperature.
  - RemoteDebug inactivity time increased from 10min to 30min.
  - Add missing line-break in brew() debug logging.
- 2.0.1_beta2:
  - Better Installation Guide
  - Fix brew() function
  - Support virtual BrewReady LED in blynk
  - Improve pastChange() in Pid.compute(). (default Kd value has to be doubled)
  - Improve Auto-tuning.
  - Default PID Value Tunings
- 2.0.1_beta:
  - ATTENTION: EEPROM has changed. Therefore you have to connect to blynk at least once, and manually set correct settings in blynk app (see screenshots for default values).
  - New PID Controller "Multi-state PID with steadyPower (Bias)"
  - "Brew Ready" Detection implemented, which detects when the temperature has stabilized at setPoint. It can send an
    MQTT event or have hardware pin 15 triggered (which can be used to turn a LED on).
  - Because deadtime of Silvia5E is around 45seconds PID.compute() runs every 5 seconds (previous 1sec) and reduce header on/off switches by factor 5
  - Temperature polling is also now set to once every second (previous 400ms)
  - Code refactoring/cleanup and fixes of bugs.
  - BurstShot feature added to temporary overwrite PID controls (useful mainly for tests)
  - Added RemoteDebug over telnet so that we dont need USB to debug/tune pid anymore (https://github.com/JoaoLopesF/RemoteDebug)
    - Just "$ telnet rancilio_ip 23"
  - Fix: Recover EmergencyStop when temperature poll in setup() fails
  - EmergencyStop state logging improved
- 1.9.8h_alpha:
  - Feature: Implemented special 3 step cold-start mechanism. See special tuning instructions above.
- 1.9.8g:
  - Improvement: Broken temperature is detected when temp has increased more than >5 degrees (previous 25 degrees) in the last 0.4 seconds.
  - Improvement: Better brewReady detection by waiting for stable temperature within a longer time window (from 6 to 14secs).
  - TemperatureHistory increased from 6 seconds to 30seconds.
- 1.9.8f:
  - Added missing config.h.SAMPLE variables.
  - Added some more mqtt events.
- 1.9.8e:
  - Removed movAvg() due to several issues:
    - This is no moving avg, but something different (but it is working for "stable" temperature curves).
    - readIndex=0 is not used.
    - movingAvg does not compares currentValue with previousValue.
    - Faktor *100 makes no sense (?).
    - initializing methods/firstreading is refactored.
  - movAvg() is replaced by pastTemperatureChange() (+ updateTemperatureHistory())
  - Change order of brew-detection in main loop() to not confuse bPid.compute() due to flapping setTunings()
  - Feature: isBrewReady() to determine if temperature is stable to start brewing
  - Bugfix: Replace abs() with fabs().
- 1.9.8d:
- 1.9.8c:
  - Add more mqtt updates: kp,ki,kd, coldstart
  - Safe-guard: be sure pid internals are cleared when pid is dis/enabled.
- 1.9.8b:
  - Feature: Add support for MQTT.
  - Safeguard: HeaterPreventFlapping must never be > windowSize.
- 1.9.7:
  - Fix: If temperature sensor detects more than 150Celcius then it must be an error.
  - Fix: Typos / explanations.
  - Fix: Race condition when code is blocked for some time
  - Fix: Added initial temp poll to prevent error on start.
  - Feature: Detect missing temperature polls.
  - Feature: Add configurable BLYNKPORT.
  - Feature: Wifi hostname configuable.
  - New feature: Safe-guard: Stop heating forever if sensor is flapping!
  - Fix: Pid.Compute() is now in sync with isrCounter and is not loosing one tick each second. Heater flapping is reduced.

# Disclaimer
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
