#include "srvr.h"
#include "sd_log.h"

#include <WiFi.h>
#include <ESP32WebServer.h> //https://github.com/Pedroalbuquerque/ESP32WebServer download and place in your Libraries folder
#include <ESPmDNS.h>
#include "CSS.h" //Includes headers of the web and de style file
#include "FS.h"
#include "SD_MMC.h"

void srvr_init(void);
void SD_dir(void);
void File_Upload(void);
void handleFileUpload(void);
void printDirectory(const char *dirname, uint8_t levels);
void SD_file_download(String filename);
void SD_file_download_all_csv(bool delete_after_download);
void SD_file_delete(String filename);
void delete_log_csv_files(void);
void delete_zero_byte_csv_files(void);
void SendHTML_Header(void);
void SendHTML_Content(void);
void SendHTML_Stop(void);
void ReportSDNotPresent(void);
void ReportFileNotPresent(String target);
void ReportCouldNotCreateFile(String target);
String file_size(int bytes);

void ReportFileDeleted(const String &message)
{
  Serial.println(message);
}

// server stuff
ESP32WebServer server(80);
char buf_ssid[64];
char buf_pass[64];

void srvr_cred(void)
{
  char fname[32];
  int idx;
  char c;
  sprintf((char *)(&fname[0]), "/logs/x%08x.csv", 0);
  File credentialsfile = SD_MMC.open(fname);

  if (credentialsfile.available())
  {
    // Serial.printf( "reading credentials:");
    c = '-';
    // 1st arg
    idx = 0;
    while (credentialsfile.available() && idx != -1)
    {
      c = credentialsfile.read();
      if (c == ',')
      {
        idx = -1;
      }
      else
      {
        buf_ssid[idx++] = c;
      }
    }
    idx = 0;
    while (credentialsfile.available() && idx != -1)
    {
      c = credentialsfile.read();
      if (c == '\r' || c == '\n')
      {
        idx = -1;
      }
      else
      {
        buf_pass[idx++] = c;
      }
    }
    credentialsfile.close();
    // Serial.printf( "%s:%s:\n", (const char*)&buf_ssid[0], (const char*)&buf_pass[0]);
  }
}

void srvr_init(void)
{
  static char dly = 10;
  sd_log.srvr_active = 0;
  srvr_cred();

  if (buf_ssid[0] != NULL && buf_pass[0] != NULL)
  {
    Serial.println("---srvr-init.");
  }

  WiFi.disconnect();
  // WiFi.begin((const char *)&buf_ssid[0], (const char *)&buf_pass[0]);
  // WiFi.mode(WIFI_STA);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("flatball-ahrs", "");
  IPAddress apIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(apIP);
  /*
  while ((WiFi.status() != WL_CONNECTED) && (dly > 0))
  {
    delay(500);
    dly--;
    Serial.print(".");
  }
  Serial.println();
  if (WiFi.status() == WL_CONNECTED)
  */
  if (true) // Always true for AP mode
  {
    // Serial.println("");
    // Serial.print("Connected to ");
    // Serial.println((const char *)&buf_ssid[0]);
    // Serial.print("IP address: ");
    // Serial.println(WiFi.localIP());

    // Set your preferred server name, if you use "mcserver" the address would be http://mcserver.local/
    if (!MDNS.begin("flatball_ahrs"))
    {
      Serial.println(F("Error setting up MDNS responder!"));
    }

    /*********  Server Commands  **********/
    server.on("/", SD_dir);
    server.on("/get_all", []()
              { SD_file_download_all_csv(0); });
    server.on("/purge", delete_log_csv_files);
    server.on("/cleanup", delete_zero_byte_csv_files);
    server.on("/upload", File_Upload);
    server.on("/fupload", HTTP_POST, []()
              { server.send(200); }, handleFileUpload);

    server.begin();
    Serial.println("HTTP server started");
    sd_log.srvr_active = 1;
  }
  else
  {
    Serial.println("");
    Serial.print("Connection to ");
    Serial.print((const char *)&buf_ssid[0]);
    Serial.print(" : ");
    Serial.print((const char *)&buf_pass[0]);
    Serial.println(" - failed");
    delay(1000);
    ESP.restart();
  }
}

void srvr_step(void)
{
  if (!sd_log.logging && sd_log.srvr_active)
  {
    // if (WiFi.status() == WL_CONNECTED)
    //{
    server.handleClient(); // Listen for client connections
    //}
  }
}

void srvr_stop(void)
{
  if (sd_log.srvr_active)
  {
    server.stop();
    server.close();
    WiFi.disconnect(); // Disconnects from the current network
    sd_log.srvr_active = 0;
  }
}

// sd functions
// Initial page of the server web, list directory and give you the chance of deleting and uploading
void SD_dir()
{
  if (!sd_log.logging)
  {
    // Action acording to post, dowload or delete, by MC 2022
    if (server.args() > 0) // Arguments were received, ignored if there are not arguments
    {
      Serial.println(server.arg(0));

      String Order = server.arg(0);
      Serial.println(Order);

      if (Order.indexOf("download_") >= 0)
      {
        Order.remove(0, 9);
        SD_file_download(Order);
        Serial.println(Order);
      }

      if ((server.arg(0)).indexOf("delete_") >= 0)
      {
        Order.remove(0, 7);
        SD_file_delete(Order);
        Serial.println(Order);
      }
    }

    File root = SD_MMC.open("/logs");
    if (root)
    {
      root.rewindDirectory();
      SendHTML_Header();
      webpage += F("<table align='center'>");
      webpage += F("<tr><th>Name/Type</th><th style='width:20%'>Type File/Dir</th><th>File Size</th></tr>");
      printDirectory("/logs", 0);
      webpage += F("</table>");
      SendHTML_Content();
      root.close();
    }
    else
    {
      SendHTML_Header();
      webpage += F("<h3>No Files Found</h3>");
    }
    append_page_footer();
    SendHTML_Content();
    SendHTML_Stop(); // Stop is needed because no content length was sent
  }
  else
    ReportSDNotPresent();
}

// Upload a file to the SD
void File_Upload()
{
  append_page_header();
  webpage += F("<h3>Select File to Upload</h3>");
  webpage += F("<FORM action='/fupload' method='post' enctype='multipart/form-data'>");
  webpage += F("<input class='buttons' style='width:25%' type='file' name='fupload' id = 'fupload' value=''>");
  webpage += F("<button class='buttons' style='width:10%' type='submit'>Upload File</button><br><br>");
  webpage += F("<a href='/'>[Back]</a><br><br>");
  append_page_footer();
  server.send(200, "text/html", webpage);
}

// Prints the directory, it is called in void SD_dir()
void printDirectory(const char *dirname, uint8_t levels)
{

  File root = SD_MMC.open(dirname);

  if (!root)
  {
    return;
  }
  if (!root.isDirectory())
  {
    return;
  }
  File file = root.openNextFile();

  int i = 0;
  while (file)
  {
    if (webpage.length() > 1000)
    {
      SendHTML_Content();
    }
    if (file.isDirectory())
    {
      webpage += "<tr><td>" + String(file.isDirectory() ? "Dir" : "File") + "</td><td>" + String(file.name()) + "</td><td></td></tr>";
      printDirectory(file.name(), levels - 1);
    }
    else
    {
      webpage += "<tr><td>" + String(file.name()) + "</td>";
      webpage += "<td>" + String(file.isDirectory() ? "Dir" : "File") + "</td>";
      int bytes = file.size();
      String fsize = "";
      if (bytes < 1024)
        fsize = String(bytes) + " B";
      else if (bytes < (1024 * 1024))
        fsize = String(bytes / 1024.0, 3) + " KB";
      else if (bytes < (1024 * 1024 * 1024))
        fsize = String(bytes / 1024.0 / 1024.0, 3) + " MB";
      else
        fsize = String(bytes / 1024.0 / 1024.0 / 1024.0, 3) + " GB";
      webpage += "<td>" + fsize + "</td>";
      webpage += "<td>";
      webpage += F("<FORM action='/' method='post'>");
      webpage += F("<button type='submit' name='download'");
      webpage += F("' value='");
      webpage += "download_" + String(file.name());
      webpage += F("'>Download</button>");
      webpage += "</td>";
      webpage += "<td>";
      webpage += F("<FORM action='/' method='post'>");
      webpage += F("<button type='submit' name='delete'");
      webpage += F("' value='");
      webpage += "delete_" + String(file.name());
      webpage += F("'>Delete</button>");
      webpage += "</td>";
      webpage += "</tr>";
    }
    file = root.openNextFile();
    i++;
  }
  file.close();
}

void SD_file_download_all_csv(bool delete_after_download)
{
  if (!sd_log.logging)
  {
    File root = SD_MMC.open("/logs");
    if (root)
    {
      File file = root.openNextFile();
      while (file)
      {
        String filename = file.name();
        if (filename == "x00000000.csv")
        {
          file = root.openNextFile();
          continue; // Skip this specific file
        }

        if (filename.endsWith(".csv"))
        {
          if (file.size() == 0)
          {
            file.close();
            SD_MMC.remove("/logs/" + filename);
            // ReportFileDeleted("0-byte file: " + filename);
          }
          else
          {
            File download = SD_MMC.open("/logs/" + filename);
            if (download)
            {
              server.sendHeader("Content-Type", "text/text");
              server.sendHeader("Content-Disposition", "attachment; filename=" + filename);
              server.sendHeader("Connection", "close");
              server.streamFile(download, "application/octet-stream");
              download.close();

              if (delete_after_download)
              {
                SD_MMC.remove("/logs/" + filename);
                // ReportFileDeleted("Downloaded file: " + filename);
              }
            }
            else
            {
              // ReportFileNotPresent(filename);
            }
          }
        }
        file = root.openNextFile();
      }
      root.close();
    }
    else
    {
      // ReportFileNotPresent("logs directory");
    }
  }
  else
  {
    // ReportSDNotPresent();
  }
}

void delete_log_csv_files(void)
{
  if (!sd_log.logging)
  {
    File root = SD_MMC.open("/logs");
    if (root)
    {
      File file = root.openNextFile();
      while (file)
      {
        String filename = file.name();
        if (filename == "x00000000.csv")
        {
          file = root.openNextFile();
          continue; // Skip this specific file
        }

        if (filename.endsWith(".csv"))
        {
          SD_MMC.remove("/logs/" + filename);
        }
        file = root.openNextFile();
      }
      root.close();
    }
    else
    {
      // ReportFileNotPresent("logs directory");
    }
  }
  else
  {
    // ReportSDNotPresent();
  }
}

void delete_zero_byte_csv_files(void)
{
  if (!sd_log.logging)
  {
    File root = SD_MMC.open("/logs");
    if (root)
    {
      File file = root.openNextFile();
      while (file)
      {
        String filename = file.name();
        if (filename.endsWith(".csv"))
        {
          if (file.size() == 0)
          {
            file.close();
            SD_MMC.remove("/logs/" + filename);
            ReportFileDeleted("0-byte file: " + filename);
          }
          else
          {
            file.close();
          }
        }
        file = root.openNextFile();
      }
      root.close();
    }
    else
    {
      ReportFileNotPresent("logs directory");
    }
  }
  else
  {
    ReportSDNotPresent();
  }
}

// Download a file from the SD, it is called in void SD_dir()
void SD_file_download(String filename)
{
  if (!sd_log.logging)
  {
    File download = SD_MMC.open("/logs/" + filename);
    if (download)
    {
      server.sendHeader("Content-Type", "text/text");
      server.sendHeader("Content-Disposition", "attachment; filename=" + filename);
      server.sendHeader("Connection", "close");
      server.streamFile(download, "application/octet-stream");
      download.close();
    }
    else
      ReportFileNotPresent("download");
  }
  else
    ReportSDNotPresent();
}

// Handles the file upload a file to the SD
File UploadFile;
// Upload a new file to the Filing system
void handleFileUpload()
{
  HTTPUpload &uploadfile = server.upload(); // See https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer/srcv
                                            // For further information on 'status' structure, there are other reasons such as a failed transfer that could be used
  if (uploadfile.status == UPLOAD_FILE_START)
  {
    String filename = uploadfile.filename;
    if (!filename.startsWith("/"))
      filename = "/" + filename;
    Serial.print("Upload File Name: ");
    Serial.println(filename);
    SD_MMC.remove(filename);                        // Remove a previous version, otherwise data is appended the file again
    UploadFile = SD_MMC.open(filename, FILE_WRITE); // Open the file for writing in SD (create it, if doesn't exist)
    filename = String();
  }
  else if (uploadfile.status == UPLOAD_FILE_WRITE)
  {
    if (UploadFile)
      UploadFile.write(uploadfile.buf, uploadfile.currentSize); // Write the received bytes to the file
  }
  else if (uploadfile.status == UPLOAD_FILE_END)
  {
    if (UploadFile) // If the file was successfully created
    {
      UploadFile.close(); // Close the file again
      Serial.print("Upload Size: ");
      Serial.println(uploadfile.totalSize);
      webpage = "";
      append_page_header();
      webpage += F("<h3>File was successfully uploaded</h3>");
      webpage += F("<h2>Uploaded File Name: ");
      webpage += uploadfile.filename + "</h2>";
      webpage += F("<h2>File Size: ");
      webpage += file_size(uploadfile.totalSize) + "</h2><br><br>";
      webpage += F("<a href='/'>[Back]</a><br><br>");
      append_page_footer();
      server.send(200, "text/html", webpage);
    }
    else
    {
      ReportCouldNotCreateFile("upload");
    }
  }
}

// Delete a file from the SD, it is called in void SD_dir()
void SD_file_delete(String filename)
{
  if (!sd_log.logging)
  {
    SendHTML_Header();
    File dataFile = SD_MMC.open("/logs/" + filename, FILE_READ); // Now read data from SD Card
    if (dataFile)
    {
      if (SD_MMC.remove("/logs/" + filename))
      {
        Serial.println(F("File deleted successfully"));
        webpage += "<h3>File '" + filename + "' has been erased</h3>";
        webpage += F("<a href='/'>[Back]</a><br><br>");
      }
      else
      {
        webpage += F("<h3>File was not deleted - error</h3>");
        webpage += F("<a href='/'>[Back]</a><br><br>");
      }
    }
    else
      ReportFileNotPresent("delete");
    append_page_footer();
    SendHTML_Content();
    SendHTML_Stop();
  }
  else
    ReportSDNotPresent();
}

// SendHTML_Header
void SendHTML_Header()
{
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
  append_page_header();
  server.sendContent(webpage);
  webpage = "";
}

// SendHTML_Content
void SendHTML_Content()
{
  server.sendContent(webpage);
  webpage = "";
}

// SendHTML_Stop
void SendHTML_Stop()
{
  server.sendContent("");
  server.client().stop(); // Stop is needed because no content length was sent
}

// ReportSDNotPresent
void ReportSDNotPresent()
{
  SendHTML_Header();
  webpage += F("<h3>No SD Card present</h3>");
  webpage += F("<a href='/'>[Back]</a><br><br>");
  append_page_footer();
  SendHTML_Content();
  SendHTML_Stop();
}

// ReportFileNotPresent
void ReportFileNotPresent(String target)
{
  SendHTML_Header();
  webpage += F("<h3>File does not exist</h3>");
  webpage += F("<a href='/");
  webpage += target + "'>[Back]</a><br><br>";
  append_page_footer();
  SendHTML_Content();
  SendHTML_Stop();
}

// ReportCouldNotCreateFile
void ReportCouldNotCreateFile(String target)
{
  SendHTML_Header();
  webpage += F("<h3>Could Not Create Uploaded File (write-protected?)</h3>");
  webpage += F("<a href='/");
  webpage += target + "'>[Back]</a><br><br>";
  append_page_footer();
  SendHTML_Content();
  SendHTML_Stop();
}

// File size conversion
String file_size(int bytes)
{
  String fsize = "";
  if (bytes < 1024)
    fsize = String(bytes) + " B";
  else if (bytes < (1024 * 1024))
    fsize = String(bytes / 1024.0, 3) + " KB";
  else if (bytes < (1024 * 1024 * 1024))
    fsize = String(bytes / 1024.0 / 1024.0, 3) + " MB";
  else
    fsize = String(bytes / 1024.0 / 1024.0 / 1024.0, 3) + " GB";
  return fsize;
}
