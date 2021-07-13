//
// Created by hao2 on 2021/7/13.
//

/*A "hello world" program for testing MPI.
 */

#include "cstdio"
#include "cstring"
#include "mpi.h"

const int MAX_STRING = 100;

int main() {
    // test
    char greeting[MAX_STRING];
    int comm_size, my_rank;

    MPI_Init(nullptr, nullptr);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int mainRank = 3;

    if (my_rank != mainRank) {
        sprintf(greeting, "Greetings from process %d of %d.", my_rank, comm_size);
        MPI_Send(greeting, strlen(greeting) + 1, MPI_CHAR, mainRank, 0, MPI_COMM_WORLD);
    } else {
        printf("This is process %d\n", my_rank);

        // printf("Greetings from process %d of %d\n", my_rank, comm_size);
        for (int rank = 0; rank < comm_size; rank++) {
            if (rank != mainRank) {
                MPI_Recv(greeting, MAX_STRING, MPI_CHAR, rank, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                printf("%s\n", greeting);
            }
        }
    }

    MPI_Finalize();

    return 0;
}
