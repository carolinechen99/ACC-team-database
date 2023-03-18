#include "exerciser.h"

void exercise(connection *C)
{
    query1(C,
	    1, 35, 40,
      0, 0, 0,
      0, 0, 0,
      0, 0, 0,
      0, 0, 0,
      0, 0, 0);
    query2(C, "Red");
    query3(C, "UNC");
    query4(C, "NC", "LightBlue");
    query5(C, 9);

}
