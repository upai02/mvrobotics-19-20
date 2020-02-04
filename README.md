# mvrobotics-19-20
VEX code for 2019-2020 robot (Tower Takeover - 8995V)

Includes normal competition template code and auton recorder/playback, both with custom created UI

## Changelog 
* **1/4/2020 (post-Huntley)**
  - First upload to repository
  - Includes drive code, auton (CompTemp), recorder (RecUI), playback (RecUI), and custom brain UI along with experimental GIF integration
  - Recorder (RecUI) is nonfunctional (saving to SD doesn't work)
  - Auton code (CompTemp) is mostly experimental stuff (tested somewhat at Huntley)
  - GIF integration classes do not work properly/at all
  - Potentiometer P-loop is commented out
  
* **1/7/2020**
   - Added inertial sensor support
   - Added turning PID
   - Added custom PID class (https://github.com/tekdemo/MiniPID/blob/master/README.md)
   
* **1/11/2020**
   - Updated drivetrain to smartdrive with inertial integration
   - Synced changes between the programs
   - Added new controller UI (https://dev.azure.com/roboVEX/_git/ACCESS_OS)
   - Added all functions in from base programs (AccessOS)
   
* **1/12/2020**
   - Tweaked stacking P-loop
   - Rewired ports (CompTemp)
   
* **1/22/2020 (post-Neuqua)**
   - Functional back auton added
   - Stacking P-loop fixed and tuned
   - Turning PID fixed and tuned
   - Synced changes between programs
   - Flipout fixed

* **1/31/2020 (post-NEIU)**
   - Experimental 7 point back auton added
   - Experimental front auton modified
   - Experimental drive speed selector added
   - Experimental bar macro added

* **2/4/2020**
   - Added newer turn PID
   - Added newer recorder and playback code (RecUI)
   - Added hybrid drive code and working drive speed selector
   - Synced changes between CompTemp and RecUI

## To-Do
* Test and fix auton code (CompTemp)
* Tweak bar macro
* Tweak autostack P-loop
* Tweak fadeback code
* Figure out exponentially scaling drive
* Sync changes between programs
* Figure out better file system for easier navigation (CompTemp and RecUI)
* Fix controller UI (AccessOS)
* Merge the different turn functions into one with PID (AccessOS)
* Fix saving issue (RecUI)
* Add PID loop for drivetrain
* Merge brain GUI and controller UI into 1 cohesive GUI (if possible)
* Fix/remove GIF integration

