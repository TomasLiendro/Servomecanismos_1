#include <iostream>
#include <chrono>
#include <vector>
#include <fstream>

#include "funciones.h"

int main()
{
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

   // parametros de funcion escalones aleatorios
   double V0;
   cout << "Ingrese voltaje V0 del escalon: ";
   cin >> V0;
   cout << "Ingrese minima duracion escalon (T1) como numero de intervalos dt: ";
   int nT1;
   cin >> nT1;
   cout << "Ingrese maxima duracion escalon (T2) como numero de intervalos dt: ";
   int nT2;
   cin >> nT2;

   time_point t0 = chrono::steady_clock::now(); //Comienza tiempo cuando inicia bucle.
   for (int i = 0; i < n; ++i)
   {
      // medimos tiempo de inicio de cada iteracion
      // calculamos t[i] restandole t0
      time_point ti = chrono::steady_clock::now();
      t[i] = chrono::duration_cast<duration>(ti - t0); // definicion de parametros del experimento como

      // se ejecutan las acciones de control
      // aca no hay I/O innecesario

      u[i] = escalones_random(V0, nT1, nT2, i);

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

   return 0;
}


