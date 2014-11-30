#define RED 4

int message = 0; // This will hold one byte of the serial message
int LEDPin = 13; // This is the pin that the led is conected to
int LED = 0; // The value or brightness of the LED, can be 0-255
int rudder = 63;
int elevator =63;
int throttle = 0;

void setup() {
pinMode(RED, OUTPUT);
pinMode(13, OUTPUT);
pinMode(12, OUTPUT);
pinMode(11, OUTPUT);

Serial.begin(9600); //set serial to 9600 baud rate
}

void loop(){
if (Serial.available() > 0) { // Check to see if there is a new message
message = Serial.read(); // Put the serial input into the message

if (message == '1'){ // If a capitol A is received
    digitalWrite(13, HIGH); // Set LED to 255 (on)
    if(throttle < 77) {
      throttle += 50;
    } else {
      throttle = 127;
    }
}
if (message == '0'){ // If a lowercase a is received
    digitalWrite(13, LOW); // Set LED to 255 (on)
    if(throttle > 60) {
      throttle -= 50;
    } else {
      throttle = 0;
    }   
}
if (message == '2'){
  digitalWrite(12, LOW);
    if(rudder > 20 && rudder < 47) {
      rudder -= 20;
    } else {
      rudder = 0;
    }
}
if (message == '3'){
  digitalWrite(12, HIGH);
    if(rudder < 107 && rudder > 77) {
      rudder += 20;
    } else {
      rudder = 127;
    }
}
if (message == '4'){
  digitalWrite(11, HIGH);
    if(elevator < 107 && elevator > 77) {
      elevator += 20;
    } else {
      elevator = 127;
    }
}
if (message == '5'){
  digitalWrite(11, LOW);
    if(elevator > 20 && elevator < 47) {
      elevator -= 20;
    } else {
      elevator = 0;
    }
}
Transmit(rudder, elevator, throttle, 63);
}
}

void Transmit(byte rudder, byte elevator, byte throttle, byte trim) {
	static byte Code[4];
	byte mask = 128; 	//bitmask
	int i;

	Code[0] = rudder; // 0 -> 127; 63 is the midpoint.
	Code[1] = elevator; // 0 -> 127; 63 is the midpoint.
	Code[2] = throttle; // 0 -> 127; 0 is throttle off
	Code[3] = trim;	// Haven't messed with this
    
	OutPulse(2002);  // Start 38Khz pulse for 2000us (2002us is evenly divided by 26)
	delayMicroseconds(2000);  // 2000us off.

	for (i = 0; i<4; i++) {		// Loops through the Code[]
		for (mask = 128; mask > 0; mask >>=1) {	// See Arduino reference for bit masking (really cool stuff!)
		OutPulse(312); 		// Sends 312 pulse each loop

			if(Code[i] & mask) {          //If both bit positions are 1 you get 1              
				delayMicroseconds(688);     // send 1 (700 off)
			} 
			else {
				delayMicroseconds(288);     // send 0 (300 off)
			}
		} //End mask loop
	}  //End i loop

	OutPulse(312);  //Send 300 microsecond Closing Pulse
	delay(60);        

} // End Transmit
 

void OutPulse(int Pulse) {  //sends 38Khz pulses over the Pulse Length
	int p;

	for(p = 0; p < (Pulse / 26) - 1; p++) {  //Takes about 26 microseconds per loop
		digitalWrite(RED, HIGH);
		  delayMicroseconds(10);
		digitalWrite(RED, LOW);
		  delayMicroseconds(10);
	}
}  //End OutPulse
