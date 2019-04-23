float highfloat = 1.0;

void loop()
{  
    drd.loop();
  OSC_Decode();
  // is configuration portal requested?
  if ( (initialConfig))
  {
    WifiPortal();
  }
  if (Serial.available())
  {
    if(Serial.read() == 'c')
    {
      initialConfig = true;
      Serial.println("SerialConfig");
    }
  }
}
