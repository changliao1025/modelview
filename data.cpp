#include "data.h"
data::data(void)
{
}
data::~data(void)
{
}
float * data::read_binary(string sFilename)
{
  float dummy;
  ifstream ifs;
  ifs.open (sFilename.c_str(), ios::in | ios::binary);
  ifs.seekg(0,ios::end);
  long lLength1=ifs.tellg();
  ifs.seekg(0,ios::beg);
  long lLength2 = lLength1 /sizeof(float);
  float * pdata = new float[lLength2];
  for (long i=0;i< lLength2;++i)
    {
      ifs.read( reinterpret_cast<char*>( &dummy ), sizeof dummy );
      pdata[i] = dummy;
    }
  return pdata;
}
float ** data::read_binary(string sFilename,long lCol,long lRow)
{
  float dummy;
  ifstream ifs;
  ifs.open(sFilename.c_str(), ios::in | ios::binary);
  float * pdata_dummy = new float[lCol*lRow];
  for (long i=0; i< lCol*lRow - 1;++i)
    {
      ifs.read( reinterpret_cast<char*>( &dummy ), sizeof dummy );
      pdata_dummy[i]=dummy;
    }
  ifs.close();
  float ** pdata=new float *[lCol];
  for (int i=0;i < lCol;++i )
    {
      pdata[i]=pdata_dummy + lRow * i ;
    }
  return pdata;
}
vector<double> data::read_binary_vector(string sFilename)
{
  float dummy;
  ifstream ifs;
  ifs.open(sFilename.c_str(), ios::in | ios::binary);
  ifs.seekg(0,ios::end);
  long lLength1=ifs.tellg();
  ifs.seekg(0,ios::beg);
  long lLength2 = lLength1 /sizeof(float);
  vector<double> vdata;
  for (long i=0;i< lLength2;++i)
    {
      ifs.read( reinterpret_cast<char*>( &dummy ), sizeof dummy );
      vdata.push_back(dummy);
    }
  return vdata;
}
int data::write_binary_vector( std::string sFilename_out, std::vector <double> vData_out)
{
  int error_code = 1 ;
  std::vector<float> vData( vData_out.begin(), vData_out.end() );
  ofstream ofs(sFilename_out.c_str(), ios::out | ios::binary);
  if( ofs.is_open() )
    {
      ofs.write(reinterpret_cast<char*>(&vData[0]), vData.size()* sizeof(float)); 
      ofs.close();
    }
  else
    {
      error_code = 0 ;
    }
  return error_code;
}
