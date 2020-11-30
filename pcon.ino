#include <Servo.h>

#define PIN_SERVO 10
#define PIN_IR A0

float ir_distance();

Servo ser;

unsigned long serial_last, servo_last, sensors_last;
float dist_ir;

const float kp_close = 0.9;
const float kp_far = 1.3;

void setup()
{
	ser.attach(PIN_SERVO);
	ser.writeMicroseconds(1610);
	Serial.begin(57600);
	serial_last = servo_last = sensors_last = 0;
}


void loop()
{
	static int duty_curr = 1610;
	float pterm;
	int duty_target;

	if (millis() - sensors_last > 10) {
		dist_ir = ir_distance();
		sensors_last + 10;
	}

	pterm = ((dist_ir > 255) ? kp_far : kp_close) * (255 - dist_ir);
	duty_target = 1610 + pterm;

	if (millis() - servo_last > 10) {
		if (duty_target - duty_curr >= 5)
			duty_curr += 5;
		else if (duty_curr - duty_target >= 5)
			duty_curr -= 5;
	}

	if (millis() - serial_last > 100) {
		Serial.print("dist_ir:");
		Serial.print(dist_ir);
		Serial.print(",pterm:");
		Serial.print(pterm);
		Serial.print(",duty_target:");
		Serial.print(duty_target);
		Serial.print(",duty_curr:");
		Serial.print(duty_curr);
		Serial.println(",Min:100,Low:200,dist_target:255,High:310,Max:410");
		serial_last += 100;
	}

	ser.writeMicroseconds(1610 + pterm);
}


float ir_distance()
{
	float val;
	static float dist_ema;
	float volt = float(analogRead(PIN_IR));
	val = 63158.4/(volt-58.2)-8;
	return dist_ema = 0.1 * val + 0.9 * dist_ema;
}
// vim: ft=cpp ts=4 sw=4 noet
