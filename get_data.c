#include "structures.h"

// //Fonction ouvre le fichier contenant toutes les données relatives aux armes et recupère les donnée de l'arme en argument
// int get_weapon_power(char* weapon, char* WeaponFile){
//     FILE* file= fopen(WeaponFile, "r");
//     if (file == NULL) {
//         printf("Erreur lors de l'ouverture du fichier d'armes.\n");
//         fclose(file);
//         return -2;
//     }
//     int power=-1;
//     char ligne[1000];
//     //Lecture du fichier ligne par ligne
//     while (fgets(ligne, sizeof(ligne), file) != NULL) {
//         //recherche du nom de l'arme dans chaque ligne
//         if (strstr(ligne, weapon) != NULL){
//             //Nom de l'arme trouvée, on retourne sa puissance
//             fgets(ligne, sizeof(ligne), file);
//             sscanf(ligne, "%d", &power);
//             //Sortir de la boucle car arme trouvée
//             break;
//         }
//     }
//     fclose(file);
//     return power;
// }
//Lecture du fichier contenant les données du boss et l'attribue à l'objet de la structure Boss
int get_boss_data(Boss* boss, char* BossDataFile){
        FILE* SaveFile = fopen(BossDataFile, "r");
        if (SaveFile == NULL) {
            printf("Erreur golds de l'ouverture du fichier de sauvegarde.\n");
            fclose(SaveFile);
            return 0;
        }

        char data_line[1000];
        //Recupreration des données du perso
        while (fgets(data_line, sizeof(data_line)-1, SaveFile) != NULL) {
            
            sscanf(data_line, "%s", boss->name);
            fgets(data_line, sizeof(data_line), SaveFile);
            sscanf(data_line, "%d", &boss->life);
            fgets(data_line, sizeof(data_line), SaveFile);
            sscanf(data_line, "%d", &boss->power);

            fgets(data_line, sizeof(data_line), SaveFile);
            sscanf(data_line, "%[^\n]", boss->weapon);
        }
        fclose(SaveFile);
        return 1;
}

//Lecture du fichier contenant les données du boss et l'attribue à l'objet de la structure Hero
int get_hero_data(Hero* hero, char* SaveFileName){
    FILE* SaveFile = fopen(SaveFileName, "r");
    if (SaveFile == NULL) {
        printf("Erreur lors de l'ouverture du fichier de sauvegarde.\n");
        fclose(SaveFile);
        return 0;
    }

    char data_line[1000];
    //Recupreration des données du perso
    while (fgets(data_line, sizeof(data_line)-1, SaveFile) != NULL) {
        
        sscanf(data_line, "%s", hero->name);
        fgets(data_line, sizeof(data_line), SaveFile);
        sscanf(data_line, "%d", &hero->life);
        fgets(data_line, sizeof(data_line), SaveFile);
        sscanf(data_line, "%d", &hero->power);
        fgets(data_line, sizeof(data_line), SaveFile);
        sscanf(data_line, "%s", hero->step);


        fgets(data_line, sizeof(data_line), SaveFile);
        sscanf(data_line, "%[^\n]", hero->stock.item1);
        fgets(data_line, sizeof(data_line), SaveFile);
        sscanf(data_line, "%[^\n]", hero->stock.item2);
        fgets(data_line, sizeof(data_line), SaveFile);
        sscanf(data_line, "%[^\n]", hero->stock.item3);
        fgets(data_line, sizeof(data_line), SaveFile);
        sscanf(data_line, "%d", &hero->gold);    
    }
    fclose(SaveFile);
    return 1;
}