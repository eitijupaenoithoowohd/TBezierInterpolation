#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

#define EPSILON 1.0e-5
#define RESOLUTION 32

class Point2D
{
public:
    double x, y;
    
    Point2D() { x = y = 0.0; };
    Point2D(double _x, double _y) { x = _x; y = _y; };
    
    Point2D operator +(const Point2D &point) const { return Point2D(x + point.x, y + point.y); };
    Point2D operator -(const Point2D &point) const { return Point2D(x - point.x, y - point.y); };
    Point2D operator *(double v) const { return Point2D(x * v, y * v); };
    void operator +=(const Point2D &point) { x += point.x; y += point.y; };
    void operator -=(const Point2D &point) { x -= point.x; y -= point.y; };
    
    void normalize()
    {
        double l = sqrt(x * x + y * y);
        x /= l;
        y /= l;
    }
};

class Segment
{
public:
    Point2D points[4];
    
    void calc(double t, Point2D &p)
    {
        double t2 = t * t;
        double t3 = t2 * t;
        double nt = 1.0 - t;
        double nt2 = nt * nt;
        double nt3 = nt2 * nt;
        p.x = nt3 * points[0].x + 3.0 * t * nt2 * points[1].x + 3.0 * t2 * nt * points[2].x + t3 * points[3].x;
        p.y = nt3 * points[0].y + 3.0 * t * nt2 * points[1].y + 3.0 * t2 * nt * points[2].y + t3 * points[3].y;
    };
};

bool calculateSpline(const vector<Point2D> &values, vector<Segment> &bezier)
{
    int n = values.size() - 1;
    
    if (n < 2)
        return false;
    
    bezier.resize(n);
    
    Point2D tgL;
    Point2D tgR;
    Point2D cur;
    Point2D next = values[1] - values[0];
    next.normalize();
    
    double l1, l2, tmp, x;
    
    
    for (int i = 0; i < n; ++i)
    {
        bezier[i].points[0] = bezier[i].points[1] = values[i];
        bezier[i].points[2] = bezier[i].points[3] = values[i + 1];
        
        tgL = tgR;
        cur = next;

        if(i+1 < n){
            next = values[i + 2] - values[i + 1];
            next.normalize();

            tgR = cur + next;
            tgR.normalize();
        }else{
            tgR.x= 0.0;
            tgR.y= 0.0;
        }

        
        if (abs(values[i + 1].y - values[i].y) < EPSILON)
        {
            l1 = l2 = 0.0;
        }
        else
        {
            tmp = values[i + 1].x - values[i].x;
            l1 = abs(tgL.x) > EPSILON ? tmp / (2.0 * tgL.x) : 1.0;
            l2 = abs(tgR.x) > EPSILON ? tmp / (2.0 * tgR.x) : 1.0;
        }
        
        if (abs(tgL.x) > EPSILON && abs(tgR.x) > EPSILON)
        {
            tmp = tgL.y / tgL.x - tgR.y / tgR.x;
            if (abs(tmp) > EPSILON)
            {
                x = (values[i + 1].y - tgR.y / tgR.x * values[i + 1].x - values[i].y + tgL.y / tgL.x * values[i].x) / tmp;
                if (x > values[i].x && x < values[i + 1].x)
                {
                    if (tgL.y > 0.0)
                    {
                        if (l1 > l2)
                            l1 = 0.0;
                        else
                            l2 = 0.0;
                    }
                    else
                    {
                        if (l1 < l2)
                            l1 = 0.0;
                        else
                            l2 = 0.0;
                    }
                }
            }
        }
        
        bezier[i].points[1] += tgL * l1;
        bezier[i].points[2] -= tgR * l2;
    }
    
    return true;
}

int main()
{
    vector<Point2D> testValues;
    vector<Segment> spline;
    Point2D p;

    testValues.push_back(Point2D(0, 0));
    testValues.push_back(Point2D(20, 0));
    testValues.push_back(Point2D(45, -47));
    testValues.push_back(Point2D(53, 335));
    testValues.push_back(Point2D(57, 26));
    testValues.push_back(Point2D(62, 387));
    testValues.push_back(Point2D(74, 104));
    testValues.push_back(Point2D(89, 0));
    testValues.push_back(Point2D(95, 100));
    testValues.push_back(Point2D(100, 0));

    calculateSpline(testValues, spline);

    for (auto s : spline)
    {
        for (int i = 0; i < RESOLUTION; ++i)
        {
            s.calc((double)i / (double)RESOLUTION, p);
            cout << p.x << " " << p.y << endl;
        }
    }

    cout << testValues.back().x << " " << testValues.back().y << endl;

    return 0;
}
