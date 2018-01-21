# Enhanced SD Web Server (ESP-12E)
An Enhanced SD Card Web Server running on an ESP-12E..

- [Enhanced SD Web Server (ESP-12E)](#enhanced-sd-web-server-esp-12e)
    - [Planned Features (And Progress)](#planned-features-and-progress)
    - [About](#about)
        - [Content Type Support](#content-type-support)
            - [Supported MIME Types](#supported-mime-types)
            - [Compressed Pages](#compressed-pages)
        - [Connecting/Running the Web Server](#connectingrunning-the-web-server)
            - [AP Mode](#ap-mode)
            - [STA Mode](#sta-mode)
        - [SD File System Interface](#sd-file-system-interface)
            - [Create Files/Directories](#create-filesdirectories)
            - [Upload Files/Directories](#upload-filesdirectories)
            - [Edit Files/Directories](#edit-filesdirectories)
            - [Delete Files/Directories](#delete-filesdirectories)
            - [Download Files/Directories](#download-filesdirectories)
        - [Generated Pages](#generated-pages)
            - [Index Retrieval](#index-retrieval)
            - [Directory Retrieval](#directory-retrieval)
            - [404 Page](#404-page)
            - [Request Log](#request-log)
        - [Per-Page Authentication](#per-page-authentication)
        - [Web Update / OTA](#web-update-ota)
        - [Server Side Scripting](#server-side-scripting)
            - [Server Side Includes](#server-side-includes)
            - [Server Side Scripts](#server-side-scripts)
        - [*Web Sockets?*](#web-sockets)
        - [*FTP Support?*](#ftp-support)
        - [Configuration File](#configuration-file)
        - [Recommended SD Card File Structure](#recommended-sd-card-file-structure)
    - [Usefull Links and References](#usefull-links-and-references)

## Planned Features (And Progress)
- [ ] Server Basis
    - [ ] WiFi AP or Station (Automatic Switch)
    - [ ] mDNS Hostname
    - [ ] Asynchronous Requests
- [ ] Expand Server
    - [ ] Interface
        - [ ] Web SD File System Interface
            - [ ] Create Files/Directories
            - [ ] Upload Files/Directories
            - [ ] Edit Files/Directories
            - [ ] Delete Files/Directories
            - [ ] Download Files/Directories
        - [ ] Generated Pages
            - [ ] 404 Page
            - [ ] Index Retrieval
            - [ ] Directory Retreival
            - [ ] Request Logging
    - [ ] Software
        - [ ] Read Config. File
        - [ ] Many Content Types
            - [ ] Serve Compressed(.gz) files
        - [ ] Per-Page Authentication Support
- [ ] Finalize Server
    - [ ] Web Update or OTA
    - [ ] Server Side Scripting (CGI)
        - [ ] Run scripts
        - [ ] Server Side Includes (SSI)
        - [ ] Dynamic Web Pages
    - *Web Sockets?*
    - *FTP Support?*

## About

### Content Type Support
#### Supported MIME Types
#### Compressed Pages

### Connecting/Running the Web Server
#### AP Mode
#### STA Mode

### SD File System Interface
#### Create Files/Directories
#### Upload Files/Directories
#### Edit Files/Directories
#### Delete Files/Directories
#### Download Files/Directories

### Generated Pages
#### Index Retrieval
#### Directory Retrieval
#### 404 Page
#### Request Log

### Per-Page Authentication

### Web Update / OTA

### Server Side Scripting
#### Server Side Includes
#### Server Side Scripts

### *Web Sockets?*
### *FTP Support?*
[Video](https://www.youtube.com/watch?v=SnCIYrGF4s8)

### Configuration File

### Recommended SD Card File Structure


## Usefull Links and References
- Similliar Projects
    - [SD Web Server Example](https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WebServer/examples/SDWebServer/SDWebServer.ino) (Basis For This Project)
        - [Video](https://www.youtube.com/watch?v=zJP3Ie3nE7c)
    - [ESP-HTTPD](http://www.esp8266.com/viewtopic.php?f=6&t=376) (Lua ESP Web Server)
    - [SSI Server](https://hackaday.io/project/28626-budget-wi-fi-nest-box-camera/log/71813-esp8266-sd-card-webserver-with-server-side-scripting) (**Incomplete**)
- Web Server Info
    - [Jibble Web Server](http://www.jibble.org/jibblewebserver.php) (Java Web Server)
- ESP Info
    - Misc
        - [Using an SD Card (Hardware)](http://doityourselfchristmas.com/forums/showthread.php?43345-Using-SD-cards-with-ESP8266-s)
        - [Using SPIFFS](http://www.esp8266.com/viewtopic.php?f=29&t=8194)
        - [Encoding/Decoding JSON](https://randomnerdtutorials.com/decoding-and-encoding-json-with-arduino-or-esp8266/)
    - Pinouts
        - [ESP-12 Pinout](https://esp8266.github.io/Arduino/versions/2.0.0/doc/esp12.png)
        - [ESP-01 Pinout](https://os.mbed.com/media/uploads/sschocke/esp8266-pinout_etch_copper_top.png)
    - Programming
        - [ESP-01](https://os.mbed.com/users/sschocke/code/WiFiLamp/wiki/Updating-ESP8266-Firmware)
    - General Guides
        - [Sparkfun Guide](https://learn.sparkfun.com/tutorials/esp8266-thing-hookup-guide/using-the-arduino-addon)
        - [Beginner's Guide](https://github.com/tttapa/ESP8266)
            - [Introduction](https://tttapa.github.io/ESP8266/Chap01%20-%20ESP8266.html)
            - [Hardware](https://tttapa.github.io/ESP8266/Chap02%20-%20Hardware.html)
            - [Software](https://tttapa.github.io/ESP8266/Chap03%20-%20Software.html)
            - [Microcontroller](https://tttapa.github.io/ESP8266/Chap04%20-%20Microcontroller.html)
            - [Network Protocols](https://tttapa.github.io/ESP8266/Chap05%20-%20Network%20Protocols.html)
            - [Uploading](https://tttapa.github.io/ESP8266/Chap06%20-%20Uploading.html)
            - [Wi-Fi connections](https://tttapa.github.io/ESP8266/Chap07%20-%20Wi-Fi%20Connections.html)
            - [mDNS](https://tttapa.github.io/ESP8266/Chap08%20-%20mDNS.html)
            - [Web Servers](https://tttapa.github.io/ESP8266/Chap09%20-%20Web%20Server.html)
            - [Simple Web Server](https://tttapa.github.io/ESP8266/Chap10%20-%20Simple%20Web%20Server.html)
            - [SPIFFS](https://tttapa.github.io/ESP8266/Chap11%20-%20SPIFFS.html)
            - [Uploading to the Server](https://tttapa.github.io/ESP8266/Chap12%20-%20Uploading%20to%20Server.html)
            - [OTA](https://tttapa.github.io/ESP8266/Chap13%20-%20OTA.html)
            - [WebSocket](https://tttapa.github.io/ESP8266/Chap14%20-%20WebSocket.html)
            - [NTP](https://tttapa.github.io/ESP8266/Chap15%20-%20NTP.html)
            - [Data Logging](https://tttapa.github.io/ESP8266/Chap16%20-%20Data%20Logging.html)
            - [Email Notifier](https://tttapa.github.io/ESP8266/Chap17%20-%20Email%20Notifier.html)
            - [Advanced](https://tttapa.github.io/ESP8266/Chap18%20-%20Advanced.html)
            - [Conclusion](https://tttapa.github.io/ESP8266/Chap19%20-%20In%20Conclusion.html)