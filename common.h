void*** newarray(int icount, int jcount, int kcount, int type_size)
{
    void*** iret = (void***)malloc(icount*sizeof(void***)+icount*jcount*sizeof(void**)+icount*jcount*kcount*type_size);
    void** jret = (void**)(iret+icount);
    char* kret = (char*)(jret+icount*jcount);
    for(int i=0;i<icount;i++)
        iret[i] = &jret[i*jcount];
    for(int i=0;i<icount;i++)
        for(int j=0;j<jcount;i++)
            jret[i*jcount+j] = &kret[i*jcount*kcount*type_size+j*kcount*type_size];
    return iret;
}



cout << "Ints have size " << sizeof(int) << endl;
 
int rows= 2;
int cols= 3;
 
int **i= new int*[rows];
int size= rows*cols;
i[0]= new int[size];
for(int j= 1; j < rows; j++) {
  i[j]= &i[0][j*cols];
}
 
for(int j= 0; j < rows; j++) {
  for(int k= 0; k < cols; k++) {
    cout << j << " " << k << " " << &i[j][k] << endl;
  }
}
delete[] i;
