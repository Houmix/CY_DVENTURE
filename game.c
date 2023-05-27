#include "structures.h"

//Ajoute d'un objet dans l'inventaire du hero
int add_item(Hero* hero, char* itemName, char* SaveFileName) {
    int ok=0;
    FILE* SaveFile = fopen(SaveFileName, "r");
    if (SaveFile == NULL) {
        printf("Erreur lors de l'ouverture du fichier de sauvegarde.\n");
        fclose(SaveFile);
        return 0;
    }
    //Lit le fichier de sauvegarde du hero pour récupérer tous les objets de l'incventaire
    char item1[100];
    char item2[100];
    char item3[100];

    char save_line[1000];

    int choice=0;
    int good=1;

    fgets(save_line, sizeof(save_line), SaveFile);
    fgets(save_line, sizeof(save_line), SaveFile);
    fgets(save_line, sizeof(save_line), SaveFile);

    fgets(save_line, sizeof(save_line), SaveFile);
    //Récupère le nom des objets dans des variable
    fgets(save_line, sizeof(save_line), SaveFile);
    sscanf(save_line, "%[^\n]", item1);
    fgets(save_line, sizeof(save_line), SaveFile);
    sscanf(save_line, "%[^\n]", item2);
    fgets(save_line, sizeof(save_line), SaveFile);
    sscanf(save_line, "%[^\n]", item3);

    //Propose aux joueur de placer son objet
    while(good){

        printf("Voici les items dont vous disposez, vous pouvez en garder que 3.\nFaites un choix pour %s:\n", itemName);
        printf("1/ Supprimer : %s\n", item1);
        printf("2/ Supprimer : %s\n", item2);
        printf("3/ Supprimer : %s\n", item3);
        printf("4/ Ne pas prendre le nouvel item.\n");
        printf("Choisissez une option :\n");
        scanf("%d", &choice);
        //choice de l'item à remplacer ou ne pas l'ajouter
        switch (choice) {
            case 1:
                strcpy(hero->stock.item1, "");
                strcpy(hero->stock.item1, itemName);
                break;
            case 2:
                strcpy(hero->stock.item2, "");
                strcpy(hero->stock.item2, itemName);
                break;
            case 3:
                strcpy(hero->stock.item3, "");
                strcpy(hero->stock.item3, itemName);
                break;
            case 4:
                // Ne rien faire pour l'option 4
                break;
            default:
                printf("choice de l'option incorrect.\n");
                continue; // Revenir au début de la boucle while
        }
        good = 0;
        /*if (choice>=1 && choice<=4){
            good=0;
        }*/  
    }
        fclose(SaveFile);
        return 1;
    }

    
//Mise à jour des stats du perso
int change_stat(Hero* hero, char* bonus_name, int amount){
    if (strcmp(bonus_name, "PV") == 0) {
        hero->life += amount;
        return 1;
    } else if (strcmp(bonus_name, "Force") == 0) {
        hero->power += amount;
        return 1;
    } else if(strcmp(bonus_name,"Or") == 0){
        hero->gold += amount;
        return 1;
    }
    else {
        printf("Stat introuvable\n");
        return 0;
    }
}

//Tirage pour obtenir une puissante arme
void draw(Hero* hero, int choiceNumber, Bonus* bonus, char* SaveFileName) {
    int ok;
    //bonus->numChoices = 0;
    int drawNum=0;
    char itemName[20];
    strcpy(itemName, "Excalibure");
    hero->gold -= GoldgoldDraw;

    if (strstr(bonus->choices[choiceNumber].description, "Tirage") != NULL) {
        //Tirage au sort pour excalibure
        printf("Choisissez un nombre entre 1 et 10 pour le tirage :\n");
        scanf("%d", &drawNum);
        //Verifie que le choice est correct
        if(drawNum<1 || drawNum>10){
            while(1){
                printf("Choix incorrect.\n");
                printf("Choisissez un nombre entre 1 et 10 pour le tirage :\n");
                scanf("%d", &drawNum);
                if (drawNum>0 && drawNum<11){
                    break;
                }
            }
            
        }
        // Initialisation de la graine de génération de nombres aléatoires
        srand(time(NULL));
        // Génération d'un entier aléatoire entre 1 et 10
        int randomNumber = rand() % 10 + 1;
        if (drawNum==randomNumber){
            printf("Bravo ! Vous avez gagné Excalibure.\n");
            //Ajout de l'item dans l'inventaire
            add_item(hero, itemName, SaveFileName);
        } else {
            printf("Dommage... Il vous reste %d pièces d'or\n", hero->gold);
        }

    } else {
        printf("Charlatant : 'Je ne reviendrais plus...'\n");
    }
}


//Ajout du bonus choisi
int applyChoiceBonuses(Hero* hero, int choiceNumber, Bonus* bonus, char* SaveFileName) {
    int ok=0;
    bonus->numChoices = 0;
    Choice chosenChoice = bonus->choices[choiceNumber - 1];

    // Vérifier si le choice commence par "Stat :"
    if (strstr(chosenChoice.description, "Statistique :") != NULL) {
        int bonusValue;
        char statName[20];
        // Extraire les modifications de statistiques
        sscanf(chosenChoice.description, "Statistique : %s +%d", statName, &bonusValue);
        // Appliquer les modifications de statistiques
        ok = change_stat(hero, statName, bonusValue);
        return ok;
    }

    // Vérifier si le choice commence par "Inventaire :"
    if (strstr(chosenChoice.description, "Inventaire :") != NULL) {
        // Extraire les modifications d'inventaire
        char itemName[30];
        sscanf(chosenChoice.description, "Inventaire : %[^\n]", itemName);
        ok = add_item(hero,itemName, SaveFileName);
        return ok;
    }
    return ok;
}