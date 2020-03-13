#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>


#define Test_Porte

#define PinPorte1 15
#define PinPorte2 16

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
		digitalWrite(PinPorte1, HIGH);  // Activé
		digitalWrite(PinPorte2, HIGH);
		delay(1000); // ms
		digitalWrite(PinPorte2, LOW);
		digitalWrite(PinPorte1, LOW);	  // Désactivé
		delay(1000);
	}
	
#endif // Test_Porte



	return 0;
}