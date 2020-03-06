#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

#define test_PIR

#define PIRPin 0

int main(void)
{
	
	if (wiringPiSetup() == -1) {
		printf("Setup wiringPi failed !");
		exit(1);
	}
	
	pinMode(PIRPin, INPUT);

#ifdef test_PIR

	printf("\n");
	printf("TEST CAPTEUR PIR:");
	printf("\n");

	while (true) {
		if (digitalRead(PIRPin)) {
			printf("Presence detectée! \n");
			delay(500);
		}
		else {
			delay(1000);
			printf("Presence non detectée! \n");
		}
	}
#endif // test_PIR

	return 0;
}



