#include <stdlib.h>
#include <nfc/nfc.h>
#include <iostream>
#include <string.h>

using namespace std;

static string print_hex(const uint8_t* pbtData, const size_t szBytes)
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
    buffer[1] = 0;
    return chaine;
} //returns NFC tag UID


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
        string  UID;
        if (nfc_initiator_select_passive_target(pnd, nmMifare, NULL, 0, &nt) > 0) {
            UID = print_hex(nt.nti.nai.abtUid, nt.nti.nai.szUidLen);
        }
    }

    nfc_close(pnd);// Close NFC device
    nfc_exit(context);// Release the context
    exit(EXIT_SUCCESS);
}