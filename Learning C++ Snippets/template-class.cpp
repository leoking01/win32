#include "pch.h"
#include <iostream>

/*
 * declaracion plantilla de clase
*/
//template <typename T>		// identica definicion de plantilla de clase
template <class T>
class mypair {
	T values[2];

public:
	mypair(T first, T second) {
		values[0] = first;
		values[1] = second;
	}

	T getmax() {
		T retval;
		retval = values[0] > values[1] ? values[0] : values[1];
		return retval;
	}

	T getmin();
};

// The first T is the template parameter. 
// The second T refers to the type returned by the function. 
// And the third T (the one between angle brackets) specifies that this function's template parameter is also the class template parameter.
template <class T>
T mypair<T>::getmin() {
	return values[0] > values[1] ? values[1] : values[0];
}

int main()
{
	int n = 30;
	int m = 20;
	
	/*
	 * llamada
	*/
	mypair<int> mp(n, m);
	std::cout << "El mayor es: " << mp.getmax() << std::endl;
	std::cout << "El menor es: " << mp.getmin() << std::endl;

	return 0;
}

