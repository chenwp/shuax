#ifndef _GESTURE_H_
#define _GESTURE_H_

#include <math.h>

TCHAR str[][4]= {_T(""),_T("U"),_T("R"),_T("D"),_T("L")};
class Gesture
{
private:
    POINT now;

    long time;

    int count[5];//0-5
    int result;

    TCHAR command[26];
    float		getDistance(POINT a, POINT b);
    int		    getOrientation(POINT a, POINT b);

public:
    //Gesture();
    void begin(POINT pt);
    void add(POINT pt);
    TCHAR* end();
};

void Gesture::begin(POINT pt)
{
    time = GetCurrentTime();
    now = pt;
    for(int i=0; i<5; i++) count[i]=0;
    command[0]=0;
    result=0;
}
TCHAR* Gesture::end()
{
    if(GetCurrentTime()-time>1000) command[0]=0;
    return command;
}
void Gesture::add(POINT pt)
{
    if(GetCurrentTime()-time>1000) return;
    if (getDistance(pt, now) > 4.0)
    {
        count[getOrientation(pt, now)]++;
        for(int i = 1; i<5; i++)
        {
            if(count[i]>8 && result!=i)
            {
                _tcscat(command,str[i]);
                result = i;
                for(int i=0; i<5; i++) count[i]=0;
            }
        }
        now = pt;
    }
}
float Gesture::getDistance(POINT a, POINT b)
{
    return sqrt((float)((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y)));
}
int Gesture::getOrientation(POINT a, POINT b)
{
    int orientation = 0;
    int x = a.x - b.x;
    int y = a.y - b.y;
    if (x >= 0 && y <= 0)
    {
        //第一象限
        y *= -1;
        if (x > y && sqrt(3.0)*y < x) orientation = 2;
        if (x < y && sqrt(3.0)*x < y) orientation = 1;
        y *= -1;
    }
    else if (x >= 0 && y >= 0)
    {
        //第四象限
        if (x > y && sqrt(3.0)*y < x) orientation = 2;
        if (x < y && sqrt(3.0)*x < y) orientation = 3;
    }
    else if (x<=0 && y>=0)
    {
        //第三象限
        x *= -1;
        if (x > y && sqrt(3.0)*y < x) orientation = 4;
        if (x < y && sqrt(3.0)*x < y) orientation = 3;
        x *= -1;
    }
    else  if (x <= 0 && y <= 0)
    {
        //第二象限
        x *= -1;
        y *= -1;
        if (x > y && sqrt(3.0)*y < x) orientation = 4;
        if (x < y && sqrt(3.0)*x < y) orientation = 1;
    }
    return orientation;
}
#endif /* _GESTURE_H_ */
