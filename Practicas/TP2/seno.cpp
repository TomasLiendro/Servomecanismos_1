// includes necesarios y prototipos de funciones
#include <chrono>
#include <thread>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>

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
    std::cout << "Ingresar w: ";
    double w;
    std::cin >> w;
    std::cout << "Ingresar V0: ";
    double V0;
    std::cin >> V0;
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
    std::vector<double> u(n);

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

        u[i] = V0 * sin(w * t[i].count()) + V0;

        // esperamos hasta que sea el tiempo del
        // siguiente intervalo guardando la espera esp[i]
        esp[i] = espera_siguiente_intervalo(t0, i + 1, Ddt);
    }

    // una vez terminado el experimento, se guardan los
    // valores a archivos para su procesamiento
    std::ofstream fout(fileName);

    for (int i = 0; i < n; ++i)
        fout << t[i].count() << "\t" << u[i] << std::endl;
    fout.close();
    return 0;
}

// funcion de espera

duration espera_siguiente_intervalo(const time_point &t, int n, const duration &Dt)
{
    duration tiempo = n * Dt - (std::chrono::steady_clock::now() - t);
    if (tiempo.count() > 0)
        std::this_thread::sleep_for(tiempo);
    return tiempo;
}