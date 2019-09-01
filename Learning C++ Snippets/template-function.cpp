#include "pch.h"
#include <iostream>

/*
 * declaracion plantilla de funcion
*/
//template <typename T>		// identica definicion de plantilla de funcion
template <class T>
T Comp(T a, T b) {
	return a > b ? a : b;
}


int main()
{
	int n = 10;
	int m = 20;
	
	/*
	 * llamada a plantilla de funcion 
	*/
	std::cout << "El mayor es: " << Comp<int>(n, m) << std::endl;
	std::cout << "El mayor es: " << Comp(n, m) << std::endl;

	return 0;
}
