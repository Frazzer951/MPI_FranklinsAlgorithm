#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Request ireq_1;
    MPI_Status istatus_1;
    MPI_Request ireq_2;
    MPI_Status istatus_2;

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
            MPI_Isend(&send, 1, MPI_INT, left, 0, MPI_COMM_WORLD, &ireq_1);
            std::cout << rank << " is sending to " << right << std::endl;
            MPI_Isend(&send, 1, MPI_INT, right, 0, MPI_COMM_WORLD, &ireq_2);

            MPI_Irecv(&q, 1, MPI_INT, left, 0, MPI_COMM_WORLD, &ireq_1);
            std::cout << rank << " received from " << left << std::endl;
            MPI_Irecv(&r, 1, MPI_INT, right, 0, MPI_COMM_WORLD, &ireq_2);
            std::cout << rank << " received from " << right << std::endl;

            MPI_Wait(&ireq_1, &istatus_1);
            MPI_Wait(&ireq_2, &istatus_2);

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
            MPI_Irecv(&q, 1, MPI_INT, left, 0, MPI_COMM_WORLD, &ireq_1);
            MPI_Irecv(&r, 1, MPI_INT, right, 0, MPI_COMM_WORLD, &ireq_2);

            MPI_Wait(&ireq_1, &istatus_1);
            std::cout << rank << " received from " << left << " passive" << std::endl;
            MPI_Wait(&ireq_2, &istatus_2);
            std::cout << rank << " received from " << right << " passive" << std::endl;

            if (q[0] == -1 or r[0] == -1)
            {
                // leader found
                running = false;
            }

            MPI_Isend(&r, 1, MPI_INT, left, 0, MPI_COMM_WORLD, &ireq_1);
            std::cout << rank << " is relaying to " << left << std::endl;
            MPI_Isend(&q, 1, MPI_INT, right, 0, MPI_COMM_WORLD, &ireq_2);
            std::cout << rank << " is relaying to " << right << std::endl;
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
