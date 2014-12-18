#ifndef TEST_Enemy_h
#define TEST_Enemy_h

#include "../includes/gameProperties.h"
#include "../includes/Display.h"

#define DELAY_BETWEEN_ENEMIES	1
#define INT_BASIC_HEALTH 100
#define INT_BASIC_ARMOUR 0
#define INT_BASIC_SPEED 3
#define INT_BASIC_DAMAGE 10
#define INT_BASIC_HEIGHT 64
#define INT_BASIC_WIDTH 32

#define INT_HEAVY_HEALTH 1000
#define INT_HEAVY_ARMOUR 1
#define INT_HEAVY_SPEED 1
#define INT_HEAVY_DAMAGE 100
#define INT_HEAVY_HEIGHT 64
#define INT_HEAVY_WIDTH 32

#define CHAR_BASIC_HEALTH 100
#define CHAR_BASIC_ARMOUR 0
#define CHAR_BASIC_SPEED 3
#define CHAR_BASIC_DAMAGE 10
#define CHAR_BASIC_HEIGHT 64
#define CHAR_BASIC_WIDTH 32

#define CHAR_HEAVY_HEALTH 1000
#define CHAR_HEAVY_ARMOUR 1
#define CHAR_HEAVY_SPEED 1
#define CHAR_HEAVY_DAMAGE 100
#define CHAR_HEAVY_HEIGHT 64
#define CHAR_HEAVY_WIDTH 32

typedef struct path *Path;
typedef struct levelPaths *LevelPaths;
typedef struct enemy *Enemy;
typedef struct enemyGroup *EnemyGroup;



typedef enum typeOfEnemy	{

	intBasic = 1,
	intHeavy = 2,
	charBasic =3,
	charHeavy =4
} TypeOfEnemy;


typedef enum BOOL {TRUE, FALSE} BOOL;

typedef enum enemyFamily {

  intEnemy = 1,
  charEnemy = 2,
} Family;

void createEnemyGroup();
void freeEnemyGroup();
Enemy createEnemy();
void createSpecificEnemy(TypeOfEnemy eType, int lvl, int entranceNum);
int getNumberOfEnemies();
void Test_createEnemy();
EnemyGroup getEnemyGroup(EnemyGroup enemyList);
int getEnemyHealth();
void freeEnemy(int enemyID);
int moveEnemy(int enemyID);
int isDead(int enemyID);
void killEnemy(int enemyID);
int inRange(int tX, int tY, int tRange, int enemyID);
void damageEnemy(int damage, int enemyID);
void towerGetTargetPos(int * towerTargetPosition, int enemyID);
void printEnemy(int enemyID);
void createPath();
void freePath(Path p);
void freeLevelPaths();
int **getPathPointer(int **newPath);
void testEnemy();
int setEnemyX(int enemyID, int newX);
int setEnemyY(int enemyID, int newY);
int setEnemyHealth(int enemyID, int newHealth);
int setEnemyArmour(int enemyID, int newArmour);
void present_enemy(Display d);
void createLevelPaths();
LevelPaths getLevelPaths(LevelPaths pathList);
void assignMemoryForPaths(int numberOfPaths);

void layPaths(int numberOfPaths, int levelNum);
char *getFilePath(int levelNum, int pathNum);
void readInPath(int levelNum, int pathNum);

void initialiseEnemy(Enemy newEnemy, int lvl, Family fam, TypeOfEnemy eType, int health, int armour, int speed, int damage, int height, int width);
int distanceToEndOfPath(int enemyID);


#endif
