#include "TMath.h"
#include "Cmn.h"

namespace talga
{
	F32 abs(F32 num)
	{
		if (num < 0)
		{
			return -num;
		}
		else
		{
			return num;
		}
	}
}
