//
// sputFunctions.c
// Created by bclarke on 2014/09/11
// tags: stack, abstraction, struct pointers
//
/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/*---------- Hash Defines -----------*/
#define ITEMS_IN_ACTION_QUEUE 5

/*---------- Custom Headers	-----------*/

#include "../includes/sput.h"
#include "../includes/actionQueueDataStructure.h"
#include "../includes/abilities.h"

/*---------- Data Types -----------*/

struct queueNode {
	
	cmdType command; //command for execution
	cmdOption option; //stat to change
	int target;	//target of command
    struct queueNode *nextNode;

};

struct actionQueueStructure  {
	
    struct queueNode *start;
    struct queueNode *current;
    int nItems;

};


/*---------- Functions ----------*/

/*int main()	{
	//testingGameStructure();
	testingActionQueue();
}*/
void testingActionQueue()	{

	sput_start_testing();
	
	sput_set_output_stream(NULL);

	sput_enter_suite("testCheckMem(): memory check");
    sput_run_test(testCheckMem);
    sput_leave_suite();
	
	//sput_enter_suite("testPopFromQueue(): Popping from queue");
	//sput_run_test(testPopFromQueue);
	//sput_leave_suite();

	sput_enter_suite("testPushToQueue(): Pushing to queue");
	sput_run_test(testPushToQueue);
	sput_leave_suite();

	sput_finish_testing();
}

/*
 *Creates queue.  Must be run at the start of every game.
 */
ActionQueueStructure createActionQueue()	{

	ActionQueueStructure newActionQueue = (ActionQueueStructure) malloc(sizeof(*newActionQueue));
	newActionQueue->nItems = 0;
	newActionQueue->start = newActionQueue->current = NULL;
	getQueue(newActionQueue);

	return newActionQueue;
}

/*
 *puts node to back of queue
 */
int createNode(ActionQueueStructure queue)	{
	
	if(queue->start == NULL )	{
		queue->start = queue->current = (QueueNode) malloc(sizeof(*(queue->start)));
		queue->start->nextNode = NULL;
	} else {
		queue->current->nextNode = (QueueNode) malloc(sizeof(*(queue->current->nextNode)));
		queue->current = queue->current->nextNode;
		queue->current->nextNode = NULL;
	}

	return 1;

}

/*
 *Returns actionqueue address
 */
ActionQueueStructure getQueue(ActionQueueStructure queue)	{

	static ActionQueueStructure stQueue;

	if(queue != NULL)	{
		stQueue = queue;	
	}
	
	return stQueue;

}
/*
 * Pushes values to newly created node at back of queue
 */
int pushToQueue(ActionQueueStructure queue, cmdType command, cmdOption option, int target)	{
		createNode(queue);
		queue->current->command = command;
		queue->current->option = option;
		queue->current->target = target;
		(queue->nItems)++;
		return (queue->nItems);
}

void testPushToQueue()	{
	
    cmdType nCommand_1=cmd_upgrade;
    cmdOption nStat_1=upgrade_power;
    int tar_1 = 1;

    cmdType nCommand_2=cmd_execute;
    cmdOption nStat_2=upgrade_range;
    int tar_2 = 2;

    GameProperties newGame = createGame();
    ActionQueueStructure newQueue = createActionQueue();

	sput_fail_unless(pushToQueue(newQueue,nCommand_1,nStat_1,tar_1) == 1,"Valid: 1 Queue Item");
	sput_fail_unless(pushToQueue(newQueue,nCommand_2,nStat_2,tar_2) == 2,"Valid: 2 Queue Items");
	sput_fail_unless(getFirstCommand(newQueue) == cmd_upgrade,"Valid: Top of Queue Command");
	sput_fail_unless(getFirstOption(newQueue) == upgrade_power,"Valid: Top of Queue Option");
	sput_fail_unless(getLastCommand(newQueue) == cmd_execute,"Valid: Last of Queue Command");
	sput_fail_unless(getLastOption(newQueue) == upgrade_range,"Valid: Last of Queue Option");
	addMemory(10);

}
/*
 *Returns first command in queue
 */
cmdType getFirstCommand(ActionQueueStructure queue)	{

	return queue->start->command;
}

/*
 *Returns last command in queue
 */
cmdType getLastCommand(ActionQueueStructure queue)	{

	return queue->current->command;
}

/*
 *Returns first option in queue
 */
cmdOption getFirstOption(ActionQueueStructure queue)	{

	return queue->start->option;

}

/*
 *Returns last option in queue
 */
cmdOption getLastOption(ActionQueueStructure queue)	{

	return queue->current->option;

}

/*
 *Returns first target in queue
 */
int getFirstTarget()	{

	return (getQueue(NULL))->start->target;

}

/*
 *Returns last target in queue
 */
int getLastTarget()	{

	return (getQueue(NULL))->current->target;

}
/*
 *Returns costs of command based on current tower stats
 */
int calulateCosts(cmdType cmd, cmdOption opt, int target)    {

    switch(cmd)
    {
        case cmd_upgrade:
        {
            return ((getTowerLevel(target))*getCurrentStat(opt,target));
        }
        case cmd_mktwr:
        {
            return getCostOfNewTower();
        }
        case cmd_aptget:
        {
            return getCostOfAptget(opt);
        }
        default:

        break;
    }

    return 0;
}

int getCostOfAptget (cmdOption option)
{
    if(option==aptget_kill)
    {
        return KILL_COST;
    }
    else return 0;
    
}
/*
 *Returns specified stat of specified tower
 */
int getCurrentStat(cmdOption stat,int target)	{

	switch(stat)	{
		case upgrade_power:
				return getTowerDamage(target);
				break;	
		case upgrade_range:
				return getTowerRange(target);
				break;	
		case upgrade_speed:
				return getTowerSpeed(target);	
				break;	
		case upgrade_AOErange:
				return getTowerAOErange(target);
				break;	
		case upgrade_AOEpower:
				return getTowerAOEpower(target);
				break;	
		case upgrade_level:
				return getTowerLevel(target);
				break;	
		default:
				return 0;
				break;
	}
}

/*
 *Upgrades tower stat
 */
cmdOption upgradeTowerStat(cmdOption stat, int target)  {

    switch(stat)    {
        case upgrade_power:
        {
            if(upgradeDmg(target))  {
                return upgrade_power;
            }

        }
        case upgrade_range:
        {
            if(upgradeRange(target))    {
                return upgrade_range;
            }
        }
        case upgrade_speed:
        {
            if(upgradeSpeed(target))    {
                return upgrade_speed;
            }
        }
        case upgrade_AOErange:
        {
            if(upgradeAOErange(target)) {
                return upgrade_AOErange;
            }
        }
        case upgrade_AOEpower:
        {
            if(upgradeAOEpower(target)) {
                return upgrade_AOEpower;
            }
        }
        default:
            fprintf(stderr,"upgradeTowerStat tower.c: unrecognised stat\n");
            return optionError;
    }
}
/*
 *Checks start of action Queue for command, and actions it if all criteria are met
 */
int popToTower()	{
	ActionQueueStructure queue = getQueue(NULL);
	GameProperties Game = getGame(NULL);
	int needed;
	if(queue->start != NULL) {
		needed = calulateCosts(queue->start->command,queue->start->option,queue->start->target);
		switch(queue->start->command)	{
			case cmd_upgrade:
				if (checkQueue(queue, Game,needed)) {
					ActionQueueStructure queue = getQueue(NULL);
					GameProperties Game = getGame(NULL);
					upgradeTowerStat(queue->start->option,queue->start->target);
					useMemory(Game, needed);
					removeQueueItem();
				}
				break;
			case cmd_mktwr:
				//! request tower type ignored for now.
				if (checkQueue(queue,Game,needed)) {
					createTowerFromPositions(queue->start->target);
					useMemory(Game, needed);
					removeQueueItem();
				}
				break;
			default:

				break;
		}
	} else {
		return 0;
	}
	return 1;
}

void removeQueueItem()	{
    ActionQueueStructure queue = getQueue(NULL);
    GameProperties Game = getGame(NULL);
	QueueNode tempStart = queue->start;
    queue->start = queue->start->nextNode;
	free(tempStart);
	setlastAction(Game);
	--(queue->nItems);
}


/*
 *Pops from front of Queue.
 */
int popFromQueue(ActionQueueStructure queue, cmdType *cmd, cmdOption *stat, int *target)	{
    GameProperties Game = getGame(NULL);
    int needed = calulateCosts(*cmd,*stat,*target);

	if((queue->start != NULL) && (checkQueue(queue,Game, needed)))	{ //!	testing target, available Memory, cooldown time 
		printf("popping from queue\n");
		*cmd = queue->start->command;
		*stat = queue->start->option;
		*target = queue->start->target;
		QueueNode tempStart = queue->start;
		queue->start = queue->start->nextNode;	
		free(tempStart);	
		--(queue->nItems);
		useMemory(Game, needed);	//use memory
		setlastAction(Game);	//activate cooldown timer.

		return 1;
	}
		return 0;
}

/*
 * Checks Top of queue to ensure target and time is correct
 */

int checkQueue(ActionQueueStructure queue, GameProperties Game, int needed)	{
	if((checkMem(needed, Game)) && (checkClock(lastCmdAction,ACTIONCOOLDOWN)))	{

			printf("success!\n");
			return 1;		
	} 
	return 0;	
}

/*
 *Checks required Memory is less than the Memory available to use
 */

int checkMem(int needed, GameProperties Game)	{
	if(needed > getAvailableMemory(Game)) {
		return 0;
	}
		return 1;
}

void testCheckMem()	{

	GameProperties testGame;
    testGame = createGame();
	addMemory(10);
	sput_fail_unless(checkMem(10,testGame) == 1,"boundary Testing enough memory");
	useMemory(testGame,10);
	sput_fail_unless(checkMem(50,testGame) == 0,"Testing not enough memory");
	addMemory(100);
	sput_fail_unless(checkMem(10,testGame) == 1,"Testing enough memory");

	free(testGame);
}

/*
 *Extract display string from first N nodes in action queue and return output string
 */

char *getActionQueueString(void) {
    
    QueueNode p = getQueue(NULL)->start;
    char *outputString = malloc(500);
    char *targetString = malloc(4);
    
    for(int count = 0; p!= NULL && count < ITEMS_IN_ACTION_QUEUE; p = p->nextNode, count++) {
        cmdType command = p->command;
        cmdOption option = p->option;
        int target = p->target;
        
        switch(command) {
            case cmd_upgrade:
                strcat(outputString, "upgrade");
                break;
            case cmd_execute:
                strcat(outputString, "execute");
                break;
            case cmd_set:
                strcat(outputString, "set");
                break;
            case cmd_mktwr:
                strcat(outputString, "aptget");
                break;
            case cmd_aptget:
                strcat(outputString, "mktwr");
                break;
            default:
                continue;
        }
        
        strcat(outputString, " ");
        
        switch(option) {
            case upgrade_power:
                strcat(outputString, "p");
                break;
            case upgrade_range:
                strcat(outputString, "r");
                break;
            case upgrade_speed:
                strcat(outputString, "s");
                break;
            case upgrade_AOErange:
                strcat(outputString, "AOEr");
                break;
            case upgrade_AOEpower:
                strcat(outputString, "AOEp");
                break;
            case upgrade_level:
                strcat(outputString, "level");
            case mktwr_int:
                strcat(outputString, "INT");
            case mktwr_char:
                strcat(outputString, "CHAR");
            case aptget_kill:
                strcat(outputString, "kill");
            default:
                continue;
        }
        
        if(target) {
            sprintf(targetString, " t%d", target);
            strcat(outputString, targetString);
        }
        
        
        strcat(outputString, "\n");
        
    }
    
    return outputString;
}
