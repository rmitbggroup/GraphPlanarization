#ifndef	MENU_H
#define MENU_H

/************** Constants *******************************/

/*** Note: INVALID is always the last item in this 'list' ****/
enum {QUIT, READ_GRAPH, H_LAU, ADD_VERT_PLANARISE,REMOVE_VERT_PLANARISE, SUBSET_REMOVE,INVALID};

/************** Function Prototypes *********************/
void displayMenu(void);
int getCommand(void);

#endif

