#include "Spring.h"

Spring::Spring()
{

}

Spring::Spring(int point1, int point2, float stiffness, float initialLength) : point1(point1), point2(point2), stiffness(stiffness), initialLength(initialLength)
{

}


Spring::~Spring()
{
}
