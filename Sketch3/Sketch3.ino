/*
 Name:		Sketch3.ino
 Created:	1/16/2019 6:53:56 PM
 Author:	quang le tran
 link reference : http://arduino.vn/bai-viet/411-timercounter-tren-avrarduino
 board			: Adruino 2560
 use pin 11 and pin 6
*/
#include<avr/interrupt.h>
#define fd	(1.0f/16000000.0f)	// frequency default atmega 2560
#define dirA	7
#define dirB	4
unsigned short pwmValA;
unsigned short pwmValB;
unsigned short timeValA;
unsigned short timeValB;
// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);
	//
	pinMode(dirA, OUTPUT);
	digitalWrite(dirA, HIGH);
	pinMode(dirB, OUTPUT);
	digitalWrite(dirB, HIGH);
	//timeValA = settingPulseUseTimer3(1000);
	pwmValA = genPWMTimer1(1800);
	//timeValB = settingPulseUseTimer5(1000);
	pwmValB = genPWMTimer4(1440);
	
}

// the loop function runs over and over again until power down or reset
void loop() {
	OCR1A = timeValA/2;
	//TCCR3B |= (1 << CS32);
	OCR4A = pwmValB / 2;
	//TCCR5B |= (1 << CS52);
}

ISR(TIMER3_OVF_vect) {
	TCCR3B &= 0xF8; //stop timer
	OCR1A = pwmValA;
	TCNT3 = timeValA;
	//Serial.println("hihi");
}
ISR(TIMER5_OVF_vect) {
	TCCR5B &= 0xF8; //stop timer
	OCR4A = pwmValB;
	TCNT5 = timeValB;
	//Serial.println("hihi");
}
unsigned short genPWMTimer1(int frequency) {//use timer1
	TCCR1A = 0;// reset reg
	TCCR1B = 0;// reset reg
	DDRB |= (1 << PB5); // <=> pinMode(11,OUTPUT) ^^;
	TCCR1A |= (1 << COM1A1) | (1 << WGM11); // choose kenh A (COM1A1 COM1A0) || WGM choose mode (this mode is fast PWM )
	TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS10); // CS10 choose prescaler (this case prescaler = f default)
	// caculate 
	unsigned short _val;
	float t = (1.0f / (float)frequency)/(fd);
	_val = (unsigned short)t - 1;
	//
	//Serial.println(_val);
	ICR1 = _val;
	OCR1A = _val;
	return _val;
}
unsigned short settingPulseUseTimer3(int _time) {
	// time unit is ms
	//setting timer 3
	//reset reg
	TCCR3A = 0;
	TCCR3B = 0;
	TIMSK3 = 0;
	TIMSK3 |= (1 << TOIE3); //cho phep ngat tran

	/*	table choose prescaler		CSn2	CSn1	CSn0
	0 -> 4ms	use		/1			0		0		1

	0 -> 32ms	use		/8			0		1		0

	0 -> 262ms	use		/64			0		1		1

	0 -> 1048ms use		/256		1		0		0

	0 -> 4194ms	use		/1024		1		0		1
	*/
	
	// this case time is 1s i choose  /256
	float t = (float)_time / (256.0f*(1.0f / 16000.0f));
	// I use Nomal Mode 
	unsigned short _val = 65535 - (unsigned short)t - 1;
	TCNT3 = _val;
	//Serial.println(_val);
	return _val;
}//use timer 3

unsigned short settingPulseUseTimer5(int _time) {
	// time unit is ms
	//setting timer 3
	//reset reg
	TCCR5A = 0;
	TCCR5B = 0;
	TIMSK5 = 0;
	TIMSK5 |= (1 << TOIE5); //cho phep ngat tran

	/*	table choose prescaler		CSn2	CSn1	CSn0
	0 -> 4ms	use		/1			0		0		1

	0 -> 32ms	use		/8			0		1		0

	0 -> 262ms	use		/64			0		1		1

	0 -> 1048ms use		/256		1		0		0

	0 -> 4194ms	use		/1024		1		0		1
	*/

	// this case time is 1s i choose  /256
	float t = (float)_time / (256.0f*(1.0f / 16000.0f));
	// I use Nomal Mode 
	unsigned short _val = 65535 - (unsigned short)t - 1;
	TCNT5 = _val;
	//Serial.println(_val);
	return _val;
}//use timer 

unsigned short genPWMTimer4(int frequency) {//use timer1
	TCCR4A = 0;// reset reg
	TCCR4B = 0;// reset reg
	DDRH |= (1 << PH3); // <=> pinMode(6,OUTPUT) ^^;
	TCCR4A |= (1 << COM4A1) | (1 << WGM41); // choose kenh A (COM1A1 COM1A0) || WGM choose mode (this mode is fast PWM )
	TCCR4B |= (1 << WGM42) | (1 << WGM43) | (1 << CS40); // CS10 choose prescaler (this case prescaler = f default)
	// caculate 
	unsigned short _val;
	float t = (1.0f / (float)frequency) / fd;
	_val = (unsigned short)t - 1;
	//
	//Serial.println(_val);
	ICR4 = _val;
	OCR4A = _val;
	return _val;
}