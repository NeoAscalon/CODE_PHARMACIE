#pragma once

//
//		ATTENTION!
//	Visual N'arrive pas a compiler le code sur la raspberry
// Il faut la compiler manuelement avec la comande "sudo g++ -o TestPharmacie SasDePharmacie.cpp SasDePharmacie.h -lnfc -lwiringPi "
//

#include <wiringPi.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <nfc/nfc.h>
#include <time.h>
#include <stdlib.h>

#define Retour_Moniteur	//Acticve le retour Moniteur si definit
#define tests			//Si definit on entre en mode test sinon en execution normale

#define PIRPin 7			//pin du capteur PIR
#define PinPorte1 15	//Pin du relais de la 1ere porte
#define PinPorte2 16	//Pin du relais de la 2nde porte

using namespace std;

class SasDePharmacie
{
public:
	SasDePharmacie();	//constructeur de la classe
	~SasDePharmacie();	//destructeur de la classe
	void PrisePhoto();		//Cette fonction prend la photo et l'horodate
	void LectureBadge();		//cette fonction lit l'id du badge
	void SetPresence(bool);		//cette fonction definit l'etat de la presence dans le sas
	void clear();		//cette fonction vide UID
	string RetBadge();		//Retourne la valleur d'UID
	static string print_hex(const uint8_t*, const size_t);		//cette fonction est couplee a lecturebadge(), elle localise l'id du badge dans la chaine de caractheres lus depuis le badge et met en forme l'information.
	bool CapteurPIR();	//Cette fonction retourne true si quelqu'un est passé devant le capteur de mouvement
	bool VerifierPresence();		//retourne l'etat de la presence dans le sas
	bool VerifierAutorisation();		//Verifie les autorisations d'acces au sas
private:
	nfc_device* pnd;
	nfc_target nt;
	nfc_context* context;   // Allocate only a pointer to nfc_context
	nfc_modulation nmMifare;  //norme de lecture de badge
	string  UID; // ID badge
	string cmdCam;	//debut de Commande
	char cmd[256];  	// pour la ligne de commande permettant la mise en forme de de la commande et le lancement de la prise photo
	time_t temps;  		// pour la lecture du temps
	struct tm datetime; // pour la mise en forme du temps
	char format[32];	//horodatage de la photo
	bool presence = false;		//presence definit si une personne est presente dans le sas
};