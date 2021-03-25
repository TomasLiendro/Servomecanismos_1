#include "funciones.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <fstream>

int main()
{
    libusb_device_handle *udev =placa_init();
    // parametros comunes de discretizacion temporal
    int n; // numero de muestras
    cout << "Ingrese numero de muestras: ";
    cin >> n;

    double aux_dt;
    cout << "Ingrese periodo de muestreo: ";
    cin >> aux_dt;
    duration dt(aux_dt);
    vector<duration> t(n);
    vector<duration> esp(n);
    vector<double> u(n);

    // salida de datos
    string name;
    cout << "Ingrese el nombre del archivo de salida: ";
    cin >> name;
    name += '.txt';

    uint16_t V0;
    cout << "Ingrese voltaje V0 de salida (mV): ";
    cin >> V0;

    int channel;
    cout << "Ingrese canal de salida: ";
    cin >> channel;
    cout << "Working on that... please wait.\n";


    time_point t0 = chrono::steady_clock::now(); //Comienza tiempo cuando inicia bucle.
    for (int i = 0; i < n; ++i)
    {
        // medimos tiempo de inicio de cada iteracion
        // calculamos t[i] restandole t0
        time_point ti = chrono::steady_clock::now();
        t[i] = chrono::duration_cast<duration>(ti - t0); // definicion de parametros del experimento como

        // se ejecutan las acciones de control
        // aca no hay I/O innecesario
        write(udev, channel, V0);
        // esperamos hasta que sea el tiempo del
        // siguiente intervalo
        esp[i] = espera_siguiente_intervalo(t0, i + 1, dt);
   }

    // una vez terminado el experimento, se guardan los
    // valores a archivos para su procesamiento
    ofstream fout(name);
    for (int i = 0; i < n; i++)
        fout << t[i].count() << " " << u[i] << " " << esp[i].count() << endl;

    fout.close();
    placa_end(udev);
   return 0;
}

