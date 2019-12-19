#include "defs.h"
#include "list.h"
#include "person.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Person_ 
{
	char* name;
	int age, ID;
};

//************************************************************************************************************
//* Function name : PersonCreate
//* Description   : the function creates struct of type person
//* Parameters    : name - the name of the person
//*					age - the age of the person
//*					ID - the id of the person
//* Return value  : PPerson - pointer to the new struct of type person
//************************************************************************************************************

PPerson PersonCreate(char* name, int age, int ID)
{
// check that the name input is good
	if (name == NULL)
		return NULL;
// get the memory for struct of type person
	PPerson new_person = (PPerson)malloc(sizeof(Person));
// check that the malloc succeeded
	if (new_person == NULL)
		return NULL;
// get memory for the name
	new_person->name = (char*)malloc(sizeof(char)*(strlen(name) + 1));
// check that the malloc for the name succeeded, if not we need to free the memory of person
	if (new_person->name == NULL)
	{
		free(new_person);
		return NULL;
	}
// initiate the fields of the struct
	strcpy(new_person->name, name);
	new_person->age = age;
	new_person->ID = ID;
	return new_person;
}

//************************************************************************************************************
//* Function name : printPerson
//* Description   : the function prints the person struct
//* Parameters    : elem - the person to print
//* Return value  : None.
//************************************************************************************************************

void printPerson(pElement elem)
{
	PPerson person_elem = (PPerson)elem;
	if (person_elem == NULL)
		return;
	printf("Name: %s, ID: %d, Age: %d\n", person_elem->name, person_elem->ID, person_elem->age);
	return;
}

//************************************************************************************************************
//* Function name : destroyPerson
//* Description   : the function destroys the person struct
//* Parameters    : elem - the person to destroy
//* Return value  : None.
//************************************************************************************************************


void destroyPerson(pElement elem)
{
	PPerson person_elem = (PPerson)elem;
	if (person_elem == NULL)
		return;
	free(person_elem->name);
	free(person_elem);
	return;
}

//************************************************************************************************************
//* Function name : clonePerson
//* Description   : the function clones the person struct
//* Parameters    : elem - the person to clone
//* Return value  : pElement - pointer ti the element that was cloned
//************************************************************************************************************

pElement clonePerson(pElement elem)
{
	PPerson person_elem = (PPerson)elem;
	if (person_elem == NULL)
		return NULL;
	PPerson copy_person = PersonCreate(person_elem->name, person_elem->age, person_elem->ID);
	return copy_person;
}

//************************************************************************************************************
//* Function name : comparePerson
//* Description   : the function compares 2 person structs
//* Parameters    : elemA - first person to compare
//*					elemB - second person to compare
//* Return value  : BOOL - if the persons equal return TRUE, else return FALSE
//************************************************************************************************************

BOOL comparePersons(pElement elemA, pElement elemB)
{
	PPerson personA = (PPerson)elemA;
	PPerson personB = (PPerson)elemB;
// if atleast one of the persons is NULL they are not equal
	if (personA == NULL || personB == NULL)
		return FALSE;
// compare the ID of the two persons
	if (personA->ID == personB->ID)
		return TRUE;
	else
		return FALSE;
}