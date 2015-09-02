#ifndef GNG_LIST_H
#define GNG_LIST_H

#include <stdlib.h>
#include <string.h>
#include "util.h"

/****s* List/list
 * NAME
 *   list -- list structure
 * FUNCTION
 *   Defines the basic structure of a list.
 *   The same layout is copied in all other structures (Planets, shiptype, 
 *   battle etc) that we like to store in lists.
 *   Since all these structures start with the same three fields we
 *   can use a number of standard functions to manipulate the lists.
 *
 *   Base pointer    First element        Second element
 *   +--+        +------------+         +------------+         
 *   | *-------->| next   | *---------->| next   | *---------->  
 *   +--+        | cookie |   |         | cookie |   |
 *               | name   |   |         | name   |   |
 *               +------------+         +------------+
 *               | other data |         | other data |
 *   
 *   The cookie is used to do sanity checks. Each kind of list
 *   has its own unique cookie. This way we can check if the
 *   base pointer really points to a list of planets and not
 *   to a list of shiptypes for instance.
 * SOURCE   
 */

typedef struct list {
  struct list    *next;
  long            cookie;
  char           *name;
} list;

/**********/


/****f* List/add2List
 * NAME
 *   add2List -- macro, add an element to the beginning of a list
 * FUNCTION
 *   Add an element to the beginning of a list.
 * INPUTS
 *   l -- base address of a list.
 *   e -- address of the element to be added.
 * EXAMPLE
 *   add2List(&Player->shiptypes, newShipType);
 * SEE ALSO
 *   add2ListF(), addList()
 * SOURCE
 */

#define   add2List(l,e)  add2ListF((list **)l,(list *)e)

/**********/

/****f* List/addList
 * NAME
 *   addList -- macro, add an element to the end of a list
 * FUNCTION
 *   Add an element to a list.
 *   Uses evil casts to make it work on different 
 *   type of lists and elements, so one function can
 *   be used to add a player to a list of players
 *   and a planet to a list of planets.
 * INPUTS
 *   l -- pointer to the base of a list.
 *   e -- address of the element to be added.
 * EXAMPLE
 *   addList(&Player->shiptypes, newShipType);
 * SEE ALSO
 *   addListF(), add2List()
 * SOURCE
 */

#define   addList(l,e)    addListF((list **)l,(list *)e)

/**********/

/****f* List/insertList
 * NAME
 *   insertList -- macro, add an element to a list after the given element
 * FUNCTION
 *   Add an element to a list after a specified member, the end if the
 *   member isn't found.
 *   Uses evil casts to make it work on different 
 *   type of lists and elements, so one function can
 *   be used to add a player to a list of players
 *   and a planet to a list of planets.
 * INPUTS
 *   l -- pointer to the base of a list.
 *   a -- address of the anchor element
 *   e -- address of the element to be added.
 * EXAMPLE
 *   addList(&Player->orders, orderBeingProcessed, mistake);
 * SEE ALSO
 *   insertListF(), addList(), add2List()
 * SOURCE
 */

#define   insertList(l,a, e)    insertListF((list **)l, (list*)a, (list *)e)

/*******/


/****f* List/remList
 * NAME
 *   remList -- macro, remove an element from a list
 * SYNOPSIS
 *   remList(l, e)
 * FUNCTION
 * INPUTS
 *   l -- pointer to the base of the list.
 *   e -- address of the element to be removed.
 * SEE ALSO
 *   removeListF(), addList()
 * SOURCE
 */

#define   remList(l,e) removeListF((list **)l,(list *)e)

/********/

/****f* List/findElement
 * NAME
 *   findElement -- macro, find an element in a list
 * SYNOPSIS
 *   findElement(t,l,n)
 * INPUTS
 *   t -- type of the element
 *   l -- base of the list
 *   n -- name of the element (pointer to a string)
 * SOURCE
 */

#define   findElement(t,l,n) (t *)findElementF((list *)l,n)
#define   copyElement(t,l,n) (t *)copyElementF((list *)l,n)

/********/

/****i* List/numberOfElements
 * NAME
 *   numberOfElements
 * SOURCE
 */

#define   numberOfElements(l) numberOfElementsF((list *)l)

/********/

/****f* List/setName
 * NAME
 *   setName -- macro, set the name of an element
 * SEE ALSO
 *   setNameF()
 * SOURCE
 */

#define   setName(e, n) setNameF((list *)e, n)

/*******/


void            add2ListF(list **aList, list *anElement);
void            freelist(void *base);
void            addListF(list **aList, list *anElement);
void            insertListF(list **aList, list* anchor, list *anElement);
list           *findElementF(list *aList, char *name);
void            removeListF(list **aList, list *anElement);
void            setNameF(list *anElement, const char *name);
int             numberOfElementsF(list *aList);
int             ptonum(void *base, void *target);
void           *numtop(void *base, int n);
void            dumpList(char* label, list* aList, void* callback(void*));

#endif				/* GNG_LIST_H */
