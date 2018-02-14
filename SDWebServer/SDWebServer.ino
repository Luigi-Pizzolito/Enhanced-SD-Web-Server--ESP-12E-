/*
  SDWebServer - Example WebServer with SD Card backend for esp8266

  Copyright (c) 2015 Hristo Gochkov. All rights reserved.
  This file is part of the ESP8266WebServer library for Arduino environment.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Have a FAT Formatted SD Card connected to the SPI port of the ESP8266
  The web root is the SD Card root folder
  File extensions with more than 3 charecters are not supported by the SD Library
  File Names longer than 8 charecters will be truncated by the SD library, so keep filenames shorter
  index.htm is the default index (works on subfolders as well)

  upload the contents of SdRoot to the root of the SDcard and access the editor by going to http://esp8266sd.local/edit

*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <SPI.h>
#include <SD.h>
#include <ArduinoJson.h>

#define DBG_OUTPUT_PORT Serial

const char* ssid;
const char* password;
const char* host;
String ssids;
String passwords;
String hosts;

ESP8266WebServer server(80);

static bool hasSD = false;
File uploadFile;

String logfileTemp;

void returnOK() {
  server.send(200, "text/plain", "");
}

void returnFail(String msg) {
  server.send(500, "text/plain", msg + "\r\n");
  logadd("Fail: ", false);
  logadd(msg, true);
  logcommit();
}

bool loadFromSdCard(String path){
  bool directoryList = false;
  String dataType = "text/plain";

  if(path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
  String pathl = path;
  pathl.toLowerCase();
  //text types
  if(pathl.endsWith(".htm")) dataType = "text/html";
  else if(pathl.endsWith(".css")) dataType = "text/css";
  else if(pathl.endsWith(".xml")) dataType = "text/xml";
  else if(pathl.endsWith(".csv")) dataType = "text/csv";
  else if(pathl.endsWith(".js")) dataType = "text/javascript";
  else if(pathl.endsWith(".json") || path.endsWith(".jsn")) dataType = "text/json";
  //image types
  else if(pathl.endsWith(".png")) dataType = "image/png";
  else if(pathl.endsWith(".jpg") || path.endsWith(".jpeg")) dataType = "image/jpg";
  else if(pathl.endsWith(".gif")) dataType = "image/gif";
  else if(pathl.endsWith(".bmp")) dataType = "image/bmp";
  else if(pathl.endsWith(".svg")) dataType = "image/svg+xml";
  else if(pathl.endsWith(".ico")) dataType = "image/x-icon";
  //audio types
  else if(pathl.endsWith(".mid")) dataType = "audio/midi";
  else if(pathl.endsWith(".mp3")) dataType = "audio/mpeg";
  else if(pathl.endsWith(".ogg")) dataType = "audio/ogg";
  else if(pathl.endsWith(".wav")) dataType = "audio/wav";
  //video types
  else if(pathl.endsWith(".mp4")) dataType = "video/mp4";
  //binary data types
  else if(pathl.endsWith(".pdf")) dataType = "application/pdf";
  else if(pathl.endsWith(".zip")) dataType = "application/zip";
  else if(pathl.endsWith(".gz")) {                               //serve compressed versions too!
    //text types
    if(pathl.startsWith("/gz/htm")) dataType = "text/html";
    else if(pathl.startsWith("/gz/css")) dataType = "text/css";
    else if(pathl.startsWith("/gz/xml")) dataType = "text/xml";
    else if(pathl.startsWith("/gz/csv")) dataType = "text/csv";
    else if(pathl.startsWith("/gz/js")) dataType = "text/javascript";
    else if(pathl.startsWith("/gz/jsn")) dataType = "text/json";
    //image types
    else if(pathl.startsWith("/gz/png")) dataType = "image/png";
    else if(pathl.startsWith("/gz/jpg")) dataType = "image/jpg";
    else if(pathl.startsWith("/gz/gif")) dataType = "image/gif";
    else if(pathl.startsWith("/gz/bmp")) dataType = "image/bmp";
    else if(pathl.startsWith("/gz/svg")) dataType = "image/svg+xml";
    else if(pathl.startsWith("/gz/ico")) dataType = "image/x-icon";
    //audio types
    else if(pathl.startsWith("/gz/mid")) dataType = "audio/midi";
    else if(pathl.startsWith("/gz/mp3")) dataType = "audio/mpeg";
    else if(pathl.startsWith("/gz/ogg")) dataType = "audio/ogg";
    else if(pathl.startsWith("/gz/wav")) dataType = "audio/wav";
    //video types
    else if(pathl.startsWith("/gz/mp4")) dataType = "video/mp4";
    //binary data types
    else if(pathl.startsWith("/gz/pdf")) dataType = "application/pdf";
    else dataType = "application/x-gzip";
  }

  File dataFile = SD.open(path.c_str());
  if(dataFile.isDirectory()){ //we are serving a directory
    if (server.argName(0) == "listing" && server.arg(0) == "true") {
      DBG_OUTPUT_PORT.println("debug args");
    }
    if ((SD.exists(path+"index.htm") || SD.exists(path+"INDEX.HTM")) && !(server.argName(0) == "listing" && server.arg(0) == "true")) { //that directory has a index.htm, serve it
      //open index
      path += "/index.htm";
      dataType = "text/html";
      dataFile = SD.open(path.c_str());
    } else {                            //that directory has no index.htm, show a dir listing
      //list directory
      directoryList = true;
      server.send(200, "text/html", printDirectoryHTML(dataFile, path.c_str()));
      dataFile.close();
      logadd("Load DirList: 200 ", false);
      logadd(path, true);
      logcommit();
    }
  }
  if (!directoryList) {
    if (!dataFile)
      return false;
  
    if (server.hasArg("download")) dataType = "application/octet-stream";
  
    if (server.streamFile(dataFile, dataType) != dataFile.size()) {
      logadd("Load: Sent less data than expected!", true);
    }
  
    dataFile.close();
    logadd("Load: 200 ", false);
    logadd(dataType, false);
    logadd(" ", false);
    logadd(path, true);
    logcommit();
  }
  return true;
}

void handleFileUpload(){
  HTTPUpload& upload = server.upload();
  if(server.uri() != "/edit") return;
  if(upload.status == UPLOAD_FILE_START){
    if(SD.exists((char *)upload.filename.c_str())) SD.remove((char *)upload.filename.c_str());
    uploadFile = SD.open(upload.filename.c_str(), FILE_WRITE);
    DBG_OUTPUT_PORT.print("Upload: START, filename: "); DBG_OUTPUT_PORT.println(upload.filename);
  } else if(upload.status == UPLOAD_FILE_WRITE){
    if(uploadFile) uploadFile.write(upload.buf, upload.currentSize);
    DBG_OUTPUT_PORT.print("Upload: WRITE, Bytes: "); DBG_OUTPUT_PORT.println(String(upload.currentSize));
  } else if(upload.status == UPLOAD_FILE_END){
    if(uploadFile) uploadFile.close();
    DBG_OUTPUT_PORT.print("Upload: END, Size: "); DBG_OUTPUT_PORT.println(String(upload.totalSize));
    logadd("Upload: ", false);
    logadd((char *)upload.filename.c_str(), true);
    logcommit();
  }
}

void deleteRecursive(String path){
  File file = SD.open((char *)path.c_str());
  if(!file.isDirectory()){
    file.close();
    SD.remove((char *)path.c_str());
    return;
  }

  file.rewindDirectory();
  while(true) {
    File entry = file.openNextFile();
    if (!entry) break;
    String entryPath = path + "/" +entry.name();
    if(entry.isDirectory()){
      entry.close();
      deleteRecursive(entryPath);
    } else {
      entry.close();
      SD.remove((char *)entryPath.c_str());
    }
    yield();
  }

  SD.rmdir((char *)path.c_str());
  file.close();
}

void handleDelete(){
  if(server.args() == 0) return returnFail("BAD ARGS");
  String path = server.arg(0);
  if(path == "/" || !SD.exists((char *)path.c_str())) {
    returnFail("BAD PATH");
    return;
  }
  deleteRecursive(path);
  returnOK();
  logadd("Delete: ", false);
  logadd(path, true);
  logcommit();
}

void handleCreate(){
  if(server.args() == 0) return returnFail("BAD ARGS");
  String path = server.arg(0);
  if(path == "/" || SD.exists((char *)path.c_str())) {
    returnFail("BAD PATH");
    return;
  }

  if(path.indexOf('.') > 0){
    File file = SD.open((char *)path.c_str(), FILE_WRITE);
    if(file){
      file.write((const char *)0);
      file.close();
    }
  } else {
    SD.mkdir((char *)path.c_str());
  }
  returnOK();
  logadd("Create: ", false);
  logadd(path, true);
  logcommit();
}

void printDirectory() {
  if(!server.hasArg("dir")) return returnFail("BAD ARGS");
  String path = server.arg("dir");
  if(path != "/" && !SD.exists((char *)path.c_str())) return returnFail("BAD PATH");
  File dir = SD.open((char *)path.c_str());
  path = String();
  if(!dir.isDirectory()){
    dir.close();
    return returnFail("NOT DIR");
  }
  dir.rewindDirectory();
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/json", "");
  WiFiClient client = server.client();
  String output;
  server.sendContent("[");
  for (int cnt = 0; true; ++cnt) {
    File entry = dir.openNextFile();
    if (!entry)
    break;

    
    if (cnt > 0)
      output = ',';

    output += "{\"type\":\"";
    output += (entry.isDirectory()) ? "dir" : "file";
    output += "\",\"name\":\"";
    output += entry.name();
    output += "\"";
    output += "}";
    server.sendContent(output);
    entry.close();
 }
 server.sendContent("]");
 dir.close();
 logadd("Print Directory: [", false);
 logadd(output, false);
 logadd("]", true);
 logcommit();
}

void handleNotFound(){
  if(hasSD && loadFromSdCard(server.uri())) return;
  String message = "404 Error\n";
  message += (!hasSD) ? "SDCARD Not Detected\n\n" : "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " NAME:"+server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  logadd("Load: 404 ", false);
  logadd(server.uri(), false);
  logadd(" Method: ", false);
  logadd((server.method() == HTTP_GET)?"GET":"POST", false);
  logadd(" Arguments: ", false);
  logadd(String(server.args()), false);
  logadd(" -> [", false);
  for (uint8_t i=0; i<(server.args()-1); i++){
    logadd("{ NAME:"+server.argName(i) + ", VALUE:" + server.arg(i) + "}, ", false);
  }
  if (server.args() >= 1) {logadd("{ NAME:"+server.argName(server.args()) + ", VALUE:" + server.arg(server.args()) + "}", false);}
  logadd("]", true);
  logcommit();
}

void setup(void){
  DBG_OUTPUT_PORT.begin(115200);
  //DBG_OUTPUT_PORT.setDebugOutput(true);
  DBG_OUTPUT_PORT.print("\n");

  if (SD.begin(SS)){
     logadd("SD Card initialized.", true);
     hasSD = true;
     clearLog();
  } else {
    logadd("SD Card failed.", true);
    logadd("", true);
    logcommit();
    while(1) delay(500);
  }
  if (!loadConfig()) {
    logadd("Load or Parse Config.jsn failed.", true);
    logadd("", true);
    logcommit();
    while(1) delay(500);
  } else {
    logadd("Config file read and parsed.", true);
  }
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  logadd("Connecting to ", false);
  logadd(ssids, true);

  // Wait for connection
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) {//wait 10 seconds
    delay(500);
  }
  if(i == 21){
    logadd("Could not connect to", false);
    logadd(ssids, true);
    while(1) delay(500);
  }
  logadd("Connected! IP address: ", false);
  logadd(WiFi.localIP().toString(), true);
  /*
  ArduinoOTA.setPort(8266);
  ArduinoOTA.setHostname(host);
  ArduinoOTA.setPassword("admin");
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  */
  if (MDNS.begin((const char *)hosts.c_str())) {
    MDNS.addService("http", "tcp", 80);
    logadd("MDNS responder started", true);
    logadd("You can now connect to http://", false);
    logadd(hosts, false);
    logadd(".local", true);
  }


  server.on("/list", HTTP_GET, printDirectory);
  server.on("/edit", HTTP_DELETE, handleDelete);
  server.on("/edit", HTTP_PUT, handleCreate);
  server.on("/edit", HTTP_POST, [](){ returnOK(); }, handleFileUpload);
  server.onNotFound(handleNotFound);

  server.begin();
  logadd("HTTP server started", true);

  logadd("", true);
  logcommit();
}

void loop(void){
  server.handleClient();
  //ArduinoOTA.handle();
}

//low-level universal functions

//log functions
void logadd(String msg, bool newline) {
  msg = String(msg);
  logfileTemp += msg;
  if (newline) {
    logfileTemp += "\n";
    DBG_OUTPUT_PORT.println(msg);
  } else {
    DBG_OUTPUT_PORT.print(msg);
  }
}
void logcommit() {
  File logfile = SD.open("server.log", FILE_WRITE);
  if (logfile) {
    logfile.print(logfileTemp);
    logfile.close();
    //DBG_OUTPUT_PORT.println("Log: saved");
    logfileTemp = "";
  } else {
    DBG_OUTPUT_PORT.println("Log: file does not exist!");
    while(1) delay(500);
  }
}
void clearLog() {
  SD.remove("server.log");
  logadd("Log: cleared log", true);
  logcommit();
}

//sd listing functions
String printDirectoryHTML(File dir, String path) {
   if (!path.endsWith("/")) path += "/";
   String message = "<h2>Directory Listing: ";
   message += path;
   message += "</h2><table><tr><td><i>Filename</i></td><td><i>File Size</i></td></tr>";
   if (!(path == "/")) {message += "<tr><td><a href='../'>../</a></td></tr>";}
   message += "<title>Directory Listing: ";
   message += path;
   message += "</title>";
   while(true) {
     File entry =  dir.openNextFile();
     if (! entry) { //no more files
       break;
     }
     if (entry.isDirectory()) {
       message += "<tr><td><b><a href='";
       message += path;
       //message += "/";
       message += entry.name();
       message += "/'>";
       message += entry.name();
       message += "</a></b></td></tr>";
     } else {
       // files have sizes, directories do not
       message += "<tr><td><a href='";
       message += path;
       //message += "/";
       message += entry.name();
       message += "'>";
       message += entry.name();
       message += "</a></td><td><p>";
       message += entry.size();
       message += "B</p></td></tr>";
     }
     entry.close();
   }
   message += "</table><style>td {padding-top:0px;}</style>";
   return message;
  }

//configuration file
bool loadConfig() {
  StaticJsonBuffer<200> jsonBuffer;
  String json;
  File configFile = SD.open("config.jsn");
  if (configFile) {
    while (configFile.available()) {
      char c = configFile.read();
      if (c==';'){
        
        JsonObject& root = jsonBuffer.parseObject(json);
        if (!root.success()) return false;
        
        ssid = root["wifi"]["ssid"];
        password = root["wifi"]["pass"];
        host = root["wifi"]["host"];
        ssids = (const char *)root["wifi"]["ssid"];
        //passwords = root["wifi"]["pass"];
        hosts = (const char *)root["wifi"]["host"];
      }
      json.concat(c);
    }
    configFile.close();
    return true;
  } else {
    return false;
  }
}
