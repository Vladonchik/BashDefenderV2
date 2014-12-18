#include "../includes/abilities.h"

void init_abilities()
{
	static Ability psx, kill_abil, killall;
	psx.unlocked = 1;
	psx.cost = PSX_COST;
	kill_abil.unlocked = 0;
	kill_abil.cost = KILL_COST;
	killall.unlocked = 0;
	killall.cost = KILLALL_COST;
}
	
void unlock_ability(Ability *abilityx)
{
	char unlockstring[17] = "Ability unlocked";
	abilityx->unlocked = 1;
	updateTowerMonitor(unlockstring);		
}

int is_available_ability(Ability *ability)
{

	if(ability->unlocked == 1 && getTotalMemory() > ability->cost)
	{
		return 1;
	}
	return 0;
}

void psx_ability()
{
	char psxlist[500] = {'\0'};
	char template[15] = "EnemyID Health";
	char newline[2] = "\n";
	char IDstr[9];
	char healthstr[10];
	char clear[500] = {'\0'};
	int enemy_number = getNumberOfEnemies();

	int health = 0, ID = 0, i, j, tmp = 0, tmp2 = 0;
	int healtha[200] = {0};
	int enemyIDa[200] = {0};
    //if(is_available_ability(psx) == 1)
	{
		for(j = 1; j <= enemy_number; j++)
		{
			if(healtha[j] <= getEnemyHealth(j))
			{
				tmp = healtha[j];
				healtha[j] = getEnemyHealth(j);
				healtha[j+1] = tmp;
				tmp2 = enemyIDa[j];
				enemyIDa[j+1] = tmp2;
				enemyIDa[j] = j;
				
			}
			else
			{
				healtha[j+1] = getEnemyHealth(j);
				enemyIDa[j+1] = j;
			}
		}
		strcat(psxlist, template);
		for(i = 1; i <= enemy_number; i++)
		{
			strcpy(psxlist, template);
			if(!isDead(i))
			{
				ID = enemyIDa[i];
				health = healtha[i];
				sprintf(IDstr, "%d ", ID);
				sprintf(healthstr, "%d", health); 
				strcat(psxlist, newline);
				strcat(psxlist, IDstr);
				strcat(psxlist, healthstr);
				strcat(psxlist, newline);
			}
		}
		updateTowerMonitor(psxlist);
		strcpy(psxlist, clear);
	}
}

void man_ps()
{
	towerMonitor(-1, "GENERAL COMMANDS MANUAL: \n\nps\n\ntype ""ps"" followed by a command\n ( -x\n ) to discover information about one or more enemies\nExamples:\nps -x\n");
}

void man_kill()
{
	towerMonitor(-1, "GENERAL COMMANDS MANUAL: \n\nps\n\ntype ""kill -9"" followed by a target enemyID (eg 6) or *all*\n to kill one or more enemies\nExamples:\nkill -9 7\n kill -9 all");
}


/*
void kill_ability(int enemyID, Ability *kill)
{
	if(is_available_ability(kill) == 1)
	{
		killEnemy(enemyID);
	}
}

int kill_all_ability(Ability *killall)
{
	int i;
	int enemy_number = getNumberOfEnemies();

	if(is_available_ability(killall) == 1)
	{
		for(i = 1; i <= enemy_number; i++)
		{
			killEnemy(i);
		}
		return 0;
	}
	else
	{
		return -1;
	}
	return 0;
}
*/

	



