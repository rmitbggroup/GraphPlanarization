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


#include "linkList2.h"
#include "node2.h"
#include <stdio.h>
#include <stdlib.h>

/*****************************************************************************
 * Acknowledgements:  The code for the following functions is based on the code
 * presented in cse1303 lecture notes, Monash University.  There may be some minor changes to the 
 * functions 
 * ***************************************************************************/

/*****************************************************************************
 * This list takes note of multiple copies of same item stored ***************
 * **************************************************************************/


/******************************************************************************
** Function:	initList2
** Description: Function initialises the list at listPtr to an emptyList
** Inputs:	listPtr
** Return Value:none
** Side Effects:The list is initialised
** Created:	Kerri Morgan	13th June, 2005
** Modified:
** Comments:	
** Acknowledgements:see acknowledgement at top of this file
******************************************************************************/
void initList2(List2* listPtr)
{
	listPtr->count =0;
	listPtr->startPtr=NULL;
	listPtr->endPtr=NULL;
}

/******************************************************************************
** Function:	emptyList2
** Description: Function checks if the list is empty
** Inputs:
** Return Value:
** Side Effects:
** Created:	Kerri Morgan	13th June, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int emptyList2(List2* listPtr)
{
	if(listPtr->count <= 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/******************************************************************************
** Function:	insertInList2
** Description:	Function inserts a vertex number into list
** Inputs:	listPtr - points to the list
**		item - item to insert into list
** Return Value:none
** Side Effects:list has contents updated
** Created:	Kerri Morgan	13th June, 2005
** Modified:	
** Acknowledgements: see acknowledgement above.
******************************************************************************/
void insertInList2(List2* listPtr, int item)
{
	C_Node* tmpPtr=NULL;
	C_Node* curPtr=NULL;
	C_Node* prevPtr=NULL;
	/* empty list */
	if(listPtr->startPtr==NULL)
	{
		listPtr->startPtr = makeNode2(item);
		if(listPtr->startPtr==NULL)
		{
			fprintf(stderr, "ERROR 13.1: Unable to create node to insert vertex %d into list in function insertInList\n", item);
			exit(1);
		}
		listPtr->endPtr=listPtr->startPtr;
		listPtr->count++;
	}
	else
	{
		curPtr=listPtr->startPtr;
		/* find place in list, but don't go past end of list */
		while(curPtr->component < item) 
		{
			prevPtr=curPtr;
			curPtr=curPtr->next;
			if(curPtr==NULL)
			{
				break;
			}
		}
		/* insert at end of list */
		if( curPtr == NULL) 
		{
			tmpPtr=listPtr->endPtr;
			listPtr->endPtr = makeNode2(item);
			if(listPtr->endPtr==NULL)
			{
				fprintf(stderr, "ERROR 13.2: Unable to create node to insert vertex %d into list in function insertInList\n", item);
				exit(1);
			}
			listPtr->endPtr->prev=tmpPtr;
			tmpPtr->next=listPtr->endPtr;
			listPtr->count++;
		}
		else if(curPtr->component == item) /* already in list */
		{
			curPtr->numAdjacent = curPtr->numAdjacent + 1;
		}							
		else /* insert between prevPtr and curPtr */
		{
			tmpPtr=makeNode2(item);
			if(tmpPtr ==NULL)
			{
				fprintf(stderr, "ERROR 13.3: Unable to create node to insert vertex %d into list in function insertInList\n", item);
				exit(1);
			}
			tmpPtr->prev=prevPtr;
			if(prevPtr !=NULL)
			{
				prevPtr->next=tmpPtr;
			}
			else
			{
				listPtr->startPtr=tmpPtr;
			}
			curPtr->prev=tmpPtr;
			tmpPtr->next = curPtr;		
			listPtr->count++;
		}
	}
}
	
/******************************************************************************
** Function:	removeFromList2
** Description:	Function removes a item from the list
** Inputs:	listPtr - address of list
**		item - item to be removed
** Return Value: none
** Side Effects: updates list
** Created:	Kerri Morgan	13th June, 2005	
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void removeFromList2(List2* listPtr, int item)
{
	C_Node* cur=NULL;
	C_Node* prev =NULL;
	
	cur=listPtr->startPtr;
	while(cur!=NULL)
	{
		if(cur->component==item)
		{
			if(cur->numAdjacent == 1)
			{
				if(prev ==NULL)
				{
					listPtr->startPtr=cur->next;
				}
				else
				{
					prev->next=cur->next;
				}
				if(cur->next !=NULL)
				{
					cur->next->prev=prev;
				}
				else  /* last in list*/
				{
					listPtr->endPtr=prev;	
	
				}
				free(cur); /***********HERE***********/
				listPtr->count--;
				if(listPtr->count == 0)
				{
					listPtr->endPtr=NULL;
					listPtr->startPtr=NULL;
				}
				break;
			}
			else
			{
				cur->numAdjacent = cur->numAdjacent - 1;
				break;
			}
		}
		prev=cur;
		cur = cur->next;
	}

}
/******************************************************************************
** Function:	printList2
** Description: Function prints the contents of a list
** Inputs:	listPtr - points to the list
** Return Value:none
** Side Effects: prints list to screen
** Created:	Kerri Morgan	13th June, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void printList2(List2* listPtr)
{
	C_Node* cur=NULL;

	cur=listPtr->startPtr;
	while(cur!=NULL)
	{
		printf("(%d, ", cur->component);
		printf("%d )", cur->numAdjacent);
		cur=cur->next;

	}
	printf("\n\n");
}

/******************************************************************************
** Function:	killList2
** Description: Function frees memory and re-inits list
** Inputs:	listPtr - address of list
** Return Value:none
** Side Effects:list ceases exist
** Created:	Kerri Morgan 	13th June, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void killList2(List2* listPtr)
{
	C_Node* cur=NULL; 
	C_Node* old=NULL;

	cur=listPtr->startPtr;

	while(cur!=NULL)
	{
		old=cur;
		cur=cur->next;
		free(old);
	}
	initList2(listPtr);
}

/******************************************************************************
** Function:	inList2
** Description: Function checks if an item is in the list
** Inputs:	listPtr - address of list
** Return Value:TRUE if found else FALSE
** Side Effects:none
** Created:	Kerri Morgan 	6 March, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int inList2(List2* listPtr, int item)
{
	C_Node* cur=NULL;

	if(listPtr->count== 0)
	{
		return FALSE;
	}
	cur=listPtr->startPtr;
	while(cur!=NULL)
	{
		if(cur->component == item)
		{
			return TRUE;
		}
		cur=cur->next;
	}
	return FALSE;
}


/******************************************************************************
** Function:	insertAtEnd
** Description:	Function inserts a vertex number at end of list
** Inputs:	listPtr - points to the list
**		item - item to insert into list
** Return Value:none
** Side Effects:list has contents updated
** Created:	Kerri Morgan	11 March, 2005
** Modified:	
** Comments:	Currently inserts at the end
** Acknowledgements: see achknowledgement above.
******************************************************************************/
void insertAtEnd2(List2* listPtr, int item)
{
	C_Node* curPtr=NULL;

	/* empty list */
	if(listPtr->startPtr==NULL)
	{
		listPtr->startPtr = makeNode2(item);
		if(listPtr->startPtr==NULL)
		{
			fprintf(stderr, "ERROR 13.4: Unable to create node to insert vertex %d into list in function insertInList\n", item);
			exit(1);
		}
		listPtr->endPtr=listPtr->startPtr;
	}
	else
	{
		curPtr=listPtr->endPtr;
		listPtr->endPtr = makeNode2(item);
		if(listPtr->endPtr==NULL)
		{
			fprintf(stderr, "ERROR 13.5: Unable to create node to insert vertex %d into list in function insertInList\n", item);
			exit(1);
		}
		listPtr->endPtr->prev=curPtr;
		curPtr->next=listPtr->endPtr;
	}
}
