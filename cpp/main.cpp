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

	return 0;
}
