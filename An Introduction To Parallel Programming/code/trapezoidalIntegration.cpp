//
// Created by hao2 on 2021/7/13.
//

/*An example of calculating integration of a function using trapezoidal integration.
 */


#include "mpi.h"
#include "iostream"
#include "iomanip"

using namespace std;


double function(double);
double calTrapezoidArea(double leftBound, double rightBound, int n);
void serialIntegration(double lowerBound, double upperBound, int partitionNumber);
void parallelIntegration(int comm_size, int my_rank, double lowerBound, double upperBound,
                         int partitionNumber, double data, double *p);


int main() {
    cout << setiosflags(ios::fixed); // keep N decimals, rather than N effective digits

    double lowerBound = 0, upperBound = 4;
    int partitionNumber = 2e9;

    double data;
    double *p = &data;

    int my_rank, comm_size;

    MPI_Init(nullptr, nullptr);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    MPI_Barrier(MPI_COMM_WORLD);
    double startTime = MPI_Wtime();

    if(comm_size == 1) {
        serialIntegration(lowerBound, upperBound, partitionNumber);
    } else {
        parallelIntegration(comm_size, my_rank, lowerBound, upperBound, partitionNumber, data, p);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double endTime = MPI_Wtime();
    MPI_Finalize();

    if(my_rank == 0) {
        cout << "Total parallel time = " << setprecision(4) << endTime-startTime << "s" << endl;
    }

    return 0;
}

// serial version of calculating trapezoidal integration
void serialIntegration(double lowerBound, double upperBound, int partitionNumber) {
    double integralResult = calTrapezoidArea(lowerBound, upperBound, partitionNumber);
    cout << "The result is: " << setprecision(4) << integralResult << endl;
}

// parallel version of calculating trapezoidal integration
void parallelIntegration(int comm_size, int my_rank, double lowerBound, double upperBound,
                         int partitionNumber, double data, double *p) {
    if (my_rank == 0) {
        // cout << "MPI initialize complete." << endl;
        double integralResult = 0;
        for (int rank = 1; rank < comm_size; rank++) {
            MPI_Recv(p, 1, MPI_DOUBLE, rank, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            integralResult += data;
        }
        // cout << "The result is: " << setprecision(4) << integralResult << endl;
    } else {
        double localLowerBound = lowerBound + (my_rank - 1) * (upperBound - lowerBound) / (comm_size - 1);
        double localUpperBound = lowerBound + my_rank * (upperBound - lowerBound) / (comm_size - 1);
        data = calTrapezoidArea(localLowerBound, localUpperBound, partitionNumber / (comm_size - 1));

        // cout << my_rank << " sent to 0" << endl;
        MPI_Send(p, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
}

// A random function to be integrated
double function(double x) {
    return x * x / 4 - x + 2;
}

// Calculate a single trapezoid's area
double calTrapezoidArea(double leftBound, double rightBound, int n) {
    double area = (function(leftBound) + function(rightBound)) / 2.0;
    double x = leftBound;
    double h = (rightBound - leftBound) / n;

    for (int i = 1; i < n; i++) {
        x += h;
        area += function(x);
    }

    area = area * h;
    return area;
}