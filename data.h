#pragma once


//========================================================
//only include what are needed!
//C header

//C++ header

#include <fstream>
#include <string>
#include <vector>
//========================================================
const long Ncol = 388;
const long Nrow = 310;
const double Cellsize = 500.0;
using namespace std;

class data
{
 public:
  data(void);
  ~data(void);
  //Traditional data IO
  static float * read_binary(string sFilename);
  static float ** read_binary(string sFilename,  long lCol, long lRow);
  static vector<double> read_binary_vector(string sFilename);
  //dataIO using PETSc
  //Mat Read_Binary(string filErtame,int m,int n);

  static int write_binary_vector(std::string sFilename_out, vector <double> vData_out);


};



