#ifndef HEADER_H
#define HEADER_H

// Enum for enemies
typedef enum {
	ENEMY_GOBLIN,
	ENEMY_ORC,
	ENEMY_DRAGON
} EnemyType;

// Character structure
typedef struct {
	char name[50];
	int health;
	float attackPower;
} Character;

// Enemy structure
typedef struct {
	EnemyType type;
	int health;
	float attackPower;
} Enemy;

// Function prototype
int createCharacter(Character* character, const int* characterCount);
void readCharacter(const Character* character);
void updateCharacter(const Character* character);
void createEnemy(Enemy* enemy, const EnemyType type);
void readEnemy(const Enemy* enemy);
void deleteEnemy(const Enemy* enemy);
void battle(Character* character, Enemy* enemy);
void menu();
void clearScreen();
void waitForInput();
void saveCharacterToFile(const Character* character);
void loadCharactersFromFile(const Character** characters, int* count);
void printAllCharacters(const Character* characters,const int count);
static compareCharacters(const void* a, const void* b);
static Character* searchCharacterByName(const Character* characters, const int count, const char* name);
void removeCharacterFromFile(const char* name);
static deleteAll(Character*);
int* getInput();
void inputFix();

#endif
