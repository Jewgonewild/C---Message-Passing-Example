#include "mpi.h"
#include <stdio.h>
#include <string>

int main (int argc, char *argv[])
{

	//Vars to store num processes and rank of each. Assigned by a mster process.
	int processes, rankNumber;
	
	//Prepares the messsage passing interface communciation between all nodes.
	MPI_Init(&argc, &argv);

	//Allocate the number of processes to the app.
	MPI_Comm_size(MPI_COMM_WORLD, &processes);
	
	//Initialize each node with a rank number
	MPI_Comm_rank(MPI_COMM_WORLD, &rankNumber);
	
	MPI_mpiStatus mpiStatus;
	char greeting[200];

	int matrix[processes-1][processes-1];
	int result[processes-1][processes-1];
 	int threeD[processes-1][processes-1][processes-1];
	int temp  = 0;	
	
	if (rankNumber == 0)
	{

		printf("Preloading the matrix.\n");

		for (int i=0; i < processes-1; i++)
		{
			for (int j=0; j < processes-1; j++)
			{
				matrix[i][j] = 0;
			}
		}

		printf("Contents of Matrix.\n");
		for (int i=0; i < processes-1; i++)
		{
			for (int j=0; j < processes-1; j++)
			{
				printf("%i\t", matrix[i][j]);
			}
			printf("\n");
		}

		printf("Passing the data structure around now.\n");
		
		/*Passes messages between nodes. The node can receive the message 
		or it can be ignored alltogether.*/
		for (int k=1; k < processes; k++)
		{
			MPI_Send(matrix, (processes-1)*(processes-1), 
					MPI_INT, k, 1, 
					MPI_COMM_WORLD);
		}

		printf("Waiting for answer from nodes.\n");
		for (int k=1; k < processes; k++) 
		{
			/*This is one way in which the processor will receive data.By default the
			execution will be synchronous. If you want asynchronous calls just specify a non blocking parameter.
			*/

			MPI_Recv(matrix, (processes-1)*(processes-1),MPI_INT, k, 1,MPI_COMM_WORLD, &mpiStatus);
			for (int i=0; i < processes-1; i++)
			{
				for (int j=0; j < processes-1; j++)
				{
					threeD[k-1][i][j] = matrix[i][j];
				}
			}
		}

		printf("Results:\n");
		for (int k=0; k < processes-1; k++)
		{
			printf("Level %i:\n", k);
			for (int i=0; i < processes-1; i++)
			{
				for (int j=0; j < processes-1; j++)
				{
					printf("%i\t", threeD[k][i][j]);
				}
				printf("\n");
			}
		}
	
		for (int i=0; i < processes-1; i++)
		{
			for (int j=0; j < processes-1; j++)
			{
				printf("%i\t", result[i][j]);
			}
			printf("\n");
		}
	}
	else
	{
		MPI_Recv(matrix, (processes-1)*(processes-1), MPI_INT, 0, 1,MPI_COMM_WORLD, &mpiStatus);
		sprintf(greeting, "Got the matrix");
		for (int j=0; j < processes-1; j++)
		{
			matrix[rankNumber-1][j] = rankNumber*j;
		}
		MPI_Send(matrix, (processes-1)*(processes-1), MPI_INT, 0, 1,MPI_COMM_WORLD);		
	}

	//closes all MPI connections.
	MPI_Finalize();
	return 0;
}