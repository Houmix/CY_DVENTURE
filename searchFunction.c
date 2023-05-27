#include "structures.h"


int searchTag(char* Tag, char* NameFile, Step* currentStepData) {
    FILE* file = fopen(NameFile, "r");

    if (file == NULL) {
        printf("Impossible d'ouvrir le fichier %s\n", NameFile);
        return 0;
    }

    char* line = NULL; // Pointeur initialisé à NULL
    size_t lineSize = 0; // Taille initiale de la ligne

    // Lire la description
    while (getline(&line, &lineSize, file) != -1) {
        if (strstr(line, Tag) != NULL) {
            // La ligne correspondant au tag est trouvée
            break;
        }
    }
    fgets(line, sizeof(line), file);
    getline(&line, &lineSize, file);
    // Lecture de la ligne suivante qui contient le texte du tag
    sscanf(line, "%[^\n]", currentStepData->description);
    //printf("voici le texe de la description : %s", currentStepData->description);
    free(line); // Libérer la mémoire allouée dynamiquement

    fclose(file);
    return 1;
}


void searchEvent(FILE* file, Step* currentStepData, bool* hasEvent) {
    char line[1000];

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, "# Evenement") != NULL) {
            fgets(currentStepData->event, sizeof(currentStepData->event), file);
            *hasEvent = true;
            break;
        }
    }
}

void searchBonus(FILE* file, Bonus* bonus, bool* hasBonus) {
    char line[1000];
    bonus->numChoices = 0;
    *hasBonus = true;
    while (fgets(line, sizeof(line), file) != NULL) {
                    
        if (line[0] == '\n') {
            break;  // Quitter la boucle interne si on atteint une ligne vide
        }

        sscanf(line, "%d. %[^\n]", &bonus->choices[bonus->numChoices].number,
                bonus->choices[bonus->numChoices].description);
        bonus->numChoices++;
    }

}

void searchExcalibure(FILE* file, Bonus* bonus, bool* hasExcalibure) {
    char line[1000];
    bonus->numChoices = 0;
    *hasExcalibure = true;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] == '\n') {
            break;  // Quitter la boucle interne si on atteint une ligne vide
        }
        sscanf(line, "%d. %[^\n]", &bonus->choices[bonus->numChoices].number,
            bonus->choices[bonus->numChoices].description);
        bonus->numChoices++; // Incrémenter numChoices ici
    }
}


void searchMonster(FILE* file, Step* currentStepData, bool* hasMonster) {
    char line[1000];
    *hasMonster = true;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] == '\n') {
            break;  // Quitter la boucle interne si on atteint une ligne vide
        }
        fgets(currentStepData->monster, sizeof(currentStepData->monster), file);
        
    }
        
}


void searchChoices(char* fileName, Step* currentStepData, bool* hasChoices, bool* hasHeroVsBoss) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Impossible d'ouvrir le fichier %s\n", fileName);
        return;
    }

    char line[1000];



    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, "# Choix")) {
            
            *hasChoices = true;
            while (fgets(line, sizeof(line), file) != NULL) {
                if (line[0] == '\n') {
                    break;  // Quitter la boucle interne si on atteint une ligne vide
                }
                sscanf(line, "%d. %[^\n]", &currentStepData->choices[currentStepData->numChoices].number,
                       currentStepData->choices[currentStepData->numChoices].description);
                currentStepData->numChoices++;
            }
            break;
        } else if (strstr(line, "# Combat final")) {
            *hasHeroVsBoss = true;
            break;
        }
    }

    fclose(file);
}

void processStepData(FILE* file, Step* currentStepData, Bonus* bonus, bool* hasEvent, bool* hasBonus, bool* hasMonster, bool* hasExcalibure) {
    char line[1000];

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, "# Evenement") != NULL) {
            searchEvent(file, currentStepData, hasEvent);
        } else if (strstr(line, "# Bonus") != NULL) {
            searchBonus(file, bonus, hasBonus);
        } else if (strstr(line, "# Charlatant") != NULL) {
            searchExcalibure(file, bonus, hasExcalibure);
        } else if (strstr(line, "# Monstre") != NULL) {
            searchMonster(file, currentStepData, hasMonster);
        }
    }
    
}

void readStepDataFromFile(char* fileName, Step* currentStepData, Bonus* bonus, bool* hasChoices, bool* hasHeroVsBoss, bool* hasEvent, bool* hasBonus, bool* hasMonster, bool* hasExcalibure) {
    printf("Ouvverture du fichier %s", fileName);
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Impossible d'ouvrir le fichier %s\n", fileName);
        return;
    }
    *hasChoices = false;
    *hasHeroVsBoss = false;
    *hasEvent = false;
    *hasBonus = false;
    *hasMonster = false;
    *hasExcalibure = false;
    
    processStepData(file, currentStepData, bonus, hasEvent, hasBonus, hasMonster, hasExcalibure);

    fclose(file);
    
    
    searchChoices(fileName, currentStepData, hasChoices, hasHeroVsBoss);
    
}
