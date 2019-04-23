bool readConfigFile()
//This function reads SPIFFS from flash to get previously saved config.
{
  // this opens the config file in read-mode
  File f = SPIFFS.open(CONFIG_FILE, "r");
  
  if (!f)
  {
    Serial.println("Configuration file not found");
    return false;
  }
  else
  {
    // we could open the file
    size_t size = f.size();
    // Allocate a buffer to store contents of the file.
    std::unique_ptr<char[]> buf(new char[size]);

    // Read and store file contents in buf
    f.readBytes(buf.get(), size);
    // Closing file
    f.close();
    // Using dynamic JSON buffer which is not the recommended memory model, but anyway
    // See https://github.com/bblanchon/ArduinoJson/wiki/Memory%20model

    DynamicJsonBuffer jsonBuffer; //DynamicJsonBuffer for version before 6.0, DynamicJsonDocument after
  // Parse JSON string
    JsonObject& json = jsonBuffer.parseObject(buf.get());
    // Test if parsing succeeds.
    if (!json.success()) {
      Serial.println("JSON parseObject() failed");
      return false;
    }
    json.printTo(Serial);

    // Parse all config file parameters, override 
    // local config variables with parsed values
//STEPA6
    {
      if (json.containsKey("strRemoteIP"))
      {
        strcpy(strRemoteIP, json["strRemoteIP"]); 
        parseBytes(strRemoteIP, '.', RemoteIP, 4, 10); 
      }
    }
//!STEPA6
  }
  Serial.println("\nConfig file was successfully parsed");
  return true;
}

bool writeConfigFile()
//This function saves config SPIFFS to flash
{
  Serial.println("Saving config file");
  DynamicJsonBuffer jsonBuffer; //DynamicJsonBuffer for version before 6.0
  JsonObject& json = jsonBuffer.createObject();

  // JSONify local configuration parameters
  //json["thingspeakApiKey"] = thingspeakApiKey;
//STEPA5
  json["strRemoteIP"] = strRemoteIP;
//!STEPA5

  // Open file for writing
  File f = SPIFFS.open(CONFIG_FILE, "w");
  if (!f)
  {
    Serial.println("Failed to open config file for writing");
    return false;
  }

  json.prettyPrintTo(Serial);
  
  // Write data to file and close it
  json.printTo(f);
  f.close();

  Serial.println("\nConfig file was successfully saved");
  return true;
}
