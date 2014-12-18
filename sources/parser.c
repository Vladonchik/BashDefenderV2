//
//  paser.c
//  The parser needs to accept a string from the text input
//
//  Created by ben on 07/11/2014.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../includes/tower.h"
#include "../includes/actionQueueDataStructure.h"
#include "./../includes/parser.h"
#include "../includes/Information_Window.h"
#include "../includes/abilities.h"
#include "../includes/enemy.h"



/*
 * Parse called with string of user input from terminal window.
 * reads the first token and calls the relevant command function 
 * returns zero if syntax error.
 */
void testStringLists()
{
    stringList * cmdList = getCommandList(NULL);
    testCommandArray(cmdList->stringArray,cmdList->numberOfStrings);
    stringList * optList = getOptionList(NULL);
    testCommandArray(optList->stringArray,optList->numberOfStrings);
}
int parse(char *inputString)
{
    size_t len = 1+strlen(inputString);//gets the size of inputString
    if( len < 3*sizeof(char)  )
    {
        optionUsageError();
        return 0;
    }

    int numberOfChunks;
    char **commandArray = breakUpString(inputString, &numberOfChunks, " ");
    //array of strings, each elem holds a token string from the input command
    int minNumberOfChunks = 2,//as of cat man and upgrade
        maxNumberOfChunks = 3;//being implemented
    if( numberOfChunks<minNumberOfChunks || maxNumberOfChunks>3)
    {
        optionUsageError();
        freeCommandArray(commandArray, numberOfChunks);
        return 0;//no valid commands with less than 2 strings or more than 3
    }

    //enumerated type cmdType can describe each of the possible commands(see actionQueue.h)
    cmdType command = getCommandType(commandArray[0]);//the first string in the command should contain the action

    if(command==cmd_commandError)//if getAction returns commandError then the input is invalid
    {                //Error messaging handled in getAction function
        freeCommandArray(commandArray, numberOfChunks);
        return 0;
    }
    printf("cmdTyp = %d \n",command);
    int specificReturns=0;//stores return values of the different functions that execute the commands
    /**** Now we deal with each possible command separately as they all have different syntax ****/
    switch (command)
    {
        case cmd_upgrade:
        {
            if(numberOfChunks!=3) {
                optionUsageError();
                specificReturns = 0;
            }
            else {
            specificReturns = parseUpgrade(commandArray, numberOfChunks);
            }
            break;

        }
        case cmd_cat:
        {
            if(numberOfChunks!=2) {
                optionUsageError();
                specificReturns = 0;
            }
            else {
                specificReturns = parseCat(commandArray[1]);
            }
            break;

        }
        case cmd_man:
        {
            if(numberOfChunks!=2) {
                optionUsageError();
                specificReturns = 0;
            }
            else {
                specificReturns = parseMan(commandArray[1]);
            }
            break;

        }
        case cmd_mktwr:
        {
            if(numberOfChunks!=3)
            {
                optionUsageError();
                specificReturns = 0;
            }
            else {
                specificReturns = parseMktwr(commandArray);
            }
            break;

        }
        case cmd_aptget:
        {
            if(numberOfChunks!=2) {
                optionUsageError();
                specificReturns = 0;
            }
            else {
                specificReturns = parseAptget(commandArray[1]);
            }
            break;

        }
        case cmd_ps:
        {
            if(numberOfChunks!=2) {
                optionUsageError();
                specificReturns = 0;
            }
            else {
                specificReturns = parsePs(commandArray[1]);
                
            }
            break;
        }
        case cmd_kill:
        {
            parseKill(commandArray, numberOfChunks);
            break;

        }
        case cmd_commandError:
        {
            printf("command was not read\n");
            break;
        }
        case cmd_execute:
        case cmd_set:
        default:
            fprintf(stderr,"\n***parsing not implemented yet returning***\n");
            specificReturns = 0;
    }
    freeCommandArray(commandArray, numberOfChunks);
    return specificReturns;//0 for error
}

/*
 *
 */
int parseKill(char ** commandArray,int numberOfChunks)
{
    cmdOption option = getCommandOption(commandArray[1]);
    
    if(option!=kill_minus9 && option!=kill_all)
    {
        optionUsageError();
        return 0;
    }
    if(option==kill_minus9) {
        if(numberOfChunks!=3) {
            optionUsageError();
            return 0;
        }
        else {
            int targetEnemyID = getTargetEnemy(commandArray[2]);//pass 3rd token
            //kill_ability(targetEnemyID);
            return 1;
        }
    }
    else if(option==kill_all) {
        //kill_all_ability();
        return 2;
    }
 
    return 0;
}

/*
 *  Called on cat and upgrade commands with the target specifying token.
 looks at the 2nd char in the string to find an int 1-9 to be the target.
 Note, wont work for anything > 9, would just see 1.
 Will print its own error message.
 Returns TargetTowerID if sucessful
 Returns 0 if error
 */
unsigned int getTargetEnemy(const char * inputStringTargeting)
{
    unsigned int numberOfEnemies = getNumberOfEnemies();// this is func in enemy.c
    
    size_t len = strlen(inputStringTargeting);//gets the size of string
    if( len<(2*sizeof(char)) )
    {
        fprintf(stderr,"*** SYNTAX ERROR: You must target a tower with this command ***\n");
        fprintf(stderr,"to target a tower enter t followed by a number 1 - %d \n",numberOfEnemies);
        return 0;
    }
    if (inputStringTargeting[0]!='e' && inputStringTargeting[0]!='E')
    {
        fprintf(stderr,"*** ERROR: You must target a enemy with this command ***\n");
        fprintf(stderr,"to target a enemy enter e followed by a number 1 - %d \n",numberOfEnemies);
        return 0;
    }
    
    unsigned int targetEnemyID = (unsigned int)(inputStringTargeting[1]-'0');
    
    if(targetEnemyID > numberOfEnemies || targetEnemyID<1)
    {
        fprintf(stderr,"*** ERROR: target enemy does not exist ***\n");
        fprintf(stderr,"there are only %d enemies you entered e%d\n",
                numberOfEnemies,targetEnemyID);
        return 0;
    }
    return targetEnemyID;
}
/*
 *
 */
int parsePs(char * optionString)
{
    cmdOption option = getCommandOption(optionString);
    if(option != ps_x) {
        optionUsageError();
        return 0;
    }
    else {
        psx_ability();
        return 1;
    }
}

/*
 *
 */
int parseAptget(char * aptToGetString)
{
    cmdOption aptToGet = getCommandOption(aptToGetString);
    if(aptToGet!=aptget_kill)
    {
        fprintf(stderr,"\n***app not recognised***\n");
        fprintf(stderr,"type man aptget to see availible apps\n");
        return 0;
    }
    if(pushToQueue(getQueue(NULL),cmd_aptget,aptToGet,0)>=1)
    {
        printf("pushing tower to queue\n");
        return 1;
    }
    else return 0;
}

/*
 *  Called when we read mktwr cmd.
 *  gets tower position and pushes to queue
 *  returns 1 if cmd was probably successfully pushed to queue
 *  returns 0 if definately not succesful or if target or stat call failed
 */
int parseMktwr(char ** commandArray)
{
    int towerPosition = tolower((int)commandArray[2][0]) - 'a' + 1;
    cmdOption twrType = getCommandOption(commandArray[1]);
    unsigned int numberOfTowers = getNumberOfTowers();//getNumberOfTowers(); this is func in tower.c
    if(towerPosition<1  || (twrType!=mktwr_int && twrType!=mktwr_char) )
    {
        optionUsageError();
        return 0;
    }
    
    if(pushToQueue(getQueue(NULL),cmd_mktwr,twrType,towerPosition)>=1)
    {
		printf("pushing tower to queue\n");
        return 1;
    }
    else return 0;
}

/*  calls man printing functions
 *  returns 1 if ok
    returns 0 if error and prints message
 */
int parseMan(char * inputStringCommandMan)
{
    cmdType commandToMan = getCommandType(inputStringCommandMan);
    switch (commandToMan)
    {
        case cmd_upgrade:
        {
            manUpgrade();
            return 1;
        }
        case cmd_cat:
        {
            manCat();
            return 1;
        }
        case cmd_man:
        {
            manMan();
            return 1;//0 for error
        }
        case cmd_execute:
        {
            //manExecute();
            return 1;
        }
        case cmd_set:
        {
            //manSet();
            return 1;
        }
        case cmd_mktwr:
        {
            return 1;
        }
        default:
        {
            fprintf(stderr,"\n*** Man Command Error ***\n");
            fprintf(stderr,"second command not recognised \n");
            fprintf(stderr,"you entered: %s\n",inputStringCommandMan);
            actionUsageError();
            return 0;
        }
    }
}



/*
 *  Called when we read cat cmd.
 *  gets target and pushes to info window.
 *  returns 1 if cmd was probably successfully pushed.
 *  returns 0 if definately not succesful or if target call failed.
 */
int parseCat(char * inputStringTargeting)
{
    //looks for tower type target:
    if( inputStringTargeting[0]=='t' || inputStringTargeting[0]=='T' )
    {
        unsigned int targetTower = getTargetTower(inputStringTargeting);
        if(targetTower)
        {
            towerMonitor(targetTower, NULL);//function in Information_Window.c
            return 1;
        }
        else
            return 0;
    }
    //can we also cat other things eg enemies?
    //for now
    else
        return 0;
    
}

/*  
 *  Called when we read upgrade cmd.
 *  gets stat and target and pushes to queue
 *  returns 1 if cmd was probably successfully pushed to queue
 *  returns 0 if definately not succesful or if target or stat call failed
 */
int parseUpgrade(char ** commandArray, int numberOfChunks)
{
    cmdOption statToUpgrade = getCommandOption(commandArray[1]);
    
    int target = getTargetTower(commandArray[2]);
    
    if(target!=0 && statToUpgrade<=5 && statToUpgrade!=optionError )
    {
        cmdType action = cmd_upgrade;
        if(pushToQueue(getQueue(NULL),action,statToUpgrade,target)>=1)
            //push to queue returns number of items on queue
            return 1;
    }
    optionUsageError();
    return 0;
}

/* 
 *  Called on cat and upgrade commands with the target specifying token.
    looks at the 2nd char in the string to find an int 1-9 to be the target.
    Note, wont work for anything > 9, would just see 1.
    Will print its own error message.
    Returns TargetTowerID if sucessful
    Returns 0 if error
 */
unsigned int getTargetTower(const char * inputStringTargeting)
{
    unsigned int numberOfTowers = getNumberOfTowers();//getNumberOfTowers(); this is func in tower.c

    size_t len = strlen(inputStringTargeting);//gets the size of string
    if( len<(2*sizeof(char)) )
    {
        fprintf(stderr,"*** SYNTAX ERROR: You must target a tower with this command ***\n");
        fprintf(stderr,"to target a tower enter t followed by a number 1 - %d \n",numberOfTowers);
        return 0;
    }
    if (inputStringTargeting[0]!='t' && inputStringTargeting[0]!='T')
    {
        fprintf(stderr,"*** ERROR: You must target a tower with this command ***\n");
        fprintf(stderr,"to target a tower enter t followed by a number 1 - %d \n",numberOfTowers);
        return 0;
    }
    
    unsigned int targetTower = (unsigned int)(inputStringTargeting[1]-'0');
    
    if(targetTower > numberOfTowers)
    {
        fprintf(stderr,"*** ERROR: target tower does not exist ***\n");
        fprintf(stderr,"You have only %d towers you entered t%d\n",
                numberOfTowers,targetTower);
        return 0;
    }
    return targetTower;
}

/*  Called when after we read a command, tests the next token against the
 *  possible options returns the corresponding cmdOption Or
    returns optionError  and calls the optUsageError function
 */
cmdOption getCommandOption(char * secondToken)
{
    for(int i = 0; secondToken[i]; i++) {
        secondToken[i] = tolower(secondToken[i]);
    }
    if(secondToken[0]=='-')//eat leading minus
    {
        secondToken=secondToken+1;
    }
    /*first lets get the array of strings that hold all the possible action commands*/
    stringList * optionList = getOptionList(NULL);
    int numberOfOptions=optionList->numberOfStrings;

    //now test the input string against all valid stats
    cmdOption option = optionError;
    for(int i=1; i<=numberOfOptions; ++i)
    {
        if(strcmp(secondToken,optionList->stringArray[i])==0)//if the string is identical to one of the commands
        {                                        //then action is set to that command
            switch (i)
            {
                case 1:
                    option = upgrade_power;
                    break;
                case 2:
                    option = upgrade_range;
                    break;
                case 3:
                    option = upgrade_speed;
                    break;
                case 4:
                    option = upgrade_AOErange;
                    break;
                case 5:
                    option = upgrade_AOEpower;
                    break;
                case 6:
                    option = upgrade_level;
                    break;
                case 7:
                    option = mktwr_int;
                    break;
                case 8:
                    option = mktwr_char;
                    break;
                case 9:
                    option = ps_x;
                    break;
                case 10:
                    option = kill_minus9;
                    break;
                case 11:
                    option = kill_all;
                    break;
                case 12:
                    option = aptget_kill;
                    break;
            }
            break;
        }
    }
    
    if(option==optionError)//if it is still set to ERROR then the user made a mistake
    {
        optionUsageError();
    }
    return option;
}




/* 
 *  If there was a syntax error in the users command call this function which
    will print usage advice to the terminal window
 */
void optionUsageError()
{
  
    fprintf(stderr,"*** Syntax error: Could not execute command.***\n");
    fprintf(stderr,"\nType man [COMMAND] for usage\n");//we advise them on usage
    //error messages will need to be passed back to the terminal to be printed. hopefully can do this by setting up a custom stream. For now will print to stderr.
}






/* 
 *  Takes the first string of the input command and tests it against the correct
    syntax to find which command they want to execute then returns that command 
    as a enum cmdType variable. Returns cmdType correspodning to the
    validated command or a commandError cmdType
 */
cmdType getCommandType(char * firstToken )
{
    for(int i = 0; firstToken[i]; i++) {
        firstToken[i] = tolower(firstToken[i]);
    }
    stringList * commandList = getCommandList(NULL);
    //now test the input string against all valid actions
    cmdType command = cmd_commandError;
    for(int i=1; i<=commandList->numberOfStrings; ++i)
    {
        if(strcmp(firstToken,commandList->stringArray[i])==0)//if the string is identical to one of the commands
        {                                        //then action is set to that command
            switch (i)
            {
                case 1:
                    command = cmd_upgrade;
                    break;
                case 2:
                    command = cmd_execute;
                    break;
                case 3:
                    command = cmd_set;
                    break;
                case 4:
                    command = cmd_man;
                    break;
                case 5:
                    command = cmd_cat;
                    break;
                case 6:
                    command = cmd_mktwr;
                    break;
                case 7:
                    command = cmd_aptget;
                    break;
                case 8:
                    command = cmd_ps;
                    break;
                case 9:
                    command = cmd_kill;
                    break;
            }
            break;
        }
    }
    
    if(command==cmd_commandError)//if it is still set to ERROR then the user made a mistake
    {
        actionUsageError(firstToken);
    }
    return command;
}



/*
 *   If there was a syntax error in the users command call this function which
     will print usage advice to the terminal window.
 */
void actionUsageError(const char * firstToken)
{
    stringList * commandList = getCommandList(NULL);
    int numberOfCommands=commandList->numberOfStrings;
    fprintf(stderr,"*** ""%s"" command not recognised ***\n",firstToken);
    fprintf(stderr,"installed commands: \n");
    for(int i=1; i<=numberOfCommands; ++i)
    {
        fprintf(stderr,"%s\n",commandList->stringArray[i]);
    }
    fprintf(stderr,"\nType man [COMMAND] for usage\n");//we advise them on usage
    //error messages will need to be passed back to the terminal to be printed.
    //hopefully can do this by setting up a custom stream. For now will print to stderr.

}



/*
 *  Takes the input string and breaks into separate words (where there is a 
    space and new string starts) each of these words is stored in the 
    commandArray which is an array of strings
 */
char **breakUpString(const char * inputString, int *numberOfChunksPtr, const char * delimiter)
{
    char    *stringChunk,                       //holds the chunks on the input string as we break it up
            *inputStringDuplicate = strdup(inputString),//duplicate input string for editting
            **commandArray = NULL;              //this will be an array to hold each of the chunk strings
    int     numberOfChunks=0;
    
    //using http://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
    stringChunk = strtok(inputStringDuplicate, delimiter); // gets the first chunk (up to the first space)
    
    // walk through rest of string
    while( stringChunk != NULL )
    {
        ++numberOfChunks;
        commandArray=(char **)realloc(commandArray,numberOfChunks*sizeof(char*));//array of strings
        commandArray[numberOfChunks-1]=(char *)malloc((size_t)(strlen(stringChunk)*sizeof(char)+1));
        strcpy(commandArray[numberOfChunks-1],stringChunk);
        
        stringChunk = strtok(NULL, delimiter);
    }
    free(inputStringDuplicate);//frees the malloc made in strdup()
                               //$(numberOfChunks) strings now stored in the commandArray
    *numberOfChunksPtr=numberOfChunks;
    return commandArray;
}

/*
 *  Duplicates a string
 */
char *strdup(const char * s)
{
    size_t len = 1+strlen(s);//gets the size of s
    char *p = malloc(len);//allocates a block big enough to hold s
    
    return p ? memcpy(p, s, len) : NULL;//if p is non 0 ie malloc worked, then copy everything in s into p and return p. if p is NULL malloc didnt work so return NULL
}

/*
 *  frees the memory allocated in breakup string funct
 */
void freeCommandArray(char **commandArray,int numberOfChunks)
{
    for(int i=0; i<numberOfChunks; ++i)
    {
        free(commandArray[i]);
    }
    free(commandArray);
}

/*
 *  Test function. Prints contents of commandArray
 */
void testCommandArray(char ** commandArray, int numberOfChunks)
{
    for(int i=1; i<=numberOfChunks; ++i)
    {
        printf("%s",commandArray[i]);
        printf("|\n");
    }
}

void freeParseLists()
{
    stringList * commandList = getCommandList(NULL);
    for(int i=1; i<=commandList->numberOfStrings; ++i) {
        free(commandList->stringArray[i]);
    }
    free(commandList->stringArray+1);
    free(commandList);
    
    stringList * optsList = getOptionList(NULL);
    for(int i=1; i<=optsList->numberOfStrings; ++i) {
        free(optsList->stringArray[i]);
    }
    free(optsList->stringArray+1);
    free(optsList);
}
void initialiseParseLists()
{
    stringList * commandList = intialiseCommandList();
    getCommandList(commandList);
    stringList * optionList = intialiseOptionList();
    getOptionList(optionList);
}
stringList * getCommandList(stringList * commandList)
{
    static stringList * storedCommandList = NULL;
    if(commandList != NULL && storedCommandList == NULL ) {
        storedCommandList = commandList;
    }
    return storedCommandList;
}
stringList *  getOptionList(stringList * optionList)
{
    static stringList * storedOptionList = NULL;
    if(optionList != NULL && storedOptionList == NULL ) {
        storedOptionList = optionList;
    }
    return storedOptionList;
}


stringList * intialiseCommandList()
{
    /* make an array of strings to hold all the possible action commands*/
    static char **validActions;
    int numberOfActions=9;//have 5 action commands at this time: upgrade, execute, set, man, cat
    validActions=malloc((numberOfActions)*sizeof(char*));//array of $[numberOfActions] strings
    validActions-=1;
    validActions[1]=strdup("upgrade");
    validActions[2]=strdup("execute");
    validActions[3]=strdup("set");
    validActions[4]=strdup("man");
    validActions[5]=strdup("cat");
    validActions[6]=strdup("mktwr");
    validActions[7]=strdup("ps");
    validActions[8]=strdup("apt-get");
    validActions[9]=strdup("kill");


    stringList * commandList = malloc(sizeof(stringList));
    commandList->stringArray=validActions;
    commandList->numberOfStrings=numberOfActions;
    
    return commandList;
}

stringList * intialiseOptionList()
{
    /*first lets make an array of strings to hold all the possible action commands*/
    char **validOptions;
    int numberOfOptions=12;//have 5 action commands at this time: upgrade, execute, set, man, cat
    validOptions=malloc((numberOfOptions)*sizeof(char*));    //upgrade opts
    validOptions-=1;
    validOptions[1]=strdup("p");
    validOptions[2]=strdup("r");
    validOptions[3]=strdup("s");
    validOptions[4]=strdup("aoer");
    validOptions[5]=strdup("aoep");
    validOptions[6]=strdup("lvl");
    //mktwr opts:
    validOptions[7]=strdup("int");
    validOptions[8]=strdup("char");
    //ps opts:
    validOptions[9]=strdup("x");
    //kill opts:
    validOptions[10]=strdup("9");
    validOptions[11]=strdup("all");
    //aptget opts:
    validOptions[12]=strdup("kill");
    
    stringList * optionsList = malloc(sizeof(stringList));
    optionsList->stringArray=validOptions;
    optionsList->numberOfStrings=numberOfOptions;
    
    return optionsList;
}
