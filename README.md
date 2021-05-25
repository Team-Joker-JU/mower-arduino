# mower-arduino

## Dependencies

* Makeblock package
Can be downloaded from `http://learn.makeblock.com/en/Makeblock-library-for-Arduino/`

## Component description
### Parts
* mBot
* Rasperry pi Zero
* Arduino Mega 2560

## Communication
* Raspberry pi (RPI) is activated from a python script on the raspberry. Check repository `mower-rasperry-pi` for more details
* The RPI talks with the arduino through a USB cable (Serial connection)
* The RPI is connected with the app via bluetooth
* The RPI can send REQUESTS to our backend API and send data, like coordinates

## Documentation

### Different modes on the Arduino Mega 2560
<table border=1>
  <tr>
    <th>Mode</th>
    <th>Description</th>
  </tr>
  <tr>
    <td>Manual</td>
    <td>Activated by sending some code. When activated<br>you can steer the mBot from our app. In the app<br>there is a joystick where you can steer the mBot.</td>
  </tr>
  <tr>
    <td>Auto</td>
    <td>Activated by sending some code. When activated<br> the robot will drive autonoumosly inside a marked <br> area. It will will also avoid obstacles. </td>
  </tr>
</table>

### Code schema
<table border=1>
  <tr>
    <th>Name</th>
    <th>Code</th>
  </tr>
  <tr>
    <td>CONNECTED</td>
    <td>0</td>
  </tr>
  <tr>
    <td>DISCONNECTED</td>
    <td>1</td>
  </tr>
  <tr>
    <td>ACCELERATION</td>
    <td>2</td>
  </tr>
  <tr>
    <td>STEERING</td>
    <td>3</td>
  </tr>
    <tr>
    <td>COLLISION</td>
    <td>4</td>
  </tr>
  <tr>
    <td>MODE</td>
    <td>5</td>
  </tr>
</table>

