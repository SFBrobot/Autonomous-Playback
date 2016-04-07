#pragma config(Sensor, dgtl1,  ,               sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  ,               sensorQuadEncoder)
#pragma config(Motor,  port2,           leftMotor,     tmotorVex393TurboSpeed_MC29, openLoop, driveLeft, encoderPort, dgtl1)
#pragma config(Motor,  port9,           rightMotor,    tmotorVex393TurboSpeed_MC29, openLoop, reversed, driveRight, encoderPort, dgtl3)
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

task main()
{
	int rx,
		ry,
		lx,
		ly,
		lEnc,
		rEnc,
		lEncVals[STORE_MAX],
		rEncVals[STORE_MAX],
		storeCount;

	bool arcade,
		split,
		reversed;

	word arcadeLast,
		splitLast,
		reversedLast,
		storeLast;

	while(true) {
		rx = (vexRT[ChRX] > STICK_THRESH) ? vexRT[ChRX] : 0;
		ry = (vexRT[ChRY] > STICK_THRESH) ? vexRT[ChRY] : 0;
		ly = (vexRT[ChLY] > STICK_THRESH) ? vexRT[ChLY] : 0;
		lx = (vexRT[ChLX] > STICK_THRESH) ? vexRT[ChLX] : 0;

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

		if(vexRT[NUM_STORE] != storeLast) {
			storeLast = vexRT[NUM_STORE];

			lEncVals[storeCount % STORE_MAX] = lEnc;
			rEncVals[storeCount % STORE_MAX] = rEnc;
		
			if(storeCount >= STORE_MAX) {
				displayLCDCenteredString(0, "%d VALUES STORED", STORE_MAX);
				displayLCDCenteredString(1, "OVERWRITING");
				
				bLCDBacklight = !bLCDBacklight;
				
				wait1Msec(50);
				
				bLCDBacklight = !bLCDBacklight;
			}
			
			storeCount++;
			
		}

		wait1Msec(20);
	}
}
