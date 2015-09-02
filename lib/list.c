#include "list.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/****h* GalaxyNG/List
 * NAME
 *  List -- a number of list manipulating functions.
 *****
 */

/****i* List/numberOfElementsF
 * NAME
 *   numberOfElementsF --
 *******
 */

int
numberOfElementsF(list *aList)
{
  list           *el;
  int             noEl;

  noEl = 0;
  if (aList)
    for (el = aList; el; el = el->next)
      noEl++;
  return noEl;
}

/****i* List/add2ListF
 * NAME
 *   add2ListF --
 ******
 */

void
add2ListF(list **aList, list *anElement)
{
  anElement->next = *aList;
  *aList = anElement;
}


/****f* List/freelist
 * NAME
 *   freelist -- free all members in a list
 *****
 */

void
freelist(void *base)
{
  list           *p, *p2;

  pdebug(DFULL2, "freelist");
  p = base;
  while (p) {
    p2 = p->next;
    free(p->name);
    free(p);
    p = p2;
    pdebug(DFULL2, ".");
  }
  pdebug(DFULL2, "\n");
}



/****i* List/addListF
 * NAME
 *   addListF --
 ******
 */
 
void
addListF(list **aList, list *anElement)
{
  list           *curElement;

  if (*aList == NULL) {
    *aList = anElement;
  } else {
    for (curElement = *aList;
	 curElement->next;
	 curElement = curElement->next);
    curElement->next = anElement;
  }

}

/****i* List/insertListF
 * NAME
 *   insertListF --
 ******
 */
 
void
insertListF(list **aList, list* where, list *anElement)
{
  list *curElement;
  list *listEnd;

  if (*aList == NULL) {
    *aList = anElement;
  } else {
    for (curElement = *aList; curElement->next;
	 curElement = curElement->next) {
      if (curElement == where) {
	break;
      }
    }

    for (listEnd = anElement; listEnd->next; listEnd = listEnd->next)
      ;

    listEnd->next = curElement->next;
    curElement->next = anElement;

  }

}

/****** List/findElementF
 * NAME
 *   findElementF --
 * SYNOPSIS
 *   list *findElementF(list *aList, char *name)
 * FUNCTION
 *   Find an element in a list based on the name
 *   of the element.
 * INPUTS
 *   aList -- pointer to the first element in the list.
 *   name  -- name of the element to find.
 * RESULT
 *   Address of the element or NULL in case the
 *   element was not found.
 ******
 */

list* findElementF(list *aList, char *name) {
	list           *anElement;

	for (anElement = aList;
		 anElement;
		 anElement = anElement->next) {
		if (!noCaseStrcmp(anElement->name, name))
			break;
	}
	return anElement;
}


/****f* List/removeListF
 * NAME
 *   removeListF -
 * FUNCTION
 *   Remove an element from a list.
 * NOTE
 *   Do not use this function directly, use the
 *   macro remList()
 * SEE ALSO
 *   remList()
 ******
 */

void
removeListF(list **aList, list *anElement)
{
  list           *prevElement;

  assert(anElement != NULL);

  if (*aList == anElement)
    *aList = anElement->next;
  else {
    for (prevElement = *aList;
	 prevElement->next != anElement;
	 prevElement = prevElement->next) {
      assert(prevElement != NULL);
    }
    prevElement->next = anElement->next;
  }
  free(anElement);
}


/****f* List/ptonum
 * NAME
 *  ptonum -- convert pointer to index number 
 * SYNOPSIS
 *   void *ptonum(void *base, void *target)
 * FUNCTION
 *   Gives the index number of an element in a list.
 * INPUTS
 *   base   - base address of the list
 *   target - the element 
 * RESULT
 *   the index number of the element.
 * SEE ALSO
 *   numtop()
 *******
 */

int
ptonum(void *base, void *target)
{
  list           *p;
  int             i;

  for (p = base, i = 1; p; p = p->next, i++)
    if (p == target)
      return i;
  return 0;
}


/****f* List/numtop
 * NAME
 *   numtop -- convert index number to pointer 
 * SYNOPSIS
 *   void *numtop(void *base, int n)
 * FUNCTION
 *   Look up an element in a list using it's index number.
 * INPUTS
 *   base - base address of the list
 *   n    - index number
 * RESULT
 *   address of the element.
 *******
 */

void *
numtop(void *base, int n)
{
  list           *p;
  int             i;

  i = 0;
  for (p = base; p; p = p->next) {
    i++;
    if (i == n)
      return p;
  }
  return 0;
}



/****f* List/setName
 * NAME
 *   setNameF
 * FUNCTION
 *   Set the name of an element in a list.
 * NOTE
 *   Do not use this function directly, use the
 *   macro setName()
 * SEE ALSO
 *   setName()
 ******
 */

void setNameF(list *anElement, const char *name)
{
  free(anElement->name);
  anElement->name = strdup(name);
}

void
dumpList(char* label, list* aList, void* callback(void*))
{
    list* curElement;

    fprintf(stderr, "*** %s ***\n", label);
    for (curElement = aList; curElement; curElement=curElement->next) {
	fprintf(stderr, "%lX: %s\n", curElement->cookie, curElement->name);
	callback((void*)curElement);
    }
}

void*
allocStructF(unsigned int n, long cookie)
{
  list           *p;
  
  p = calloc(1, n);
  if (p == 0) {
    pdebug(DERR, "Out of memory, aborting program.\n");
    exit(1);
  }
  p->cookie = cookie;
  p->name = NULL;
  p->next = NULL;

  return (void *) p;
}

