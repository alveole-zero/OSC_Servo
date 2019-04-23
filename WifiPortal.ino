/* WifiPortal*/

/*
 * To add a field in the captive portal and save/restore it from SPIFFS we need to:
  StepA1: Create a global variable. In this example variable is called RemoteIP
  
  StepA2: In WifiPortal, Create a WiFiManagerParameter variable, typically named with the following syntax: p_ + VariableName = p_strRemoteIP

  StepA3: In WifiPortal, Add the parameter created in previous step to wifiManager

  StepA4: In WifiPortal, Get the value entered in captive portal thanks to WiFiManagerParameter and store it in the revelent string

  StepA5: In JSON_Functions, Write strings to JSON

  StepA6: In JSON_Functions, Read strings from JSON
*/

void parseBytes(const char* str, char sep, byte* bytes, int maxBytes, int base)
//This function converts a string to an array of chars separated by char 'sep'
{
    for (int i = 0; i < maxBytes; i++) {
        bytes[i] = strtoul(str, NULL, base);  // Convert byte
        str = strchr(str, sep);               // Find next separator
        if (str == NULL || *str == '\0') {
            break;                            // No more separators, exit
        }
        str++;                                // Point to next character after separator
    }
}

void WifiPortal()
//This function is used to open an access point with captive web portable where SSID, password and user defined variables can be configured
{
     Serial.println("Configuration portal requested");
    //Local intialization. Once its business is done, there is no need to keep it around

    // Extra parameters to be configured
    // After connecting, parameter.getValue() will get you the configured value
    // Format: <ID> <Placeholder text> <default value> <length> <custom HTML> <label placement>
  char convertedValue[3];
  // StepA2 :  
  WiFiManagerParameter p_strRemoteIP("remoteip", "OSC address: /oscControl/slider*", strRemoteIP, 30);

  //!StepA2
  
    // Just a quick hint
    WiFiManagerParameter p_hint("<small>*Hint: if you want to reuse the currently active WiFi credentials, leave SSID and Password fields empty</small>");
    
    // Initialize WiFIManager
    WiFiManager wifiManager;
    
    //add all parameters here.
    
    wifiManager.addParameter(&p_hint);
    
  //StepA3
    wifiManager.addParameter(&p_strRemoteIP);
  //!StepA3

    // Sets timeout in seconds until configuration portal gets turned off.
    // If not specified device will remain in configuration mode until
    // switched off via webserver or device is restarted.
    wifiManager.setConfigPortalTimeout(180);//180 is 3 minutes

    // It starts an access point 
    // and goes into a blocking loop awaiting configuration.
    // Once the user leaves the portal with the exit button
    // processing will continue
    if (!wifiManager.startConfigPortal())
    {
      Serial.println("Not connected to WiFi but continuing anyway.");
    }
    else
    {
      // If you get here you have connected to the WiFi
      Serial.println("Connected...yeey :)");
    }

    // Getting posted form values and overriding local variables parameters
    // Config file is written regardless the connection state
    //strcpy(thingspeakApiKey, p_thingspeakApiKey.getValue());
    
  //StepA4
    strcpy(strRemoteIP,p_strRemoteIP.getValue());
  //!StepA4
  
    // Writing JSON config file to flash for next boot
    writeConfigFile();

    ESP.reset(); // This is a bit crude. For some unknown reason webserver can only be started once per boot up 
    // so resetting the device allows to go back into config mode again when it reboots.
}
