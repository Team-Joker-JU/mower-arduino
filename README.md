# mower-arduino

## Dependencies
### python library
`serial`
```
pip install serial
```
### Makeblock package
Can be downloaded from `http://learn.makeblock.com/en/Makeblock-library-for-Arduino/`

## Component description
### Parts
* mBot
* Rasperry pi Zero
* Arduino Mega 2560

## Communication
* Connect to `Raspberry pi` from an app via Bluetooth
* `Raspberry pi` controls arduino through serial communication
* `Raspberry pi` can send GET and POST requests to an `API` that is saved to a database
* `mBot` is controlled by `Arduino Mega 2560`

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
    <td>HANDSHAKE</td>
    <td>0</td>
  </tr>
  <tr>
    <td>CONNECTED</td>
    <td>1</td>
  </tr>
  <tr>
    <td>RECEIVED</td>
    <td>2</td>
  </tr>
  <tr>
    <td>ACCELERATION</td>
    <td>3</td>
  </tr>
  <tr>
    <td>STEERING</td>
    <td>4</td>
  </tr>
  <tr>
    <td>COLLISION</td>
    <td>5</td>
  </tr>
</table>

