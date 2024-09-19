#ifndef HEADER_H
#define HEADER_H


// Enum for enemies
typedef enum {
	ENEMY_GOBLIN,
	ENEMY_ORC,
	ENEMY_DRAGON,
	ENEMY_KNIGHT
}EnemyType;

// Character structure
typedef struct {
	char name[50];
	int health;
	float attackPower;
}Character;

// Enemy structure
typedef struct {
	EnemyType type;
	int health;
	float attackPower;
}Enemy;

//Function prototypes
void menu();
int* getInput();
static void inputEmpty();
void clearScreen();
int createCharacter(Character* character, const int characterCount);
void readCharacter(const Character* character);
void loadCharactersFromFile(Character** characters, int* count);
void printAllCharacters(Character* characters, const int count);
static compareCharacters(const void* a, const void* b);
void searchAndDestroy(const Character* characters, const int characterCount);
static Character* searchCharacterByName(const Character* characters, const int count, const char* name);
void removeCharacterFromFile(const char* name);
void updateCharacter(Character* character);
void createEnemy(Enemy* enemy, const EnemyType type);
void battle(Character* character, Enemy* enemy);
void deleteEnemy(Enemy* enemy);
void readEnemy(const Enemy* enemy);
void saveCharacterToFile(const Character* character);
static deleteAll(Character*);
void story1(const Character* hero);
void story2(const Character* hero);

#endif