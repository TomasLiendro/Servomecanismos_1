// includes necesarios y prototipos de funciones
#include <chrono>
#include <thread>

#include <iostream>
#include <fstream>
#include <vector>

#include "funciones.h"

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
    std::cout << "Ingresar V0: ";
    float V0;
    std::cin >> V0;
    std::cout << "Ingresar w0: ";
    double w0;
    std::cin >> w0;
    std::cout << "Ingresar w1: ";
    double w1;
    std::cin >> w1;
    std::cout << "Ingresar C: ";
    double C;
    std::cin >> C;
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

    // Chirp parameters:
    std::vector<double> u(n);
    std::vector<double> w(n); // frecuency
    std::vector<double> V(n);
    double t2 = n * Dt;
    double tt0 = 0.10 * t2;
    double t1 = tt0 + 0.80 * t2;

    // bucle controlado temporalmente
    // primero establecemos el tiempo inicial t0
    t0 = std::chrono::steady_clock::now(); // Get current time
    // e inmediatamente comenzamos con las iteraciones
    for (int i = 0; i < n; ++i)
    {
        // medimos tiempo de inicio de cada iteracion
        time_point t_start = std::chrono::steady_clock::now(); // Get current time
        // calculamos t[i] restandole t0
        t[i] = std::chrono::duration_cast<duration>(t_start - t0);

        // se ejecutan las acciones de control
        // aca no hay I/O innecesario
        u[i] = chirp(i, w0, w1, t2, t1, tt0, V0, C, Ddt);
        // esperamos hasta que sea el tiempo del
        // siguiente intervalo guardando la espera esp[i]
        esp[i] = espera_siguiente_intervalo(t0, i + 1, Ddt);
    }
    
    // una vez terminado el experimento, se guardan los
    // valores a archivos para su procesamiento
    std::ofstream fout(fileName);

    for (int i = 0; i < n; i++)
      fout << t[i].count() << " " << u[i] << " " << esp[i].count() << endl;

    fout.close();

    return 0;
}
