import processing.serial.*;

import g4p_controls.*;

Serial myPort;  // Create object from Serial class
int val;        // Data received from the serial port
String message = null;
int bgcol = 15;

public void setup() {
  size(500, 360);
  createGUI();
  String portName = "COM3";
  myPort = new Serial(this, portName, 9600);
  String s;
  s = "Starting...";
  txtMsgReceive.setText(s);
}

public void draw() {
  background(200, 200, 200);
  while (myPort.available() > 0)
  {
    txtMsgReceive.setText("port available and going in");
    message = myPort.readStringUntil(10);
    if(message != null){
     //String s = txtMsgReceive.getText();
     //s += "\n" + message;
     txtMsgReceive.setText(message);
  }
}

}
