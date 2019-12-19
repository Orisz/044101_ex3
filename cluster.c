#include "defs.h"
#include "list.h"
#include "point.h"
#include "cluster.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Cluster_
{
	PList cluster_pts;
	int dimension_cluster;
};

//************************************************************************************************************
//* Function name : ClusterCreate
//* Description   : the function creates a cluster of points
//* Parameters    : dimension - the dimension of all the points in the cluster
//* Return value  : PCluster - pointer to the cluster that was created
//************************************************************************************************************

PCluster ClusterCreate(int dimension)
{
// get the memory for the cluster struct
	PCluster new_cluster = (PCluster)malloc(sizeof(struct Cluster_));
// check that the malloc succeeded
	if (new_cluster == NULL)
		return NULL;
// create list to save the points of the cluster
	new_cluster->cluster_pts = ListCreate(clonePoint, destroyPoint, comparePoint, printPoint);
// check that ListCreate was succeeded, if not we need to free the cluster
	if (new_cluster->cluster_pts == NULL)
	{
		free(new_cluster);
		return NULL;
	}
// initiate the dimension of the cluster
	new_cluster->dimension_cluster = dimension;
	return new_cluster;
}

//************************************************************************************************************
//* Function name : ClusterDestroy
//* Description   : the function destroys the cluster and all the points in it
//* Parameters    : cluster - the cluster to destroy
//* Return value  : None.
//************************************************************************************************************

void ClusterDestroy(PCluster cluster)
{
// first check that the cluster is not null	
	if (cluster == NULL)
		return;
// check that we have the list of points in the cluster, if not we just need to free the cluster
	if (cluster->cluster_pts == NULL)
	{
		free(cluster);
		return;
	}
// if we have list, first destroy the list and then free the cluster
	ListDestroy(cluster->cluster_pts);
	free(cluster);
	return;
}

//************************************************************************************************************
//* Function name : ClusterAddPoint
//* Description   : the function adds new point to the cluster
//* Parameters    : cluster - the cluster which we need to add point to it
//*					point - the point we need to add
//* Return value  : Result - SUCCESS if the point was added to the cluster, FAIL if not
//************************************************************************************************************

Result ClusterAddPoint(PCluster cluster, PPoint point)
{
// first we check that the input arguments are good
	if (cluster == NULL || point == NULL)
		return FAIL;
// then we need to check that the dimension of the point is equal to the dimension of the cluster
	int point_dim = PointGetDim(point);
	if (point_dim == cluster->dimension_cluster)
	{
// now we need to check for each point in the cluster that we don't have the same point
		PPoint tmp_point = (PPoint)ListGetFirst(cluster->cluster_pts);
		while (tmp_point != NULL)
		{
			if (PointCompare(point, tmp_point))
				return FAIL;
			tmp_point = (PPoint)ListGetNext(cluster->cluster_pts);
		}
// if this point is new we clone it and add it to the list
		PPoint new_point = (PPoint)clonePoint(point);
		if (new_point == NULL)
			return FAIL;
		if (ListAdd(cluster->cluster_pts, new_point))
		{	
			destroyPoint(new_point);
			return SUCCESS;
		}
		else
		{
			destroyPoint(new_point);
			return FAIL;
		}
	}
// if the dimensions are not equal we return fail
	return FAIL;
}

//************************************************************************************************************
//* Function name : ClusterGetMinDistance
//* Description   : given a cluster and a point returns the min square distance between the point and the closest point of the cluster
//* Parameters    : pCluster cluster - pointer to the cluster
//*					PPoint point - pointer to the point
//* Return value  : int MSD - the minimum square distance
//************************************************************************************************************

int ClusterGetMinDistance(PCluster cluster, PPoint point)
{	//check if one of the variables needed is NULL
	if (cluster == NULL || cluster->cluster_pts == NULL || cluster->dimension_cluster == 0 || point == NULL)
		return 1000;//error code
	PPoint point_dist = (PPoint)ListGetFirst(cluster->cluster_pts);//the first point from cluster to compare to given point
	int MSD = -1;//the min square distance variable 
	int coor_point = 0;//coordinate of given point
	int coor_point_dist = 0;//coordinate of the current cluster point
	int cur_SD = 0;//the variable that holds the current square distance
	int i = 0;
	while (point_dist != NULL)//loop over the cluster points
	{
		if (!PointCompare(point_dist, point))//point are not the same
		{
			coor_point = PointGetFirstCoordinate(point);
			coor_point_dist = PointGetFirstCoordinate(point_dist);
			for (i = 0; i < cluster->dimension_cluster; i++)//loop over point coordinates
			{
				cur_SD = cur_SD + (coor_point - coor_point_dist)*(coor_point - coor_point_dist);//MSD
				coor_point = PointGetNextCoordinate(point);//moving the iterator step ahead(of given point)
				coor_point_dist = PointGetNextCoordinate(point_dist);//moving the iterator step ahead(of cluster point)
			}
			if (MSD == -1)//for the first point itteration
			{
				MSD = cur_SD;
				cur_SD = 0;
			}
			else if (MSD > cur_SD)//better SD update MSD!
			{
				MSD = cur_SD;
				cur_SD = 0;
			}
			else//result isnt better start over with the new point from the cluster
			{
				cur_SD = 0;
			}
		}
		point_dist = (PPoint)ListGetNext(cluster->cluster_pts);//get the next point from the cluster
	}
	return MSD;
}

//************************************************************************************************************
//* Function name : ClusterPrint
//* Description   : the function prints the cluster and all the points in it, and the minimum square distance
//* Parameters    : cluster - the cluster we need to print
//* Return value  : None.
//************************************************************************************************************

void ClusterPrint(PCluster cluster)
{
// first we check that the cluster is not null
	if (cluster == NULL)
		return;
// print the first line
	printf("Cluster's dimension: %d\n", cluster->dimension_cluster);
// gat the first point, the loop will run on all the points in the cluster
	PPoint tmp_point = (PPoint)ListGetFirst(cluster->cluster_pts);
// the minimum square distance 
	int MSD = 0;
// the current square distance of the current point and all the cluster
	int cur_SD;
// counter we need in order to know in which point we are taking care now, because every time wi call the function ClusterGetMinDistance the iterator is null and we don't know which point to print and send to the function
	int counter = 0;
	int i = 0;
	while (tmp_point != NULL)
	{
		counter++;
// print the point
		PointPrint(tmp_point);
// take the current square distance
		cur_SD = ClusterGetMinDistance(cluster, tmp_point);
// if MSD is 0 it means that we send ClusterGetMinDistance the first point of the cluster, there is no option that cur_SD is zero.
		if (MSD == 0)
		{
			MSD = cur_SD;
		}
// if cur_SD is less than MSD, we have reached new minimum and we need to change MSD
		else if (MSD > cur_SD)
		{
			MSD = cur_SD;
		}
// now we set tmp_point to the next point we need to print by call ListGetFirst, and call ListGetNext # of times as the counter
		tmp_point = (PPoint)ListGetFirst(cluster->cluster_pts);
		for (i = 0; i < counter; i++)
		{
			tmp_point = (PPoint)ListGetNext(cluster->cluster_pts);
		}
	}
// if MSD is zero it means that we have only one point in the cluster and the MSD should be 1000
	if (MSD == -1)
		MSD = 1000;
// print the MSD
	printf("\nMinimum Square Distance: %d\n", MSD);
	return;
}

//************************************************************************************************************
//* Function name : printPoint
//* Description   : function already exists as Pointprint
//* Parameters    : elem.
//* Return value  : None.
//************************************************************************************************************

void printPoint(pElement elem)
{
	return;
}

//************************************************************************************************************
//* Function name : destroyPoint
//* Description   : call yhe function PointDestroy in order to free all the memory of point
//* Parameters    : pElement elem - pointer to void* the point to destroy
//* Return value  : void
//************************************************************************************************************

void destroyPoint(pElement elem)
{
	PPoint point = (PPoint)elem;
	PointDestroy(point);
	return;
}

//************************************************************************************************************
//* Function name : clonePoint
//* Description   : the function clones the point
//* Parameters    : elem - the poimt we need to clone
//* Return value  : pElement - pointer to the point that we cloned
//************************************************************************************************************

pElement clonePoint(pElement elem)
{
// first we check that the element, that we casting it to point, is not null
	PPoint point = (PPoint)elem;
	if (point == NULL)
		return NULL;
// then we create the point with the function PointCreate
	PPoint copy_point = PointCreate(PointGetDim(point));
	if (copy_point == NULL)
		return NULL;
// then we need to insert all the coordinates of the point to the copied point, so the loop does that using PointGetFirst/NextCoordinate, if at the middle of the loop something goes wrong we need to destroy the point
	int tmp_coor = PointGetFirstCoordinate(point);
	int i = 0;
	for (i=0 ; i < PointGetDim(point) ; i++)
	{
		if (!PointAddCoordinate(copy_point, tmp_coor))
		{
			destroyPoint(copy_point);
			return NULL;
		}
		else
		{
			tmp_coor = PointGetNextCoordinate(point);
		}
	}
	return copy_point;
}

//************************************************************************************************************
//* Function name : comparePoint
//* Description   : does nothing
//* Parameters    : pElement elemA , pElement elemB
//* Return value  : BOOL always return true
//************************************************************************************************************

BOOL comparePoint(pElement elemA, pElement elemB)
{
	return TRUE;
}
