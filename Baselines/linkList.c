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


#include "linkList.h"
#include "node.h"
#include <stdio.h>
#include <stdlib.h>

/*****************************************************************************
 * Acknowledgements:  The code for the following functions is based on the code
 * presented in cse1303 lecture notes (Monash university).  There may be some minor changes to the 
 * functions 
 * ***************************************************************************/


/******************************************************************************
** Function:	initList
** Description: Function initialises the list at listPtr to an emptyList
** Inputs:	listPtr
** Return Value:none
** Side Effects:The list is initialised
** Created:	Kerri Morgan	2 March, 2005
** Modified:
** Comments:	
** Acknowledgements:see acknowledgement at top of this file
******************************************************************************/
void initList(List* listPtr)
{
	listPtr->degree =0;
	listPtr->startPtr=NULL;
	listPtr->endPtr=NULL;
}

/******************************************************************************
** Function:	emptyList
** Description: Function checks if the list is empty
** Inputs:
** Return Value:
** Side Effects:
** Created:	Kerri Morgan	2 March, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int emptyList(List* listPtr)
{
	if(listPtr->degree <= 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/******************************************************************************
** Function:	insertInList
** Description:	Function inserts a vertex number into list
** Inputs:	listPtr - points to the list
**		item - item to insert into list
** Return Value:none
** Side Effects:list has contents updated
** Created:	Kerri Morgan	2 March, 2005
** Modified:	3rd March, 2005 to insert in numeric order
**		12 May, 2005 to not insert duplicates
** Comments:	Currently inserts at the end
** Acknowledgements: see achknowledgement above.
******************************************************************************/
void insertInList(List* listPtr, int item)
{
	Node* tmpPtr=NULL;
	Node* curPtr=NULL;
	Node* prevPtr=NULL;

	if(inList(listPtr, item) == FALSE)
	{
		/* empty list */
		if(listPtr->startPtr==NULL)
		{
			listPtr->startPtr = makeNode(item);
			if(listPtr->startPtr==NULL)
			{
				fprintf(stderr, "ERROR 12.1: Unable to create node to insert vertex %d into list in function insertInList\n", item);
				exit(1);
			}
			listPtr->endPtr=listPtr->startPtr;
		}
		else
		{
			curPtr=listPtr->startPtr;
			/* find place in list, but don't go past end of list */
			while(curPtr->vertex < item) 
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
				listPtr->endPtr = makeNode(item);
				if(listPtr->endPtr==NULL)
				{
					fprintf(stderr, "ERROR 12.2 Unable to create node to insert vertex %d into list in function insertInList\n", item);
					exit(1);
				}
				listPtr->endPtr->prev=tmpPtr;
				tmpPtr->next=listPtr->endPtr;
			}
			else /* insert between prevPtr and curPtr */
			{
				tmpPtr=makeNode(item);
				if(tmpPtr ==NULL)
				{
					fprintf(stderr, "ERROR 12.3: Unable to create node to insert vertex %d into list in function insertInList\n", item);
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
			}
		}
		listPtr->degree++;
	}
}
	
/******************************************************************************
** Function:	removeFromList
** Description:	Function removes a vertex from the list
** Inputs:	lsitPtr - address of list
**		vertex - item to be removed
** Return Value: none
** Side Effects: updates list
** Created:	Kerri Morgan	2 March, 2005	
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void removeFromList(List* listPtr, int vertex)
{
	Node* cur=NULL;
	Node* prev =NULL;
	
	cur=listPtr->startPtr;
	while(cur!=NULL)
	{
		if(cur->vertex==vertex)
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
			free(cur); /*************HERE***************/
			listPtr->degree--;
			if(listPtr->degree == 0)
			{
				listPtr->startPtr=NULL;
				listPtr->endPtr=NULL;
			}	
			break;
		}
		prev=cur;
		cur = cur->next;
		
	}

}
/******************************************************************************
** Function:	printList
** Description: Function prints the contents of a list
** Inputs:	listPtr - points to the list
** Return Value:none
** Side Effects: prints list to screen
** Created:	Kerri Morgan	2 March, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void printList(List* listPtr)
{
	Node* cur=NULL;

	if(listPtr != NULL)
	{
		cur=listPtr->startPtr;
		while(cur!=NULL)
		{
			printf("%d ", cur->vertex);
			cur=cur->next;
		}
		printf("\n\n");
	}
	else
	{
		printf("NO LIST TO PRINT\n");
	}
}

/******************************************************************************
** Function:	killList
** Description: Function frees memory and re-inits list
** Inputs:	listPtr - address of list
** Return Value:none
** Side Effects:list ceases exist
** Created:	Kerri Morgan 	2 March, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void killList(List* listPtr)
{
	Node* cur=NULL; 
	Node*old=NULL;
	if(listPtr->degree > 0)
	{
		cur=listPtr->startPtr;

		while(cur!=NULL)
		{
			old=cur;
			cur=cur->next;
			free(old);
		}
	}
	initList(listPtr);
}

/******************************************************************************
** Function:	inList
** Description: Function checks if an item is in the list
** Inputs:	listPtr - address of list
** Return Value:TRUE if found else FALSE
** Side Effects:none
** Created:	Kerri Morgan 	6 March, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
int inList(List* listPtr, int item)
{
	Node* cur=NULL;

	if(listPtr->degree == 0)
	{
		return FALSE;
	}
	cur=listPtr->startPtr;
	while(cur!=NULL)
	{
		if(cur->vertex == item)
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
void insertAtEnd(List* listPtr, int item)
{
	Node* curPtr=NULL;

	/* empty list */
	if(listPtr->startPtr==NULL)
	{
		listPtr->startPtr = makeNode(item);
		if(listPtr->startPtr==NULL)
		{
			fprintf(stderr, "ERROR 12.4: Unable to create node to insert vertex %d into list in function insertInList\n", item);
			exit(1);
		}
		listPtr->endPtr=listPtr->startPtr;
	}
	else
	{
		curPtr=listPtr->endPtr;
		listPtr->endPtr = makeNode(item);
		if(listPtr->endPtr==NULL)
		{
			fprintf(stderr, "ERROR 12.5: Unable to create node to insert vertex %d into list in function insertInList\n", item);
			exit(1);
		}
		listPtr->endPtr->prev=curPtr;
		curPtr->next=listPtr->endPtr;
	}
	listPtr->degree++;
}

/******************************************************************************
** Function:	addList
** Description:	Function takes two lists L1 and L2.  Any items in L2 not in L1
**		are added to L1.  L1 = L1 U L2
** Inputs:	L1, L2 - lists
** Return Value:none
** Side Effects:L1 has addtional elemnts added to it.  L2 remains unchanged.
** Created:	Kerri Morgan	6th July, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void addList(List* L1, List* L2)
{
	Node* cur=NULL;

	if(L2->degree !=0)
	{
		cur = L2->startPtr;
		while(cur != NULL)
		{
			insertInList(L1, cur->vertex);
			cur = cur->next;
		}
	}
}

/******************************************************************************
** Function:	append
** Description:	Function appends L2 to end of L1
** Inputs:	L1, L2 - lists
** Return Value:none
** Side Effects:L1  contains two lists appended.  L2 is empty list
** Created:	Kerri Morgan	12th July, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void append(List* L1, List* L2)
{
	Node* end=NULL;
	Node* start=NULL;

	if(L2->degree !=0)
	{
		start = L2->startPtr;
		end = L1->endPtr;

		end->next =start;
		start->prev = end;

		L1->endPtr = L2->endPtr;
		L1->degree = L1->degree + L2->degree;
		L2->degree =0;
		L2->startPtr = NULL;
		L2->endPtr=NULL;
	}
}

/******************************************************************************
** Function:	revPrePend
** Description:	Function prepends a reversed L2 to start of L1
** Inputs:	L1, L2 - lists
** Return Value:none
** Side Effects:L1  contains two lists.  L2 is empty list
** Created:	Kerri Morgan	12th July, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void revPrePend(List* L1, List* L2)
{
	Node* cur=NULL;

	if(L2->degree !=0)
	{
		cur = L2->startPtr;

		while(cur != NULL)
		{
			L2->startPtr = cur->next;
			if(L2->startPtr != NULL)
			{
				cur->next->prev=NULL;
			}
			cur->next = L1->startPtr;
			L1->startPtr->prev = cur;
			L1->startPtr = cur;
			cur->prev=NULL;
			L1->degree++;
			L2->degree--;

			cur = L2->startPtr;
		}
	}
	L2->degree =0;
	L2->startPtr=NULL;
	L2->endPtr=NULL;

}

/******************************************************************************
** Function:	prepend
** Description:	Function prepends L2 to start of L1
** Inputs:	L1, L2 - lists
** Return Value:none
** Side Effects:L1  contains two lists.  L2 is empty list
** Created:	Kerri Morgan	12th July, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void prepend(List* L1, List* L2)
{
	Node* end=NULL;
	Node* start=NULL;

	if(L2->degree !=0)
	{
		start = L1->startPtr;
		end = L2->endPtr;
		end->next = start;
		start->prev = end;
		L1->startPtr = L2->startPtr;
		L1->degree = L1->degree + L2->degree;
		L2->startPtr = NULL;
		L2->endPtr = NULL;
		L2->degree=0;
	}
}

/******************************************************************************
** Function:	revAppend
** Description:	Function appends a reversed L2 to end of L1
** Inputs:	L1, L2 - lists
** Return Value:none
** Side Effects:L1  contains two lists.  L2 is empty list
** Created:	Kerri Morgan	12th July, 2005
** Modified:
** Comments:	
** Acknowledgements:
******************************************************************************/
void revAppend(List* L1, List* L2)
{
	Node* cur=NULL;

	if(L2->degree !=0)
	{
		cur = L2->endPtr;

		while(cur != NULL)
		{
			L2->endPtr = cur->prev;
			if(L2->endPtr != NULL)
			{
				cur->prev->next=NULL;
			}
			L1->endPtr->next = cur;
			cur->prev = L1->endPtr;
			L1->endPtr=cur;

			L1->degree++;
			L2->degree--;

			cur = L2->endPtr;
		}
	}
	L2->degree =0;
	L2->startPtr=NULL;
	L2->endPtr=NULL;

}

/******************************************************************************
** Function:	insertAtStart
** Description:	Function inserts a vertex number at start of list
** Inputs:	listPtr - points to the list
**		item - item to insert into list
** Return Value:none
** Side Effects:list has contents updated
** Created:	Kerri Morgan	12 July, 2005
** Modified:	
** Comments:	Currently inserts at the start
** Acknowledgements: see acknowledgement above.
******************************************************************************/
void insertAtStart(List* listPtr, int item)
{
	Node* curPtr=NULL;

	/* empty list */
	if(listPtr->startPtr==NULL)
	{
		listPtr->startPtr = makeNode(item);
		if(listPtr->startPtr==NULL)
		{
			fprintf(stderr, "ERROR 12.6: Unable to create node to insert vertex %d into list in function insertAtStart\n", item);
			exit(1);
		}
		listPtr->endPtr=listPtr->startPtr;
	}
	else
	{
		curPtr=listPtr->startPtr;
		listPtr->startPtr = makeNode(item);
		if(listPtr->startPtr==NULL)
		{
			fprintf(stderr, "ERROR 12.7: Unable to create node to insert vertex %d into list in function insertAtStart\n", item);
			exit(1);
		}
		listPtr->startPtr->prev=NULL;
		listPtr->startPtr->next = curPtr;
		curPtr->prev = listPtr->startPtr;
	}
	listPtr->degree++;
}


/******************************************************************************
** Function:	reverseList
** Description:	Function reverses the contents of a list
** Inputs:	L  - points to the list
** Return Value:none
** Side Effects:list has contents updated
** Created:	Kerri Morgan	16th March, 2006
** Modified:	
** Comments:	
** Acknowledgements: 
******************************************************************************/
void reverseList(List* L)
{
	Node* curPtr=NULL;
	Node* tempPtr=NULL;

	/* not empty list */
	if(L->startPtr!=NULL)
	{
		curPtr = L->startPtr;
		L->startPtr = L->endPtr;
	        L->endPtr = curPtr;	
		while(curPtr != NULL)
		{
			tempPtr = curPtr->next;
			curPtr->next = curPtr->prev;
			curPtr->prev = tempPtr;
			curPtr = tempPtr;
		}
	}
}
