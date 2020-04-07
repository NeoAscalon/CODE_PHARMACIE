#include "SasDePharmacie.h"

SasDePharmacie::SasDePharmacie()
{
	nfc_init(&context); // Initialize libnfc and set the nfc_context

	UID = "";

	if (context == NULL) {
		printf("Unable to init libnfc (malloc)\n");
		exit(EXIT_FAILURE);
	}   //quit if libnfc unable to initialize

	nmMifare = {
	  .nmt = NMT_ISO14443A,
	  .nbr = NBR_106,
	}; // Poll for a ISO14443A (MIFARE) tag

	if (wiringPiSetup() == -1) {
		printf("Setup wiringPi failed !");
		exit(EXIT_FAILURE);
	}

	pinMode(PIRPin, INPUT);
	pinMode(PinPorte1, OUTPUT);
	pinMode(PinPorte2, OUTPUT);

	cmdCam = "sudo raspistill -vf -o /Evenements/";

}

SasDePharmacie::~SasDePharmacie()
{
	nfc_exit(context);// Release the context NFC Reader
}

void SasDePharmacie::PrisePhoto()
{
	// mise en forme de la commande systeme de capture de la photo
	time(&temps);
	datetime = *localtime(&temps);
	strftime(format, 32, "-%H:%M_%d-%m-%Y", &datetime);
	strcpy(cmd, cmdCam.c_str());
	strcat(cmd, UID.c_str());
	strcat(cmd, format);
	strcat(cmd, ".jpeg");

#ifdef Retour_Moniteur
	cout << "execute la commande: " << cmd << endl;
#endif // Retour_Moniteur

	system(cmd);

}

void SasDePharmacie::LectureBadge()
{
	pnd = nfc_open(context, NULL); //open comunication-enable poll

	if (pnd == NULL) {
		printf("ERROR: %s\n", "Unable to open NFC device.");
		exit(EXIT_FAILURE);
	}   //Quit if unable to open or access device

	if (nfc_initiator_init(pnd) < 0) {
		nfc_perror(pnd, "nfc_initiator_init");
		exit(EXIT_FAILURE);
	}   // Set opened NFC device to initiator mode

	if (nfc_initiator_select_passive_target(pnd, nmMifare, NULL, 0, &nt) > 0) {
		if (UID != print_hex(nt.nti.nai.abtUid, nt.nti.nai.szUidLen)) {
			UID = print_hex(nt.nti.nai.abtUid, nt.nti.nai.szUidLen);

#ifdef Retour_Moniteur
			cout << "Badge lu: " << UID << endl;
#endif // Retour_Moniteur

		}
	}

	nfc_close(pnd);// Close NFC device
}

string SasDePharmacie::print_hex(const uint8_t* pbtData, const size_t szBytes)
{
	int j = 0;
	char buffer[1];
	char chaine[7];
	for (size_t i = 0; i < szBytes; i++)
	{
		sprintf(buffer, "%02x", pbtData[i]);
		chaine[0 + j] = buffer[0];
		chaine[1 + j] = buffer[1];
		j = j + 2;
	}
	buffer[0] = 0;
	buffer[1] = 0;  //Nettoie le buffer
	return chaine;
} //returns NFC tag UID

void SasDePharmacie::SetPresence(bool RP)
{
	presence = RP;
}

void SasDePharmacie::clear()
{
	UID = "";
}

string SasDePharmacie::RetBadge()
{
	return UID;
}

bool SasDePharmacie::CapteurPIR() //retourne si le capteur a detecté quelqu'un
{
	if (!digitalRead(PIRPin)) {
		return false;
	}
	else {
		return true;
	}
}

bool SasDePharmacie::VerifierPresence()
{
	return presence;  //retourne l'etat de presence
}

bool SasDePharmacie::VerifierAutorisation()
{
	//A Faire___Attend travail Antoine pour fichier XML autorisations

	return true; //retourne true pour le moment
}

int main() {

	SasDePharmacie a;
	digitalWrite(PinPorte1, LOW);
	digitalWrite(PinPorte2, LOW);

#ifdef tests
	bool x = false;
	while (true) {
		while (a.RetBadge() == "")
		{
			a.LectureBadge();
		}
		cout << "Sorti de while test lecture badge!" << endl;
		while (x == false)
		{
			a.PrisePhoto();
			x = true;
		}
		cout << "Photo prise!" << endl;


		cout << "TEST CAPTEUR PIR:" << endl;
		while (true) {
			if (!digitalRead(PIRPin)) {
				cout << "Presence non detectée!" << endl;
				delay(1000);
			}
			else {
				cout << "Presence detectée!" << endl;
				delay(1000);
			}
		}

		exit(EXIT_SUCCESS);
	}
#endif // tests

#ifndef tests
	while (true)
	{
#ifdef Retour_Moniteur
		cout << "Passez un badge!" << endl;
#endif // Retour_Moniteur
		do
		{
			a.LectureBadge();
		} while (a.RetBadge() == "");
		if (a.VerifierPresence() == false)
		{
			if (a.VerifierAutorisation() == true)
			{
				digitalWrite(PinPorte1, HIGH);
				int tempo = 0;
				do
				{

#ifdef Retour_Moniteur
					cout << "Il n'y a personne!" << endl;
#endif // Retour_Moniteur

					delay(1000);
					tempo++;
				} while (a.CapteurPIR() == false && tempo < 20);  //ok

				if (tempo < 20) {

#ifdef Retour_Moniteur
					cout << "Quelq'un est rentre!" << endl;
#endif // Retour_Moniteur

					a.PrisePhoto();

#ifdef Retour_Moniteur
					cout << "Photo prise!" << endl;
#endif // Retour_Moniteur

					a.SetPresence(true);
					digitalWrite(PinPorte1, LOW);
					digitalWrite(PinPorte2, HIGH);
				}
			}
			else
			{
				cout << "Acces non Autorise!" << endl;
			}
			a.clear();
			do
			{

#ifdef Retour_Moniteur
				if (a.VerifierPresence() == true) {
					cout << "Dans la Pharmacie!" << endl;
					delay(1000);
				}
#endif // Retour_Moniteur

				digitalWrite(PinPorte1, LOW);
				digitalWrite(PinPorte2, LOW);
				if (a.VerifierPresence() == true) {
					a.LectureBadge();
					a.SetPresence(false);
				}
			} while (a.VerifierPresence() == true && a.RetBadge() == "");
		}
		else
		{
			cout << "Il y a deja quelq'un!" << endl;
		}
		a.clear();
	}
#endif // !tests

	exit(EXIT_SUCCESS);
}