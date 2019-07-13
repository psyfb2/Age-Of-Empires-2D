#pragma once

/*
Grow an array - has to be a basic C-type array of pointers, where the values can just be copied.
Example usage:
	if ( (m_iFontCount+1) > m_iArraySize )
		m_pArrayFonts = GrowArray( m_pArrayFonts, m_iArraySize, 10 );
This is used by the framework code where basic arrays are being used rather than classes.
Note that the size is passed by REFERENCE so will be changed.
A pointer to the new array is returned so will need to be stored.
The old array is deleted.

The main purpose of having this code here is so that you can see an example of a template function which is used.
This is currently only used in the FontManager.cpp file, but used to be used in more places until I rewrote a lot of the code.
*/
template <class T> 
T* growArrayOfObjects( T* pOldArray, int& iSize, int iGrowBy )
{
	T* pNewArray = new T[iSize+iGrowBy];
	if ( pOldArray != nullptr )
	{
		for ( int i = 0 ; i < iSize ; i++ )
		{
			pNewArray[i] = pOldArray[i];
		}
		delete[] pOldArray;
	}
	iSize += iGrowBy;
	return pNewArray;
}





