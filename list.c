#include "defs.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node_ {
	pElement elem;
	struct Node_* next_node;
} Node;
typedef Node* pNode;

struct List_ {
	pNode first_node;
	pNode iterator;
	clone_func clone_elem;
	destroy_func destroy_elem;
	compare_func compare_elem;
	print_func print_elem;
};

//************************************************************************************************************
//* Function name : ListCreate
//* Description   : the function creates the list and initiate its fields
//* Parameters    : clone_elem - pointer to the clone function of the ADT
//*					destroy_elem - pointer to the destroy function of the ADT
//*					compare_elem - pointer to the compare function of the ADT
//*					print_elem - pointer to the print function of the ADT
//* Return value  : PList - pointer to the list that was created
//************************************************************************************************************

PList ListCreate(clone_func clone_elem, destroy_func destroy_elem, compare_func compare_elem, print_func print_elem)
{
// first check if we have all the input parameters
	if (clone_elem == NULL || destroy_elem == NULL || compare_elem == NULL || print_elem == NULL)
		return NULL;
// use malloc to get the memory for the list
	PList list = (PList)malloc(sizeof(struct List_));
	if (list == NULL)
		return NULL;
// initiate all the list fields
	list->clone_elem = clone_elem;
	list->destroy_elem = destroy_elem;
	list->compare_elem = compare_elem;
	list->print_elem = print_elem;
	list->first_node = NULL;
	list->iterator = NULL;
	return list;
}

//************************************************************************************************************
//* Function name : ListDestroy
//* Description   : the function destroy the list and all the elements in it
//* Parameters    : list - the list we need to destroy
//* Return value  : None.
//************************************************************************************************************

void ListDestroy(PList list)
{
// first check if the input parameter is good
	if (list == NULL)
		return;
// if the list is empty, no nodes, we need to free the list
	if (list->first_node == NULL)
	{
		free(list);
		return;
	}
// if the list is not empty we need first to destroy all the list elements using the destroy function of the list, and then free the list
	pNode tmp = list->first_node->next_node;
	while (list->first_node != NULL)
	{
		list->destroy_elem(list->first_node->elem);
		free(list->first_node);
		list->first_node = tmp;
		if (tmp != NULL)
			tmp = tmp->next_node;
	}
	free(list);
	return;
}

//************************************************************************************************************
//* Function name : ListAdd
//* Description   : the function adds new element to the end of the list
//* Parameters    : list - the list we need to add an element
//*					element - the element we need to add to the list
//* Return value  : Result - SUCCESS if the insertion was good, anf FAIL if don't
//************************************************************************************************************

Result ListAdd(PList list, pElement element)
{
// first check that the input parameters are good
	if (list == NULL || element == NULL)
		return FAIL;
// create a copy of the element
	pElement new_elem = list->clone_elem(element);
	if (new_elem == NULL)
		return FAIL;
// create a node for the new element
	pNode new_node = (pNode)malloc(sizeof(Node));
	if (new_node == NULL)
	{
		list->destroy_elem(new_elem);
		return FAIL;
	}
	new_node->elem = new_elem;
	new_node->next_node = NULL;
// if the list is empty we add the new element to the begining of the list
	if (list->first_node == NULL)
	{
		list->first_node = new_node;
		return SUCCESS; //iterator?????
	}
// if we have elements in the list we search the end of the list and we insert the element there
	pNode tmp = list->first_node;
	while (tmp->next_node != NULL)
	{
		tmp = tmp->next_node;
	}
	tmp->next_node = new_node;
	return SUCCESS;
}

//************************************************************************************************************
//* Function name : ListRemove
//* Description   : the function removes the given element from the list
//* Parameters    : list - the list we need to remove an element
//*					element - the element we need to remove from the list
//* Return value  : Result - SUCCESS if the remove was good, anf FAIL if don't
//************************************************************************************************************

Result ListRemove(PList list, pElement element)
{
// first we check that the input parameters are good
	if (list == NULL || element == NULL || list->first_node == NULL) //list->first_node == NULL????
	{
		return FAIL;
	}
// if we need to remove the first we save the second element, remove the first and then list->first_node points to the saved
	if (list->compare_elem(list->first_node->elem, element))//case we need to remove the first
	{
		pNode tmp = list->first_node->next_node;
		list->destroy_elem(list->first_node->elem);
		free(list->first_node);
		list->first_node = tmp;
		return SUCCESS;
	}
// if we need to remove not the first flight, we use 2 tmp variables, one points the element to remove and the second pints to the one before
	pNode tmp0 = list->first_node;
	pNode tmp1 = tmp0->next_node;
	while (tmp1 != NULL)
	{
		if (list->compare_elem(tmp1->elem, element))
		{
			tmp0->next_node = tmp1->next_node;
			list->destroy_elem(tmp1->elem);
			free(tmp1);
			return SUCCESS;//if we remove only one at a time
		}
		tmp0 = tmp1;
		tmp1 = tmp1->next_node;
	}
	return FAIL;
}

//************************************************************************************************************
//* Function name : ListGetFirst
//* Description   : the function return the first element of the list, and change the iterator to point to the first element
//* Parameters    : list - the list we need to change the iterator and return the first element
//* Return value  : pElement - pointer to element, the first element of the list
//************************************************************************************************************

pElement ListGetFirst(PList list)
{
	if (list == NULL || list->first_node == NULL)
		return NULL;
	list->iterator = list->first_node;
	return list->iterator->elem;
}

//************************************************************************************************************
//* Function name : ListGetNext
//* Description   : the function change the iterator to point to the next element and return it
//* Parameters    : list - the list we need to change the uterator and return the element
//* Return value  : pElement - the element we need to return
//************************************************************************************************************

pElement ListGetNext(PList list)
{
// first check that the list is not null
	if (list == NULL)
		return NULL;
// if the iterator points to null we change him to point to the first element
	if (list->iterator == NULL)
	{
		list->iterator = list->first_node;
		return list->iterator->elem;
	}
// if the next element is null we return null
	if (list->iterator->next_node == NULL)
	{
		return NULL;
	}
// any other case we change the iterator to point to the next element and return it
	list->iterator = list->iterator->next_node;
	return list->iterator->elem;
}

//************************************************************************************************************
//* Function name : ListCompare
//* Description   : the function compares the two lists
//* Parameters    : listA - first list to compare
//*					listB - second list to compare
//* Return value  : BOOL - TRUE oif all the elements at the two lists are equal, FALSE any other case
//************************************************************************************************************

BOOL ListCompare(PList listA, PList listB)
{
// first we check that the two lists are not null, or that one list is null, and the first element of the other list is null
	if (listA == NULL && listB == NULL)
		return FALSE;
	if ((listA == NULL && listB != NULL) || (listB == NULL && listA != NULL))
		return FALSE;
// if the two lists are not null but they are both empty we return TRUE because they are equal
	if (listA->first_node == NULL && listB->first_node == NULL)
		return TRUE;
	if (listA == listB)
		return TRUE;
// two tmp to run over the two lists
	pElement tmpA = ListGetFirst(listA);
	pElement tmpB = ListGetFirst(listB);
// the loop end when one of the lists is over
	while (tmpA != NULL || tmpB != NULL)
	{
// if we got one element that is not the same the lists are not equal and we return FALSE
		if (!listA->compare_elem(tmpA, tmpB))
			return FALSE;
		tmpA = ListGetNext(listA);
		tmpB = ListGetNext(listB);
	}
// if the loop finished, we need to check if the two lists were ended, if is the lists are equal and we return TRUE
	if (tmpA == NULL && tmpB == NULL)
		return TRUE;
	return FALSE;
}

//************************************************************************************************************
//* Function name : ListPrint
//* Description   : the function prints the list
//* Parameters    : list - the list we need to print
//* Return value  : None.
//************************************************************************************************************

void ListPrint(PList list)
{
// first check if the input parameters are good
	if (list == NULL)
		return;
// loop that runs over all the elements of the list and each time prints one element using the print function of the current ADT
	pNode tmp = list->first_node;
	while (tmp != NULL)
	{
		list->print_elem(tmp->elem);
		tmp = tmp->next_node;
	}
	printf("\n");
	return;
}
