#include "defs.h"
#include "list.h"
#include "point.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Point_
{
	int dimension;
	int size;
	PList point_coor;
};

//************************************************************************************************************
//* Function name : PointCreate
//* Description   : create a new point struct
//* Parameters    : int dim_size - he dimension size
//*					
//*					
//* Return value  : PPoint - pointer to the new struct
//************************************************************************************************************

PPoint PointCreate(int dim_size)
{
	PPoint new_point = (PPoint)malloc(sizeof(Point));
	if (new_point == NULL)
	{
		return NULL;
	}
	new_point->point_coor = ListCreate(cloneCoor, destroyCoor, compareCoor, printCoor);//uses the list struct
	if (new_point->point_coor == NULL)
	{
		free(new_point);
		return NULL;
	}
	new_point->dimension = dim_size;
	new_point->size = 0;//no coors added yet
	return new_point;
}

//************************************************************************************************************
//* Function name : PointDestroy
//* Description   : given a pointer to the struct destroys it
//*
//* Parameters    : PPoint point - pointer to the struct to be destroyed
//* Return value  : void
//************************************************************************************************************

void PointDestroy(PPoint point)
{
	if (point == NULL)
		return;
	ListDestroy(point->point_coor);
	free(point);
	return;
}

//************************************************************************************************************
//* Function name : PointAddCoordinate
//* Description   : we get a point struct and we want to add a coordinate to it when its given
//*					but we need to check first if the point is already full(max dimension)
//* Parameters    : PPoint point - pointer to the struct , int coor - the coordinate we want to add
//* Return value  : Result - SUCCES if we did it else FAIL
//************************************************************************************************************

Result PointAddCoordinate(PPoint point, int coor)
{
	if (point == NULL)//???? point->point_coor == NULL
	{//there is no point struct
		return FAIL;
	}
	if (point->dimension == point->size)//the point is already with full size avilabel
	{
		return FAIL;
	}
	if (!ListAdd(point->point_coor, &coor))///???DEBUG
		return FAIL;//using the generic list "add node to list" function
	point->size++;//if we added the coor we need to increase the size by 1
	return SUCCESS;
}

//************************************************************************************************************
//* Function name : PointGetFirstCoordinate
//* Description   : gets the irst coordinate of a point if we fail return 0
//*
//* Parameters    : PPoint point - a pointer to the point struct
//* Return value  : int - return value - the integer value of the first coordinate
//************************************************************************************************************

int PointGetFirstCoordinate(PPoint point)
{
	if (point == NULL || point->point_coor == NULL)//there is no point or coors
	{
		return 0;
	}
	pElement return_value = ListGetFirst(point->point_coor);//the generic list function fail
	if (return_value == NULL)
		return 0;
	return *(int*)return_value;
}

//************************************************************************************************************
//* Function name : PointGetNextCoordinate
//* Description   : using the "ListGetNext" function of the generic list we move the iterator one step 
//*					to the end of the list and return the coordinate 
//* Parameters    : PPoint point  - pointer to the point struct
//* Return value  : int - return value - the value of the coordinate ifwe fail return 0
//************************************************************************************************************

int PointGetNextCoordinate(PPoint point)
{
	if (point == NULL || point->point_coor == NULL)
	{
		return 0;
	}
	pElement return_value = ListGetNext(point->point_coor);
	if (return_value == NULL)
		return 0;
	return *(int*)return_value;
}

//************************************************************************************************************
//* Function name : PointPrint
//* Description   : using the "ListPrint" function of the generic list we print the coordinates of the point
//*
//* Parameters    : PPoint point - pointer to the strcut
//* Return value  : void
//************************************************************************************************************

void PointPrint(PPoint point)
{
	if (point == NULL || point->point_coor == NULL)
		return;
	printf("Point Dimension: %d, Size: %d, Coordinates: ", point->dimension, point->size);
	ListPrint(point->point_coor);
	return;
}

//************************************************************************************************************
//* Function name : printCoor
//* Description   : given a pointer to the coordinate prints it
//*
//* Parameters    : pElement elem - pointer to the coor 
//* Return value  : void
//************************************************************************************************************

void printCoor(pElement elem)
{
	int* coor = (int*)elem;
	if (coor == NULL)
		return;
	printf("%d ", *coor);
	return;
}

//************************************************************************************************************
//* Function name : destroyCoor
//* Description   : given a pointer to a list element (a coordinate in this case)
//*					destroys it by freeing it
//* Parameters    : pElement elem - pointer to the coordinate
//* Return value  : void
//************************************************************************************************************

void destroyCoor(pElement elem)
{
	int* coor = (int*)elem;
	if (coor == NULL)
		return;
	free(coor);
	return;
}

//************************************************************************************************************
//* Function name : cloneCoor
//* Description   : given a pointer to a coordinate create a clone of it
//*
//* Parameters    : pElement elem - pointer to a coordinate
//* Return value  : pElement - copy_coor - pointer to the cloned coordinate
//************************************************************************************************************

pElement cloneCoor(pElement elem)
{
	int* coor = (int*)elem;
	if (coor == NULL)
		return NULL;
	int* copy_coor = (int*)malloc(sizeof(int));
	if (copy_coor == NULL)
		return NULL;
	*copy_coor = *coor;
	return copy_coor;
}

//************************************************************************************************************
//* Function name : compareCoor
//* Description   : given 2 coordinates check if they are the same
//*
//* Parameters    : pElement elemA , pElement elemB - pointers to the 2 coordinates
//* Return value  : BOOL - TRUE if they are the same else FALSE
//************************************************************************************************************

BOOL compareCoor(pElement elemA, pElement elemB)
{
	int* coorA = (int*)elemA;
	int* coorB = (int*)elemB;
	if (coorA == NULL || coorB == NULL)
		return FALSE;
	if (*coorA == *coorB)
		return TRUE;
	return FALSE;
}

//************************************************************************************************************
//* Function name : PointGetDim
//* Description   : given a pointer to point struct returns the point dimenssion
//*
//* Parameters    : PPoint point - pointer to the point struct
//* Return value  : int dimession - the dim of the point
//************************************************************************************************************

int PointGetDim(PPoint point)
{
	if (point == NULL)
		return -1;
	return point->dimension;
}

//************************************************************************************************************
//* Function name : PointCompare
//* Description   : check if two points are the same by comparing thier dimenssion
//*					size and their coordinates
//* Parameters    : PPoint pointA, PPoint pointB - pointers to the 2 points structs to be compared
//* Return value  : BOOL TRUE if the points are the same else FALSE
//************************************************************************************************************

BOOL PointCompare(PPoint pointA, PPoint pointB)
{
	if (pointA == NULL || pointB == NULL)
		return FALSE;
	if (pointA->dimension != pointB->dimension || pointA->size != pointB->size)
		return FALSE;
	if (ListCompare(pointA->point_coor, pointB->point_coor))
		return TRUE;
	else
		return FALSE;

}
