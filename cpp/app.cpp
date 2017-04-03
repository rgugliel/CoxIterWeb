#include "app.h"

App::App()
: 	iDimension(0),
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
		cout << "Reading: #" << strPieces[i] << "#" << endl;
		if (regexp.preg_match_all( "^\\[([[:digit:]]{1,4})(,\\[[[:digit:]]{1,4},[[:digit:]]{1,4}\\])+\\]$", strPieces[i], regexpRes)) // TODO: 4 --> option
		{
			cout << "Row: " << regexpRes[0][0] << endl;
			iSource = stoi(regexpRes[1][0]);
			
			if (regexp.preg_match_all( "\\[([[:digit:]]{1,4},[[:digit:]]{1,4})\\]", strPieces[i], regexpRes))
			{
				iCount = regexpRes[1].size();
				for (j = 0; j < iCount; j++)
				{
					explode(",", regexpRes[1][j], strPiecesTemp);
					if (strPiecesTemp.size() != 2)
					{
						cout << "ERROR: " << regexpRes[1][j] << endl; // TODO
						strError = "graph invalid format";
						return false;
					}
					
					cout << iSource << " - " << stoi(strPiecesTemp[0]) << ": " << stoi(strPiecesTemp[1]) << endl;
				}
				
				for (unsigned int r(0); r < regexpRes.size(); r++)
				{
					for (unsigned c(0); c < regexpRes[r].size(); c++)
						cout << "[" << r << "][" << c << "]=" << regexpRes[r][c] << endl;
				}
			}
			else
				cout << "Not found" << endl;
			
		}
		else if (regexp.preg_match_all( "dimension=([[:digit:]]{1,2})", strPieces[i], regexpRes))
			iDimension = stoi(regexpRes[1][0]);
			
		cout << endl;
	}
	
	return true;
}

string App::get_strError() const
{
	return strError;
}
