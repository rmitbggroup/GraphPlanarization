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

#include "node.h"
#include <stdlib.h>

/******************************************************************************
** Function:	makeNode
** Description: Function creates a node and places item into it
** Inputs:	item 
** Return Value:the address of the new node, or NULL if unable to make node
** Side Effects:item is stored in the node, prev and next pointers set to NULL
** Created:	Kerri Morgan	2 March, 2005
** Modified:
** Comments:	
******************************************************************************/
Node* makeNode(int item)
{
	Node* newNode=NULL;

	newNode = (Node*) malloc(sizeof(Node));
	if(newNode !=NULL)
	{
		newNode->vertex = item;
		newNode->prev =NULL;
		newNode->next=NULL;
	}

	return newNode;
}
