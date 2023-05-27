#include "structures.h"

//Ouvre le fichier de sauvegarde pour savoir si le joueur existe déjà
int open_save_file(char* SaveFileName){
    
    FILE* SaveFile = fopen(SaveFileName, "r");
    if (SaveFile == NULL) {
        printf("Erreur lors de l'ouverture du fichier de sauvegarde.\n");
        fclose(SaveFile);
        return 0;
    }
    fclose(SaveFile);
    return 1;
}

//Le nom des fichiers des étapes est organisé de telle sorte que les nombres des choix soit concaténé par 
//exemple : à l'étape 1, il y a 2 choix : 1 et 2, le nom du fichier étape suivant sera soit etape11 ou etape12
void generateNextStepFileName(char* currentStepFN, int currentStep) {
    char stepNumber[10];
    sprintf(stepNumber, "%d", currentStep);
    strcat(currentStepFN, stepNumber);
    //printf("Le nombre concaténé en chaîne de caractères : %s\n", currentStepFN);
}

//Mise à jour du fichier de sauvegarde d'un joueur à l'aide d'un fichier temporaire
int db_update(Hero* hero, char* SaveFileName) {
    //Création du fichier temporaire
    FILE* TempFile = fopen("temp.txt", "w");
    if (TempFile == NULL) {
        printf("Erreur lors de l'ouverture du fichier de sauvegarde.\n");
        return 0;
    }
    //Ecriture de toutes les données du joueur dans le fichier temporaire
    fprintf(TempFile,"%s\n", hero->name);
    fprintf(TempFile, "%d\n", hero->life);
    fprintf(TempFile, "%d\n", hero->power);
    fprintf(TempFile, "%s\n", hero->step);

    
    fprintf(TempFile, "%s\n", hero->stock.item1);
    fprintf(TempFile, "%s\n", hero->stock.item2);
    fprintf(TempFile, "%s\n", hero->stock.item3);

    fprintf(TempFile,"%d", hero->gold);


    fclose(TempFile);
    //Suppression du fichier de sauvegarde du joueur
    remove(SaveFileName);
    //Changer le nom du fichier temporaire en celui du sauvegarde du joueur
    rename("temp.txt",SaveFileName);
    return 1;
}

//Création du fichier de sauvegarde du joueur si on arrive à la fin du jeu ou si le joueur n'existe pas
int new_hero(Hero* hero, char* dataFileName, char* SaveFileName ){
    // Ouvrir le fichier data.txt pour avoir les données d'un nouveau personnage
    FILE* DataFile = fopen(dataFileName, "r");
    
    if (DataFile == NULL) {
        printf("Le fichier data.txt n'existe pas\n");
        fclose(DataFile);
        return 0;
    }
    char data_line[200];
    while (fgets(data_line, sizeof(data_line), DataFile) != NULL) {
        sscanf(data_line, "%d", &hero->life);
        
        fgets(data_line, sizeof(data_line), DataFile);
        sscanf(data_line, "%d", &hero->power);
        
        fgets(data_line, sizeof(data_line), DataFile);
        sscanf(data_line, "%s", hero->step);
        
        fgets(data_line, sizeof(data_line), DataFile);
        sscanf(data_line, "%[^\n]", hero->stock.item1);
        
        fgets(data_line, sizeof(data_line), DataFile);
        sscanf(data_line, "%[^\n]", hero->stock.item2);
        
        fgets(data_line, sizeof(data_line), DataFile);
        sscanf(data_line, "%[^\n]", hero->stock.item3);

        fgets(data_line, sizeof(data_line), DataFile);
        sscanf(data_line, "%d", &hero->gold);
    }

    fclose(DataFile);

    // Sauvegarder les données du nouveau personnage dans son nouveau fichier de sauvegarde

    FILE* SaveFile = fopen(SaveFileName, "w");
    fprintf(SaveFile, "%s\n", hero->name);
    fprintf(SaveFile, "%d\n", hero->life);
    fprintf(SaveFile, "%d\n", hero->power);
    fprintf(SaveFile, "%s\n", hero->step);
    fprintf(SaveFile, "%s\n", hero->stock.item1);
    fprintf(SaveFile, "%s\n", hero->stock.item2);
    fprintf(SaveFile, "%s\n", hero->stock.item3);
    fprintf(SaveFile, "%d\n", hero->gold);
    

    fclose(SaveFile);
    return 1;
}
