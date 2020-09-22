// 25 Agosto 2020, Starting to work to this code.

// Steering Stuff//
#include "Servo.h"
Servo myServo;
#define pinServo 4
// motor pins
	// 1st motor
	#define pwmA 2
	#define in1A 22
	#define in2A 23

	// 2nd motor
	#define pwmB 3
	#define in1B 24
	#define in2B 25

// For "human" driving
// i-o joystick:
#define  pinX A0
#define  pinY A1
#define  buttonPin 52

// Ultra-sound sensor connections
#define firstSensor_trigpin 5
#define firstSensor_echopin 6
/////////////////////////////
#define secondSensor_trigpin 7
#define secondSensor_echopin 8
/////////////////////////////
#define thirdSensor_trigPin 9
#define thirdSensor_echoPin 10
/////////////////////////////
#define fourthSensor_trigPin 11
#define fourthSensor_echoPin 12

void setup(){
	Serial.begin(9600);

	myServo.attach(pinServo);

	/*First motor setting*/
  	pinMode(pwmA, OUTPUT);
  	pinMode(in1A, OUTPUT);
  	pinMode(in2A, OUTPUT);
  /*Second motor setting*/
  	pinMode(pwmB, OUTPUT);
  	pinMode(in1B, OUTPUT);
  	pinMode(in2B, OUTPUT);

  	//Ultra-sound sensors setting
  	//First
  	pinMode(firstSensor_trigpin, OUTPUT);
  	pinMode(firstSensor_echopin, INPUT);
  	//Second
  	pinMode(secondSensor_trigpin, OUTPUT);
  	pinMode(secondSensor_echopin, INPUT);
  	//Third
  	pinMode(thirdSensor_trigPin, OUTPUT);
  	pinMode(thirdSensor_echoPin, INPUT);
		//Fourth
		pinMode(fourthSensor_trigPin, OUTPUT);
		pinMode(fourthSensor_echoPin, INPUT);
}

#define val 0.03438 // To know what this constant is check out the README file
										// in the "define val" zone.

void loop(){

	set_motor_speed_and_steering(pwmA, pwmB, firstSensor_trigpin, firstSensor_echopin, secondSensor_trigpin, secondSensor_echopin, thirdSensor_trigPin, thirdSensor_echoPin, fourthSensor_trigPin, fourthSensor_echoPin);

	delay(200);
}


void set_motor_speed_and_steering(int pin1, int pin2, int trigPin1, int echoPin1, int trigPin2, int echoPin2, int trigPin3, int echoPin3, int trigPin4, int echoPin4) /*-> None*/{
	bool going_forward = true;

	// Reading the values returned by the joystick to calculate speed and steering
	int _speed = analogRead(pinY); // Y axis is the acceleration axis
	int _steering = analogRead(pinX); // X axis is the steering axis

	// Calculating the distances of the sensors
//1st sensor (front)
	digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  float returnedTime1 = pulseIn(echoPin1, HIGH);
	float distanza_davanti = val * returnedTime1 / 2;

	delayMicroseconds(10);
//2nd sensor (behind)
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  float returnedTime2 = pulseIn(echoPin2, HIGH);
	float distanza_dietro = val * returnedTime2 / 2;

	delayMicroseconds(10);
//3rd sensor (right)
	digitalWrite(trigPin3, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin3, LOW);
	float returnedTime3 = pulseIn(echoPin3, HIGH);
	float distanza_destra = val * returnedTime3 / 2;

	delayMicroseconds(10);
//4th sensor (left)
	digitalWrite(trigPin4, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin4, LOW);
	float returnedTime4 = pulseIn(echoPin4, HIGH);
	float distanza_sinistra = val * returnedTime4 / 2;


	// Taking the values returned by the joystick and "translating" them from
	// 0-1023 to (negative)100-100. Look at the README file for more infos,
	// "Translating joystick values from 0/1023 to -100/100 and to 45/135" section.
	int effective_speed = map(_speed, 0, 1023, 100, -100);

	int effective_steering = map(_steering, 0, 1023, 45, 135);

	//some debugging things
	//Serial.println("Steering: " + String(effective_steering));
	//Serial.println(effective_speed);
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
		//Serial.println("\nBuonasera adesso il valore di going_forward e' uguale a 1!");
		going_forward = true;
		digitalWrite(in1A, HIGH);
		digitalWrite(in2A, LOW);
		digitalWrite(in1B, HIGH);
		digitalWrite(in2B, LOW);
	}

	//Some debugging things
	//Serial.println("Speed: " + String(effective_speed) + " Questo è il valore di going_forward: " + String(going_forward));
	//Serial.println(effective_steering);
	/*
		IMPORTANT:
		If "effective_steering" is greater than 90 the car is steering right, otherwise it's steering left.
	*/
	if ((distanza_davanti >= 15.0)&&(going_forward==1)){
		//Some debugging things
		//Serial.println("Valore going_forward: " + String(going_forward) + "STO ANDANDO AVANTI MADERFADERS!!!!");
		//Serial.println("Valore Sterzo: " + String(effective_steering) + "Questa è la distanza destra: " + String(distanza_destra));

		myServo.write(effective_steering);

		if (effective_steering == 90){
			//Going forward
			Serial.println("Sto andando avanti" + String(effective_speed));
			analogWrite(pin1, effective_speed);
			analogWrite(pin2, effective_speed);
		}
		if ((effective_steering > 90)&&(distanza_destra >= 15.0)){
			//Steering right
			Serial.println("Sto girando a destra");
			analogWrite(pin1, effective_speed);
			analogWrite(pin2, effective_speed);
		}else{
			analogWrite(pin1, 0);
			analogWrite(pin2, 0);
		}

		if ((effective_steering < 90)&&(distanza_sinistra >= 15.0)){
			//Steering left
			/*Debugging things*/Serial.println("Sto girando a sinistra");
			analogWrite(pin1, effective_speed);
			analogWrite(pin2, effective_speed);
		}else{
			analogWrite(pin1, 0);
			analogWrite(pin2, 0);
		}


	}else if((distanza_dietro >= 15.0)&&(going_forward==0)){
		//Some debugging things
		//Serial.println("Valore going_forward: " + String(going_forward) + "STO ANDANDO INDIETRO MADERFADERS!!!!");

		myServo.write(effective_steering);

		if ((effective_steering > 90)&&(distanza_destra >= 15.0)){

			/*Debugging things*/Serial.println("Sto facendo retro verso destra");

			analogWrite(pin1, effective_speed);
			analogWrite(pin2, effective_speed);
		}else if (effective_steering == 90){
			analogWrite(pin1, effective_speed);
			analogWrite(pin2, effective_speed);
		}

		if ((effective_steering < 90)&&(distanza_sinistra >= 15.0)){

			/*Debugging things*/Serial.println("Sto facendo retro verso sinistra");
			//Some debugging things
			//Serial.println("sinistra occupata" + String(distanza_sinistra));
			analogWrite(pin1, effective_speed);
			analogWrite(pin2, effective_speed);
		}else{
			analogWrite(pin1, 0);
			analogWrite(pin2, 0);
		}

	}else {
		/*Debugging things*/Serial.println("Zona occupata!");
		analogWrite(pin1, 0);
		analogWrite(pin2, 0);
	}

}
