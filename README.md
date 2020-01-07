# mvrobotics-19-20
VEX code for 2019-2020 robot (Tower Takeover - 8995V)

Includes normal competition template code and auton recorder/playback, both with custom created UI

## Changelog 
* **1/4/2020 (post-Huntley)**
  - First upload to repository
  - Includes drive code, auton (CompTemp), recorder (RecUI), playback (RecUI), and custom brain UI
  - Recorder (RecUI) is nonfunctional (saving to SD doesn't work)
  - Auton code (CompTemp) is mostly experimental stuff (tested somewhat at Huntley)
  - Potentiometer P-loop is commented out
  
* **1/7/2020**
   - Added inertial sensor support
   - Added turning PID
   - Added custom PID class (https://github.com/tekdemo/MiniPID/blob/master/README.md)

## To-Do
* ~~Add inertial sensor support (turning PID, drive PID)~~
* Tweak potentiometer stacking P-loop
* Test and fix auton code (CompTemp)
* Fix saving issue (RecUI)
* Add/fix drive speed selector
* Add PID loop for drivetrain
