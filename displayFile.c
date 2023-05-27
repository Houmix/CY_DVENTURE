#include "structures.h"

void printCharByChar(char* string, unsigned int delay) {
    const char* ptr = string;
    while (*ptr != '\0') {
        putchar(*ptr);
        fflush(stdout);  // Rafraîchir la goldtie pour afficher immédiatement le caractère

        usleep(delay);  // Délai en microsecondes (1 microseconde = 0.000001 seconde)

        ptr++;
    }
}

//Affiche un fichier dans son intégralité
void displayFile(char* nameFichier) {
    FILE* fichier = fopen(nameFichier, "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        fclose(fichier);
    }
    else {
        char ligne[1000];
        while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
            printCharByChar(ligne, 10000);
        }
    }
    fclose(fichier);
}
