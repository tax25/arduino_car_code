// 25 Agosto 2020 Inizio a lavorare a questo codice. 
// L'altro è un po' rotto, per questo ne faccio uno nuovo.

// Steering Stuff// 
#include "Servo.h"
Servo myServo;
#define pinServo 4
// Pin dei 2 motori
	// motore 1
	#define pwmA 2
	#define in1A 22
	#define in2A 23

	// motore 2
	#define pwmB 3
	#define in1B 24
	#define in2B 25

// Per la guida performata da una persona
// i-o joystick:
#define  pinX A0
#define  pinY A1
#define  buttonPin 52

// Collegamenti dei sensori
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
	
	//Settaggio primo motore//
  	pinMode(pwmA, OUTPUT);
  	pinMode(in1A, OUTPUT);
  	pinMode(in2A, OUTPUT);
  	//Settaggio secondo motore//
  	pinMode(pwmB, OUTPUT);
  	pinMode(in1B, OUTPUT);
  	pinMode(in2B, OUTPUT);

  	//Settaggio sensori ad ultrasuoni
  	//Primo
  	pinMode(firstSensor_trigpin, OUTPUT);
  	pinMode(firstSensor_echopin, INPUT);	
  	//Secondo
  	pinMode(secondSensor_trigpin, OUTPUT);
  	pinMode(secondSensor_echopin, INPUT);
  	//Terzo
  	pinMode(thirdSensor_trigPin, OUTPUT);
  	pinMode(thirdSensor_echoPin, INPUT);

}

#define val 0.03438 

void loop(){

	set_motor_speed_and_steering(pwmA, pwmB, firstSensor_trigpin, firstSensor_echopin, secondSensor_trigpin, secondSensor_echopin, thirdSensor_trigPin, thirdSensor_echoPin, fourthSensor_trigPin, fourthSensor_echoPin);

	delay(200);
}


void set_motor_speed_and_steering(int pin1, int pin2, int trigPin1, int echoPin1, int trigPin2, int echoPin2, int trigPin3, int echoPin3, int trigPin4, int echoPin4) /*-> None*/{
	bool going_forward = true;

	//Leggo i valori che arrivano dal joystick per ricavare velocità e sterzo. 
	int _speed = analogRead(pinY); //L'asse Y è l'asse della velocità
	int _steering = analogRead(pinX); //L'asse X è l'asse dello sterzo

	//Calcolo le distanze dai vari sensori:
//Primo sensore (davanti)
	digitalWrite(trigPin1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin1, LOW);
    float returnedTime1 = pulseIn(echoPin1, HIGH);
	float distanza_davanti = val * returnedTime1 / 2;
	
	delayMicroseconds(10);
//Secondo sensore (dietro)
    digitalWrite(trigPin2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin2, LOW);
    float returnedTime2 = pulseIn(echoPin2, HIGH);
	float distanza_dietro = val * returnedTime2 / 2;

	delayMicroseconds(10);
//Terzo sensore (destra)
	digitalWrite(trigPin3, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin3, LOW);
	float returnedTime3 = pulseIn(echoPin3, HIGH);
	float distanza_destra = val * returnedTime3 / 2;

	delayMicroseconds(10);
//Quarto sensore (sinistra)
	digitalWrite(trigPin4, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin4, LOW);
	float returnedTime4 = pulseIn(echoPin4, HIGH);
	float distanza_sinistra = val * returnedTime4 / 2;



	int effective_speed = map(_speed, 0, 1023, 100, -100); // Cambio i loro valori da 0 a 1023 -> 100 a -100.
														  // Abbiamo scelto 100 e -100 per avere la velocità massima di 100
														  // sia in forward che in backward, ma non avere una velocità di 
														  // di punta degna di un 747. 
	int effective_steering = map(_steering, 0, 1023, 45, 135); //Invece per il servo abbiamo deciso di mettere come valore 
														   // minimo 45 gradi, che è il massimo sterzo a sinistra, e 
														   // 135 gradi sono il massimo sterzo a destra, esattamente 90 gradi 
														   // di sterzo. Nella posizione "di riposo" siamo a 90 gradi. 

	/*
		Certe volte potrebbe servire mettere un numero in più nella funzione
		map() per avere l'esatto valore massimo che vogliamo. 
		Infatti potrebbe accadere che mettiate come conversione da 0 a 100 per 
		esempio, e il massimo numero che ricevete sia 99. Non so da cosa venga
		causata la faccienda, ma quando mi è acccaduto ho risolto in questo modo.
	*/

	
	//Serial.println("Steering: " + String(effective_steering));
	//Serial.println(effective_speed);
	if (effective_speed < 0){
		
		going_forward = false;
		digitalWrite(in1A, LOW);
		digitalWrite(in2A, HIGH);
		digitalWrite(in1B, LOW);
		digitalWrite(in2B, HIGH);

		effective_speed *= -1;
		// Moltiplico per -1 la velocità che arriva nel caso sia negativa, 
		// perché al driver non piacciono i numeri negativi e spara il 
		// motore a velocità massima (250).
	}else{
		//Serial.println("\nBuonasera adesso il valore di going_forward e' uguale a 1!");
		going_forward = true;
		digitalWrite(in1A, HIGH);
		digitalWrite(in2A, LOW);
		digitalWrite(in1B, HIGH);
		digitalWrite(in2B, LOW);
	}

	//Serial.println("Speed: " + String(effective_speed) + " Questo è il valore di going_forward: " + String(going_forward));
	//Serial.println(effective_steering);  
	/*
		IMPORTANTE:
		Se effective_steering è maggiore di 90 allora la macchina sta girando a destra, altrimenti a sinistra.
	*/
	if ((distanza_davanti >= 15.0)&&(going_forward==1)){
		
		//Serial.println("Valore going_forward: " + String(going_forward) + "STO ANDANDO AVANTI MADERFADERS!!!!");
		//Serial.println("Valore Sterzo: " + String(effective_steering) + "Questa è la distanza destra: " + String(distanza_destra));
		
		myServo.write(effective_steering);

		if (effective_steering == 90){
			//Sto andando avanti
			Serial.println("Sto andando avanti" + String(effective_speed));
			analogWrite(pin1, effective_speed);
			analogWrite(pin2, effective_speed);
		}
		if ((effective_steering > 90)&&(distanza_destra >= 15.0)){
			//Sto girando a destra
			Serial.println("Sto girando a destra");
			analogWrite(pin1, effective_speed);
			analogWrite(pin2, effective_speed);
		}else{
			analogWrite(pin1, 0);
			analogWrite(pin2, 0);
		}

		if ((effective_steering < 90)&&(distanza_sinistra >= 15.0)){
			//Sto girando a sinistra
			Serial.println("Sto girando a sinistra");
			analogWrite(pin1, effective_speed);
			analogWrite(pin2, effective_speed);
		}else{
			analogWrite(pin1, 0);
			analogWrite(pin2, 0);
		}	

	
	}else if((distanza_dietro >= 15.0)&&(going_forward==0)){

		//Serial.println("Valore going_forward: " + String(going_forward) + "STO ANDANDO INDIETRO MADERFADERS!!!!");
		
		myServo.write(effective_steering);

		if ((effective_steering > 90)&&(distanza_destra >= 15.0)){

			Serial.println("Sto facendo retro verso destra");

			analogWrite(pin1, effective_speed);
			analogWrite(pin2, effective_speed);
		}else if (effective_steering == 90){
			analogWrite(pin1, effective_speed);
			analogWrite(pin2, effective_speed);
		}

		if ((effective_steering < 90)&&(distanza_sinistra >= 15.0)){

			Serial.println("Sto facendo retro verso sinistra");
			
			//Serial.println("sinistra occupata" + String(distanza_sinistra));
			analogWrite(pin1, effective_speed);
			analogWrite(pin2, effective_speed);
		}else{
			analogWrite(pin1, 0);
			analogWrite(pin2, 0);
		}

	}else {
		Serial.println("Zona occupata!");
		analogWrite(pin1, 0);
		analogWrite(pin2, 0);
	}
	
}
