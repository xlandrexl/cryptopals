#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/set2.h"
#include "../include/basics.h"

int main()
{
	int ret = 0;
	char pt1[]="ICE ICE BABY\x04\x04\x04\x04";
	char pt2[]="ICE ICE BABY\x05\x05\x05\x05";
	char pt3[]="ICE ICE BABY\x01\x02\x03\x04";
	ret = challenge15(pt1);
	ret = challenge15(pt2);
	ret = challenge15(pt3);

	return ret;
}
