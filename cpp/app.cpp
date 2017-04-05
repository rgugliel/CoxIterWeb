#include "app.h"

App::App()
:
	iDimension(0),
	iVerticesCount(0),
	strError("")
{
}

bool App::bCreateCoxeterMatrix(int argc, char **argv)
{
	string strTemp("");
	PCRERegexp regexp;
	PCREResult regexpRes;
	vector<string> strPieces;
	
	// -------------------------------
	// Tokenization
	for( int i = 0; i < argc; ++i ) 
		strTemp = std::string( argv[i] );
	
	explode(";", strTemp, strPieces);
	unsigned int iPiecesCount(strPieces.size());
	
	// -------------------------------
	// Creating the matrix
	unsigned int iSource, iCount, j;
	vector<string> strPiecesTemp;
	
	// TODO: check max vertices
	
	for (unsigned int i(0); i < iPiecesCount; i++)
	{
		if (regexp.preg_match_all( "^\\[([[:digit:]]{1,4})(,\\[[[:digit:]]{1,4},[[:digit:]]{1,4}\\])+\\]$", strPieces[i], regexpRes)) // TODO: 4 --> option
		{
			iSource = stoi(regexpRes[1][0]);
			
			if (regexp.preg_match_all( "\\[([[:digit:]]{1,4},[[:digit:]]{1,4})\\]", strPieces[i], regexpRes))
			{
				iCount = regexpRes[1].size();
				for (j = 0; j < iCount; j++)
				{
					explode(",", regexpRes[1][j], strPiecesTemp);
					if (strPiecesTemp.size() != 2)
					{
						strError = "graph invalid format";
						return false;
					}
					
					if (!addEdge(iSource, stoi(strPiecesTemp[0]), stoi(strPiecesTemp[1])))
						return false;
				}
			}
		}
		else if (regexp.preg_match_all( "dimension=([[:digit:]]{1,2})", strPieces[i], regexpRes))
			iDimension = stoi(regexpRes[1][0]);
	}
	
	// TODO: test graph empty
	
	return true;
}

bool App::addEdge(const unsigned int& iV1, const unsigned int&  iV2, const unsigned int& iWeight)
{
	unsigned int iMax(max(iV1, iV2));
	
	if (COXITERWEB_MAXIMAL_NUMBER_VERTICES <= iMax)
	{
		strError = "vertices maximal number";
		return false;
	}
	
	if (iVerticesCount <= iMax) // We have to update matrix' size
	{
		for (unsigned int r(0); r < iVerticesCount; r++ )
		{
			for (unsigned int c(iVerticesCount); c <= iMax; c++)
				iCoxeterMatrix[r].push_back(2);
		}
		
		vector<unsigned int> iRow(iMax + 1, 2);
		for (unsigned int r(iVerticesCount); r <= iMax; r++)
			iCoxeterMatrix.push_back(iRow);
	}

	if (iCoxeterMatrix[iV1][iV2] != 2 && iCoxeterMatrix[iV1][iV2] != iWeight)
	{
		strError = "edge weight multiple";
		return false;
	}
	
	iCoxeterMatrix[iV1][iV2] = iCoxeterMatrix[iV2][iV1] = iWeight;
	iVerticesCount = iCoxeterMatrix.size();
	
	return true;
}

CoxIter* App::doComputations()
{
	CoxIter* ci(new CoxIter(iCoxeterMatrix, iDimension));
	ci->set_bCheckCofiniteness(true);
	ci->set_bCheckCocompactness(true);
	
	ci->exploreGraph();
	ci->computeGraphsProducts();
	
	if (!ci->bEulerCharacteristicFVector())
		return ci;
	
	#ifdef _COMPILE_WITH_PARI_
	ci->growthSeries();
	#endif
	
	ci->isFiniteCovolume();
	ci->iIsGraphCocompact();
	
	return ci;
}

string App::get_strError() const
{
	return strError;
}
