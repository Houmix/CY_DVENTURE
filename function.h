#include "structures.h"

void printCharByChar(char* string, unsigned int delay);

//Fonction ouvre le fichier contenant toutes les données relatives aux armes et recupère les donnée de l'arme en argument
int get_weapon_power(char* weapon, char* WeaponFile);

int get_tot_hero_weapon_power(Hero* hero);

int get_tot_boss_weapon_power(Boss* boss);
//Génère une valeur aléatoire pour l'attribuer au shield des personnages
int get_random_shield();

int get_random_number();

//Affichage stat du perso
void displayHero(Hero* a);

void displayBoss(Boss* a);

void displayMonster(Monster* a);

//Simule le combat. Récupère toutes les données des combatants comme puissance, vie... Puis les persos s'attaque à tour de role.
int fight(Hero* hero, Boss* boss );

int fightMonster(Hero* hero);

//Ouvre le fichier de sauvegarde pour savoir si le joueur existe déjà
int open_save_file(char* SaveFileName);

//Affiche un fichier dans son intégralité
void displayFile(char* nameFichier);

//Le nom des fichiers des étapes est organisé de telle sorte que les nombres des choix soit concaténé par 
//exemple : à l'étape 1, il y a 2 choix : 1 et 2, le nom du fichier étape suivant sera soit etape11 ou etape12
void generateNextStepFileName(char* currentStepFN, int currentStep);

//Lecture du fichier contenant les données du boss et l'attribue à l'objet de la structure Boss
int get_boss_data(Boss* boss, char* BossDataFile);

//Lecture du fichier contenant les données du boss et l'attribue à l'objet de la structure Hero
int get_hero_data(Hero* hero, char* SaveFileName);

//Mise à jour du fichier de sauvegarde d'un joueur à l'aide d'un fichier temporaire
int db_update(Hero* hero, char* SaveFileName);

//Ajoute d'un objet dans l'inventaire du hero
int add_item(Hero* hero, char* itemName, char* SaveFileName);
    
//Mise à jour des stats du perso
int change_stat(Hero* hero, char* bonus_name, int amount);

//Tirage pour obtenir une puissante arme
void draw(Hero* hero, int choiceNumber, Bonus* bonus, char* SaveFileName);

//Ajout du bonus choisi
int applyChoiceBonuses(Hero* hero, int choiceNumber, Bonus* bonus, char* SaveFileName);

//Création du fichier de sauvegarde du joueur si on arrive à la fin du jeu ou si le joueur n'existe pas
int new_hero(Hero* hero, char* dataFileName, char* SaveFileName );

//Lecture des fichier dans le but de récuperer toutes les données de l'étape
void readStepDataFromFile(char* fileName, Step* currentStepData, Bonus* bonus, bool* hasChoices, bool* hasHeroVsBoss, bool* hasEvent, bool* hasBonus, bool* hasMonster, bool* hasExcalibure);

void processStepData(FILE* file, Step* currentStepData, Bonus* bonus, bool* hasEvent, bool* hasBonus, bool* hasMonster, bool* hasExcalibure, bool* hasChoices, bool* hasHeroVsBoss);

void searchChoices(FILE* file, Step* currentStepData, bool* hasChoices, bool* hasHeroVsBoss);

void searchMonster(FILE* file, char* monster, bool* hasMonster);

void searchExcalibure(FILE* file, Bonus* bonus, bool* hasExcalibure);

void searchBonus(FILE* file, Bonus* bonus, bool* hasBonus);

void searchEvent(FILE* file, Step* currentStepData, bool* hasEvent);

int searchTag(char* Tag, char* NameFile, Step* currentStepData);