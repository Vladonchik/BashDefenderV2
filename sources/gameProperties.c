//
// sputFunctions.c
// Created by bclarke on 2014/09/11
//
/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/*---------- Custom Headers	-----------*/
#include "../includes/sput.h"
#include "./../includes/gameProperties.h"

/*---------- Data Types -----------*/

struct gameClock	{

	clock_t start_t; //! time game started.  Must be ran at startup. 
	clock_t lastAction;
	ClockNode first;
	ClockNode last;
};

struct clockNode	{

	clockType type;
	clock_t time;
	ClockNode next;

};


struct gameProperties {

	int memoryUsed;
	int totalMemory;
	int currWaveNo;
	int totalWaveNo;
	int health;
    int costOfNewTower;
	GameClock clock;
	int deathCount;
	int createEnemyGroupDelay;
};

/*---------- Functions ----------*/

/*
 *Delays game by specified amount
 */
clock_t delayGame(int delayN)	{

	clock_t ticks1, ticks2,timeWaited;
    ticks1=clock();
    timeWaited = ticks2=ticks1;
    while((ticks2/CLOCKS_PER_SEC-ticks1/CLOCKS_PER_SEC)<delayN){
        ticks2=clock();
		timeWaited = (ticks2/CLOCKS_PER_SEC-ticks1/CLOCKS_PER_SEC);
	}
	return timeWaited;
}


/*
 *Sets lastAction member to current clock time
 */
int setlastAction(GameProperties Game)	{
	Game->clock->lastAction = clock()/CLOCKS_PER_SEC;
	return (int) Game->clock->lastAction;
}

void setClock(clockType clockToSet)	{

		//*clockToSet = clock()/CLOCKS_PER_SEC;
}

/*
 *Increases Death Count
 */
void increaseDeathCnt()	{

	GameProperties game = getGame(NULL);
	game->deathCount++;
}


/*
 *Returns Death Count
 */
int getDeathCnt()	{

	return getGame(NULL)->deathCount;
}

/*
 *Returns total waves for current level
 */
int getTotalWaveNo()	{

	return getGame(NULL)->totalWaveNo;
}

/*
 *Sets total Waves this level
 */
void setTotalWaveNo(int totalW)	{

		getGame(NULL)->totalWaveNo = totalW;
	
}

void testSetLastAction()	{

 	GameProperties newGame = createGame();
	clock_t currTime = clock()/CLOCKS_PER_SEC;
	delayGame(2);
	sput_fail_unless(setlastAction(newGame) == (currTime + 2),"Setting Last Action to current time");
	delayGame(2);
	sput_fail_unless(setlastAction(newGame) == (currTime + 4),"Setting Last Action to current time");
	free(newGame->clock);
	free(newGame);
}

/*
 * returns true is last action time is more than global cooldown
 */
int lastAction(GameProperties Game)	{

	clock_t currTime = clock() / CLOCKS_PER_SEC;

	clock_t timeSinceLastAction = currTime - Game->clock->lastAction;
	if (timeSinceLastAction >= ACTIONCOOLDOWN)	{
		Game->clock->lastAction = currTime;
		return 1;	
	}
		return 0;
}



/*
 *Damages play health with specified amount of damage
 */
void damageHealth(int damage)	{

	getGame(NULL)->health -= damage;
}



void testlastAction()	{

	GameProperties newGame = createGame();
	delayGame(ACTIONCOOLDOWN);
	sput_fail_unless(lastAction(newGame) == 1,"Checking delay more than Cooldown returns true");
	delayGame(ACTIONCOOLDOWN-1);
	sput_fail_unless(lastAction(newGame) == 0,"Checking delay less than Cooldown returns false");
	free(newGame->clock);
	free(newGame);

}

void testingGameStructure()	{
	sput_start_testing();
	
	sput_set_output_stream(NULL);

	sput_enter_suite("testlastAction(): Cooldown checking");
	sput_run_test(testlastAction);
	sput_leave_suite();
	
	sput_enter_suite("testSetLastAction(): Setting last action to current clock");
	sput_run_test(testSetLastAction);
	sput_leave_suite();

	sput_enter_suite("CreateGameTest(): Creation & Initialization");
	sput_run_test(CreateGameTest);
	sput_leave_suite();

	sput_enter_suite("TestGetAvailableMemory(): Current Memory Available");
	sput_run_test(TestGetAvailableMemory);
	sput_leave_suite();

	sput_enter_suite("TestAddMemory(): Adding Memory");
	sput_run_test(TestAddMemory);
	sput_leave_suite();

	sput_enter_suite("TestUseMemory(): Using More Memory");
	sput_run_test(TestUseMemory);
	sput_leave_suite();

	sput_finish_testing();
}

void CreateGameTest()	{

	GameProperties testGame;
	testGame = createGame();
	sput_fail_if((createGame()) == NULL,"Creating Game");
	sput_fail_unless(getAvailableMemory(testGame) == 0,"Initializing Memory");
	sput_fail_unless(getWave(testGame) == 0,"Initializing WaveNo");
	sput_fail_unless(getHealth(testGame) == 0,"Initializing Health");
	free(testGame);
}

/*
 *Returns or sets memory address of game structure`
 */
GameProperties getGame(GameProperties createdGame)	{

	static GameProperties game;

	if (createdGame != NULL)	{
		game = createdGame;
	}
	return game;

}

/*
 *Returns the current wave number
 */
int getWave(GameProperties game)	{

	return game->currWaveNo;

}

int getHealth(GameProperties game)	{

	return game->health;
}

/*
 * Checks if health is 0
 */
int checkIfPlayerDead()   {

    if(getHealth(getGame(NULL)) <= 0)   {
        return 1;
    }
    
    return 0;

}

/*
 *Returns cost of new tower
 */
int getCostOfNewTower() {

    return getGame(NULL)->costOfNewTower;
}
/*
 *Creating Game Clock linked list
 */
GameClock createClock()	{
	GameClock clock = (GameClock) malloc(sizeof(*clock));	
	clock->first = NULL;
	clock->last = NULL;
	getClock(clock);
	return clock;
}

GameClock getClock(GameClock clock)	{

		static GameClock currClock;
		if(clock != NULL)	{
			currClock = clock;
		}

		return currClock;

}

/*
 * Add Clock Node
 */
void addClock(clockType type)	{
	
	GameClock clock = getClock(NULL);	
	if(checkUniqueClockType(type))	{
		if(clock->first == NULL)	{
			clock->first = clock->last = createClockNode(type);
		} else {
			clock->last->next = createClockNode(type);
			clock->last = clock->last->next;
		}
	}
}

ClockNode createClockNode(clockType type)	{
	ClockNode newNode;
	newNode = (ClockNode) malloc(sizeof(*newNode));
	newNode->next = NULL;
    newNode->time = clock() / (CLOCKS_PER_SEC/100);
	newNode->type = type;
	return newNode;
}

/*
 *Checks Clock Type is unique
 */

int checkUniqueClockType(clockType type)	{

	GameClock clock = getClock(NULL);
	ClockNode currNode;
	currNode = clock->first;
	while(currNode!= NULL)	{
		if (currNode->type == type)	{
			return 0;
		}
		currNode = currNode->next;
	}

	return 1;

}

int checkClock(clockType cType,int coolDown)	{
	GameClock gClock = getClock(NULL);
	ClockNode currNode;
	currNode = gClock->first;

	clock_t currTime = (double) clock() / (CLOCKS_PER_SEC/100);
	while(currNode!= NULL)	{
		if (currNode->type == cType)	{
			if((currTime - currNode->time) >= coolDown)	{
				setCurrTime(currNode);
				return 1;
			} else {
				return 0;
			}	
		}
		currNode = currNode->next;
	}

	fprintf(stderr,"clock does not exist\n");
	return 0;
}

void setCurrTime(ClockNode node)	{
	node->time = (double) clock() / (CLOCKS_PER_SEC/100);
}

/*
 *Creates game structure.  Needs to be run in level init
 */
GameProperties createGame()	{

	GameProperties newGame = (GameProperties) malloc(sizeof(*newGame));
	newGame->clock = createClock();

	newGame->totalMemory=1000;
	newGame->memoryUsed=0;
	newGame->currWaveNo=0;
	newGame->totalWaveNo = 0;
	newGame->health=100;
    newGame->costOfNewTower = 300;
	newGame->deathCount = 0;
	newGame->clock->start_t  = newGame->clock->lastAction = (double) clock()/CLOCKS_PER_SEC;
	newGame->createEnemyGroupDelay=0;
	getGame(newGame);
	return newGame;

}

int getEnemyGroupDelay()	{
	return(getGame(NULL)->createEnemyGroupDelay);
}

void setCreateEnemyGroupDelay(int delay)	{

	getGame(NULL)->createEnemyGroupDelay = delay;
}

/*
 *Returns amount of Memory available
 */
int getAvailableMemory(GameProperties game)	{

	return game->totalMemory - game->memoryUsed;
}

int getTotalMemory()	{

	return(getAvailableMemory(getGame(NULL)));
}


void TestGetAvailableMemory()	{
	GameProperties testGame;
    testGame = createGame();
	testGame->totalMemory = 10;
	sput_fail_unless(getAvailableMemory(testGame) == 10,"Getting Memory");	
	free(testGame);
}

/*
 *Adds specified amount of Memory to resources
 */
int addMemory(int mem)	{
	GameProperties game = getGame(NULL);
	if(mem > 0)	{
		game->totalMemory+=mem;	
		return 1;
	} 

	return 0;
}

void TestAddMemory()	{

	GameProperties testGame;
    testGame = createGame();
	addMemory(100);
	sput_fail_unless(getAvailableMemory(testGame) == 100,"Adding MEmory");
	sput_fail_unless(addMemory(-100) == 0,"Adding Negative Memory");
	free(testGame);
}

/*
 *Uses specified amount of memory
 */
int useMemory(GameProperties game,int mem)	{

	if (game->totalMemory-game->memoryUsed >= mem)	{
		game->totalMemory+=mem;
		return 1;
	} else {
		return 0;
	}
}

void TestUseMemory()	{

	GameProperties testGame;
    testGame = createGame();
	testGame->totalMemory = 100;
	useMemory(testGame,50);
	sput_fail_unless(getAvailableMemory(testGame) == 50,"Subtracting Memory");
	sput_fail_unless(useMemory(testGame,100) == 0,"Subtracting too much Memory");
	free(testGame);
}
