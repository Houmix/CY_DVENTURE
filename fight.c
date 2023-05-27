#include "structures.h"
//Affichage stat du perso
void displayHero(Hero* a){
    printf("=== %s ===\n", a->name);
    sleep(1); // Fait une pause de 1 seconde
    printf("Force   = %d\n", a->power);
    printf("Vie  = %d\n", a->life);
    printf("Shield  = %d\n", a->shield);
    sleep(2); // Fait une pause de 1 seconde
}
void displayBoss(Boss* a){
    printf("=== %s ===\n", a->name);
    sleep(1);
    printf("Force   = %d\n", a->power);
    printf("Vie  = %d\n", a->life);
    printf("Shield  = %d\n", a->shield);
    sleep(2);
}
void displayMonster(Monster* a){
    printf("=== %s ===\n", a->name);
    sleep(1);
    printf("Force   = %d\n", a->power);
    printf("Vie  = %d\n", a->life);
    printf("Shield  = %d\n", a->shield);
    sleep(2);
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


//Génère une valeur aléatoire pour l'attribuer au shield des personnages
int get_random_shield(){
    return (rand()%21 + 20);//Génère un nombre aléatoire entre 20 et 40 inclus
}

int get_random_number(){
    return (rand()%11 + 20);//Génère un nombre aléatoire entre 20 et 40 inclus
}


//Simule le combat. Récupère toutes les données des combatants comme puissance, vie... Puis les persos s'attaque à tour de role.
int fight(Hero* hero, Boss* boss ){
    boss->shield = get_random_shield();//Obtien du shield
    hero->shield = get_random_shield();//Obtien du shield
    printf("Du shield t'as été attribue\n");
    boss->power += get_tot_boss_weapon_power(boss);//Récupère la puissance du perso
    hero->power += get_tot_hero_weapon_power(hero);//Récupère la puissance du perso
    printf("Voici les stats :");
    sleep(1);
    displayBoss(boss);//Affiche les données du perso
    sleep(1);
    displayHero(hero);//Affiche les données du perso
    sleep(1);
    printf("Le combat commance");
    int dmg = 0;
    while( (hero->life>0) && (boss->life>0) ){//Vérifie si l'un des perso et mort, si c'est le cas alors on sort du while
        dmg = hero->power - boss->shield;
        printf("Vous avez infligé %d dégats au boss", dmg);
        sleep(1);
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
        printf("Le boss vous a infligé %d dégats", dmg);
        sleep(1);
        if(dmg < 0){
            hero->shield = -dmg;
            dmg = 0;
        }
        else {
            hero->shield = 0;
            hero->life -= dmg;
        }
        displayBoss(boss);
        sleep(1);
        displayHero(hero);
        sleep(1);
    }             
    //Verifie qui est sortie vivant et on attribue la victoire
    if(hero->life > 0 ){
        printf("VICTOIRE DU HERO");
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
    sleep(1);
    hero->power += get_tot_hero_weapon_power(hero);//Récupère la puissance du perso
    sleep(1);
    displayMonster(&monster);//Affiche les données du perso
    sleep(1);
    displayHero(hero);//Affiche les données du perso
    sleep(1);
    int dmg;
    while( (hero->life>0) && (monster.life>0) ){//Vérifie si l'un des perso et mort, si c'est le cas alors on sort du while
        dmg = hero->power - monster.shield;
        printf("Vous avez infligé %d dégats au monstre", dmg);
        sleep(1);
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
        printf("Le boss vous a infligé %d dégats", dmg);
        sleep(1);
        if(dmg < 0){
            hero->shield = -dmg;
            dmg = 0;
        }
        else {
            hero->shield = 0;
            hero->life -= dmg;
        }
        displayMonster(&monster);
        sleep(1);
        displayHero(hero);
        sleep(1);
    }             
    //Verifie qui est sortie vivant et on attribue la victoire
    if(hero->life > 0 ){
        printf("VICTOIRE DU HERO");
       displayHero(hero);
       
       return 1;
    }
    else{
       displayMonster(&monster);
       return 0;  
    }   
}

