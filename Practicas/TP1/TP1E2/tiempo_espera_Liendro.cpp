// includes necesarios y prototipos de funciones
#include <chrono>
#include <thread>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>

using duration = std::chrono::duration<double>;
using time_point = std::chrono::steady_clock::time_point;

// funcion de espera

duration espera_siguiente_intervalo(const time_point &t, int n, const duration &Dt){
   duration tiempo = n * Dt - (std::chrono::steady_clock::now() - t);
   if (tiempo.count() > 0)
         std::this_thread::sleep_for(tiempo);
   return tiempo;
}