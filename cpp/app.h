#ifndef __APP_H__
#define __APP_H__

#include <string>
#include <vector>
#include <cmath>

#include "config.h"

#include "coxiter/coxiter.h"
#include "coxiter/lib/regexp.h"
#include "coxiter/lib/string.h"

#ifdef _COMPILE_WITH_PARI_
#include "coxiter/growthrate.h"
#endif

using namespace std;

class App
{
	private:
		string strError;
		
		vector<vector<unsigned int>> iCoxeterMatrix;
		unsigned int iDimension;
		unsigned int iVerticesCount;
		
		static string verticesMaximalDigits;
		static string graphDescriptionRegexp;
		
	public:
		App();
		
		bool bCreateCoxeterMatrix(int argc, char **argv);
		CoxIter* doComputations();
		string get_strError() const;
		
	private:
		bool addEdge(const unsigned int& iV1, const unsigned int&  iV2, const unsigned int& iWeight);
};

#endif
