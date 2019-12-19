#ifndef _POINT_H_
#define _POINT_H_

typedef struct Point_ Point,* PPoint;

PPoint PointCreate(int dim_size);
void PointDestroy(PPoint point);
Result PointAddCoordinate(PPoint point, int coor);
int PointGetFirstCoordinate(PPoint point);
int PointGetNextCoordinate(PPoint point);
void PointPrint(PPoint point);
void printCoor(pElement elem);
void destroyCoor(pElement elem);
pElement cloneCoor(pElement elem);
BOOL compareCoor(pElement elemA, pElement elemB);
int PointGetDim(PPoint point);
BOOL PointCompare(PPoint pointA, PPoint pointB);
int PointGetSize();

#endif
