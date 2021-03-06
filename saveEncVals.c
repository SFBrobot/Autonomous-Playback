#pragma config(Sensor, dgtl1,  brWheelEnc,     sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  frWheelEnc,     sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  flWheelEnc,     sensorQuadEncoder)
#pragma config(Sensor, dgtl7,  blWheelEnc,     sensorQuadEncoder)
#pragma config(Motor,  port2,           flWheel,       tmotorVex393TurboSpeed_MC29, openLoop, driveLeft, encoderPort, dgtl5)
#pragma config(Motor,  port3,           frWheel,       tmotorVex393TurboSpeed_MC29, openLoop, reversed, driveRight, encoderPort, dgtl3)
#pragma config(Motor,  port4,           blWheel,       tmotorVex393TurboSpeed_MC29, openLoop, driveLeft, encoderPort, dgtl7)
#pragma config(Motor,  port5,           brWheel,       tmotorVex393TurboSpeed_MC29, openLoop, reversed, driveRight, encoderPort, dgtl1)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define ChRX Ch1
#define ChRY Ch2
#define ChLY Ch3
#define ChLX Ch4

#define ARCADE_BTN Btn8U
#define REVERSED_BTN Btn8D
#define SPLIT_BTN Btn8L

#define NUM_STORE Btn8R

#define STICK_THRESH 15

#define STORE_MAX 10

#define HOLONOMIC

typedef struct {
	float encVal,
		valLast,
		time,
		timeLast,
		deltaEnc,
		deltaLast,
		accel,
		savedVals[STORE_MAX];
	tSensors name;
	bool reversed;
} Encoder;

Encoder flEnc,
	frEnc,
	blEnc,
	brEnc;

int saveCount = 0;

void updateEnc(Encoder *enc) {
	enc->valLast = enc->encVal;
	enc->timeLast = enc->time;
	enc->deltaLast = 1.0 * enc->deltaEnc;
	
	enc->encVal = SensorValue[enc->name];
	enc->time = nSysTime;
	
	enc->deltaEnc = 1.0 * (enc->encVal - enc->valLast) / (enc->time - enc->timeLast);
	
	enc->accel = 1.0 * (enc->deltaEnc - enc->deltaLast) / (enc->time - enc->timeLast);
}

void saveEncVal(Encoder *enc) {
	enc->savedVals[saveCount] = enc->encVal;
}

task main() {
	int rx,
		ry,
		lx,
		ly;
#ifndef HOLONOMIC
	bool arcade,
		split,
		reversed;
#endif

#ifdef HOLONOMIC
	word storeLast;
#else
	word arcadeLast,
		splitLast,
		reversedLast,
		storeLast;
#endif

flEnc.name = flWheelEnc;
frEnc.name = frWheelEnc;
blEnc.name = blWheelEnc;
brEnc.name = brWheelEnc;

	while(true) {
		rx = (abs(vexRT[ChRX]) > STICK_THRESH) ? vexRT[ChRX] : 0;
		ry = (abs(vexRT[ChRY]) > STICK_THRESH) ? vexRT[ChRY] : 0;
		ly = (abs(vexRT[ChLY]) > STICK_THRESH) ? vexRT[ChLY] : 0;
		lx = (abs(vexRT[ChLX]) > STICK_THRESH) ? vexRT[ChLX] : 0;

		updateEnc(&flEnc);
		updateEnc(&frEnc);
		updateEnc(&blEnc);
		updateEnc(&brEnc);

#ifdef HOLONOMIC
    motor[flWheel] = ly + rx + lx;
    motor[frWheel] = ly - rx - lx;
    motor[blWheel] = ly + rx - lx;
    motor[brWheel] = ly - rx + lx;
#else
		if(vexRT[ARCADE_BTN] != arcadeLast) {
			arcadeLast = vexRT[ARCADE_BTN];
			arcade = !arcade;
		}

		if(vexRT[SPLIT_BTN] != splitLast) {
			splitLast = vexRT[SPLIT_BTN];
			split = !split;
		}

		if(vexRT[REVERSED_BTN] != reversedLast) {
			reversedLast = vexRT[REVERSED_BTN];
			reversed = !reversed;
		}


		if(arcade) {
			if(split) {
				motor[leftMotor] = reversed ? rx - ly : ly + rx;

				motor[rightMotor] = reversed ? -ly - rx : ly - rx;
			}

			else {
				motor[leftMotor] = reversed ? lx - ly : ly + lx;

				motor[rightMotor] = reversed ? -ly - lx : ly - lx;
			}
		}

		else {
			motor[leftMotor] = reversed ? -ry : ly;
			motor[rightMotor] = reversed ? -ly : ry;
		}
#endif

		if(vexRT[NUM_STORE] != storeLast) {
			storeLast = vexRT[NUM_STORE];
			
			if(saveCount < STORE_MAX) {
				saveEncVal(&flEnc);
				saveEncVal(&frEnc);
				saveEncVal(&blEnc);
				saveEncVal(&brEnc);
				
				saveCount++;
			}
			
		}

		wait1Msec(20);
	}
}
