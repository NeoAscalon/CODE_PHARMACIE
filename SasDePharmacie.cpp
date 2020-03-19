#include "SasDePharmacie.h"

SasDePharmacie::SasDePharmacie()
{
	nfc_init(&context); // Initialize libnfc and set the nfc_context
	
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
		exit(1);
	}

	pinMode(PIRPin, INPUT);
	pinMode(PinPorte1, OUTPUT);
	pinMode(PinPorte2, OUTPUT);

	cmdCam[] = "sudo raspistill -vf -o ";

	time(&temps);


}

SasDePharmacie::~SasDePharmacie()
{
	nfc_exit(context);// Release the context NFC Reader
	exit(EXIT_SUCCESS);
}

void SasDePharmacie::PrisePhoto()
{
	datetime = *localtime(&temps);
	strftime(format, 32, "-%H:%M_%d-%m-%Y", &datetime);
	// mise en forme de la commande systeme de capture de la photo
	strcpy(cmd, cmdCam);
	strcat(cmd, UID.c_str());
	strcat(cmd, format);
	strcat(cmd, ".jpeg");
	cout << "execute la commande: " << cmd << endl;

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

#ifdef test_Porte
			cout << "Badge lu: " << UID << endl;
#endif // test_Porte

		}
	}

	nfc_close(pnd);// Close NFC device
}

void SasDePharmacie::SwitchPorte(bool Presence) //Actionne la porte corespondant a l'etat de la presence locale, C'es Le PIR qui prends le relais ensuite pour le reste de la procedure justqu'a la sortie
{
	if (Presence = true)
	{
		digitalWrite(PinPorte2, HIGH);
		CapteurPIR();
	}
	if (Presence = false)
	{
		digitalWrite(PinPorte1, HIGH);
		CapteurPIR();
	}
#ifdef test_Porte
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
}

void SasDePharmacie::CapteurPIR()
{
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
	//on peut verifier la presence locale a l'aide d'UID
} // a Faire, il faut mettre en place une procedure pour utiliser eficacement le capteur de mouvement PIR et definir l'etat de la presence locale

string SasDePharmacie::print_hex(const uint8_t* pbtData, const size_t szBytes)
{
	short int j = 0;
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

bool SasDePharmacie::VerifierPresence()
{
	return presence;  //retourne l'etat de presence
}

bool SasDePharmacie::VerifierAutorisation()
{
	//A Faire___Attend travail Antoine pour fichier XML autorisations 
	
	return true; //retourne true pour le moment
}

void main() {

	SasDePharmacie a;
	a.LectureBadge();
	while (true) {
		if (a.VerifierPresence == false)
		{
			if (a.VerifierAutorisation == true)
			{
				//prendre la photo
				//actionner la porte
				//changer l'etat de presence a true en utilisant le capteur pir //++faire une fonction qui verifie si quelqu'un a accédé au sas apres avoir badgé et desactive la porte si personne n'est rentré apres n temps
				//une fois que la presence a été detecté ouvrir la seconde porte et verouiller la premiere
				//et inversement si elle ne detecte rien verouille toutes les portes
			}
			else
			{
				cout << "Acces non Autorise!" << endl;
			}
			while (a.VerifierPresence == true)
			{
				//Decider si on le fait par badge ou bouton
				//tant que la personne a l'interieur n'as pas badgé ou appuye sur le bouton presence reste sur true et la porte se verouillé
			}
		}
		else
		{
			cout << "Il y a deja quelq'un!" << endl;
		}
	}
}