#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    bool is_active = true;
    bool is_leader = false;
    bool running = true;

    int q[1] = {0};
    int r[1] = {0};
    int send[1] = {rank};

    int left = rank - 1;
    int right = (rank + 1) % size;

    if (rank == 0)
        left = size - 1;

    while (running)
    {
        if (is_active)
        {
            std::cout << rank << " is sending to " << left << std::endl;
            MPI_Send(&send, 1, MPI_INT, left, 0, MPI_COMM_WORLD);
            MPI_Recv(&r, 1, MPI_INT, right, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << rank << " received from " << right << std::endl;

            std::cout << rank << " is sending to " << right << std::endl;
            MPI_Send(&send, 1, MPI_INT, right, 0, MPI_COMM_WORLD);
            MPI_Recv(&q, 1, MPI_INT, left, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << rank << " received from " << left << std::endl;

            int max_v = (q[0] < r[0]) ? r[0] : q[0];
            if (max_v > rank)
            {
                std::cout << rank << " is now passive" << std::endl;
                is_active = false;
            }
            else if (max_v == rank)
            {
                std::cout << rank << " is now the leader" << std::endl;
                is_leader = true;
                running = false;
            }
            else
            {
                std::cout << rank << " continues to next round" << std::endl;
            }
        }
        else
        {
            MPI_Recv(&q, 1, MPI_INT, left, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << rank << " received from " << left << " passive" << std::endl;
            std::cout << rank << " is relaying to " << right << std::endl;
            MPI_Send(&q, 1, MPI_INT, right, 0, MPI_COMM_WORLD);

            MPI_Recv(&r, 1, MPI_INT, right, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << rank << " received from " << right << " passive" << std::endl;
            std::cout << rank << " is relaying to " << left << std::endl;
            MPI_Send(&r, 1, MPI_INT, left, 0, MPI_COMM_WORLD);

            if (q[0] == -1 or r[0] == -1)
            {
                // leader found
                running = false;
            }
        }
    }

    if (is_leader)
    {
        std::cout << rank << " is sending leader message" << std::endl;
        // Send message indicating leader was found
        send[0] = -1;
        MPI_Send(&send, 1, MPI_INT, left, 0, MPI_COMM_WORLD);
        MPI_Send(&send, 1, MPI_INT, right, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
