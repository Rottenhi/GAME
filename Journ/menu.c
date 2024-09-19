#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "header.h"
extern int heal = 0;
#define NAMESIZE 50

void menu() {
	Character hero = { 0,0,0 };
	srand((unsigned)time(NULL));
	int choice = 0;
	Enemy enemy;
	Character *characters = NULL;
	int characterCount = 0;
	int characterCount2 = 0;
	int characterExist = 0;
	heal = 10;
	const char* fileName = "characters.bin";

	while (1) {
		clearScreen();
		printf("\tMenu\n");
		printf("1. Create a new character\n");
		printf("2. Show current character\n");
		printf("3. Print all saved characters\n");
		printf("4. Delete saved character by name\n");
		printf("5. Search and choose a saved character by name to play with\n");
		printf("6. Update current character (name,health and attackPower)\n");
		printf("7. Start the game\n");
		printf("8. Exit\n");
		printf("9. Delete characters.bin so you have no file (for testing program or just deleting all characters)\n(Create a character first for 9.!)");
		printf("\nEnter choice (only accepts numbers): ");
		choice = getInput();
		printf("%d\n", choice);
		clearScreen();
		
		switch (choice) {
		case 1:
			characterCount = createCharacter(&hero, characterCount);
			characterExist++;
			inputEmpty();
			break;

		case 2:
			if (characterCount == 0 && characterExist == 0) {
			printf("No character.\n");
			}
			else{
			readCharacter(&hero);
			}
			inputEmpty();
			break;

		case 3:
			loadCharactersFromFile(&characters, &characterCount2);
			printAllCharacters(characters, characterCount2);
			inputEmpty();
			break;

		case 4:
			loadCharactersFromFile(&characters, &characterCount2);
			printAllCharacters(characters, characterCount2);
			searchAndDestroy(characters, characterCount2);
			inputEmpty();
			break;

		case 5:
			loadCharactersFromFile(&characters, &characterCount2);
			printAllCharacters(characters, characterCount2);
			if (characterCount2 > 0) {
				char searchName[50];
				printf("Enter the characters name to search: ");
				scanf("%[^\n]s", searchName);
				getchar();
				Character* found = searchCharacterByName(characters, characterCount2, searchName);
				if (found != NULL) {
					hero = *found;
					printf("Character %s is selected for the game.\n", hero.name);
					characterExist++;
				}
				else {
					printf("Character with name %s is not found.\n", searchName);
					characterCount = 0;
				}
			}
			inputEmpty();
			break;

		case 6:
			if (characterExist != 0) {
				updateCharacter(&hero);
			}
			else {
				printf("No character to update, please create one first\n");
			}
			inputEmpty();
			break;

		case 7:
			if (characterExist == 0) {
				printf("No character created, please create a character first.\n");
				inputEmpty();
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
			}
			battle(&hero, &enemy);
			if (hero.health > 0) {
				inputEmpty();
				clearScreen();
				story1(&hero);
				hero.health += heal;
				inputEmpty();
				clearScreen();
				createEnemy(&enemy, ENEMY_KNIGHT);
				battle(&hero, &enemy);

				if (hero.health > 0) {
					hero.attackPower += 8; //Taken knight sword, yes the sword.
					inputEmpty();
					clearScreen();
					story2(&hero);
				}
			}
			inputEmpty();
			break;

		case 8:
			if (hero.health > 0) {
				char saveConfirm;
				printf("Save character? (y/n): ");
				scanf(" %c", &saveConfirm);
				if (saveConfirm == 'y' || saveConfirm == 'Y') {

					loadCharactersFromFile(&characters, &characterCount);

					if (characters == NULL || characterCount == 0) {
						printf("No characters to delete.\n");
						inputEmpty();
						return;
					}

					FILE* file = fopen("characters.bin", "w");
					if (file == NULL) {
						perror("Error opening file to delete character.");
						inputEmpty();
						return;
					}

					for (int i = 0; i < characterCount; i++) {
						if (strcmp(characters[i].name, hero.name) != 0) {
							fwrite(&characters[i], sizeof(Character), 1, file);
						}
					}

					fclose(file);
				saveCharacterToFile(&hero);
				}
				else {
					printf("Character wasn't saved\n");
				}
			}
			printf("\nGood bye!\n");
			if (characters != NULL) {
				free(characters);
				characters = NULL;
			}
			exit(0);

		case 9:
			if (characters != NULL || characterCount != 0 || characterCount2 != 0) {
				char confirm;
				printf("Are you sure you want to delete all of the characters? (y/n): ");
				scanf(" %c", &confirm);
				if (confirm == 'y' || confirm == 'Y') {
					deleteAll(characters);
					characters = NULL;
					characterCount = 0;
					characterExist = 0;
					getchar();
				}
				else {
					printf("Not deleted\n");
					getchar();
				}
			}
			else {
				printf("Wrong number for now :)\n");
			}
			inputEmpty();
			break;

		}
	}
}


//Function for deleting an character saved in characters.bin
void searchAndDestroy(const Character* characters, const int characterCount) {
	if (characterCount > 0) {
		char searchName[50];
		printf("Enter the character name to delete: ");
		scanf("%[^\n]s", searchName);
		getchar();
		Character* found = searchCharacterByName(characters, characterCount, searchName);
		if (found != NULL) {
			char confirm;
			printf("Character %s is found. \nDo you want to delete the character? (y/n): ", searchName);
			confirm = getchar();
			if (confirm == 'y' || confirm == 'Y') {
				removeCharacterFromFile(searchName);
				printf("Character %s is deleted.\n", searchName);
				getchar();
			}
			else {
				printf("Character %s isn't deleated.\n", searchName);
				getchar();
			}

		}
		else {
			printf("Character with name %s wasn't found.\n", searchName);

		}
	}
}

//Function for pressing enter
static void inputEmpty() {
	printf("Press enter to continue");
	int inputEmpty;
	inputEmpty = getchar();
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
		inputEmpty();
		return;
	}

	FILE* file = fopen("characters.bin", "w");
	if (file == NULL) {
		perror("Error opening file to delete character.");
		inputEmpty();
		return;
	}

	for (int i = 0; i < characterCount; i++) {
		if (strcmp(characters[i].name, name) != 0) {
			fwrite(&characters[i], sizeof(Character), 1, file);
		}
	}

	free(characters);
	fclose(file);
	return;
}

// Gets input for menu choice (1 - 9)
int* getInput() {
	int* input = 0;
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

//Clean console function
void clearScreen() {

#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

//Function for creating a character (User inputs only the name health and attack is hard coded)
int createCharacter(Character* character, const int characterCount) {
	printf("Enter character name: ");
	fgets(character->name, 49, stdin);
	character->health = 100;
	character->attackPower = 10.0f;

	character->name[strcspn(character->name, "\n")] = '\0';

	printf("Character %s is created.\n", character->name);
	return characterCount + 1;
}

//Function which prints the current character (active or main character)
void readCharacter(const Character* character) {
	printf("Character: %s\n", character->name);
	printf("Health: %d\n", character->health);
	printf("Attack power: %.2f\n", character->attackPower);
}

//Function to load a character
void loadCharactersFromFile(Character** characters, int* count) {
	FILE* file = fopen("characters.bin", "rb");
	if (file == NULL) {
		perror("No file: characters.bin");
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
		*characters = NULL;
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
	return;
}

//Function for printing all characters
void printAllCharacters(Character* characters, const int count) {
	if (count == 0) {
		printf("No saved characters.\n");
		return;
	}
	qsort(characters, count, sizeof(Character), compareCharacters);

	for (int i = 0; i < count; i++) {
		printf("Character %d: %s, Health: %d, Attack Power: %.2f\n", i + 1, characters[i].name, characters[i].health, characters[i].attackPower);
	}
}

//Function for name comparison of characters works with printAllCharacters function
static compareCharacters(const void* a, const void* b) {
	const Character* charA = (const Character*)a;
	const Character* charB = (const Character*)b;
	return strcmp(charA->name, charB->name);
}

//Function which allows the user to change the name, health and attackPower
void updateCharacter(Character* character) {
	if (character == NULL) {
		fprintf(stderr, "Error: NULL character pointer in update\n");
		return;
	}
	printf("Update character %s\n", character->name);
	printf("Enter new character name: ");
	fgets(character->name, 49, stdin);
	do {
		printf("Enter new health (100 - 500): ");
		scanf("%d", &character->health);
	} while (character->health < 100 || character->health > 500);
	do {
		printf("Enter new attack power (10 - 100): ");
		scanf("%f", &character->attackPower);
	} while (character->attackPower < 10.0f || character->attackPower > 100.0f);

	character->name[strcspn(character->name, "\n")] = '\0';
	printf("Character %s is updated.\n", character->name);
	getchar();
}

//Enemy function, give them a name, health and attackpower. Currently supports these types
void createEnemy(Enemy* enemy, const EnemyType type) {
	enemy->type = type;
	switch (type) {
	case ENEMY_GOBLIN:
		enemy->health = 30;
		enemy->attackPower = 3.0f;
		break;
	case ENEMY_ORC:
		enemy->health = 50;
		enemy->attackPower = 7.0f;
		break;
	case ENEMY_DRAGON:
		enemy->health = 100;
		enemy->attackPower = 10.0f;
		break;
	case ENEMY_KNIGHT:
		enemy->health = 60;
		enemy->attackPower = 8.0f;
		break;
	}
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
		printf("The journey comes to an end :)\n");
	}
}

//Similar function as readCharacter function, reads the enemy before battle and prints the information
void readEnemy(const Enemy* enemy) {
	const char* typeStr;
	switch (enemy->type) {
	case ENEMY_GOBLIN: typeStr = "Goblin"; break;
	case ENEMY_ORC: typeStr = "Orc"; break;
	case ENEMY_DRAGON: typeStr = "Dragon"; break;
	case ENEMY_KNIGHT: typeStr = "Knight"; break;
	default: typeStr = "Unknown"; break;
	}

	printf("Enemy: %s\n", typeStr);
	printf("Health: %d\n", enemy->health);
	printf("Attack Power: %.2f\n", enemy->attackPower);
}


//Deletes the current enemy (add in battle) not really needed (I didn't include it in the programm just here if one needs memset function)
void deleteEnemy(Enemy* enemy) {
	memset(enemy, 0, sizeof(Enemy));
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

//Function 9 deletes the character.bin file
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