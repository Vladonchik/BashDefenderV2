#include <stdio.h>
#include <stdlib.h>
#include "../includes/sput.h"
#include <math.h>
#include <string.h>

#include "../includes/enemy.h"

struct path {
    int pathLength;
    int **pathCoords;
};

struct levelPaths {
    int numberOfPaths;
    Path *paths;
};

struct enemy {

    TypeOfEnemy eType;
    Family eFamily;
    int level;
    int enemyID;
    
    int x, y;
    Path enemyPath;
    int pathProgress;
    int maxHealth;
    int health;
    int armour;
    int speed;
    int damage;
    
    int slowEffect;
    int slowEffectStepsRemaining;
    int poisonEffect;
    int poisonEffectStepsRemaining;

    int height;
    int width;

    int dead;
};

struct enemyGroup {
    int numberOfEnemies;
    Enemy * enemyArray;
};

/*
* mallocs memory and creates 2d arrays containing path coords.
* currently number of paths is chosen within function.
*/
void createLevelPaths()
{
    LevelPaths pathList = (LevelPaths) malloc(sizeof(*pathList));
    getLevelPaths(pathList);

    int numberOfPaths = 4;
    
    assignMemoryForPaths(numberOfPaths);
    layPaths(numberOfPaths, 1); // currently 1 as only one level
  
}

/*
* frees all paths, and then frees the level path structure itself
*/
void freeLevelPaths()
{
    LevelPaths lP = getLevelPaths(NULL);
    for(int i = 0; i < lP->numberOfPaths; i++) {
        freePath(lP->paths[i]);
    }
    free(lP->paths);
    free(lP);
}

/*
* frees the array of a path structure and then the path structure itself
*/
void freePath(Path p)
{
    for(int i = 0; i < p->pathLength; i++) {
        free(p->pathCoords[i]);
    }
    free(p->pathCoords);
    free(p);
}

/*
* creates the specified number of path structures within the level paths structure
*/

void layPaths(int numberOfPaths, int levelNum)
{
  
   // LevelPaths lP = getLevelPaths(NULL);
    for(int i = 1; i <= numberOfPaths; i++) {
      readInPath(levelNum, i);
    }
}

void readInPath(int levelNum, int pathNum) {
  
  char *filePath = getFilePath(levelNum, pathNum);
  FILE *fp = fopen(filePath, "r");
  if(fp == NULL) {
    fprintf(stderr,"****ERROR Unable to open path file at '%s' ****\n", filePath);
    exit(1);
  }
  LevelPaths lP = getLevelPaths(NULL);
  Path P = lP->paths[pathNum-1];
  
  int lastRowScanned = 0;
    int backW, backH;
  int x, y;
    getBackgroundDimensions(&backW, &backH);
  if(fscanf(fp,"%d\n",&P->pathLength) != 1) {
    fprintf(stderr,"****ERROR Unable to read path length from path file at '%s' ****\n", filePath);
    exit(1);
  }
  lastRowScanned++;
  
  P->pathCoords = (int **) malloc(sizeof(int *) * P->pathLength);
  for(int i = 0; i < P->pathLength; i++) {
    
    if(fscanf(fp,"%d,%d\n", &x, &y) != 2) {
      fprintf(stderr,"****ERROR Unable to read path coordinates in file at '%s', last row successfully scanned was %d ****\n", filePath, lastRowScanned);
      exit(1);
    }
    
    lastRowScanned++;
    x = (int)((double) x * ((double) SCREEN_WIDTH_GLOBAL/ (double) backW) );
    y = (int)((double) y * ((double) SCREEN_HEIGHT_GLOBAL/ (double) backH) );
  
    P->pathCoords[i] = (int *)malloc(sizeof(int) * 2);
    P->pathCoords[i][0] = x;
    P->pathCoords[i][1] = y;
  }
  
  fclose(fp);
  
}

/*
** returns file path of .txt to be opened and read to make a path
*/
char *getFilePath(int levelNum, int pathNum) {
  
  char levelNumStr[10];
  sprintf(levelNumStr, "%d", levelNum);
  
  char pathNumStr[10];
  sprintf(pathNumStr, "%d", pathNum);
  
  char *filePath = calloc((strlen("../data/levels/lvl_") + strlen(levelNumStr) + strlen("/paths/path_") +strlen(pathNumStr) + strlen(".txt") ), sizeof(char) );
  if(filePath == NULL) {
    fprintf(stderr,"Unable to calloc space for path filePath\n");
    exit(1);
  }
  
  strcat(filePath, "../data/levels/lvl_");
  strcat(filePath, levelNumStr);
  strcat(filePath, "/paths/path_");
  strcat(filePath, pathNumStr);
  strcat(filePath, ".txt");
  
  return filePath;
}

/*
* assigns memory in the level paths structure for the desired number of path structure pointers
*/
void assignMemoryForPaths(int numberOfPaths)
{

      LevelPaths lP = getLevelPaths(NULL);
      lP->numberOfPaths = numberOfPaths;
      lP->paths = (Path *)malloc(sizeof(Path) * numberOfPaths);
      if(lP->paths == NULL) {
          fprintf(stderr,"****ERROR malloc in create path pointers failed****\n");
          exit(1);
      }
      for(int i = 0; i < numberOfPaths; i++) {
          lP->paths[i] = (Path)malloc(sizeof(struct path));
      }
}

/*
* if passed NULL, returns static LevelPaths pointer. If passed pointer, reassigns static LevelPaths pointer.
*/
LevelPaths getLevelPaths(LevelPaths pathList)
{
    
    static LevelPaths lP;
    
    if(pathList != NULL) {
        lP = pathList;
    }
    
    return lP;
}

/*
* creates an empty structure to hold all enemies
*/
void createEnemyGroup()
{
    EnemyGroup enemyList = (EnemyGroup) malloc(sizeof(*enemyList));
    getEnemyGroup(enemyList);
    enemyList->enemyArray=malloc(sizeof(Enemy));
    enemyList->numberOfEnemies = 0;
}

/*
* frees up all enemies and enemy pointers from the enemy group, then frees the group structire itself
*/
void freeEnemyGroup()
{
    EnemyGroup enemyList =  getEnemyGroup(NULL);
    for(int i = 0; i <= enemyList->numberOfEnemies; i++) {
        free(enemyList->enemyArray[i]);
    }
    free(enemyList->enemyArray);
    free(enemyList);
}


/*
* creates a new blank enemy within the enemy list structure, updates the enemy list structure to reflect the new number of enemies and populates the enemy (currently running for standard enemy only)
*/
Enemy createEnemy()
{
    EnemyGroup enemyList =  getEnemyGroup(NULL);
    ++(enemyList->numberOfEnemies);
    
    enemyList->enemyArray = (Enemy*)realloc(enemyList->enemyArray, (enemyList->numberOfEnemies+1)*(sizeof(Enemy)));
    if(enemyList->enemyArray==NULL) {
	      printf("****ERROR realloc in createEnemy failed****\n");
	      exit(1);
    }
    
    enemyList->enemyArray[enemyList->numberOfEnemies]=(Enemy)malloc(sizeof(struct enemy));
    if( enemyList->enemyArray[enemyList->numberOfEnemies]==NULL) {
	      printf("****ERROR malloc in createEnemy failed****\n");
        exit(1);
    }
    
    return enemyList->enemyArray[enemyList->numberOfEnemies];

}

/*
 returns the number of enemies that currently exist (dead & alive)
*/

int getNumberOfEnemies()
{
    return getEnemyGroup(NULL)->numberOfEnemies;
}

/*
* creates two enemies and checks their defaut values
*/
void Test_createEnemy()
{

    createLevelPaths();
    createEnemyGroup();
    
    createEnemy();
    sput_fail_unless(getNumberOfEnemies() == 1, "Valid: Number of enemies held in group is one.");
    
    sput_fail_unless(getEnemyHealth(getNumberOfEnemies()) == 100,"Valid: Enemy health is default." );

    createEnemy();
    sput_fail_unless(getNumberOfEnemies() == 2, "Valid: Number of enemies held in group is two.");
    sput_fail_unless(getEnemyHealth(getNumberOfEnemies()) == 100,"Valid: Enemy 2  health is default." );

}


void createSpecificEnemy(TypeOfEnemy eType, int lvl, int entranceNum) {

  Enemy e = createEnemy();
  
  switch (eType) {
    case intBasic :
      initialiseEnemy(e, lvl, intEnemy, eType, INT_BASIC_HEALTH, INT_BASIC_ARMOUR, INT_BASIC_SPEED, INT_BASIC_DAMAGE, INT_BASIC_HEIGHT, INT_BASIC_WIDTH);
      break;
    case intHeavy :
      initialiseEnemy(e, lvl, intEnemy, eType, INT_HEAVY_HEALTH, INT_HEAVY_ARMOUR, INT_HEAVY_SPEED, INT_HEAVY_DAMAGE, INT_HEAVY_HEIGHT, INT_HEAVY_WIDTH);
      break;
    case charBasic :
      initialiseEnemy(e, lvl, charEnemy, eType, CHAR_BASIC_HEALTH, CHAR_BASIC_ARMOUR, CHAR_BASIC_SPEED, CHAR_BASIC_DAMAGE, CHAR_BASIC_HEIGHT, CHAR_BASIC_WIDTH);
      break;
    case charHeavy :
      initialiseEnemy(e, lvl, charEnemy, eType, CHAR_HEAVY_HEALTH, CHAR_HEAVY_ARMOUR, CHAR_HEAVY_SPEED, CHAR_HEAVY_DAMAGE, CHAR_HEAVY_HEIGHT, CHAR_HEAVY_WIDTH);
      break;
    default :
      fprintf(stderr,"ERROR**** incorrect value for TypeOfEnemy (value = %d) passed to createSpecificEnemy() ****\n", eType);
      exit(1);
  }
}
    
    


/*
* populates relevant fields for standard enemies
*/                                            
void initialiseEnemy(Enemy newEnemy, int lvl, Family fam, TypeOfEnemy eType, int health, int armour, int speed, int damage, int height, int width)
{
    LevelPaths lP = getLevelPaths(NULL);
    newEnemy->enemyPath = lP->paths[rand()%lP->numberOfPaths];
    newEnemy->pathProgress = 0;
    newEnemy->x = newEnemy->enemyPath->pathCoords[0][0];
    newEnemy->y = newEnemy->enemyPath->pathCoords[0][1];



    newEnemy->eFamily = fam;
    newEnemy->level = lvl;
    newEnemy->eType = eType;
    newEnemy->maxHealth = health*lvl;
    newEnemy->health = newEnemy->maxHealth;
    newEnemy->armour = armour*lvl;
    newEnemy->speed = speed;

    newEnemy->enemyID=getNumberOfEnemies();
    newEnemy->dead = 0;

    newEnemy->height = height;
    newEnemy->width = width;
    newEnemy->damage = damage*lvl;

}

/*
* populates relevant fields for heavy enemies
*/   
void initialiseHeavyEnemy(Enemy newEnemy)
{
    LevelPaths lP = getLevelPaths(NULL);
    newEnemy->enemyPath = lP->paths[rand()%lP->numberOfPaths];
    newEnemy->pathProgress = 0;
    newEnemy->x = newEnemy->enemyPath->pathCoords[0][0];
    newEnemy->y = newEnemy->enemyPath->pathCoords[0][1];
    newEnemy->maxHealth = 1000;
    newEnemy->health = newEnemy->maxHealth;
    newEnemy->armour = 0;
    newEnemy->speed = 1;
    newEnemy->enemyID=getNumberOfEnemies();
    newEnemy->dead = 0;
    newEnemy->height = 64;
    newEnemy->width = 32;
    newEnemy->damage = 100;

}

/*
* manually sets enemy health. Used in testing.
*/
int setEnemyHealth(int enemyID, int newHealth)	{
	  getEnemyGroup(NULL)->enemyArray[enemyID]->health = newHealth;
	  return getEnemyGroup(NULL)->enemyArray[enemyID]->health;	
}                      

/*
* manually sets enemy armour. Used in testing.
*/
int setEnemyArmour(int enemyID, int newArmour)	{
	  getEnemyGroup(NULL)->enemyArray[enemyID]->armour = newArmour;
	  return getEnemyGroup(NULL)->enemyArray[enemyID]->armour;	
}   
/*
* returns pointer to the enemy group structure if input is NULL. If input is not NULL, reassigns pointer to the pointer passed to the function
*/
EnemyGroup getEnemyGroup(EnemyGroup enemyList)
{
    
    static EnemyGroup e;
    
    if(enemyList != NULL) {
        e = enemyList;
    }
    
    return e;
}

/*
* for each enemy, if not dead, calls the draw enemy function using their relevant information
*/
void present_enemy(Display d)
{
    EnemyGroup enemyList = getEnemyGroup(NULL);
    for(int i=1; i<=enemyList->numberOfEnemies; ++i)
    {
        Enemy e = enemyList->enemyArray[i];

        if(!isDead(i))
        {
            drawEnemy(e->x, e->y, 50, 50, 11250, 1250, 1, 9, 200);
            drawRect(e->x, e->y -20, 0, 0, 80, 10, (double)e->health, (double)e->maxHealth);
        }
    }
}

/*
* returns the current health of the specified enemy. Used in testing
*/
int getEnemyHealth(int enemyIndex)
{
    Enemy e = getEnemyGroup(NULL)->enemyArray[enemyIndex];
    return e->health;
}

/*
* frees contents of specified enemy structure
*/
void freeEnemy(int enemyID)
{
    free(getEnemyGroup(NULL)->enemyArray[enemyID]);
}

/*
* moves specified enemy along their assigned path by a set number of steps as designated by the enemy's speed characteristic
*/
int moveEnemy(int enemyID )
{
    Enemy e = getEnemyGroup(NULL)->enemyArray[enemyID];
    if(!isDead(enemyID) ) {
        if(e->pathProgress < e->enemyPath->pathLength - e->speed) {
            e->pathProgress += e->speed;
            e->x = e->enemyPath->pathCoords[e->pathProgress][0];
            e->y = e->enemyPath->pathCoords[e->pathProgress][1];
            return 0;
        }
        else {
           damageHealth(e->damage); 
            e->dead = 1;
			increaseDeathCnt();
            return 0;
        }
    }
    return 1;

}


/*
* manually set the enemy's x coord - used in testing
*/
int setEnemyX(int enemyID, int newX)
{
	  Enemy e = getEnemyGroup(NULL)->enemyArray[enemyID];
	  e->x = newX;
	  return e->x;
}

/*
* manually set the enemy's y coord - used in testing
*/
int setEnemyY(int enemyID, int newY)
{
	  Enemy e = getEnemyGroup(NULL)->enemyArray[enemyID];
	  e->y = newY;
	  return e->y;
}

/*
* Checks if specified enemy is dead. To be used before doing anything to the enemy (moving/firing etc.)
*/
int isDead(int enemyID)
{
    Enemy e = getEnemyGroup(NULL)->enemyArray[enemyID];
    if(e->dead == 1) {
        return 1;
    }
    else {
        return 0;
    }
}

/*
* for a given tower co-ord and range, returns 1 if the specified enemy is in range. retruns 0 if out of range.
*/
int inRange(int tX, int tY, int tRange, int enemyID)
{
    if( enemyID > getNumberOfEnemies() )
    {
        fprintf(stderr,"ERROR: tower calling inRange on enemy that never existed");
        return -1;
    }
    
    Enemy e = getEnemyGroup(NULL)->enemyArray[enemyID];
    

    int distanceBetweenTowerAndEnemy = (int)sqrt( pow((double)(e->x+(e->width/2)-tX),2) +
                                              pow((double)(e->y+(e->height/2)-tY),2)    );

    if(distanceBetweenTowerAndEnemy<tRange){
        return 1;
    }
    else {
        return 0;
    }

}

/*
* Does the specified ammount of damage to the specified enemy. Reduces damage by the amount of armour the enemy has first.
* If damage reduces health to less than 0, kills enemy and adds memory equivalent to enemy's max health/10.
*/
void damageEnemy(int damage, int enemyID)
{
    Enemy e = getEnemyGroup(NULL)->enemyArray[enemyID];
    
    int damageToBeDone = damage - e->armour;
    if(damageToBeDone <= 0) {
      damageToBeDone = 0;
    }
    
    e->health -= damageToBeDone;
    if(e->health<=0)
    {
        e->dead=1;
        addMemory(e->maxHealth/10);
        // drawDeath(e->x, e->y);
        //drawKillAll();

    }
}

/*
* kills specified enemy
*/
void killEnemy(int enemyID)
{
    Enemy e = getEnemyGroup(NULL)->enemyArray[enemyID];
    e->dead = 1;
    e->health = 0;
    addMemory(e->maxHealth/10);
}


/*
* calculates how far the specified enemy is from the end of their path. Used for tower target aquisition.
*/

int distanceToEndOfPath(int enemyID)
{
    Enemy e = getEnemyGroup(NULL)->enemyArray[enemyID];
    return e->enemyPath->pathLength - e->pathProgress;
}

/*
* when passed a pointer to a tower's target position array, updates this array to contain target x & y co-ords for specified enemy
*/
void towerGetTargetPos(int * towerTargetPosition, int enemyID)
{
    Enemy e = getEnemyGroup(NULL)->enemyArray[enemyID];
    

    towerTargetPosition[0] = e->x+(e->width/2);
    towerTargetPosition[1] = e->y+(e->height/2);
}

/*
* function for unit testing - prints enemy stats
*/
void printEnemy(int enemyID)
{
    Enemy e = getEnemyGroup(NULL)->enemyArray[enemyID];
    printf("Enemy x = %d, enemy y = %d, enemy health = %d\n\n", e->x, e->y, e->health);
}

void testEnemy()
{
    SCREEN_WIDTH_GLOBAL = 1000;
    SCREEN_HEIGHT_GLOBAL = 1000;
    sput_start_testing();
    sput_set_output_stream(NULL);
    
    sput_enter_suite("Test_createEnemy(): Creating valid enemies");
    sput_run_test(Test_createEnemy);
    sput_leave_suite();
    sput_finish_testing();
}



/*
* main function for unit testing


int main()
{
    createLevelPaths();
    createEnemyGroup();
    createSpecificEnemy(charBasic, 1, 0);
    createSpecificEnemy(charBasic, 1, 0);
    createSpecificEnemy(charHeavy, 1, 0);
    for(int i = 0; i < 100; i++) {
      moveEnemy(3);
      printEnemy(3);
    }
}
*/
