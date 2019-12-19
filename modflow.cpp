/*
//The continus memory array method is used as long as the visitwriter libray is used.
*/

#include "modflow.h"
modflow::modflow(void)
{
}
modflow::~modflow(void)
{
}

///===========================================================================================
///
///===========================================================================================
int modflow::visualize_model_3d
(
 std::vector<string> vFilename ,
 std::string sFilename_permafrost,
 std::string sWorkspace_out
 )
{
  int error_code  = 1;
  const int NX = ncolumn + 1 ;
  const int NY = nrow + 1 ;
  const int NZ = nlayer + 1 ;
  if ( NZ != vFilename.size())
    {
      error_code = 0;
      cout<<"Dimensional error"<<endl;
      return error_code;
    }
  vector <double> vPermafrost = data::read_binary_vector(sFilename_permafrost);
  //the first layer is the dem
  vector <double> vDem = data::read_binary_vector(vFilename[0]);
  //the following layers are thickness in theory
  //however in this special case, the second layer has constant thickness 100.0 meter
  vector <  vector <double>  > vvElevation ;
  //add dem as the defaul elevation first
  for (int iLayer =0;iLayer<= nlayer;  iLayer++)
    {
      vvElevation.push_back( vDem);
    }

  for (int iLayer =1;iLayer<= nlayer;iLayer++)
    {
      vector <double> vElevation;
      switch (iLayer)
        {
        case 1:
          {
            vector <double> vDummy = data::read_binary_vector(vFilename[iLayer]);
            for (int iRow =0;iRow<nrow;iRow++)
              {
                for(int j=0;j<ncolumn;j++)
                  {
                    if (vDem[iRow*ncolumn + j] == -9999.0 )
                      {
                        vElevation.push_back(-9999.0);
                      }
                    else
                      {
                        vElevation.push_back( (vvElevation[iLayer-1])[iRow*ncolumn + j]  - vDummy[iRow*ncolumn + j]);
                      }
                  }
              }
          }
          break;
          //the second layer
        case 2:
          {
            for (int iRow =0;iRow<nrow;iRow++)
              {
                for(int j=0;j<ncolumn;j++)
                  {
                    if (vDem[iRow*ncolumn + j] == -9999.0 )
                      {
                        vElevation.push_back(-9999.0);
                      }
                    else
                      {

                        vElevation.push_back( (vvElevation[iLayer-1])[iRow*ncolumn + j]  - 10.0);
                      }
                  }
              }

          }
          break;
        case 3:
          {
            vector <double> vDummy = data::read_binary_vector(vFilename[iLayer]);
            for (int iRow =0;iRow<nrow;iRow++)
              {
                for(int j=0;j<ncolumn;j++)
                  {
                    if (vDem[iRow*ncolumn + j] == -9999.0 )
                      {
                        vElevation.push_back(-9999.0);
                      }
                    else
                      {

                        vElevation.push_back( (vvElevation[iLayer-1])[iRow*ncolumn + j]  - vDummy[iRow*ncolumn + j] * 0.1 );
                      }
                  }
              }

          }
          break;
        }
      vvElevation[iLayer] = vElevation;
    }
  cout<<"coordiantes read finished"<<endl;
  //point coordinates
  float pts[NZ*NY*NX*3];
  for (int iLayer=0; iLayer< NZ; iLayer++)
    {
      std::vector<double> dummy = vvElevation[3-iLayer];
      for (int iRow=0; iRow< NY ; iRow++)
        {
          for(int iColumn=0; iColumn< NX; iColumn++)
            {
              long index= iLayer * NY * NX * 3 + iRow * NX * 3 + iColumn * 3 + 0;
              //                      cout<<index<<endl;
              pts[ index+ 0 ] =  iColumn * 50.0 ;
              pts[ index+ 1 ] =  iRow * 50.0 ;

              //transfer x and y coordinates

              pts[ index+ 2 ] = search_neighbor( nrow-iRow,iColumn, dummy);

            }
        }
    }
  cout<<"coordiantes assign finished"<<endl;
  int dims[] = {NX, NY, NZ};
  /* Zonal and nodal variable data. */
  float zonal[NZ-1][NY-1][NX-1], nodal[NZ][NY][NX];
  int       index = 0;
  for(int iLayer = 0; iLayer < NZ; ++iLayer)
    {
      for(int iRow = 0; iRow < NY; ++iRow)
        {

          for(int iColumn = 0; iColumn < NX; ++iColumn, ++index)
            {
              nodal[iLayer][iRow][iColumn] = index;
            }
        }
    }
  /* Info about the variables to pass to visit_writer. */
  int nvars = 2;
  int vardims[] = {1, 1};
  int centering[] = {0, 1};
  const char *varnames[] = {"zonal", "nodal"};
  float *vars[] = {(float *)zonal, (float *)nodal};
  for (int  iLayer=0; iLayer< nlayer; iLayer++)
    {
      for (int iRow=0; iRow< nrow; iRow++)
        {
          for(int iColumn=0; iColumn< ncolumn; iColumn++)
            {

              int ipidx =  vPermafrost[(nrow-1-iRow)*ncolumn + iColumn] ;

              if ( vPermafrost[(nrow-1-iRow)*ncolumn + iColumn] == 1  )
                {

                  switch (iLayer)
                    {
                    case  0:
                      {
                        zonal[iLayer][iRow][iColumn] = 5;
                      }
                      break;
                    case 1:
                      {
                        zonal[iLayer][iRow][iColumn] = 3;
                      }
                      break;
                    case 2:
                      {
                        zonal[iLayer][iRow][iColumn] = 1;
                      }
                      break;
                    }
                }
              else
                {
                  if(vPermafrost[(nrow-1-iRow)*ncolumn + iColumn] == -1)
                    {

                      switch (iLayer)
                        {
                        case  0:
                          {
                            zonal[iLayer][iRow][iColumn] = 6;
                          }
                          break;
                        case 1:
                          {
                            zonal[iLayer][iRow][iColumn] = 4;
                          }
                          break;
                        case 2:
                          {
                            zonal[iLayer][iRow][iColumn] = 2;
                          }
                          break;
                        }

                    }
                  else //nondata
                    {
                      zonal[iLayer][iRow][iColumn] = -1;
                    }
                }
            }
        }
    }
  cout<<"read data finished!"<<endl;
  /* Pass the data to visit_writer to write a binary VTK file. */


  string sFilenameOut = sWorkspace_out + slash + "model.vtk";
  write_curvilinear_mesh(sFilenameOut.c_str(), 1, dims, pts, nvars,
                         vardims, centering, varnames, vars);
  //write_regular_mesh(sFilenameOut.c_str(), 0, dims, nvars, vardims, centering, varnames1, vars);


  return error_code ;

}

double modflow::search_neighbor(int iRow, int iColumn, std::vector<double> vDem)
{

  double dElevation = 0.0;
  std::array<double,4> aNeighbor;
  if (iColumn != ncolumn  && iRow!= nrow && iColumn != 0 && iRow!= 0)  ///not on edges
    {
      dElevation = vDem[(iRow) * ncolumn + iColumn];
      if (dElevation == -9999.0)
        {
          dElevation = vDem[(iRow-1) * ncolumn + iColumn-1];
          if (dElevation == -9999.0)
            {
              dElevation = vDem[(iRow) * ncolumn + iColumn-1];
              if (dElevation == -9999.0)
                {
                  dElevation = vDem[(iRow-1) * ncolumn + iColumn];
                  if (dElevation == -9999.0)
                    {
                      dElevation = 0.0;
                    }
                }
            }
        }
    }
  else
    {
      if (iColumn == 0 && iRow !=0 && iRow!= nrow )
        {

          dElevation = 0.0;
        }

      else
        {
          if (iColumn == ncolumn  && iRow!=0 && iRow!= (nrow) )
            {

              dElevation = 0.0;

            }
          else
            {
              if(iRow == 0  && iColumn!=0 && iColumn!= ncolumn )
                {

                  dElevation = 0.0;
                }
              else
                {
                  if(iRow == (nrow)  && iColumn!=0 && iColumn!= ncolumn )
                    {

                      dElevation = 0.0;

                    }
                  else
                    {
                      ///corners
                      dElevation = 0.0;

                    }

                }


            }
        }

    }



  return dElevation;

}
double modflow::calculate_mean(std::array<double,4> aArray, double missing_value)
{
  double total=0.0;
  int count =0;
  for (int i=0;i<4;i++)
    {
      if( aArray[i] != missing_value )
        {
          total = total + aArray[i];
          count = count + 1;
        }

    }

  double mean = 0.0;
  if (count > 0)
    {
      mean=  total / count;
    }
  return mean;
}

int modflow::visualize_sfr_data(string sFilenameIn,string sWorkspaceOut)

{

  int i,j,k,l,r,s;

  string line=" ";

  int dims[] = {ncolumn+1, nrow+1, nlayer+1};

  reachinfo sreachinfo[nreach];

  ifstream fsFileIn(sFilenameIn.c_str());

  int nvars = 1;

  int vardims[] = {1};

  int centering[] = {0};

  int iFlag=1;

  string sFilenameOut;

  const char *varnames1[] = {"Stream"};

  if (fsFileIn.is_open() )

    {

      for (s = 0; s < nstress; s++)

        {

          float stream[nlayer][nrow][ncolumn];

          float *vars[] = { (float *) stream };

          //read the first 8 lines, which are header

          for (l = 0; l < 8; l++)  getline (fsFileIn,line);

          //read the stream data

          for (r = 0; r < nreach; r++)

            {

              getline (fsFileIn,line);

              sscanf (line.c_str(),"%i %i %i %i %i %f %f %f %f %f %f %f %f %f %f %f %f %f",

                      &sreachinfo[r].layer,&sreachinfo[r].row,&sreachinfo[r].col,

                      &sreachinfo[r].segment, &sreachinfo[r].reachnum,

                      &sreachinfo[r].flowin,&sreachinfo[r].streamloss,

                      &sreachinfo[r].flowout,&sreachinfo[r].runoff,

                      &sreachinfo[r].prec, &sreachinfo[r].et,

                      &sreachinfo[r].head,&sreachinfo[r].depth,

                      &sreachinfo[r].width,&sreachinfo[r].conductance,

                      &sreachinfo[r].leakage, &sreachinfo[r].storagechange,

                      &sreachinfo[r].gwhead);

              /* Create zonal variable */

              stream[2][ nrow + 1 - sreachinfo[r].row][sreachinfo[r].col-1]=sreachinfo[r].flowout;

            }

          stringstream ss;

          ss << setw(3) << setfill('0') << (s+1);

          sFilenameOut = sWorkspaceOut +  "\flowout" + ss.str() + ".vtk";

          write_regular_mesh(sFilenameOut.c_str(), 0, dims, nvars, vardims, centering, varnames1, vars);

        }

      fsFileIn.close();

    }

  else

    {

      iFlag=0;

    }

  return iFlag;



}



int modflow::visualize_block_data(string sFilenameIn, string sWorkspaceOut)
{
  int i,j, k, index, v;
  const int NX = ncolumn + 1 ;
  const int NY = nrow + 1 ;
  const int NZ = nlayer + 1 ;
  int iFlag=1;
  int iTimestep, iStress;
  char cBuffer[4], cDesc[16];
  float z[4]={50,45,30,0.0};
  string sVariable, sFilenameOut;
  ifstream fsFileIn(sFilenameIn.c_str(), ios::binary|ios::in );
  if(fsFileIn.is_open())
    {
      float pts[NZ*NY*NX*3];
      for ( k=0; k< NZ; k++)
        {
          for ( j=0; j< NY ; j++)
            {
              for( i=0; i< NX; i++)
                {
                  pts[ k * NY * NX * 3 + j * NX * 3 + i * 3 + 0 ] = i ;
                  pts[ k * NY * NX * 3 + j * NX * 3 + i * 3 + 1 ] = j ;
                  pts[ k * NY * NX * 3 + j * NX * 3 + i * 3 + 2 ] = z[k] ;
                }
            }
        }

      //read the steady-state simulation, only four variables saved
      int dims[] = {NX, NY, NZ};

      /* Zonal and nodal variable data. */

      float zonal[NZ-1][NY-1][NX-1], nodal[NZ][NY][NX];
      index = 0;

      for( k = 0; k < NZ; ++k)

        {

          for( j = 0; j < NY; ++j)

            {

              for( i = 0; i < NX; ++i, ++index)

                {

                  nodal[k][j][i] = index;

                }

            }

        }


      /* Info about the variables to pass to visit_writer. */

      int nvars = 2;

      int vardims[] = {1, 1};

      int centering[] = {0, 1};

      const char *varnames[] = {"zonal", "nodal"};

      float *vars[] = {(float *)zonal, (float *)nodal};


      for ( v=0 ; v<4; v++)
        {

          fsFileIn.read(cBuffer, 4);
          fsFileIn.read(cBuffer, 4);
          //read description
          fsFileIn.read(cDesc, 16);
          fsFileIn.read(cBuffer, 4);
          fsFileIn.read(cBuffer, 4);
          fsFileIn.read(cBuffer, 4);
          for ( k=0; k< nlayer; k++)
            {
              for ( j=0; j< nrow; j++)
                {
                  for( i=0; i< ncolumn; i++)
                    {
                      fsFileIn.read(cBuffer, 4);
                      zonal[k][j][i]= *((float *)cBuffer);
                    }
                }
            }

          /* Pass the data to visit_writer to write a binary VTK file. */

          sVariable= (string)cDesc;
          sVariable.erase( std::remove( sVariable.begin(), sVariable.end(),' ' ), sVariable.end()  );
          sFilenameOut = sWorkspaceOut + "SS_" + sVariable + ".vtk";
          write_curvilinear_mesh(sFilenameOut.c_str(), 1, dims, pts, nvars,
                                 vardims, centering, varnames, vars);

        }



      //read the transient simulation, five variables are saved

      for (int s = 1; s < nstress; s++)
        {
          for( v = 0; v < 5; v++)
            {
              fsFileIn.read(cBuffer, 4);
              fsFileIn.read(cBuffer, 4);
              fsFileIn.read(cDesc, 16);
              fsFileIn.read(cBuffer, 4);
              fsFileIn.read(cBuffer, 4);
              fsFileIn.read(cBuffer, 4);
              for ( k=0; k< nlayer; k++)
                {
                  for ( j=0; j< nrow; j++)
                    {
                      for( i=0; i< ncolumn; i++)
                        {
                          fsFileIn.read(cBuffer, 4);
                          zonal[k][j][i]= *((float *)cBuffer);
                        }
                    }
                }

              sVariable= (string)cDesc;
              sVariable.erase( std::remove( sVariable.begin(), sVariable.end(),' ' ), sVariable.end()  );
              stringstream ss;

              ss << setw(3) << setfill('0') << (s+1);


              sFilenameOut = sWorkspaceOut + "TR_" + sVariable + ss.str()  + ".vtk";
              write_curvilinear_mesh(sFilenameOut.c_str(), 1, dims, pts, nvars,
                                     vardims, centering, varnames, vars);
            }
        }

      fsFileIn.close();

    }

  else

    {

      //              cout<< "Failed to open the input file!";
      iFlag=0;

    }

  return iFlag;



}



int modflow::verify_outlet_discharge(string sFilenameIn,string sObservation,string sFilenameOut)

{

  int i,j,k,l,r,s;

  string line=" ";

  float discharge[nstress];

  float flowout[nstress];

  char *cAgent[10],*cSite[10],*cPara[10],*cNum[2];

  int iDummy[5];

  reachinfo sreachinfo[nreach];

  ifstream ifsFileIn(sFilenameIn.c_str());

  int iFlag=1;

  if (ifsFileIn.is_open() )

    {

      for ( s = 0; s < nstress; s++)

        {

          // Read the first 8 lines, which are header

          for ( l = 0; l < 8;l++)  getline (ifsFileIn,line);

          for ( r = 0; r < nreach; r++)

            {

              getline (ifsFileIn,line);

              sscanf(line.c_str(),"%i %i %i %i %i %f %f %f %f %f %f %f %f %f %f %f %f %f",

                     &sreachinfo[r].layer,&sreachinfo[r].row,&sreachinfo[r].col,

                     &sreachinfo[r].segment, &sreachinfo[r].reachnum,

                     &sreachinfo[r].flowin,&sreachinfo[r].streamloss,

                     &sreachinfo[r].flowout,&sreachinfo[r].runoff,

                     &sreachinfo[r].prec, &sreachinfo[r].et,

                     &sreachinfo[r].head,&sreachinfo[r].depth,

                     &sreachinfo[r].width,&sreachinfo[r].conductance,

                     &sreachinfo[r].leakage, &sreachinfo[r].storagechange,

                     &sreachinfo[r].gwhead);

            }

          flowout[s] = sreachinfo[nreach-1].flowout;

        }

      ifsFileIn.close();

    }

  else

    {

      iFlag= 0;

    }

  // Read observation stream discharge at the outlet

  ifstream ifsFileObs(sObservation.c_str());

  if( ifsFileObs.is_open() )

    {

      while( getline(ifsFileObs, line))

        {

          // Get the first char from the line

          if( line != "" &&'#' != line[0])

            {

              getline(ifsFileObs, line);

              // Read the data

              for (i=0;i<nstress;i++)

                {

                  getline(ifsFileObs, line);

                  sscanf(line.c_str(),"%s\t%s\t%s\t%s\t%i\t%i\t%i\t%i\t%i\t%i\t%f",

                         cAgent, cSite, cPara, cNum,

                         &iDummy[0], &iDummy[1], &iDummy[2], &iDummy[3], &iDummy[4], &iDummy[5],

                         &discharge[i]);

                }

              break;

            }

        }

      ifsFileObs.close();

    }

  else

    {

      iFlag= 0;

    }

  // Convert the unti from CFS (Cubic feet per second) to MODFLOW (Cubic meter per day)

  for (j=0 ;j < nstress; j++)

    discharge[j] = discharge[j] * pow(0.3048, 3);

  // Write VTK format curve datasets

  ofstream ofsFileOut(sFilenameOut.c_str() );

  if( ofsFileOut.is_open() )

    {

      ofsFileOut<<"#Simulation"<<endl;

      for (s=0; s<nstress; s++)

        {

          ofsFileOut<<(s+1)<<" "<<flowout[s]/86400.0<<endl;

        }

      ofsFileOut<<"#Observation"<<endl;

      for (s=0; s<nstress; s++)

        {

          ofsFileOut<<(s+1)<<" "<<discharge[s]<<endl;

        }

      ofsFileOut<<"#Scatter"<<endl;

      for (s=0; s<nstress; s++)

        {

          ofsFileOut<<discharge[s]<<" "<<flowout[s]/86400.0<<endl;

        }

      ofsFileOut.close();

    }

  else

    {

      iFlag=0;

    }

  return iFlag;



}
