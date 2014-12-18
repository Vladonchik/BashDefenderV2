#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#include "../includes/sput.h"
#include "../includes/levelController.h"
#include "../includes/abilities.h"
#include "../includes/tower.h"
#include "./../includes/actionQueueDataStructure.h"
#include "../includes/parser.h"
#include "../includes/main.h"
#include "../includes/enemy.h"
#include "../includes/Sound.h"


int main(int argc, char ** argv)
{
    Display d = init_SDL();
    initLevel();
	/*while(1)	{
		levelQueueReader();
	}*/

//    //testing();

    //userCreateTower(rand()% (int) (MAP_WIDTH-80), rand()% (int) (MAP_HEIGHT-80));//createEnemy();
  	//createEnemy();
    char text[128] = {'>', '>'};
    char empty[128] = {'>', '>'};
    char *pass, *clear, *inputCommand=NULL;
    pass = text;
    clear = empty;
	int started = 0;
    int ended = 0;
    
    addMemory(100);
    int steps=0;

    initialiseParseLists();

	init_abilities();
    //init_sound();
    //playBackgroundSound();

	
    do{
        startFrame(d);
        while(started == 0){
            menu_screen(d, &started);
        }
        ++steps;
        drawBackground();
        
        levelQueueReader();
        terminal_window(d, pass, clear);
		popToTower();
        if(inputCommand)
        {
            parse(inputCommand);
        }
        present_enemy(d);
        present_tower(d);

    	fire();
        for(int i=1; i<=getNumberOfEnemies(); ++i)
        {
            int move = moveEnemy(i);
        }
        presentAnimation();
		drawAllTowerPositions();
        statsBar();
        towerMonitor(0, NULL);
        actionQueueMonitor();

        drawRect(10, 10, 150, 150, 80, 10, 100, 100);
        endFrame(d);
        
        ended = checkIfPlayerDead();
        while (ended) {
            //final screen returns 1 if restart button was pressed...
            //if (final_screen()){
            //    ended = 0;
            //}
            final_screen();
        }
        
    } while(!terminal_window(d, pass, clear));
    
    shutSDL(d);
    quitGame();
    return 0;
}

void quitGame()
{
    //shutSound();
    freeEnemyGroup();
    freeLevelPaths();
    freeParseLists();
}

void testing()	{

	testingTowerPositions();
    //testingGameStructure();
    //testingActionQueue();
    //parseToQueueTesting();
    //testEnemy();
    //testingTowerModule();
	parseToTowerTesting();
    //towerToEnemyTesting();

    //testValidParses();
    

}

void towerToEnemyTesting()	{

	sput_start_testing();
	sput_set_output_stream(NULL);

	sput_enter_suite("testEnemyInRange(): tower detecting single enemy in range");
	sput_run_test(testEnemyInRange);
	sput_leave_suite();

	sput_finish_testing();
}

void testEnemyInRange()	{

	createEnemy();
	setEnemyHealth(1,100);
	setEnemyArmour(1,0);
	setEnemyX(1,50);
	setEnemyY(1,50);
	setTowerY(1,400);
	setTowerX(1,400);
	setTowerRange(1,10);
	setTowerDamage(1,10);
	sput_fail_unless(inRange(400,400,10,1)== 0, "Enemy 1 is out of range of tower 1");
	fire();
	sput_fail_unless(getEnemyHealth(1) == 100, "Out of range enemy 1 has full health after tower has fired");
	setEnemyX(1,395);
	setEnemyY(1,395);
	sput_fail_unless(inRange(400,400,10,1)== 1, "Enemy 1 is in range of tower 1");
	sput_fail_unless(getEnemyHealth(1) == 100, "Enemy 1 has full health");
	fire();
	//sput_fail_unless(getEnemyHealth(1) == 100 - getTowerDamage(1),"In range enemy has reduced health from tower damage");
	int i;
	for(i = 0; i < 9; i++)	{
	fire();
	}
	sput_fail_unless(isDead(1) == 1, "Enemy dead after being fired on 10 times");	
	
}

void parseToTowerTesting()	{

	sput_start_testing();
	sput_set_output_stream(NULL);

	sput_enter_suite("testParseToTower():Tower popping parsed commands");
	sput_run_test(testParseToTower);
	sput_leave_suite();
	
	sput_finish_testing();

}

void parseToQueueTesting()	{
	
	sput_start_testing();
	sput_set_output_stream(NULL);

	sput_enter_suite("testValidParses():Testing Valid parsed commands being placed on queue");
	sput_run_test(testValidParses);
	sput_leave_suite();
	
	sput_finish_testing();
}

void testParseToTower()	{

	createTower();
	createTower();
	setTowerRange(1,10); //Setting tower range to 10 for tests.
	setTowerDamage(2,10); //Setting tower damage to 10 for tests.
	addMemory(1000);
	parse("upgrade r t1");
	parse("upgrade p t2");
	sput_fail_unless(getFirstTarget() == 1, "First target is 1");
	sput_fail_unless(getLastTarget() == 2, "Last target is 2");
	delayGame(ACTIONCOOLDOWN);
	popToTower();
	sput_fail_unless(getTowerRange(1) == 11, "Upgraded range is 11");
	delayGame(ACTIONCOOLDOWN);
	popToTower();
	sput_fail_unless(getTowerDamage(2) == 11, "Upgraded damage is 11");
	freeAllTowers();
}

void testValidParses()	{
	
	createActionQueue();
	createTowerGroup();
	createLevelPaths();
	createTower();
    //  printf("\n135\n\n");
    sput_fail_unless(parse("upgrade r t1")== 1, "upgrade r t1 is valid command");
	sput_fail_unless(getFirstCommand(getQueue(NULL)) == upgrade, "First command in queue: upgrade");
	sput_fail_unless(getFirstOption(getQueue(NULL)) == upgrade_range, "First option in queue: range");
    //printf("\n139\n\n");
    sput_fail_unless(parse("upgrade p t1")== 1, "upgrade p t1 is valid command");
	sput_fail_unless(getLastCommand(getQueue(NULL)) == upgrade, "Last comand in queue: upgrade");
	sput_fail_unless(getLastOption(getQueue(NULL)) == upgrade_power, "Last option in queue: power");
    //printf("\n143\n\n");

    sput_fail_unless(parse("upgrade s t1")== 1, "upgrade s t1 is valid command");
	sput_fail_unless(getLastCommand(getQueue(NULL)) == upgrade, "Last comand in queue: upgrade");
	sput_fail_unless(getLastOption(getQueue(NULL)) == upgrade_speed, "Last option in queue: speed");
	sput_fail_unless(getFirstCommand(getQueue(NULL)) == upgrade, "First command in queue: upgrade");
	sput_fail_unless(getFirstOption(getQueue(NULL)) == upgrade_range, "First option in queue: range");
    //printf("\n149\n\n");
    sput_fail_unless(parse("  ??D--") == 0, "  ??D-- is invalid command");
    //printf("\n151\n\n");
    sput_fail_unless(parse("upgrade r r1") == 0, "upgrade r r1 is invalid command");
    //printf("\n153\n\n");
    sput_fail_unless(parse("upgrade r t") == 0, "upgrade r t is invalid command");
    //printf("\n155\n\n");
    sput_fail_unless(parse("upgrade t") == 0, "upgrade t is invalid command");
    //printf("\n157\n\n");
    sput_fail_unless(parse("cat t") == 0, "cat t is invalid command");
    //printf("\n159\n\n");




	free(getQueue(NULL));
}

