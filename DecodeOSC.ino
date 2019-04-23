// La version 1 recoit l'angle de chaque servo
// La version 2 recoit un angle moyen et un angle d'ouverture entre les deux ciseaux


int packetSize;
char incomingPacket[255];
float sliders[8];

void slider1(OSCMessage &msg)
//This function is called by OSC_Decode() depending on msg.dispatch()
{
  DecodeServo(msg,0);
}

void slider2(OSCMessage &msg)
//This function is called by OSC_Decode() depending on msg.dispatch()
{
  DecodeServo(msg,1);
}

void slider3(OSCMessage &msg)
//This function is called by OSC_Decode() depending on msg.dispatch()
{
  DecodeServo(msg,2);
}

void slider4(OSCMessage &msg)
//This function is called by OSC_Decode() depending on msg.dispatch()
{
  DecodeServo(msg,3);
}

void slider5(OSCMessage &msg)
//This function is called by OSC_Decode() depending on msg.dispatch()
{
  DecodeServo(msg ,4);
}

void slider6(OSCMessage &msg)
//This function is called by OSC_Decode() depending on msg.dispatch()
{
  DecodeServo(msg,5);
}

void slider7(OSCMessage &msg)
//This function is called by OSC_Decode() depending on msg.dispatch()
{
  DecodeServo(msg,6);
}

void slider8(OSCMessage &msg)
//This function is called by OSC_Decode() depending on msg.dispatch()
{
  DecodeServo(msg,7);
}

void DecodeServo(OSCMessage &msg, int i)
{
  if(msg.isFloat(0))
  {
    if (DEBUG_LEVEL >=1 ) Serial.println("Float received");
//    float val = 
    sliders[i]=msg.getFloat(0);
    int serv1 = 1.0*(sliders[0]+sliders[1])*180-90;
    int serv2 = 1.0*(sliders[0]-sliders[1])*180+90;

    Serial.print("Servos "); 
    Serial.print(serv1);
    Serial.print(" / ");
    Serial.print(serv2);
    Serial.println("");
//    myServo[i].write(serv);
    myServo[0].write(serv1);
    myServo[1].write(serv2);
    
  }
}

void OSC_Decode()
//This function shall be called periodicaly to check if UDP packet have been received and decode it
{
  packetSize = Udp.parsePacket();
  if (packetSize)
  {
    if (DEBUG_LEVEL >= 1) Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    OSCMessage msg;
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    if (DEBUG_LEVEL >= 1) Serial.print("UDP packet contents: ");
    for(int i=0;i<len;i++)
    {
      msg.fill(incomingPacket[i]);
      if (DEBUG_LEVEL >= 1) Serial.print(incomingPacket[i]);
    }
    if (DEBUG_LEVEL >= 1) Serial.println(" ");

//Dispatch OSC messages to actions (modify code bellow)
//    msg.dispatch("/oscControl/button1", button1);

    // /oscControl/slider*
    strRemoteIP[OscIndexPos]='1';
    msg.dispatch(strRemoteIP, slider1);  
    strRemoteIP[OscIndexPos]='2';
    msg.dispatch(strRemoteIP, slider2);    
    strRemoteIP[OscIndexPos]='3';
    msg.dispatch(strRemoteIP, slider3);    
    strRemoteIP[OscIndexPos]='4';
    msg.dispatch(strRemoteIP, slider4);    
    strRemoteIP[OscIndexPos]='5';
    msg.dispatch(strRemoteIP, slider5);    
    strRemoteIP[OscIndexPos]='6';
    msg.dispatch(strRemoteIP, slider6);    
    strRemoteIP[OscIndexPos]='7';
    msg.dispatch(strRemoteIP, slider7);    
    strRemoteIP[OscIndexPos]='8';
    msg.dispatch(strRemoteIP, slider8);    

    if (DEBUG_LEVEL >= 1) Serial.println(" ");
  }
}
