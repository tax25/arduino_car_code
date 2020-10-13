// 25 Agosto 2020, Starting to work to this code.
// Steering Stuff//
#include "Servo.h"
#include "all_defines.h" //With this include i include the file that has all the defines in the secondSensor_echoPin

Servo myServo; //Creating servo object

// For more infos about this 2 arrays, check out the
// "Calculating distances of the sensors" section
int sensorsTrigPinArray[4] = {firstSensor_trigPin, secondSensor_trigPin, thirdSensor_trigPin, fourthSensor_trigPin};
int sensorsEchoPinArray[4] = {firstSensor_echoPin, secondSensor_echoPin, thirdSensor_echoPin, fourthSensor_echoPin};
float distancesArray[4]; // This array stores the distances variables

void setup(){
	Serial.begin(9600);

	myServo.attach(pinServo);

	/* First motor setting */
  	pinMode(pwmA, OUTPUT);
  	pinMode(in1A, OUTPUT);
  	pinMode(in2A, OUTPUT);
  /* Second motor setting */
  	pinMode(pwmB, OUTPUT);
  	pinMode(in1B, OUTPUT);
  	pinMode(in2B, OUTPUT);

  	// Ultra-sound sensors setting
  	// First
  	pinMode(firstSensor_trigPin, OUTPUT);
  	pinMode(firstSensor_echoPin, INPUT);
  	// Second
  	pinMode(secondSensor_trigPin, OUTPUT);
  	pinMode(secondSensor_echoPin, INPUT);
  	// Third
  	pinMode(thirdSensor_trigPin, OUTPUT);
  	pinMode(thirdSensor_echoPin, INPUT);
		// Fourth
		pinMode(fourthSensor_trigPin, OUTPUT);
		pinMode(fourthSensor_echoPin, INPUT);
}

#define val 0.03438 // To know what this constant is, check out the README file
										// in the "define val" section.

void loop(){

	set_motor_speed_and_steering(pwmA, pwmB, sensorsTrigPinArray, sensorsEchoPinArray, number_of_Sensors);

	delay(200);
}

void set_motor_speed_and_steering(int pin1, int pin2, int *trigArray, int *echoArray, int nOfSensors) /*-> None*/{
	bool going_forward = true;

	// Reading the values returned by the joystick to calculate speed and steering
	int _speed = analogRead(pinY); // Y axis is the acceleration axis
	int _steering = analogRead(pinX); // X axis is the steering axis

	for (int i = 0; i < nOfSensors; i++){
		digitalWrite(trigArray[i], HIGH);
		delayMicroseconds(10);
		digitalWrite(trigArray[i], LOW);
		float returnedTime = pulseIn(echoArray[i], HIGH);
		float distanceCalculated = val * returnedTime / 2;
		distancesArray[i] = distanceCalculated;
		delayMicroseconds(10);
	}

	float distance_infront = distancesArray[0];
	float distance_behind = distancesArray[1];
	float distance_right = distancesArray[2];
	float distance_left = distancesArray[3];

	// Taking the values returned by the joystick and "translating" them from
	// 0-1023 to (negative)100-100. Look at the README file for more infos,
	// "Translating joystick values from 0/1023 to -100/100 and to 45/135" section.
	int effective_speed = map(_speed, 0, 1023, 100, -100);

	int effective_steering = map(_steering, 0, 1023, 45, 135);

	// some debugging things
	// Serial.println("Steering: " + String(effective_steering));
	// Serial.println(effective_speed);
	if (effective_speed < 0){

		going_forward = false;
		digitalWrite(in1A, LOW);
		digitalWrite(in2A, HIGH);
		digitalWrite(in1B, LOW);
		digitalWrite(in2B, HIGH);

		effective_speed *= -1;

		// Multiplying for -1 the speed in case it's negative because if the motor
		// driver receives a negative value, it assigns speed 250 to the motor.
		// When the speed is negative, the car goes in reverse.

	}else{
		// some debugging things
		// Serial.println("\nBuonasera adesso il valore di going_forward e' uguale a 1!");
		going_forward = true;
		digitalWrite(in1A, HIGH);
		digitalWrite(in2A, LOW);
		digitalWrite(in1B, HIGH);
		digitalWrite(in2B, LOW);
	}

	/*
	Some debugging things
	Serial.println("Speed: " + String(effective_speed) + " Questo è il valore di going_forward: " + String(going_forward));
	Serial.println(effective_steering);
	*/

	/*
		IMPORTANT:
		If "effective_steering" is greater than 90 the car is steering right, otherwise it's steering left.
	*/
	if ((distance_infront >= 15.0)&&(going_forward == true)){
		/*
		Some debugging things
		Serial.println("Valore going_forward: " + String(going_forward) + "STO ANDANDO AVANTI MADERFADERS!!!!");
		Serial.println("Valore Sterzo: " + String(effective_steering) + "Questa è la distanza destra: " + String(distanza_destra));
		*/
		myServo.write(effective_steering);

		if (effective_steering == 90){
			// Going forward
			Serial.println("Sto andando avanti" + String(effective_speed));
			analogWrite(pin1, effective_speed);
			analogWrite(pin2, effective_speed);
		}
		if ((effective_steering > 90)&&(distance_right >= 15.0)){
			// Steering right
			Serial.println("Sto girando a destra");
			analogWrite(pin1, effective_speed);
			analogWrite(pin2, effective_speed);
		}else{
			analogWrite(pin1, 0);
			analogWrite(pin2, 0);
		}

		if ((effective_steering < 90)&&(distance_left >= 15.0)){
			// Steering left
			/* Debugging things */Serial.println("Sto girando a sinistra");
			analogWrite(pin1, effective_speed);
			analogWrite(pin2, effective_speed);
		}else{
			analogWrite(pin1, 0);
			analogWrite(pin2, 0);
		}


	}else if((distance_behind >= 15.0)&&(going_forward == false)){
		// Some debugging things
		// Serial.println("Valore going_forward: " + String(going_forward) + "STO ANDANDO INDIETRO MADERFADERS!!!!");

		myServo.write(effective_steering);

		if ((effective_steering > 90)&&(distance_right >= 15.0)){

			/* Debugging things */Serial.println("Sto facendo retro verso destra");

			analogWrite(pin1, effective_speed);
			analogWrite(pin2, effective_speed);
		}else if (effective_steering == 90){
			analogWrite(pin1, effective_speed);
			analogWrite(pin2, effective_speed);
		}

		if ((effective_steering < 90)&&(distance_left >= 15.0)){

			/* Debugging things */Serial.println("Sto facendo retro verso sinistra");
			// Some debugging things
			// Serial.println("sinistra occupata" + String(distanza_sinistra));
			analogWrite(pin1, effective_speed);
			analogWrite(pin2, effective_speed);
		}else{
			analogWrite(pin1, 0);
			analogWrite(pin2, 0);
		}

	}else {
		/* Debugging things */Serial.println("Zona occupata!");
		analogWrite(pin1, 0);
		analogWrite(pin2, 0);
	}

}
