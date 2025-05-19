# Web Server Rover

   A rover controlled by a local web server.

## Description

Web Server rover that replaces the esp12 with esp32 leveraging UART to communicate with Cytron Maker Pi robot controller leveraging arduino IDE. 

### Hardware

* Xiao Esp32S3 Sense
* Cytron Maker Pi RP20240
* 2 dc motors and wheels
* 2 IR proximity sensors
* 3 AA batteries
* Wooden Frame

  
### Dependencies

#### Xiao
* ArduinoJson
#### Maker Pi
* hardware/uart


### Executing program

* Upload TetraWebSrv.ino to the Xiao then navigate to Ip address of ESP local webserver printed in the serial console.
  <img src='images/XiaoSerial.PNG' style=" width:252px;height:336px;" >
* Upload TetraWebsrvControl.ino to the Cytron Maker Pi the Local web server listens and communicates gamepad or mouse events to the Cytron Maker Pi, providing more precise control of the rover's motor system. 
  
<div class= 'flex-col'>
  <img src='images/controls.jpeg' style=" width:250px;height:445px;" >
   <img src='images/controller.jpeg' style=" width:336px;height:252px;" >
  <img src='images/XiaoRover.jpeg' style=" width:252px;height:336px;" >
</div>
  
## Author

Samuel Kalu
  
* email : [samkalu@ttu.edu](mailto:samkalu@ttu.edu)
* linkedin : [@SamuelKalu](https://www.linkedin.com/in/samuel-kalu-74a359342/)


## Acknowledgments

Inspiration, code snippets, etc.
* [Webots](https://cyberbotics.com/doc/guide/tutorial-4-more-about-controllers?tab-language=c++)
