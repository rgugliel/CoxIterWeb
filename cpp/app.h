#ifndef __APP_H__
#define __APP_H__

#include <string>
#include <vector>

#include "coxiter/lib/regexp.h"
#include "coxiter/lib/string.h"

using namespace std;

class App
{
	private:
		string strError;
		
		vector<vector<unsigned int>> iCoxeterMatrix;
		unsigned int iDimension;
		unsigned int iVerticesCount;
		
	public:
		App();
		
		bool bCreateCoxeterMatrix(int argc, char **argv);
		string get_strError() const;
};

#endif
