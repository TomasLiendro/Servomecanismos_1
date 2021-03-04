#include "funciones.h"
#include <thread>
#include <random>
#include <cmath>
// funcion de espera

duration espera_siguiente_intervalo(const time_point &t, int n, const duration &Dt)
{
    duration tiempo = n * Dt - (std::chrono::steady_clock::now() - t);
    if (tiempo.count() > 0)
        std::this_thread::sleep_for(tiempo);
    return tiempo;
}
double escalones_random(double V0, int T1, int T2, int i)
{
    static double Vout = 0;
    static int idx_cambio = 0;
    const unsigned seed = 123;
    static mt19937_64 rng(seed);
    static uniform_int_distribution<int> dist(T1, T2);

    if (i == idx_cambio)
    {
        Vout = Vout == 0 ? V0 : 0;
        idx_cambio += dist(rng);
    }
    return Vout;
}

double chirp(int i, double w0, double w1, double t2, double t1, double tt0, double V0, double C, const duration &Dt)
{
    static double w = 0;
    double V = 0;
    duration tiempo = i * Dt;
    w = w0 +  tiempo.count() / t2 * (w1 - w0);
    if ( tiempo.count() <= tt0)
        V = V0 *  tiempo.count() / tt0;
    else
    {
        if ( tiempo.count() <= t1)
            V = V0;
        else
            V = V0 * (t2 -  tiempo.count()) / (t2 - t1);
    }
    return V * sin(w * tiempo.count()) - C;
}