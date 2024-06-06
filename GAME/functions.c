#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "header.h"


void menu() {
	Character hero = { 0,0,0 };
	Character* characters = NULL;
	int characterCount = 0;
	int characterCount2 = 0;
	Enemy enemy;
	static int choice;
	int characterExist = NULL;
	srand((unsigned)time(NULL));


	// menu
	while (1) {
		printf("\n_________________________Menu_________________________\n\n");
		printf("1. Create a new character\n");
		printf("2. Show current character\n");
		printf("3. Print all saved characters\n");
		printf("4. Delete saved character by name\n");
		printf("5. Search and choose a saved character by name to play with\n");
		printf("6. Update current character\n");
		printf("7. Battle\n");
		printf("8. Exit\n");
		printf("\nEnter choice: ");
		choice = getInput();
		clearScreen();


		switch (choice) {
		case 1:
			characterCount = createCharacter(&hero, characterCount);
			characterExist++;
			inputFix();
			break;

		case 2:
			if (characterCount == 0 && characterExist == 0) {
				printf("No character.\n");
			}
			else {
				readCharacter(&hero);
			}
			break;

		case 3:
			loadCharactersFromFile(&characters, &characterCount2);
			printAllCharacters(characters, characterCount2);
			break;

		case 4:
			loadCharactersFromFile(&characters, &characterCount2);
			printAllCharacters(characters, characterCount2);
			if (characterCount2 > 0) {
				char searchName[50];
				printf("Enter the character name to delete: ");
				scanf("%s", searchName);
				Character* found = searchCharacterByName(characters, characterCount2, searchName);
				if (found != NULL) {
					char confirm;
					printf("Character %s is found. \nDo you want to delete the character? (y/n): ", searchName);
					scanf(" %c", &confirm);
					if (confirm == 'y' || confirm == 'Y') {
						removeCharacterFromFile(searchName);
						printf("Character %s is deleted.\n", searchName);
					}
					else {
						printf("Character %s isn't deleated.\n", searchName);
					}
					
				}
				else {
					printf("Character with name %s wasn't found.\n", searchName);
				}
				inputFix();
			}
			break;

		case 5:
			loadCharactersFromFile(&characters, &characterCount);
			printAllCharacters(characters, characterCount);
			if (characterCount > 0) {
				char searchName[50];
				printf("Enter the characters name to search: ");
				scanf("%s", searchName);

				Character* found = searchCharacterByName(characters, characterCount, searchName);
				if (found != NULL) {
					hero = *found;
					printf("Character %s is selected for the game.\n", hero.name);
					characterExist++;
					inputFix();
				}
				else {
					printf("Character with name %s is not found.\n", searchName);
					characterCount = 0;
					inputFix();
				}
			}
			break;

		case 6:
			if (characterExist != NULL) {
				updateCharacter(&hero);
			}
			else {
				printf("No character to update, please create one first\n");
			}
			break;

		case 7:
			if (characterCount == NULL || &hero == NULL) {
				printf("No character created, please create a character first.\n");
				break;
			}
			else {
				int luck = rand() % 3;
				switch (luck) {
				case 0:
					createEnemy(&enemy, ENEMY_GOBLIN);
					break;
				case 1:
					createEnemy(&enemy, ENEMY_ORC);
					break;
				case 2:
					createEnemy(&enemy, ENEMY_DRAGON);
					break;
				default:
					createEnemy(&enemy, ENEMY_GOBLIN);
					break;
				}
				battle(&hero, &enemy);
				break;
			}

		case 8:
			if (hero.health > 0) {
				char last;
				printf("Save character? (y/n): ");
				scanf(" %c", &last);
				if (last == 'y' || last == 'Y') {
					saveCharacterToFile(&hero);
				}
				else {
					printf("Character wasn't saved\n");
				}
			}
			printf("Good bye!\n");
			if (characters != NULL) {
				free(characters);
				characters = NULL;
			}
			exit(0);

		case 666:
			if (characters != NULL || characterCount != NULL || characterCount2 != NULL) {
				char confirm;
				printf("Are you sure you want to delete all of the characters? (y/n): ");
				scanf(" %c", &confirm);
				if (confirm == 'y' || confirm == 'Y') {
					deleteAll(characters);
					characters = NULL;
					characterCount = 0;
					characterExist = 0;
					inputFix();
					break;
				}
				else {
					printf("Not deleted\n");
				}
			}
			else {
				printf("Wrong number for now :)\n");
				break;
			}
			inputFix();
			break;
		default:
			printf("Wrong input, try again.\n");
			break;
		}
		waitForInput();
		clearScreen();
	}
}


// Gets input for menu choice (1 - 8 + 666)
int* getInput() {
	int input = 0;
	char keyInput[50];
	if (fgets(keyInput, 40, stdin) == NULL) {
		memset(keyInput, '\0', 40);
		input = getInput();
	}
	if (keyInput[0] == '0') {
		input = 0;
		return input;
	}
	input = atoi(keyInput);
	if (input == 0) {
		memset(keyInput, '\0', 40);
		input = getInput();
	}
	printf("\n");
	return input;
}



int createCharacter(Character* character, const int* characterCount) {
	printf("Enter character name: ");
	scanf("%s", character->name);
	character->health = 100;
	character->attackPower = 10.0f;
	printf("Character %s is created.\n", character->name);
	return characterCount + 1;
}



void readCharacter(const Character* character) {
	printf("Character: %s\n", character->name);
	printf("Health: %d\n", character->health);
	printf("Attack power: %.2f\n", character->attackPower);
}


//Function which allows the user to change health and attack power of the current character
void updateCharacter(const Character* character) {
	if (character == NULL) {
		fprintf(stderr, "Error: NULL character pointer in update\n");
		return;
	}
	printf("Update character %s\n", character->name);
	do {
		printf("Enter new health (100 - 500): ");
		scanf("%d", &character->health);
	} while (character->health < 100 || character->health > 500);
	do {
		printf("Enter new attack power (10 - 100): ");
		scanf("%f", &character->attackPower);
	} while (character->attackPower < 10.0f || character->attackPower > 100.0f);
	inputFix();
	printf("Character %s is updated.\n", character->name);
}


//Enemy function, give them a name, health and attackpower. Currently supports 3 types
void createEnemy(Enemy* enemy, const EnemyType type) {
	enemy->type = type;
	switch (type) {
	case ENEMY_GOBLIN:
		enemy->health = 30;
		enemy->attackPower = 5.0f;
		break;
	case ENEMY_ORC:
		enemy->health = 50;
		enemy->attackPower = 8.0f;
		break;
	case ENEMY_DRAGON:
		enemy->health = 100;
		enemy->attackPower = 15.0f;
		break;
	}
}


//Similar function as readCharacter function, reads the enemy before battle and prints the information
void readEnemy(const Enemy* enemy) {
	const char* typeStr;
	switch (enemy->type) {
	case ENEMY_GOBLIN: typeStr = "Goblin"; break;
	case ENEMY_ORC: typeStr = "Orc"; break;
	case ENEMY_DRAGON: typeStr = "Dragon"; break;
	default: typeStr = "Unknown"; break;
	}

	printf("Enemy: %s\n", typeStr);
	printf("Health: %d\n", enemy->health);
	printf("Attack Power: %.2f\n", enemy->attackPower);
}


//The battle function:
void battle(Character* character, Enemy* enemy) {
	printf("Battle between %s and the enemy.\n", character->name);
	readEnemy(enemy);
	printf("\n");
	while (character->health > 0 && enemy->health > 0) {
		enemy->health -= character->attackPower;
		printf("%s Attcks the enemy. Enemy has %d health.\n", character->name, enemy->health);

		if (enemy->health > 0) {
			character->health -= enemy->attackPower;
			printf("Enemy attack's %s. %s has %d health.\n", character->name, character->name, character->health);
		}
	}

	if (character->health > 0) {
		printf("%s has won!\n", character->name);
		deleteEnemy(enemy);
	}
	else {
		printf("%s is defeated.\n", character->name);
	}
}


//Deletes the current enemy (add in battle) not really needed
void deleteEnemy(const Enemy* enemy) {
	memset(enemy, 0, sizeof(Enemy));
}



void waitForInput() {
	char input;
	printf("Press enter to continue...");
	input = getchar();
}


//Function for saving the character to file
void saveCharacterToFile(const Character* character) {
	FILE* file = fopen("characters.bin", "ab");
	if (file == NULL) {
		perror("Error in opening file - saveCharacterToFile error!");
		return;
	}

	if (fwrite(character, sizeof(Character), 1, file) != 1) {
		perror("Error saving character to file - saveCharacterToFile error!");
	}
	else {
		printf("Character %s is saved to file.\n", character->name);
	}
	fclose(file);
}


//Function to load a character
void loadCharactersFromFile(const Character** characters, int* count) {
	FILE* file = fopen("characters.bin", "rb");
	if (file == NULL) {
		perror("Error in opening file characters.bin. - loadCharactersFromFile error!");
		return;
	}

	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	rewind(file);

	int characterCount = fileSize / sizeof(Character);
	*characters = (Character*)malloc(characterCount * sizeof(Character));
	if (*characters == NULL) {
		perror("Error in alocation of memory for characters. - loadCharactersFromFile function error!");
		free(*characters);
		fclose(file);
		return;
	}

	if (fread(*characters, sizeof(Character), characterCount, file) != characterCount) {
		perror("Error reading characters from file.");
		free(*characters);
		*characters = NULL;
		characterCount = 0;
	}

	if (count != NULL) {
		*count = characterCount;
	}

	fclose(file);
}



void printAllCharacters(const Character* characters, const int count) {
	if (count == 0) {
		printf("No saved characters.\n");
		return;
	}
	qsort(characters, count, sizeof(Character), compareCharacters);

	for (int i = 0; i < count; i++) {
		printf("Character %d: %s, Health: %d, Attack Power: %.2f\n", i + 1, characters[i].name, characters[i].health, characters[i].attackPower);
	}
}



static compareCharacters(const void* a, const void* b) {
	const Character* charA = (const Character*)a;
	const Character* charB = (const Character*)b;
	return strcmp(charA->name, charB->name);
}


//Function for searching a character by name
static Character* searchCharacterByName(const Character* characters, const int count, const char* name) {
	Character key;
	strncpy(key.name, name, sizeof(key.name) - 1);
	key.name[sizeof(key.name) - 1] = '\0';

	return bsearch(&key, characters, count, sizeof(Character), compareCharacters);
}


//Function for removing a character
void removeCharacterFromFile(const char* name) {
	Character* characters = NULL;
	int characterCount = 0;

	loadCharactersFromFile(&characters, &characterCount);

	if (characters == NULL || characterCount == 0) {
		printf("No characters to delete.\n");
		return;
	}

	FILE* file = fopen("characters.bin", "w");
	if (file == NULL) {
		perror("Error opening file to delete character.");
		return;
	}

	for (int i = 0; i < characterCount; i++) {
		if (strcmp(characters[i].name, name) != 0) {
			fwrite(&characters[i], sizeof(Character), 1, file);
		}
	}

	free(characters);
	fclose(file);
}


//Clean console function
void clearScreen() {

#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}


//Function 666 deletes the character.bin file
static deleteAll(Character* characters) {
		int ret;
		ret = remove("characters.bin");
		if (ret == 0) {
			perror("File deleted successfully!\n");
		}
		else {
			perror("Unable to delete the file!\n");
		}
		free(characters);
		characters = NULL;
}



void inputFix() {
	char input;
	input = getchar();
}
