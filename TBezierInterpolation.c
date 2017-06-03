// TBezierInterpolation.c
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define RESOLUTION 32
#define POINTS_LEN 10
#define EPSILON 1.0e-5

typedef struct {
	double x, y;
} Point2D;


Point2D Point2DAdd( Point2D *left,  Point2D *right)
{
	Point2D newPoint = {left->x + right->x, left->y + right->y};
	return newPoint;
}

Point2D Point2DSubtract( Point2D *left,  Point2D *right)
{
	Point2D newPoint = {left->x - right->x, left->y - right->y};
	return newPoint;
}

Point2D Point2DMultiply( Point2D *left, double v)
{
	Point2D newPoint = {left->x * v, left->y * v};
	return newPoint;
}

void Point2DNormalize(Point2D *point)
{
	double l = sqrt(point->x * point->x + point->y * point->y);
	point->x /= l;
	point->y /= l;
}

typedef struct {
	Point2D points[4];
} Segment;

void SegmentCalc(Segment *seg,double t, Point2D *p)
{
	double t2 = t * t;
	double t3 = t2 * t;
	double nt = 1.0 - t;
	double nt2 = nt * nt;
	double nt3 = nt2 * nt;
	p->x = nt3 * seg->points[0].x + 3.0 * t * nt2 * seg->points[1].x + 3.0 * t2 * nt * seg->points[2].x + t3 * seg->points[3].x;
	p->y = nt3 * seg->points[0].y + 3.0 * t * nt2 * seg->points[1].y + 3.0 * t2 * nt * seg->points[2].y + t3 * seg->points[3].y;
}


bool CalculateSpline( Point2D values[], int valuesSize, Segment bezier[])
{
	if (valuesSize < 3) {
		return false;
	}

	int n = valuesSize - 1;

	Point2D tgL= {0.0,0.0};
	Point2D tgR= {0.0,0.0};
	Point2D cur= {0.0,0.0};
	Point2D next = Point2DSubtract(&values[1] ,&values[0]);
	Point2D tmpPoint= {0.0,0.0};
	Point2DNormalize(&next);

	double l1 = 0.0, l2= 0.0, tmp= 0.0, x= 0.0;

	for (int i = 0; i < n; ++i) {
		bezier[i].points[0] = bezier[i].points[1] = values[i];
		bezier[i].points[2] = bezier[i].points[3] = values[i + 1];

		cur = next;
		tgL = tgR;

		if(i+1 < n){
			next = Point2DSubtract(&values[i + 2],&values[i + 1]);
			Point2DNormalize(&next);

			tgR = Point2DAdd(&cur,&next);
			Point2DNormalize(&tgR);
		}else{
			tgR.x= 0.0;
			tgR.y= 0.0;
		}

		if (fabs(values[i + 1].y - values[i].y) < EPSILON) {
			l1 = l2 = 0.0;
		} else {
			tmp = values[i + 1].x - values[i].x;
			l1 = fabs(tgL.x) > EPSILON ? tmp / (2.0 * tgL.x) : 1.0;
			l2 = fabs(tgR.x) > EPSILON ? tmp / (2.0 * tgR.x) : 1.0;
		}

		if (fabs(tgL.x) > EPSILON && fabs(tgR.x) > EPSILON) {
			tmp = tgL.y / tgL.x - tgR.y / tgR.x;
			if (fabs(tmp) > EPSILON) {
				x = (values[i + 1].y - tgR.y / tgR.x * values[i + 1].x - values[i].y + tgL.y / tgL.x * values[i].x) / tmp;
				if (x > values[i].x && x < values[i + 1].x) {
					if (tgL.y > 0.0) {
						if (l1 > l2)
							l1 = 0.0;
						else
							l2 = 0.0;
					} else {
						if (l1 < l2)
							l1 = 0.0;
						else
							l2 = 0.0;
					}
				}
			}
		}
		tmpPoint = Point2DMultiply(&tgL , l1);
		bezier[i].points[1] = Point2DAdd(&bezier[i].points[1],&tmpPoint);
		tmpPoint = Point2DMultiply(&tgR , l2);
		bezier[i].points[2] = Point2DSubtract(&bezier[i].points[2],&tmpPoint);
	}

	return true;
}

int main()
{

	Point2D values[POINTS_LEN] = {
		{0.0, 0.0},
		{20.0, 0.0},
		{45.0, -47.0},
		{53.0, 335.0},
		{57.0, 26.0},
		{62.0, 387.0},
		{74.0, 104.0},
		{89.0, 0.0},
		{95.0, 100.0},
		{100.0, 0.0}
	};

	Segment spline[100];

	Point2D p = {0.0,0.0};

	CalculateSpline(values,POINTS_LEN, spline);

	for(int i = 0; i < POINTS_LEN - 1; i++) {
		for (int j = 0; j < RESOLUTION; j++) {
			SegmentCalc(&spline[i],(double)j / (double)RESOLUTION, &p);
			printf("%lf %lf\n", p.x, p.y);
		}
	}

	printf("%lf %lf\n", values[POINTS_LEN - 1].x,values[POINTS_LEN - 1].y);

	return 0;
}
