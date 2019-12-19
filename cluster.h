#ifndef _CLUSTER_H_
#define _CLUSTER_H_

typedef struct Cluster_* PCluster;

PCluster ClusterCreate(int dimension);
void ClusterDestroy(PCluster cluster);
Result ClusterAddPoint(PCluster cluster, PPoint point);
int ClusterGetMinDistance(PCluster cluster, PPoint point);
void ClusterPrint(PCluster cluster);
void printPoint(pElement elem);
void destroyPoint(pElement elem);
pElement clonePoint(pElement elem);
BOOL comparePoint(pElement elemA, pElement elemB);

#endif
