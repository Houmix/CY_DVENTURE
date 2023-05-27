#include "function.h"
int main() {
    FILE* file;
    FILE* DataFile;
    char stepFileName[20];
    char SaveFileName[40];
    int heroVSboss = 0;
    int currentStep = 1;
    int choice = 0;
    char DataFileName[20];
    Hero hero;
    Boss boss;
    int charlatant = 0;
    int userChoice=0;
    int boss_found;
    int sameStep;
    char currentStepFN[10];
    currentStepFN[0] = '\0';
    int win=0;
    char name_in_save[20];
    int heroFound = 0; // Variable pour indiquer si le héros a été trouvé dans le fichier de sauvegarde
    char data_line[1000];
    char save_line[1000];
    int newHero;
    int save_choice=-1;
    int BonusChoice = 0;
    char ready[20];
    int monster = 0;
    int ok=1;





    printf("Entrez le name de votre héro : ");
    scanf("%s", hero.name);
    printf("\nLe nom choisi est : %s\n", hero.name);
    //nom du fichier de sauvegarde dans la variable SaveFileName
    sprintf(SaveFileName, "sauvegarde%s.txt", hero.name);

    
    printf("Ouverture du fichier %s\n", SaveFileName);

    //Recherche si une sauvegarde du joueur existe
    heroFound = open_save_file(SaveFileName);


    //sauvegarde trouvée, on demande au joueur s'il veut commancer une nouvelle partie sinon on récupère ses données
    if (heroFound) {
        ok = get_hero_data(&hero, SaveFileName);
        if(!ok){
            return 0;
        }
        // Afficher les données du héros
        printf("Hero trouvé !\n");
        printf("Souhaitez-vous commancer une nouvelle partie ? 1 pour oui, 0 pour non\n");
        scanf("%d", &save_choice);
        while(save_choice<0 || save_choice>1){
            printf("Choix incorrect\n");
            scanf("%d", &save_choice);
        }
        //Créer un nouveau fichier de sauvegarde initialisé
        if (save_choice==1){
            remove(SaveFileName);
            sprintf(SaveFileName, "sauvegarde%s.txt", hero.name);
            ok = new_hero(&hero,"data.txt",SaveFileName);
            if (!ok){
                return 0;
            }
            printf("Nouvelle sauvegarde créée\n");
        } else {
            //Récupère les données du joueur
            printf("name : %s\n", hero.name);
            printf("life : %d\n", hero.life);
            printf("power : %d\n", hero.power);
            printf("gold : %d\n", hero.gold);
        }
    //Création du fichier de sauvegarde du joueur et initialisation des données du hero 
    } else {
        newHero = new_hero(&hero,"data.txt",SaveFileName);
        if (newHero){
            printf("Sauvegarde du hero créée ");
            printf("name : %s\n", hero.name);
            printf("life : %d\n", hero.life);
            printf("power : %d\n", hero.power);
            printf("gold : %d\n", hero.gold);
        }
        else {
            printf("Erreur lors de la creation du hero\n");
            return 0;
        }
    }
    
    while (1) {
        //Verifie s'il existe une ancienne sauvegarde et dans le cas où le joueur veut et peux reprendre sa sauvegarde
        Step currentStepData;
        Bonus bonus;
        //Initialisation de la graine
        srand(time(NULL));


        if (!save_choice){
            strcpy(currentStepFN,hero.step);
            sprintf(stepFileName, "etape%s.txt", currentStepFN);
            save_choice=1;
        } else {
            generateNextStepFileName(currentStepFN, currentStep);
            sprintf(stepFileName, "etape%s.txt", currentStepFN);
        }


        

        printf("Etape actuelle : %d\n", currentStep);
        printf("Power : %d Life : %d Gold : %d\n", hero.power, hero.life, hero.gold);
      
        printf("Ouverture du fichier %s\n", stepFileName);
        file = fopen(stepFileName, "r");

        if (file == NULL) {
            printf("Impossible d'ouvrir le fichier %s\n", stepFileName);
            break;
        }



        char descriptionTag[] = "# Description";
        
        searchTag(descriptionTag, stepFileName, &currentStepData);

        // char line[1000];

        // // Lire la description
        // while (fgets(line, sizeof(line), file) != NULL) {
        //     if (strstr(line, descriptionTag) != NULL) {
        //         // La ligne correspondant au tag est trouvée
        //         break;
        //     }
        // }
        // //Lecture de la ligne suivante qui contient le texte du tag
        // fgets(currentStepData.description, sizeof(currentStepData.description), file);
        

        bonus.numChoices = 0;
        monster = 0;

        // searchEvent
        // Lire l'événement
        while (fgets(line, sizeof(line), file) != NULL) {
            if (strstr(line, "# Evenement") != NULL ) {
                // La ligne correspondant à "# Evenement" est trouvée
                fgets(currentStepData.event, sizeof(currentStepData.event), file);
                break;
            }
            else if (strstr(line, "# Bonus") != NULL ) {
                BonusChoice = 1;
                // La ligne goldrespondant à "# Bonus" est trouvée
                fgets(currentStepData.event, sizeof(currentStepData.event), file);
                while (fgets(line, sizeof(line), file) != NULL) {
                    
                    if (line[0] == '\n') {
                        break;  // Quitter la boucle interne si on atteint une ligne vide
                    }

                    sscanf(line, "%d. %[^\n]", &bonus.choices[bonus.numChoices].number,
                           bonus.choices[bonus.numChoices].description);
                    bonus.numChoices++;
                }
                break;
            }
            else if (strstr(line, "# Charlatant") != NULL) {
                charlatant = 1;
                // La ligne goldrespondant à "# Bonus" est trouvée
                fgets(currentStepData.event, sizeof(currentStepData.event), file);
                while (fgets(line, sizeof(line), file) != NULL) {
                    if (line[0] == '\n') {
                        break;  // Quitter la boucle interne si on atteint une ligne vide
                    }

                    sscanf(line, "%d. %[^\n]", &bonus.choices[bonus.numChoices].number,
                           bonus.choices[bonus.numChoices].description);
                    bonus.numChoices++;

                }

                break;
            }
            else if (strstr(line, "# Monstre") != NULL){
                monster = 1;
                // La ligne goldrespondant à "# monstre" est trouvée
                fgets(currentStepData.monster, sizeof(currentStepData.monster), file);
                break;
            }
            
        }







        // Lire les choix du fichier
        currentStepData.numChoices = 0;
        while (fgets(line, sizeof(line), file) != NULL) {
            if (strstr(line, "# Choix")) {
                while (fgets(line, sizeof(line), file) != NULL) {
                    if (line[0] == '\n') {
                        break;  // Quitter la boucle interne si on atteint une ligne vide
                    }
                    sscanf(line, "%d. %[^\n]", &currentStepData.choices[currentStepData.numChoices].number,
                           currentStepData.choices[currentStepData.numChoices].description);
                    currentStepData.numChoices++;
                }
                break; 
                // Quitter la boucle externe une fois les choix lus
            }
            else if (strstr(line, "# Combat final")){
                heroVSboss = 1;
                break;
            }
        }



        fclose(file);








        // Afficher la description de l'étape
        printCharByChar(currentStepData.description, 10000);





       
        //Si un monstre est rencontré, alors on lance le combat
        if (monster){
            Hero hero_temp;//hero_temp sera supprimé dès que l'on sortira du bloc if
            hero_temp.power = hero.power;//Copie toutes les données de hero dans hero_temp pour les lui reatribué en vas de victoire avec un boost
            hero_temp.life = hero.life;
            hero_temp.shield = hero.shield;
            printCharByChar(currentStepData.monster, 10000);
            win = fightMonster(&hero);
            if (win){
                printf("Bravo, c'est une belle victoire. Pour vous récompenser, on vous régenère et vous recevez un boost");
                hero.power = hero_temp.power*3;
                hero.life = hero_temp.life*3;
                hero.shield = hero_temp.shield*3;
            }
            else {//Defaite donc retour à 0 (suppression de la sauvegarde car joueur mort)
                displayFile("lose.txt");
                remove(SaveFileName);

                sprintf(SaveFileName, "sauvegarde%s.txt", hero.name);
                new_hero(&hero,"data.txt",SaveFileName);
                printf("%s", hero.name);
                currentStep = 0;
            }
            // Si aucune étape suivante n'est définie, l'histoire est terminée
            if (currentStep == 0) {
                printf("\n--- Fin de l'histoire ---\n");
                break;
            }

        }





        
        // Traiter l'événement (modifier les statistiques, inventaire, etc.)
        if (BonusChoice) {
            for (int i = 0; i < bonus.numChoices; i++) {
                    printf("%d. %s\n", bonus.choices[i].number, bonus.choices[i].description);
                }
            int userChoiceBonus;
            printf("\nChoisissez un bonus : ");
            scanf("%d", &userChoiceBonus);

            // Vérifier la validité du choice
            while(1) {
                if (userChoiceBonus >= 1 && userChoiceBonus <= bonus.numChoices){
                    break;
                }
                printf("Choix du bonus invalide. Veuillez entrer un numéro valide.\n");
                printf("\nChoisissez un bonus : ");
                scanf("%d", &userChoiceBonus);
            }

            applyChoiceBonuses(&hero, userChoiceBonus, &bonus, SaveFileName);

        } else if (charlatant) {
            for (int i = 0; i < bonus.numChoices; i++) {
                    printf("%d. %s\n", bonus.choices[i].number, bonus.choices[i].description);
                }
            if (hero.gold >= GoldgoldDraw){
                int userChoiceBonus;
                printf("\nFaite votre choix : ");
                scanf("%d", &userChoiceBonus);

                // Vérifier la validité du choice
                while(1) {
                    if (userChoiceBonus >= 1 && userChoiceBonus <= bonus.numChoices){
                        break;
                    }
                    printf("Choix du bonus invalide. Veuillez entrer un numéro valide.\n");
                    printf("\nChoisissez un bonus : ");
                    scanf("%d", &userChoiceBonus);
                }

                draw(&hero, userChoiceBonus, &bonus, SaveFileName);
                
                
            } else {
                printf("Malheuresement, vous n'avez pas assez de pièce pour accepter. Vous avez %d pieces. Pensez-y la prochaine fois !\n", hero.gold);
            }
        } else {
            printCharByChar(currentStepData.event, 10000);
        }

        //Initialisation à 0 des valeur indiquant le type de bonus de l'étape
        BonusChoice = 0;
        charlatant = 0;







        //Mise à jour des données du joueur
        ok = db_update(&hero, currentStepFN);
        if(!ok){
            break;
        }







        
        userChoice=0;
        if (!(heroVSboss)){// Demander le choix au lecteur dans le cas où ce n'est pas le combat contre le boss
            printf("herovs boss vaut %d", heroVSboss);
            // Afficher les choix possibles
            for (int i = 0; i < currentStepData.numChoices; i++) {
                printf("%d. %s\n", currentStepData.choices[i].number, currentStepData.choices[i].description);
               
            }
            printf("\nChoisissez une option pour la suite : \n");
            scanf("%d", &userChoice);
            printf("%d",userChoice);
            
            // Vérifier la validité du choice
            while(1) {
                printf("%d\n",userChoice);
                if (userChoice >= 1 && userChoice <= currentStepData.numChoices){
                    break;
                }
                printf("\nChoisissez une option valide: ");
                scanf("%d", &userChoice);
                printf("Choix invalide. Veuillez entrer un numéro valide.\n");
            }
            
            //Obligé de combattre le boss
        } else if (heroVSboss){
            printf("Voici vos stats : \n");
            displayFile(SaveFileName);
            printf("Entrez n'importe quelle valeur dès que vous voulez attaquer");
            scanf("%s", ready);
            printf("\nLe combat debute\n");
            boss_found = get_boss_data(&boss, "boss.txt");
            win = fight(&hero, &boss);
            userChoice=0;
        } else {
            //userChoice = -1;
            printf("Liste des choice inexsitante.\n");
        }







        if (userChoice){//Il y a un choix si le hero n'a pas combatu contre le boss
            //Copie le numero de la derniere etape
            strcpy(hero.step, currentStepFN); 

            //Mise à jour de l'étape, stats et inventaire du hero
            ok = db_update(&hero, SaveFileName);
            if (!ok){
                break;
            }

            // Passer à l'étape suivante
            currentStep = userChoice;

        } else if(userChoice==0){//Il y a eu le combat final
            if (win){
                displayFile("win.txt");
                remove(SaveFileName);

                sprintf(SaveFileName, "sauvegarde%s.txt", hero.name);
                new_hero(&hero,"data.txt",SaveFileName);
                printf("%s", hero.name);
                currentStep = 0;
            }
            else {
                displayFile("lose.txt");
                remove(SaveFileName);

                sprintf(SaveFileName, "sauvegarde%s.txt", hero.name);
                new_hero(&hero,"data.txt",SaveFileName);
                printf("%s", hero.name);
                currentStep = 0;
            }



            // Si aucune étape suivante n'est définie, l'histoire est terminée
            if (currentStep == 0) {
                printf("\n--- Fin de l'histoire ---\n");
                break;
            }
        }
    }

    return 0;
}










functnon.h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define MAX_CHOICE_OPTIONS 10
#define MAX_CHOICE_BONUS 3
#define MAX_STOCK 3
#define GoldgoldDraw 10

typedef struct {
    int number;
    char description[1000];
    char nextStep;
} Choice;

typedef struct {
    char item1[200];
    char item2[200];
    char item3[200];
} Items;

typedef struct {
    char description[1000];
    char event[1000];
    Choice choices[MAX_CHOICE_OPTIONS];
    int numChoices;
    char monster[1000];
} Step;

typedef struct {
    char name[30];
    int life;
    int power;
    char step[20];
    Items stock;
    int gold;
    int shield;
} Hero;

typedef struct {
    char name[30];
    int life;
    int power;
    char weapon[20];
    int shield;
} Boss;

typedef struct {
    char name[30];
    int life;
    int power;
    int shield;
} Monster;
typedef struct {
    char description[200];
    char event[200];
    Choice choices[MAX_CHOICE_BONUS];
    int numChoices;
} Bonus;

void printCharByChar(char* string, unsigned int delay) {
    const char* ptr = string;
    while (*ptr != '\0') {
        putchar(*ptr);
        fflush(stdout);  // Rafraîchir la goldtie pour afficher immédiatement le caractère

        usleep(delay);  // Délai en microsecondes (1 microseconde = 0.000001 seconde)

        ptr++;
    }
}

//Fonction ouvre le fichier contenant toutes les données relatives aux armes et recupère les donnée de l'arme en argument
int get_weapon_power(char* weapon, char* WeaponFile){
    FILE* file= fopen(WeaponFile, "r");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier d'armes.\n");
        fclose(file);
        return -2;
    }
    int power=-1;
    char ligne[1000];
    //Lecture du fichier ligne par ligne
    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        //recherche du nom de l'arme dans chaque ligne
        if (strstr(ligne, weapon) != NULL){
            //Nom de l'arme trouvée, on retourne sa puissance
            fgets(ligne, sizeof(ligne), file);
            sscanf(ligne, "%d", &power);
            //Sortir de la boucle car arme trouvée
            break;
        }
    }
    fclose(file);
    return power;
}

int get_tot_hero_weapon_power(Hero* hero){
    int total_power = 0;
    int p1, p2, p3;
    p1 = get_weapon_power(hero->stock.item1, "weapon_data.txt");
    if (p1==-1){
        printf("L'arme %s est introuvable.\n", hero->stock.item1);
        p1 = 0;
    } else if(p1==-2){
        printf("Ouverture du fichier contenant les infos des armes impossible pour l'item : %s\n", hero->stock.item1);
        p1 = 0;
    }
    p2 = get_weapon_power(hero->stock.item2, "weapon_data.txt");
    if (p2==-1){
        printf("L'arme %s est introuvable.\n", hero->stock.item2);
        p2 = 0;
    } else if(p1==-2){
        printf("Ouverture du fichier contenant les infos des armes impossible pour l'item : %s\n", hero->stock.item2);
        p1 = 0;
    }
    p3 = get_weapon_power(hero->stock.item3, "weapon_data.txt");
    if (p3==-1){
        printf("L'arme %s est introuvable.\n", hero->stock.item3);
        p3 = 0;
    } else if(p3==-2){
        printf("Ouverture du fichier contenant les infos des armes impossible pour l'item : %s\n", hero->stock.item3);
        p3 = 0;
    }
    total_power = p1 + p2 + p3;
    return total_power;

}

int get_tot_boss_weapon_power(Boss* boss){
    int total_power = 0;
    total_power = get_weapon_power(boss->weapon, "weapon_data.txt");
    if (total_power==-1){
        printf("L'arme %s est introuvable.\n", boss->weapon);
        total_power = 0;
    } else if(total_power==-2){
        printf("Ouverture du fichier contenant les infos des armes impossible pour l'item : %s\n", boss->weapon);
        total_power = 0;
    }
    return total_power;
}
//Génère une valeur aléatoire pour l'attribuer au shield des personnages
int get_random_shield(){
    return (rand()%21 + 20);//Génère un nombre aléatoire entre 20 et 40 inclus
}

int get_random_number(){
    return (rand()%11 + 20);//Génère un nombre aléatoire entre 20 et 40 inclus
}

//Affichage stat du perso
void displayHero(Hero* a){
    printf("=== %s ===\n", a->name);
    printf("PWR   = %d\n", a->power);
    printf("LIFE  = %d\n", a->life);
    printf("SHIELD  = %d\n", a->shield);
}
void displayBoss(Boss* a){
    printf("=== %s ===\n", a->name);
    printf("PWR   = %d\n", a->power);
    printf("LIFE  = %d\n", a->life);
    printf("SHIELD  = %d\n", a->shield);
}
void displayMonster(Monster* a){
    printf("=== %s ===\n", a->name);
    printf("PWR   = %d\n", a->power);
    printf("LIFE  = %d\n", a->life);
    printf("SHIELD  = %d\n", a->shield);
}


//Simule le combat. Récupère toutes les données des combatants comme puissance, vie... Puis les persos s'attaque à tour de role.
int fight(Hero* hero, Boss* boss ){
    boss->shield = get_random_shield();//Obtien du shield
    hero->shield = get_random_shield();//Obtien du shield
    printf("Du shield t'as été attribue\n");
    boss->power += get_tot_boss_weapon_power(boss);//Récupère la puissance du perso
    hero->power += get_tot_hero_weapon_power(hero);//Récupère la puissance du perso
    displayBoss(boss);//Affiche les données du perso
    displayHero(hero);//Affiche les données du perso
    int dmg;
    while( (hero->life>0) && (boss->life>0) ){//Vérifie si l'un des perso et mort, si c'est le cas alors on sort du while
        dmg = hero->power - boss->shield;
        if(dmg < 0){//Verifie si le shield a encaissé l'attaque
            boss->shield = -dmg;//On retire les points de dommage du shield
            dmg = 0;
        }
        else {
            boss->shield = 0;//Dans ce cas, le shield est détruit
            boss->life -= dmg;//On retire les points restant de la barre de vie (life)
        }
        if (boss->life<0){//Verifie si boss est mort, si c'est le cas alors on arrete le combat
            break;
        }
        dmg = boss->power - hero->shield;//Même procédé que le précedent
        if(dmg < 0){
            hero->shield = -dmg;
            dmg = 0;
        }
        else {
            hero->shield = 0;
            hero->life -= dmg;
        }
        displayBoss(boss);
        displayHero(hero);
    }             
    //Verifie qui est sortie vivant et on attribue la victoire
    if(hero->life > 0 ){
       displayHero(hero);
       return 1;
    }
    else{
       displayBoss(boss);  
       return 0;  
    }   
}


int fightMonster(Hero* hero){
    Monster monster;
    strcpy(monster.name, "Monster");
    monster.shield = get_random_shield();//Obtien du shield
    monster.power = get_random_number();
    monster.life = get_random_number();
    hero->shield = get_random_shield();//Obtien du shield
    printf("Du shield t'as été attribue\n");
    hero->power += get_tot_hero_weapon_power(hero);//Récupère la puissance du perso
    displayMonster(&monster);//Affiche les données du perso
    displayHero(hero);//Affiche les données du perso
    int dmg;
    while( (hero->life>0) && (monster.life>0) ){//Vérifie si l'un des perso et mort, si c'est le cas alors on sort du while
        dmg = hero->power - monster.shield;
        if(dmg < 0){//Verifie si le shield a encaissé l'attaque
            monster.shield = -dmg;//On retire les points de dommage du shield
            dmg = 0;
        }
        else {
            monster.shield = 0;//Dans ce cas, le shield est détruit
            monster.life -= dmg;//On retire les points restant de la barre de vie (life)
        }
        if (monster.life<0){//Verifie si boss est mort, si c'est le cas alors on arrete le combat
            break;
        }
        dmg = monster.power - hero->shield;//Même procédé que le précedent
        if(dmg < 0){
            hero->shield = -dmg;
            dmg = 0;
        }
        else {
            hero->shield = 0;
            hero->life -= dmg;
        }
        displayMonster(&monster);
        displayHero(hero);
    }             
    //Verifie qui est sortie vivant et on attribue la victoire
    if(hero->life > 0 ){
       displayHero(hero);
       return 1;
    }
    else{
       displayMonster(&monster);
       return 0;  
    }   
}


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


//Affiche un fichier dans son intégralité
void displayFile(char* nameFichier) {
    FILE* fichier = fopen(nameFichier, "r");
    if (fichier == NULL) {
        printf("Erreur golds de l'ouverture du fichier.\n");
        fclose(fichier);
    }
    else {
        char ligne[1000];
        while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
            printf("%s", ligne);
        }
    }
    fclose(fichier);
}



//Le nom des fichiers des étapes est organisé de telle sorte que les nombres des choix soit concaténé par 
//exemple : à l'étape 1, il y a 2 choix : 1 et 2, le nom du fichier étape suivant sera soit etape11 ou etape12
void generateNextStepFileName(char* currentStepFN, int currentStep) {
    char stepNumber[10];
    sprintf(stepNumber, "%d", currentStep);
    strcat(currentStepFN, stepNumber);
    //printf("Le nombre concaténé en chaîne de caractères : %s\n", currentStepFN);
}

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

        printf("Voici les items dont vous disposez, vous pouvez en garder que 3.\nFaites un choice pour %s:\n", itemName);
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
    bonus->numChoices = 0;
    int drawNum=0;
    char itemName[20];
    strcpy(itemName, "Excalibure");
    hero->gold -= GoldgoldDraw;
    Choice chosenChoice = bonus->choices[choiceNumber - 1];
    if (strstr(chosenChoice.description, "Tirage") != NULL) {
        //Tirage au sort pour excalibure
        printf("Choisissez un nombre entre 1 et 10 pour le tirage :\n");
        scanf("%d", &drawNum);
        //Verifie que le choice est correct
        if(drawNum<1 || drawNum>10){
            printf("choice incorrect.\n");
            printf("Choisissez un nombre entre 1 et 10 pour le tirage :\n");
            scanf("%d", &drawNum);
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
        printf("Charlatant : 'Je ne relifendrai plus...'\n");
    }
}


//Ajout du bonus choisi
int applyChoiceBonuses(Hero* hero, int choiceNumber, Bonus* bonus, char* SaveFileName) {
    int ok;
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
    displayFile(dataFileName);

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
    displayFile(SaveFileName);
    return 1;
}


