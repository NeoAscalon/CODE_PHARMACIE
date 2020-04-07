#pragma once

#include <wiringPi.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <nfc/nfc.h>
#include <time.h>
#include <stdlib.h>


#define Retour_Moniteur
//#define tests			//Switch entre mode de test et mode normal


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
	void SetPresence(bool);
	void clear();
	string RetBadge();
	static string print_hex(const uint8_t*, const size_t);
	bool CapteurPIR();
	bool VerifierPresence();
	bool VerifierAutorisation();
private:
	nfc_device* pnd;
	nfc_target nt;
	nfc_context* context;   // Allocate only a pointer to nfc_context
	nfc_modulation nmMifare;  //norme de lecture de badge
	string  UID; // ID badge
	string cmdCam;
	char cmd[256];  	// pour la ligne de commande permettant le lancement de la prise photo
	time_t temps;  		// pour la lecture du temps
	struct tm datetime; // pour la mise en forme du temps
	char format[32];
	bool presence = false;
};