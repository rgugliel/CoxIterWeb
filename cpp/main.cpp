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
	grr.isComputed = false;
	grr.perron = -1;
	grr.pisot = -1;
	grr.salem = -1;
	#endif

	CoxIter* ci(app.doComputations());
	if (ci->get_error() == "" && app.get_strError() == "")
	{
		unsigned int iDimension(ci->get_dimension());
		
		cout << "<cofinite>" << ci->get_isFiniteCovolume() << "</cofinite>" << endl;
		cout << "<cocompact>" << ci->get_isCocompact() << "</cocompact>" << endl;
		cout << "<euler>" << ci->get_brEulerCaracteristic() << "</euler>" << endl;
		
		if (iDimension)
		{
			vector< unsigned int > iFVector(ci->get_fVector());
			
			cout << "<vatinfinity>" << ci->get_verticesAtInfinityCount() << "</vatinfinity>" << endl;
			
			cout << "<fvector>(";
			for (unsigned int i( 0 ); i <= iDimension; i++)
				cout << ( i ? ", " : "" ) << iFVector[i];
			cout << ")</fvector>" << endl;
			
			if (!(iDimension % 2) && ci->get_isFiniteCovolume() == 1)
			{
				MPZ_rational cov(( iDimension / 2 ) % 2 ? -1 : 1);
				for (unsigned int i( 1 ); i <= iDimension; i++)
				{
					cov *= 2;
					cov /= i;
					if( i <= ( iDimension / 2 ) )
						cov *= i;
				}
				
				cout << "<covolume>pi^" << ( iDimension / 2 ) << " * " << cov * ci->get_brEulerCaracteristic() << "</covolume>" << endl;
			}
		}
		
		if (ci->get_dimensionGuessed())
			cout << "<dimensionGuessed>" << ci->get_dimension() << "</dimensionGuessed>" << endl;
			
		// ---------------------------------------------------
		// Growth series
		vector<unsigned int> iCyclotomicNumerator;
		vector<mpz_class> iPolynomialDenominator;
		bool bReduced;
		
		ci->get_growthSeries(iCyclotomicNumerator, iPolynomialDenominator, bReduced);
		cout << "<growthSeries>" << endl;
		cout << "<isReduced>" << (bReduced ? "yes" : "no") << "</isReduced>" << endl;
		cout << "<numerator>";
		unsigned int iMax(iCyclotomicNumerator.size());
		for (unsigned int i(0); i < iMax; i++)
			cout << ( i ? "," : "" ) << iCyclotomicNumerator[i];
		cout << "</numerator>" << endl;
		cout << "<denominator>";
		Polynomials::polynomialDisplay(iPolynomialDenominator);
		cout << "</denominator>";
		cout << "</growthSeries>" << endl;
		
		// ---------------------------------------------------
		// Growth rate
		#ifdef _COMPILE_WITH_PARI_
		if (ci->get_isFiniteCovolume() == 1)
		{
			GrowthRate gr;
			grr = gr.grrComputations(ci->get_growthSeries_denominator());
			
			if(grr.isComputed && ci->get_isGrowthSeriesReduced())
			{
				cout << "<growthRate>" << endl;
				cout << "<value>" << grr.growthRate << "</value>" << endl;
				cout << "<perron>" << ( grr.perron < 0 ? "?" : ( grr.perron > 0 ? "yes" : "no" ) ) << "</perron>" << endl;
				cout << "<pisot>" << ( grr.pisot < 0 ? "?" : ( grr.pisot > 0 ? "yes" : "no" ) ) << "</pisot>" << endl;
				cout << "<salem>" << ( grr.salem < 0 ? "?" : ( grr.salem > 0 ? "yes" : "no" ) ) << "</salem>" << endl;
				cout << "</growthRate>" << endl;
			}
		}
		#endif
	}
	else
	{
		cout << "<error>" << (ci->get_error() == "" ? app.get_strError() : ci->get_error()) << "</error>" << endl;
		delete ci;
		return 0;
	}
	
	delete ci;

	return 0;
}
