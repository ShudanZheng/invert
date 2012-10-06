#ifndef CONFIG_H_
#define CONFIG_H_

//include std
#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <fstream>
#include <cassert>
#include <windows.h>
#include <io.h>
#include <direct.h>
#include <omp.h>
#include <cstdio>
#include <iomanip>
#include <time.h>
#include <cstdlib>
#include <stdio.h>
#include <math.h>
#include <set>
#include <algorithm>

#pragma warning(disable:4996)
using namespace std; 

#define DELETE_OBJECT(x)	if ((x)) { delete (x); (x) = NULL; }
#define DELETE_ARRAY(x)		if ((x)) { delete [] (x); (x) = NULL; }
#define DELETE_OBJECT_ARRAY(n,x)		if ((x)) { for (int i = 0; i < n; i++) { DELETE_OBJECT(((x)[i])); } delete [] (x); (x) = NULL; }
#define FOR(i,n) for( int i=0; i<n; i++ )

typedef unsigned char		uchar;
typedef unsigned short		ushort;
typedef unsigned int		uint;
typedef unsigned long		ulong;
typedef vector<int>			vectori;
typedef vector<float>		vectorf;
typedef vector<double>		vectord;
typedef vector<uchar>		vectoru;
typedef vector<ushort>		vectorus;
typedef vector<string>		vectorString;

// basic utility functions
template<class T> int		sign( T s ) { return (s > 0 ) ? 1 : ((s<0) ? -1 : 0); }
template<class T> T			squeeze( T v, T vMin, T vMax ) { return v<vMin ? vMin : v>vMax ? vMax : v; }
inline double factorial(const int i) { double d=1.0; for(int k=2; k<=i; k++) d*=k; return d; }
inline int					roundInt(double v) {return int((v<0) ? (v-0.5): (v+0.5)); }
inline string				int2str( int i, int nDigit )
{
	char f[32], t[64]; sprintf(f,"%%0%ii",nDigit); sprintf(t,f,i); return string(t);
}

// vector arithmetic operations and display
template<class T> T         vecSum( const vector<T> &v )
{
	T sum = (T) 0.0; for( size_t i=0; i<v.size(); i++ ) sum+=v[i]; return sum;
}

template<class T> vector<T> vecAdd( const vector<T> &v1, const vector<T> &v2 )
{
	vector<T> r(v1); for( size_t i=0; i<v1.size(); i++ ) r[i]+=v2[i]; return r;
}

template<class T> vector<T> vecSub( const vector<T> &v1, const vector<T> &v2 )
{
	vector<T> r(v1); for( size_t i=0; i<v1.size(); i++ ) r[i]-=v2[i]; return r;
}

template<class T> vector<T> vecMul( const vector<T> &v1, const vector<T> &v2 )
{
	vector<T> r(v1); for( size_t i=0; i<v1.size(); i++ ) r[i]*=v2[i]; return r;
}

template<class T> vector<T> vecDiv( const vector<T> &v1, const vector<T> &v2 )
{
	vector<T> r(v1); for( size_t i=0; i<v1.size(); i++ ) r[i]/=v2[i]; return r;
}

template<class T> vector<T> vecAdd( const vector<T> &v, T val )
{
	vector<T> r(v); for( size_t i=0; i<v.size(); i++ )  r[i]+=val; return r;
}

template<class T> vector<T> vecSub( const vector<T> &v, T val )
{
	vector<T> r(v); for( size_t i=0; i<v.size(); i++ )  r[i]-=val; return r;
}

template<class T> vector<T> vecMul( const vector<T> &v, T val )
{
	vector<T> r(v); for( size_t i=0; i<v.size(); i++ ) r[i]*=val; return r;
}

template<class T> vector<T> vecDiv( const vector<T> &v, T val )
{
	vector<T> r(v); for( size_t i=0; i<v.size(); i++ ) r[i]/=val; return r;
}

template<class T> vector<T> vecSub( T val, const vector<T> &v )
{
	vector<T> r(v.size(),val); for( size_t i=0; i<v.size(); i++ ) r[i]-=v[i]; return r;
}

template<class T> ostream&	operator<<(ostream& os, const vector<T>& v)
{
	os << "[ " ; for (size_t i=0; i<v.size(); i++) os << v[i] << " "; os << "]"; return os;
}

template<class T> T			vecMin( const vector<T> &v )
{
	return *min_element(v.begin(),v.end());
}

template<class T> T			vecMax( const vector<T> &v )
{
	return *max_element(v.begin(),v.end());
}

template<class T> uint		minIdx( const vector<T> &v )
{
	return uint( min_element(v.begin(),v.end()) - v.begin() );
}
template<class T> uint		maxIdx( const vector<T> &v )
{
	return uint( max_element(v.begin(),v.end()) - v.begin() );
}


// sort method that also gives order of elements after sorting
template<class T> class				SortableElement
{
public:
	T _val; int _ind;
	SortableElement() {};
	SortableElement( T val, int ind ) { _val=val; _ind=ind; }
	bool operator< ( const SortableElement &b ) const { return (_val < b._val ); };
};

template<class T> void				sortOrder( vector<T> &v, vectori &order )
{
	int n=v.size(); vector< SortableElement<T> > v2;
	v2.resize(n); order.clear(); order.resize(n);
	for( int i=0; i<n; i++ ) { v2[i]._ind=i; v2[i]._val=v[i]; }
	std::sort( v2.begin(), v2.end() );
	for( int i=0; i<n; i++ ) { order[i]=v2[i]._ind; v[i]=v2[i]._val; }
}

template<class T> void				reorder( vector<T> &v, vectori &order )
{
	assert( v.size()==order.size() ); vector<T> copy( v.size() );
	for( int i=0; i<(int)v.size(); i++ ) copy[i]=v[order[i]];
	v = copy;
}

template<class Ta, class Tb> void	sortVia_b( vector<Ta> &a, vector<Tb> &b )
{
	assert(a.size()==b.size()); vectori order; sortOrder<Tb>(b,order); reorder<Ta>(a,order);
}


#endif //CONFIG_H

