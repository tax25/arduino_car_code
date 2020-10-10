//Here we keep all the defines of the sketch to make it easier to read
#ifndef H_ALL_DEFINES
#define H_ALL_DEFINES

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

// For "human" remote driving
	// i-o joystick:
		#define  pinX A0
		#define  pinY A1
		#define  buttonPin 52

	// Ultra-sound sensor connections
		#define number_of_Sensors 4
		//////////////////////////////
		#define firstSensor_trigPin 5
		#define firstSensor_echoPin 6
		/////////////////////////////
		#define secondSensor_trigPin 7
		#define secondSensor_echoPin 8
	  /////////////////////////////
		#define thirdSensor_trigPin 9
		#define thirdSensor_echoPin 10
		/////////////////////////////
		#define fourthSensor_trigPin 11
		#define fourthSensor_echoPin 12

#endif
