#include <assert.h>
#ifndef _COLOR_H_
#define _COLOR_H_

union ColorUnion
{
	struct
	{
		float  r;
		float  g;
		float  b;
	};

	float & operator [] (int i){
		assert(i >= 0 && i < 3);
		return color[i];
	};

private:
	float color[3] = { 0,0,0 };
};


#endif // _COLOR_H_
