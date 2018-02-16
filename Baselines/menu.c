/**
 * Copyright (C) 2005, 2009  Kerri Morgan
 *
 * This file is part of the planar program package.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * **/


#include <stdio.h>
#include "menu.h"

#define MAX 80	/* maximum read in an input line from stdin */

/******************************************************************************
** Function:	displayMenu
** Description: Displays menu options
** Inputs:	none
** Return Value:none
** Side Effects:
** Created:	Kerri Morgan	3rd March, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void displayMenu(void)
{
	printf("Select one of the following options:\n");
	printf("%d: Quit\n", QUIT);
	printf("%d: Read graph from user-specified file\n", READ_GRAPH);
	printf("%d: Planarise - HL Algorithm\n", H_LAU);

	printf("%d: Planarise - VA Algorithm\n", ADD_VERT_PLANARISE);
	printf("%d: Planarise - VR Algorithm\n", REMOVE_VERT_PLANARISE);
	printf("%d: Planarise - VSR Algorithm\n", SUBSET_REMOVE);


}

/******************************************************************************
** Function:	getCommand
** Description: Function gets a choice entered by user responding to menu 
**		displayed by displayMenu function.
** Inputs:	none
** Return Value:action - integer indicating choice made by user
** Side Effects: If choice is invalid, function prints error message to screen
** Created:	Kerri Morgan	3rd March, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int getCommand(void)
{
	char temp[MAX];
	int command;	
	
	fgets(temp, MAX, stdin);
	if(sscanf(temp, "%d", &command) !=1)
	{
		printf("WARNING 1.1: Invalid command: %s\n", temp);
		command = INVALID;
	}
	else if(command < QUIT || command > INVALID)
	{
		printf("WARNING 1.1: Invalid command: %s\n", temp);
		command = INVALID;
	}
	return command;

}
