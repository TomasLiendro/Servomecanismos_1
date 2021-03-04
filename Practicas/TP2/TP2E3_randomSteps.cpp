// includes necesarios y prototipos de funciones
#include <chrono>
#include <thread>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <random>

using duration = std::chrono::duration<double>;
using time_point = std::chrono::steady_clock::time_point;

duration espera_siguiente_intervalo(const time_point &t, int n, const duration &Dt);

int main()
{
    // definicion de parametros del experimento como
    // numero de intervalos, tiempo de muestreo, etc
    int n;
    float Dt;
    std::string fileName;
    std::cout << "Ingresar cantidad de muestras n: ";
    std::cin >> n;
    std::cout << "Ingresar Dt: ";
    std::cin >> Dt;
    std::cout << "Ingresar nombre del archivo: ";
    std::cin >> fileName;
    fileName += ".txt";
    duration Ddt = std::chrono::duration<double>(Dt);

    // inicializaciones
    time_point t0;
    duration wait;
    std::vector<duration> time_span;
    std::vector<duration> t;
    std::vector<duration> esp;
    time_span.resize(n);
    t.resize(n);
    esp.resize(n);
    // Parametros de la senal
    const int T1 = 1;
    const int T2 = 10;
    unsigned seed = 456;
    std::mt19937_64 rng(seed);
    std::uniform_int_distribution<int> dist(T1, T2);
    std::vector<double> f(n);
    double T;
    float V0 = 1;
    time_point e_time; // elapsed time from t0 until new reference
    duration e_t;      // elapsed duration from t0 until new reference
    // bucle controlado temporalmente
    // primero establecemos el tiempo inicial t0
    t0 = std::chrono::steady_clock::now(); // Get current time
    e_t = std::chrono::duration_cast<duration>(t0 - t0);
    // e inmediatamente comenzamos con las iteraciones
    for (int i = 0; i < n; ++i)
    {
        // medimos tiempo de inicio de cada iteracion
        time_point t_start = std::chrono::steady_clock::now(); // Get current time
        // calculamos t[i] restandole t0
        t[i] = std::chrono::duration_cast<duration>(t_start - t0);
        // se ejecutan las acciones de control
        // aca no hay I/O innecesario

        // esperamos hasta que sea el tiempo del
        // siguiente intervalo guardando la espera esp[i]
        esp[i] = espera_siguiente_intervalo(t0, i + 1, Ddt);
    }

    // una vez terminado el experimento, se guardan los
    // valores a archivos para su procesamiento
    std::ofstream fout(fileName);

    for (int i = 0; i < n; ++i)
        fout << t[i].count() << "\t" << f[i] << "\t" << esp[i].count() << std::endl;
    fout.close();
    return 0;
}
