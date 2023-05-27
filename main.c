#include "function.h"
int main() {
    char stepFileName[20];
    char SaveFileName[20];
    int currentStep = 1;

    Hero hero;
    Boss boss;
    int userChoice=0;

    char currentStepFN[20];
    currentStepFN[0] = '\0';
    int win=0;
    int heroFound = 0; // Variable pour indiquer si le héros a été trouvé dans le fichier de sauvegarde
    int boss_found;
    int newHero;
    int save_choice=-1;


    int ok=1;
    int userChoiceBonus;
    bool hasChoices;
    bool hasHeroVsBoss;
    bool hasEvent;
    bool hasBonus;
    bool hasMonster ;
    bool hasExcalibure;
    hasChoices = false;
    hasHeroVsBoss = false;
    hasEvent = false;
    hasBonus = false;
    hasMonster = false;
    hasExcalibure = false;


    printf("Entrez le name de votre héro : ");
    scanf("%s", hero.name);
    
    //nom du fichier de sauvegarde dans la variable SaveFileName
    sprintf(SaveFileName, "sauvegarde%s.txt", hero.name);

    
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
        printf("Souhaitez-vous commancer une nouvelle partie ? oui : 1 non : 0\n");
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
            printf("Nom : %s\n", hero.name);
            printf("PV : %d\n", hero.life);
            printf("Force : %d\n", hero.power);
            printf("Or : %d\n", hero.gold);
        }
    //Création du fichier de sauvegarde du joueur et initialisation des données du hero 
    } else {
        newHero = new_hero(&hero,"data.txt",SaveFileName);
        if (newHero){
            printf("Sauvegarde du hero créée\n");
            printf("Nom : %s\n", hero.name);
            printf("PV : %d\n", hero.life);
            printf("Force : %d\n", hero.power);
            printf("Or : %d\n", hero.gold);
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


        

        printf("Fore : %d PV : %d or : %d\n", hero.power, hero.life, hero.gold);

        char descriptionTag[] = "# Description";
        
        searchTag(descriptionTag, stepFileName, &currentStepData);

        bonus.numChoices = 0;
        currentStepData.numChoices = 0;

        readStepDataFromFile(stepFileName, &currentStepData, &bonus, &hasChoices, &hasHeroVsBoss, &hasEvent, &hasBonus, &hasMonster, &hasExcalibure);

        // Afficher la description de l'étape
        printCharByChar(currentStepData.description, 10000);
        printf("\n");




        //Si un monstre est rencontré, alors on lance le combat
        if (hasMonster){

            Hero hero_temp;//hero_temp sera supprimé dès que l'on sortira du bloc if
            hero_temp.power = hero.power;//Copie toutes les données de hero dans hero_temp pour les lui reatribué en vas de victoire avec un boost
            hero_temp.life = hero.life;
            hero_temp.shield = hero.shield;

            printCharByChar(currentStepData.monster, 10000);
            printf("\n");

            win = fightMonster(&hero);
            if (win){
                printf("Bravo, c'est une belle victoire. Pour vous récompenser, on vous régenère et vous recevez un boost\n");
                hero.power = hero_temp.power*3;
                hero.life = hero_temp.life*3;
                hero.shield = hero_temp.shield*3;
            }
            else {//Defaite donc retour à 0 (suppression de la sauvegarde car joueur mort)
                displayFile("lose.txt");
                remove(SaveFileName);
                currentStep = 0;
            }
            // Si aucune étape suivante n'est définie, l'histoire est terminée
            if (currentStep == 0) {
                printf("\n--- Fin de l'histoire ---\n");
                break;
            }

        }

        // Traiter l'événement (modifier les statistiques, inventaire, etc.)
        if (hasBonus) {
            for (int i = 0; i < bonus.numChoices; i++) {
                    printf("%d.", bonus.choices[i].number);
                    printCharByChar(bonus.choices[i].description, 10000);
                    printf("\n");
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
                while (scanf("%d", &userChoiceBonus) != 1){
                    // Effacer le flux d'entrée en cas de saisie invalide
                    while (getchar() != '\n');
                    printf("Saisie invalide. Veuillez entrer un numéro valide : ");
                }
            }

            applyChoiceBonuses(&hero, userChoiceBonus, &bonus, SaveFileName);

        } else if (hasExcalibure) {
            
            for (int i = 1; i < bonus.numChoices; i++) {
                    printf("%d.", bonus.choices[i].number);
                    printCharByChar(bonus.choices[i].description, 10000);
                    printf("\n");
                }
            if (hero.gold >= GoldgoldDraw){
                printf("\nFaites votre choix : ");
                scanf("%d", &userChoiceBonus);
                
                


                // Vérifier la validité du choice
                while(1) {
                    if (userChoiceBonus >= 1 && userChoiceBonus <= (bonus.numChoices-1)){
                        break;
                    }
                    printf("Choix du bonus invalide. Veuillez entrer un numéro valide.\n");
                    printf("\nChoisissez un bonus : ");
                    
                    while (scanf("%d", &userChoiceBonus) != 1){
                        // Effacer le flux d'entrée en cas de saisie invalide
                        while (getchar() != '\n');
                        printf("Saisie invalide. Veuillez entrer un numéro valide : ");
                    }
                    
                }
                draw(&hero, userChoiceBonus, &bonus, SaveFileName);
                
                
            } else {
                printf("Malheuresement, vous n'avez pas assez de pièce pour accepter. Vous avez %d pieces. Pensez-y la prochaine fois !\n", hero.gold);
            }
        } else if (hasEvent) {
            printCharByChar(currentStepData.event, 10000);
            printf("\n");
        } else {
            printf("Rien à affichier pour cette étape.\n");
        }

        //Mise à jour des données du joueur
        ok = db_update(&hero, currentStepFN);
        if(!ok){
            printf("Mise à jour de la bd impossible");
        }


        
        if (hasChoices){// Demander le choix au lecteur dans le cas où ce n'est pas le combat contre le boss
            // Afficher les choix possibles
            for (int i = 0; i < currentStepData.numChoices; i++) {
                printf("%d. ", currentStepData.choices[i].number);
                printCharByChar(currentStepData.choices[i].description, 10000);
                printf("\n");
               
            }
            printf("\nChoisissez une option pour la suite : \n");
            while ((scanf("%d", &userChoice) != 1) || (userChoice < 1 || userChoice > currentStepData.numChoices)) {
                // Effacer le flux d'entrée en cas de saisie invalide
                while (getchar() != '\n');
                printf("Saisie invalide. Veuillez entrer un numéro valide : ");
            }

        
            
            //Obligé de combattre le boss
        } else if (hasHeroVsBoss){
            printf("Entrez n'importe quelle valeur dès que vous voulez attaquer");
            getchar(); // Attend que l'utilisateur appuie sur une touche
            printCharByChar("\nLe combat debute\n", 10000);
            boss_found = get_boss_data(&boss, "boss.txt");
            if (!boss_found){
                printf("Erreur ouverture fichier du boss\n");
            } 
            win = fight(&hero, &boss);
            userChoice=0;
        } else {
            printf("Liste des choix inexsitante.\n");
        }

        if (userChoice){//Il y a un choix si le hero n'a pas combatu contre le boss
            //Copie le numero de la derniere etape
            strcpy(hero.step, currentStepFN); 

            //Mise à jour de l'étape, stats et inventaire du hero
            ok = db_update(&hero, SaveFileName);
            if (!ok){
                break;
            }
            // specifier le prochain fichier et passer à l'étape suivante
            currentStep = userChoice;

        } else if(userChoice==0){//Il y a eu le combat final
            if (win){
                displayFile("win.txt");
                remove(SaveFileName);
                currentStep = 0;
            }
            else {
                displayFile("lose.txt");
                remove(SaveFileName);
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
