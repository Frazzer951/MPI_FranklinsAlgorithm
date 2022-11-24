#include <iostream>
#include <mpi.h>

int genIdentifier(int rank, int size)
{
    int *nums;
    nums = new int[size];

    for (int i = 0; i < size; i++)
    {
        nums[i] = rand();
    }

    return nums[rank];
}

int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int id = genIdentifier(rank, size); // get random id
    bool is_active = true;

    int q, r;

    int left = rank - 1;
    int right = (rank + 1) % size;

    if (rank == 0)
        left = size - 1;

    while (true)
    {
        if (is_active)
        {
            // std::cout << rank << " is sending to " << right << std::endl;
            MPI_Send(&id, 1, MPI_INT, right, 0, MPI_COMM_WORLD);
            MPI_Recv(&q, 1, MPI_INT, left, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            // std::cout << rank << " received from " << left << std::endl;

            // std::cout << rank << " is sending to " << left << std::endl;
            MPI_Send(&id, 1, MPI_INT, left, 0, MPI_COMM_WORLD);
            MPI_Recv(&r, 1, MPI_INT, right, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            // std::cout << rank << " received from " << right << std::endl;

            int max_v = std::max(q, r);
            if (max_v > id)
            {
                std::cout << rank << " is now passive with id: " << id << std::endl;
                is_active = false;
            }
            else if (max_v == id)
            {
                std::cout << rank << " is now the leader with id: " << id << std::endl;
                break;
            }
            else
            {
                std::cout << rank << " continues to next round" << std::endl;
            }
        }
        else // passive
        {
            MPI_Recv(&q, 1, MPI_INT, left, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            // std::cout << rank << " is relaying to " << right << std::endl;
            MPI_Send(&q, 1, MPI_INT, right, 0, MPI_COMM_WORLD);

            MPI_Recv(&r, 1, MPI_INT, right, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            // std::cout << rank << " is relaying to " << left << std::endl;
            MPI_Send(&r, 1, MPI_INT, left, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Abort(MPI_COMM_WORLD, MPI_SUCCESS); // Kill rest of processes since leader was found
    MPI_Finalize();
    return 0;
}
