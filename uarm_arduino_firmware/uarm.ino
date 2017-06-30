// UArmForArduino - Version: Latest 
#include <UFServo.h>

#define SERVO_ROT_PIN           11
#define SERVO_LEFT_PIN          13
#define SERVO_RIGHT_PIN         12
#define SERVO_HAND_ROT_PIN      10

#define SERVO_ROT_ANALOG_PIN 		    2
#define SERVO_LEFT_ANALOG_PIN 		  0
#define SERVO_RIGHT_ANALOG_PIN 	  	1
#define SERVO_HAND_ROT_ANALOG_PIN 	3

/*

*/

Servo _servos[4] = Servo();
int _speed[4] = {0,0,0,0};

String _inputString = "";         // a string to hold incoming data
boolean _newCommandComplete = false;  // whether the string is complete

void setup() {
    _servos[0].setPulseWidthRange(500, 2500);
    _servos[1].setPulseWidthRange(500, 2500);
    _servos[2].setPulseWidthRange(500, 2500);
    _servos[3].setPulseWidthRange(600, 2400);
    
    _servos[0].attach(SERVO_ROT_PIN);
    _servos[0].writeMicroseconds(1500);
    _servos[1].attach(SERVO_LEFT_PIN);
    _servos[1].writeMicroseconds(1500);
    _servos[2].attach(SERVO_RIGHT_PIN);
    _servos[2].writeMicroseconds(1500);
    _servos[3].attach(SERVO_HAND_ROT_PIN);
    _servos[3].writeMicroseconds(1500);

    Serial.begin(9600);
    // reserve 200 bytes for the inputString:
    _inputString.reserve(200);
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    _inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      _newCommandComplete = true;
    }
  }
}

void sort(unsigned int array[], unsigned int len)
{
	unsigned char i=0,j=0;
	unsigned int temp = 0;

	for(i = 0; i < len; i++) 
	{
		for(j = 0; i+j < (len-1); j++) 
		{
			if(array[j] > array[j+1]) 
			{
				temp = array[j];
				array[j] = array[j+1];
				array[j+1] = temp;
			}
		}
	}	
}

unsigned int getAnalogData(byte pin)
{
    unsigned int dat[8];


    for(int i = 0; i < 8; i++)
    {
        dat[i] = analogRead(pin);
    }

    sort(dat, 8);

    unsigned int result = (dat[2]+dat[3]+dat[4]+dat[5])/4;

    return result;    
}

void loop() {  
    if (_newCommandComplete)
    {
      int index[4];
      index[0] = _inputString.indexOf(',') + 1;
      index[1] = _inputString.indexOf(',', index[0]) + 1;
      if (index[0] < index[1]){
        index[2] = _inputString.indexOf(',', index[1]) + 1;
        if (index[1] < index[2]){
          _speed[0] = _inputString.substring(0, index[0]).toInt();
          _speed[1] = _inputString.substring(index[0], index[1]).toInt();
          _speed[2] = _inputString.substring(index[1], index[2]).toInt();
          _speed[3] = _inputString.substring(index[2]).toInt();
        } else {
          memset(_speed, 0, sizeof(_speed));
        }
      } else {
        memset(_speed, 0, sizeof(_speed));
      }
      
      String sep = ",";
      Serial.println(index[0] + sep + index[1] + sep + index[2]);
      
      _inputString = "";
      _newCommandComplete = false;
    }

    
    unsigned int angles[4];
    angles[0] = map(getAnalogData(SERVO_ROT_ANALOG_PIN), 98, 460, 0, 180);
    angles[1] = map(getAnalogData(SERVO_LEFT_ANALOG_PIN), 98, 460, 0, 180);
    angles[2] = map(getAnalogData(SERVO_RIGHT_ANALOG_PIN), 98, 460, 0, 180);
    angles[3] = map(getAnalogData(SERVO_HAND_ROT_ANALOG_PIN), 98, 460, 0, 180);
    
    _servos[0].writeMicroseconds(_servos[0].readMicroseconds() + _speed[0]);              // tell servo to go to position in variable 'pos'
    _servos[1].writeMicroseconds(_servos[1].readMicroseconds() + _speed[1]);              // tell servo to go to position in variable 'pos'
    _servos[2].writeMicroseconds(_servos[2].readMicroseconds() + _speed[2]);              // tell servo to go to position in variable 'pos'
    _servos[3].writeMicroseconds(_servos[3].readMicroseconds() + _speed[3]);              // tell servo to go to position in variable 'pos'
    
    String sep = ",";
    //Serial.println(angles[0] + sep + angles[1] + sep + angles[2] + sep + angles[3]);
    //Serial.println(_speed[0] + sep + _speed[1] + sep + _speed[2] + sep + _speed[3]);
    delay(1);
}
