/*
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

//#define test_PIR

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
		if (!(digitalRead(PIRPin))) {
			delay(500);
		}
		else {
			delay(1000);
			printf("Presence detectée! \n");

		}
	}
#endif // test_PIR

	return 0;
}
*/
#include <stdlib.h>
#include <nfc/nfc.h>
#include <iostream>
#include <string.h>

using namespace std;

static string print_hex(const uint8_t* pbtData, const size_t szBytes)
{
    int j = 0;
    char test[2];
    char chaine[8];
    for (size_t i = 0; i < szBytes; i++)
    {
        sprintf(test, "%02x  ", pbtData[i]);
        chaine[0 + j] = test[0];
        chaine[1 + j] = test[1];
        j = j + 2;
    }
    printf("La chaine donne: %s", chaine);
    return chaine;
}


int main()
{
    nfc_device* pnd;
    nfc_target nt;
    nfc_context* context;   // Allocate only a pointer to nfc_context
        
    nfc_init(&context); // Initialize libnfc and set the nfc_context
    
    if (context == NULL) {
        printf("Unable to init libnfc (malloc)\n");
        exit(EXIT_FAILURE);
    }   //quit if libnfc unable to initialize

    // Open, using the first available NFC device which can be in order of selection:
    //   - default device specified using environment variable or
    //   - first specified device in libnfc.conf (/etc/nfc) or
    //   - first specified device in device-configuration directory (/etc/nfc/devices.d) or
    //   - first auto-detected (if feature is not disabled in libnfc.conf) device

    pnd = nfc_open(context, NULL); //open comunication-enable poll

    if (pnd == NULL) {
        printf("ERROR: %s\n", "Unable to open NFC device.");
        exit(EXIT_FAILURE);
    }   //Quit if unable to open or access device

    
    if (nfc_initiator_init(pnd) < 0) {
        nfc_perror(pnd, "nfc_initiator_init");
        exit(EXIT_FAILURE);
    }   // Set opened NFC device to initiator mode

    const nfc_modulation nmMifare = {
      .nmt = NMT_ISO14443A,
      .nbr = NBR_106,
    };  // Poll for a ISO14443A (MIFARE) tag

    while (true)
    {
        string  chaine;
        if (nfc_initiator_select_passive_target(pnd, nmMifare, NULL, 0, &nt) > 0) {
            chaine=print_hex(nt.nti.nai.abtUid, nt.nti.nai.szUidLen);
           cout << "J'ai lu: " << chaine << endl;
        }
    }

    nfc_close(pnd);// Close NFC device
    nfc_exit(context);// Release the context
    exit(EXIT_SUCCESS);
}


