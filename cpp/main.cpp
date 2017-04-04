/*
Copyright (C) 2017
Rafael Guglielmetti
*/

/*
This file is part of CoxIterWeb.

CoxIterWeb is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

CoxIter is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with AlVin. If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>

#include "app.h"

using namespace std;

int main(int argc, char **argv)
{
	App app;
	if (!app.bCreateCoxeterMatrix(argc, argv))
	{
		cout << "<error>" << app.get_strError() << "</error>" << endl;
		return 0;
	}
	
	#ifdef _COMPILE_WITH_PARI_
	GrowthRate_Result grr;
	grr.bComputed = false;
	grr.iPerron = -1;
	grr.iPisot = -1;
	grr.iSalem = -1;
	#endif

	CoxIter* ci(app.doComputations());
	if (ci->get_strError() == "")
	{
		unsigned int iDimension(ci->get_iDimension());
		
		cout << "<cofinite>" << ci->get_iIsFiniteCovolume() << "</cofinite>" << endl;
		cout << "<cocompact>" << ci->get_iIsCocompact() << "</cocompact>" << endl;
		cout << "<euler>" << ci->get_brEulerCaracteristic() << "</euler>" << endl;
		
		if(iDimension)
		{
			vector< unsigned int > iFVector(ci->get_iFVector());
			
			cout << "<vatinfinity>" << ci->get_iVerticesAtInfinityCount() << "</vatinfinity>" << endl;
			
			cout << "<fvector>(";
			for (unsigned int i( 0 ); i <= iDimension; i++)
				cout << ( i ? ", " : "" ) << iFVector[i];
			cout << ")</fvector>" << endl;
		}
		
		#ifdef _COMPILE_WITH_PARI_
		GrowthRate gr;
		grr = gr.grrComputations(ci->get_iGrowthSeries_denominator());
		
		if(grr.bComputed && ci->get_bGrowthSeriesReduced())
		{
			cout << "<growthRate>" << endl;
			cout << "<value>" << grr.strGrowthRate << "</value>" << endl;
			cout << "<perron>" << ( grr.iPerron < 0 ? "?" : ( grr.iPerron > 0 ? "yes" : "no" ) ) << "</perron>" << endl;
			cout << "<pisot>" << ( grr.iPisot < 0 ? "?" : ( grr.iPisot > 0 ? "yes" : "no" ) ) << "</pisot>" << endl;
			cout << "<salem>" << ( grr.iSalem < 0 ? "?" : ( grr.iSalem > 0 ? "yes" : "no" ) ) << "</salem>" << endl;
			cout << "</growthRate>" << endl;
		}
		#endif
	}
	else
	{
		cout << "<error>" << ci->get_strError() << "</error>" << endl;
		delete ci;
		return 0;
	}
	
	delete ci;

	return 0;
}
