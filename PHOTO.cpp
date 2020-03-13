#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iostream>

using namespace std;

int main() {

	// declaration des commandes utilisées
	char cmdCam[] = "sudo raspistill -vf -o ";
	char badge[] = "DF8A90B9";

	char cmd[256];  	// pour la ligne de commande permettant le lancement de la prise photo
	time_t temps;  		// pour la lecture du temps
	struct tm datetime; // pour la mise en forme du temps
	char format[32];	// pour mettre en forme horodatage
	time(&temps);
	datetime = *localtime(&temps);
	strftime(format, 32, "-%H:%M_%d-%m-%Y", &datetime);
	
	// mise en forme de la commande systeme de capture de la photo
	strcpy(cmd, cmdCam);
	strcat(cmd, badge);
	strcat(cmd, format);
	strcat(cmd, ".jpeg");
	cout << "execute la commande: " << cmd << endl;

	system(cmd);

	return 0;
}