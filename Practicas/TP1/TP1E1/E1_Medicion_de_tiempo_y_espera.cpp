/*Experimente con las funciones vistas en clase para medir intervalos de
tiempo y suspender la ejecución de un thread.*/

#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>

using duration = std::chrono::duration<double>;
using time_point = std::chrono::steady_clock::time_point;

int main(){
    int n = 1000; // N. iter
    time_point t_start;  // Starting time
    for (int i=0; i<n; ++i){
        t_start=std::chrono::steady_clock::now();  // Get current time
        double aux = log(sqrt(i) * pow(i, 2))/(i + 1 + pow(i, i));  // Some trash operations
        printf("Time to calculate the \"aux\" variable and to print this message is: ");
        }
    time_point t_end = std::chrono::steady_clock::now();  // Get current time
    duration time_span = std::chrono::duration_cast<duration>(t_end-t_start);  // Get thread duration
    std::cout << time_span.count() << "s" << std::endl;  // Print duration
    
    // If we want to stop a thread: 
    for (int i=0; i<10; ++i){  // same FOR loop ranging i from 0 to 9
        double aux = log(sqrt(i) * pow(i, 2))/(i + 1 + pow(i, i));  // Some trash operations
        std::this_thread::sleep_for(1);  // Suspend
        }
    return 0;
}


