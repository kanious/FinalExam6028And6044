#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include <cstdlib>

static int GetRandNum(int min, int max)
{
	return (rand() % (max - min + 1)) + min;
}

#endif //_FUNCTION_H_