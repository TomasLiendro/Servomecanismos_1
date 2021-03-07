#ifndef FUNCIONES_H
#define FUNCIONES_H
#include <chrono>
#include <cmath>
using duration = std::chrono::duration<double>;
using time_point = std::chrono::steady_clock::time_point;
using namespace std;

duration espera_siguiente_intervalo(const time_point &t0, int n, const duration &dt);
double escalones_random(double V0, int T1, int T2, int i);
double chirp(int i, double w0, double w1, double t2, double t1, double tt0, double V0, double C, const duration &Dt);
#endif