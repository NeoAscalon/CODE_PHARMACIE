#pragma once
#include <wiringPi.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <nfc/nfc.h>
#include <time.h>
#include <stdlib.h>

//#define test_NFC  //fait
//#define test_PIR   //fait
//#define test_Porte  //fait
//#define test_Photo //a faire

#define PIRPin 7
#define PinPorte1 15
#define PinPorte2 16

using namespace std;

class SasDePharmacie
{
public:
	SasDePharmacie();
	~SasDePharmacie();
	void PrisePhoto();
	void LectureBadge();
	void SwitchPorte();
	void CapteurPIR();
	static string print_hex(const uint8_t*, const size_t);
private:
	string  UID;
	nfc_device* pnd;
	nfc_target nt;
	nfc_context* context;   // Allocate only a pointer to nfc_context
	nfc_modulation nmMifare;
	string  UID; //badge
	char cmdCam[];
	char cmd[256];  	// pour la ligne de commande permettant le lancement de la prise photo
	time_t temps;  		// pour la lecture du temps
	struct tm datetime; // pour la mise en forme du temps
	char format[32];
	

};

