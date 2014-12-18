#ifndef gameProperties_h
#define gameProperties_h
#include <time.h>

#include "./../includes/debug.h"
#include "../includes/tower.h"

/*----------Enumerated Types-----------*/
enum cmdType
{

    cmd_commandError=-1,
    cmd_upgrade=1,
    cmd_execute=2,
    cmd_set=3,
    cmd_man=4,
    cmd_cat=5,
    cmd_mktwr=6,
    cmd_aptget=7,
    cmd_ps=8,
    cmd_kill=9
};


enum cmdOption
{
    optionError=-1,
    //upgrade(tower stats):
    upgrade_power=1,
    upgrade_range=2,
    upgrade_speed=3,
    upgrade_AOErange=4,
    upgrade_AOEpower=5,
    upgrade_level=6,
    //mktwr (tower types):
	mktwr_int=7,
	mktwr_char=8,
    //ps (option)
    ps_x=9,
    //kill (options)
    kill_minus9=10,//kills a single targeted enemy
    kill_all=11,//kills all enemies
    //aptget (commands to install):
    aptget_kill=12,
    
    
};

typedef enum clockType	{

	start_t = 0,
	lastCmdAction = 1,
	enemyGroupCreated1 = 2,
	singleEnemyCreated = 3,	//!delay between single enemies being created
	groupDelay = 4 	//!Delay between groups of enemies being created
} clockType;

/*----------Symbolic Constants-----------*/

#define ACTIONCOOLDOWN	30	//! minimum time between actions being removed from queue
#define ENEMYSPAWNCOOLDOWN 2 	//!minimum time between enemy spawns

/*----------TypeDefs----------*/

typedef enum cmdType cmdType;
typedef enum cmdOption cmdOption;
typedef struct gameProperties *GameProperties;
typedef struct gameClock *GameClock;
typedef struct clockNode *ClockNode;
/*----------Function Prototypes-----------*/
GameProperties createGame();
int getAvailableMemory(GameProperties game);
int useMemory(GameProperties game,int mem);
int getWave(GameProperties game);
int getHealth(GameProperties game);
clock_t delayGame(int delayN);
int lastAction(GameProperties Game);
int setlastAction(GameProperties Game);
GameProperties getGame(GameProperties createdGame);
void addClock(clockType type);
GameClock getClock(GameClock clock);
int checkUniqueClockType(clockType type);
ClockNode createClockNode(clockType type);
void damageHealth(int damage);
void setCreateEnemyGroupDelay(int delay);
int addMemory(int mem);
int getCostOfNewTower();
int getDeathCnt();
void increaseDeathCnt();
int getEnemyGroupDelay();
int checkIfPlayerDead();
void setTotalWaveNo(int totalW);
int getTotalWaveNo();
GameClock createClock();
int checkClock(clockType cType,int coolDown);
void setCurrTime(ClockNode node);
void testingGameStructure();
void CreateGameTest();
void TestGetAvailableMemory();
void TestAddMemory();
void TestUseMemory();
void setEnemyCreated1();
int getTotalMemory();
#endif
