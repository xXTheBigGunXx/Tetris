#ifndef TASKUTILS_H
#define TASKUTILS_H

#include <filesystem>

class TaskUtils
{
public:
	static int xDir;
	static bool oneDown;
	static bool goDown;
	static bool rotate;

	static int RandomInt(int range = 0);
	static char KeyPressed();
	static void Direction();
	static void Reset();
};

#endif