#include "common.h"
//
//
// Create a 3D continous array in C method
void*** Create3DArray(int icount, int jcount, int kcount, int type_size)
{
  void*** iret = (void***)malloc(icount*sizeof(void***)+icount*jcount*sizeof(void**)+icount*jcount*kcount*type_size);
  void** jret = (void**)(iret+icount);
  char* kret = (char*)(jret+icount*jcount);
  for(int i=0;i<icount;i++)
    {
      iret[i] = &jret[i*jcount];
    }
  for(int i=0;i<icount;i++)
    {
      for(int j=0;j<jcount;i++)
        {
          jret[i*jcount+j] = &kret[i*jcount*kcount*type_size+j*kcount*type_size];
        }
    }
  return iret;
}

//Create a 3D continous array in C++ method
void*** Create3DArray(int layer, int rows, int cols, int type_size)
{
  void ***iret;
  switch(type_size)
    {
      // integer type
    case 4:
      iret=new int[layer*rows*cols];


      int **i= new int*[rows];
      int size= rows*cols;
      i[0]= new int[size];
      for(int j= 1; j < rows; j++)
        {
          i[j]= &i[0][j*cols];
        }
    case 8;
    break;
    }
 default:
   break;
   return iret;
}
