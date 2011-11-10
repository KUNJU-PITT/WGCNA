// The name of this file ends in h so R CMD install doesn't compile it twice. Not very clean but works
// for now.

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include <math.h>

#include <R.h>

#include "exceptions.h"

#ifndef __array_cc__

#define __array_cc__

// #define ISNAN(x) false
using namespace std;

// define a class that can conveniently hold a big vector, matrix etc.

#define NoDim	-1

#define CheckDimensions

string NumberToString(int n)
{
  char          s[100];
  string        ss;
  sprintf(s, "%d", n);
  ss = s;
  return ss;
} 

class indArray
{
  protected:

    int 	* data_;
    int 	size_;
    int 	allocated;
    string	name_;

    int	val32[2]; 
    int	mask[8*sizeof(int)];
    int	invMask[8*sizeof(int)];

  public:

#ifdef CheckDimensions

    bool value(int i)
      {
        int ii = (i/(8*sizeof(int)));
        if (ii >= size_)
          throw(Exception(string("indArray::value: index out of range in variable") + name()));
        int j = (i % (8*sizeof(int)));
        return ((data_[ii] & mask[j]) != 0);
      }

    void value(int i, bool v)
      {
        int ii = (i/(8*sizeof(int)));
        if (ii >= size_)
          throw(Exception(string("indArray::value: index out of range in variable") + name()));
        int j = (i % (8*sizeof(int)));
        //cout << "value: i: " << i << ", ii: " << ii << ", j: " << j << ", v:" << v << endl;
        if (v)
          data_[ii] |= mask[j];
        else
          data_[ii] &= invMask[j];
      }

#else

    bool value(int i)
      {
        int ii = (i/(8*sizeof(int)));
        int j = (i % (8*sizeof(int)));
        return (data_[ii] & mask[j] != 0);
      }

    void value(int i, bool v)
      {
        int ii = (i/(8*sizeof(int)));
        int j = (i % (8*sizeof(int)));
        if (v)
          data_[ii] |= mask[j];
        else
          data_[ii] &= invMask[j];
      }

#endif


    void name(string n) {name_ = n; }
    string name() {return name_; }

    void init(int size);
    void init(int size, bool value);
    int size() { return size_ * 8 * sizeof(int); }
    int * data() { return data_; }

    void show()
     {
       cout << "data_:";
       for (int i=0; i<size_; i++)
         cout << data_[i] << ", ";
       cout << endl;
       //cout << "mask";
       //for (int i=0; i<(8*sizeof(int)); i++)
         //cout << mask[i] << ", ";
       //cout << endl;
       //cout << "invMask";
       //for (int i=0; i<(8*sizeof(int)); i++)
         //cout << invMask[i] << ", ";
       //cout << endl;
     }

    indArray() { allocated = 0; data_ = (int *) NULL; }

    indArray(int size) { init(size); }

    indArray(int size, bool value) { init(size, value); }

    // indArray(indArray arr);      // This constructor will copy the data from arr into *this

    ~indArray() { if (allocated) { delete data_; allocated = 0; } }

};

void indArray::init(int size)
{
  unsigned val = 1;
  for (unsigned i=0; i<8*sizeof(int); i++)
  {
    mask[i] = val; 
    invMask[i] = ~mask[i];
    if (i < 8*sizeof(int)-1) val*=2;
  }
  val32[0] = 0;
  val32[1] = ~0;
  size_ = (int)(ceil((size*1.0)/(8*sizeof(int))));
  data_ = new int[size_];
  allocated = 1;
}


void indArray::init(int size, bool value)
{
  init(size);
  for (int i=0; i<size_; i++)
    data_[i] = val32[(int) value];
}

#define INT_CLASS iArray

#define TYPE int
#define CLASS_NAME iArray
#include "arrayGeneric.h"
#undef TYPE
#undef CLASS_NAME

#define TYPE double
#define CLASS_NAME dArray
#include "arrayGeneric.h"
#undef TYPE
#undef CLASS_NAME

double max(vector <double> v)
{
  if (v.size()==0)
    throw(Exception(string("attempt to calculate max of an empty vector.")));
  double max = v[0];
  for (unsigned i=1; i<v.size(); i++)
    if (!ISNAN(v[i]) && (v[i] > max)) max = v[i];
  return max;
}

int max(vector <int> v)
{
  if (v.size()==0)
    throw(Exception(string("attempt to calculate max of an empty vector.")));
  int max = v[0];
  for (unsigned i=1; i<v.size(); i++)
    if (v[i] > max) max = v[i];
  return max;
}


double min(vector <double> v)
{
  if (v.size()==0)
    throw(Exception(string("attempt to calculate min of an empty vector.")));
  double min = v[0];
  for (unsigned i=1; i<v.size(); i++)
    if (!ISNAN(v[i]) && (v[i] < min)) min = v[i];
  return min;
}

int min(vector <int> v)
{
  if (v.size()==0)
    throw(Exception(string("attempt to calculate min of an empty vector.")));
  int min = v[0];
  for (unsigned i=1; i<v.size(); i++)
    if (v[i] < min) min = v[i];
  return min;
}


#endif


