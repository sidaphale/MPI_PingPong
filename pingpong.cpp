/*HPC-1, Fall-2016, Assignment-3, Siddhant S. Aphale, Person No. - 50164327
MPI Program to perform the Ping-Pong benchmark using blocking sends and receives.
Buffer Sizes range from 8 Bytes to 8 MBytes.*/

/*Code developed by referring Author: Wes Kendall, Copyright 2011 www.mpitutorial.com*/
/*Github Link : https://github.com/wesleykendall/mpitutorial/blob/gh-pages/tutorials/mpi-send-and-receive/code/ping_pong.c*/


#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <cassert>
#include <fstream>

#define PING_PONG_LIMIT 10;

int main(int argc, char** argv)
{

  // Initialize the MPI environment
  MPI_Init(&argc, &argv);
  // Find out rank, size
  int my_proc;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_proc);
  int nproc;
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);

  long int N = 10000000;

  // We are assuming at least 2 processes for this task
  if (nproc != 2) {
    std::cout <<  "Total number of processors must be two for\n";
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  double* ping_pong_count;
  double* t1;
  double* t2;
  //ping_pong_count = new double[N];
  std::ofstream output("run.dat");
  assert(output.is_open());
  /*double* t1;
  double* t2;

  t1 = new double[N];
  t2 = new double[N];*/
  for (int k=1; k<N; k=2*k)
    {
      ping_pong_count = new double [k] ;
      t1 = new double [k];
      t2 = new double [k];
      for (int i = 0; i < k; i++)
	{
	  
	  ping_pong_count[i] = 1;
	}
      
      double my_partner = (my_proc + 1) % 2;
      t1[k] = MPI_Wtime();
      for (int i = 0; i < k; i++)
	{
	  
	  
	  while (ping_pong_count[i] < 10)
	    {
	      if (double(my_proc) == int(ping_pong_count[i]) % 2)
		{
		  // Increment the ping pong count before you send it
		  ping_pong_count[i]++;
		  MPI_Send(&ping_pong_count[i], 1, MPI_DOUBLE, my_partner, 0, MPI_COMM_WORLD);
		  //	      printf("%d sent and incremented ping_pong_count %d to %d\n",
		  //     world_rank, ping_pong_count, partner_rank);
		}
	      else
		{
		  MPI_Recv(&ping_pong_count[i], 1, MPI_DOUBLE, my_partner, 0, MPI_COMM_WORLD,
			   MPI_STATUS_IGNORE);
		  //	      printf("%d received ping_pong_count %d from %d\n",
		  //     world_rank, ping_pong_count, partner_rank);
		}
	      
	    }
	  
	  
	}
      t2[k] = MPI_Wtime() - t1[k];

      if (my_proc == 0)
	{
	  output << t2[k] << "\t" << k*8 << std::endl;
	}
    }
  
  
  
  
  MPI_Finalize();
  
  delete[] ping_pong_count;
  delete[] t1;
  delete[] t2;
  
}    
