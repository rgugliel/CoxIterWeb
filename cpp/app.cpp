#include "app.h"

string App::verticesMaximalDigits = to_string((unsigned int)ceil(log10(COXITERWEB_MAXIMAL_NUMBER_VERTICES)));
string App::edgeMaximalDigits = to_string((unsigned int)ceil(log10(COXITERWEB_MAXIMAL_WEIGHT)));
string App::graphDescriptionRegexp = "^\\[([[:digit:]]{1," + verticesMaximalDigits + "})(,\\[[[:digit:]]{1," + verticesMaximalDigits + "},[[:digit:]]{1," + edgeMaximalDigits + "}\\])+\\]$";
string App::edgeDescriptionRegexp = "\\[([[:digit:]]{1," + verticesMaximalDigits + "},[[:digit:]]{1," + edgeMaximalDigits + "})\\]";

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
	
	for (unsigned int i(0); i < iPiecesCount; i++)
	{
		if (regexp.preg_match_all(graphDescriptionRegexp, strPieces[i], regexpRes))
		{
			iSource = stoi(regexpRes[1][0]);
			
			if (regexp.preg_match_all(edgeDescriptionRegexp, strPieces[i], regexpRes))
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
	
	if (!iVerticesCount)
	{
		strError = "graph empty";
		return false;
	}
	
	return true;
}

bool App::addEdge(const unsigned int& iV1, const unsigned int&  iV2, const unsigned int& iWeight)
{
	unsigned int iMax(max(iV1, iV2));
	
	if (iWeight > COXITERWEB_MAXIMAL_WEIGHT)
	{
		strError = "weight maximal";
		return false;
	}
	
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
	{
		strError = "graph encoding";
		return ci;
	}
	
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
