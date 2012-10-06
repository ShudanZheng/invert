#ifndef UTILITY_H_
#define UTILITY_H_
#include "config.h"

namespace utility
{
	class CUtility
	{
	public: 
		CUtility(); 
		~CUtility(); 
		static int GetFilesInFolder(string fName, vector<string>& nameVec); 
	};
}

#endif //UTILITY_H_