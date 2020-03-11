#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>


#define Test_Porte

#define PinPorte1 2
#define PinPorte2 3

int main(void)
{
	if (wiringPiSetup() == -1) {
		printf("Setup wiringPi failed !");
		exit(1);
	}

	pinMode(PinPorte1, OUTPUT);
	pinMode(PinPorte2, OUTPUT);

#ifdef Test_Porte
	while (true)
	{
		analogWrite(PinPorte1, HIGH);
		analogWrite(PinPorte2, HIGH);
		delay(2000);
		analogWrite(PinPorte1, LOW);
		analogWrite(PinPorte2, LOW);
		delay(2000);
	}
	
#endif // Test_Porte



	return 0;
}