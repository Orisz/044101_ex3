#ifndef _PERSON_H_
#define _PERSON_H_

typedef struct Person_ Person,*PPerson;

PPerson PersonCreate(char* name, int age, int ID);
void printPerson(pElement elem);
void destroyPerson(pElement elem);
pElement clonePerson(pElement elem);
BOOL comparePersons(pElement elemA, pElement elemB);

#endif
