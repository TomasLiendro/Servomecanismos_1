//Func generadora de señales.
#include <iostream>
#include "funciones.h"


int main()
{
    libusb_device_handle *udev = placa_init();
    
    int n;        //cantidad de intervalos
    double Ts;    //def valor de cada intervalo.
    double V0, w; //Parametros iniciales de la fun SENO
    time_point ti;
    string nameArchive;

    int ch;
    cout << "Ingrese canal de salida: ";
    cin >> ch;
    int opc;
    printf("ingrese la opcion deseada:\n");
    printf("Ingrese 'a' para funcion Senoidal.\n");
    printf("Ingrese 'b' para funcion Chirp.\n");
    printf("Ingrese 'c' para funcion Escalon Aleatorio.\n");
    while((opc = getchar()) == '\0' || opc == '\n');

    switch (opc)
    {
    case 'a': //Funcion Senoidal
        {
        printf("Genero señal senoidal\n");

        //defino parametros:

        cout << "\nIngrese el nombre del archivo de resultados: ";
        cin >> nameArchive;
        nameArchive += '.txt';
        cout << "\nIngrese el numero de iteraciones: ";
        cin >> n;
        cout << "\nIngrese la duracion del intervalo en segundos: ";
        cin >> Ts; //valor del intervalo en double.
        cout << "\nIngrese el valor V0: ";
        cin >> V0;
        cout << "\nIngrese la frecuencia W0: ";
        cin >> w;

        // uint16_t Vseno[n];
        // double Vectort_i[n];      //Vector que aloja el tiempo de inicio del bucle "for"
        // const duration dt(Ts);    //Convierto el valor del intervalo en variable Duration.
        // duration vectorEspera[n]; //Vector para alojar cada "tespera"(i).
        
        duration dt(Ts);
        vector<duration> t(n);
        vector<duration> esp(n);
        vector<double> u(n);

        //Bucle controlado
        time_point t0 = std::chrono::steady_clock::now(); //instante inicial
        for (int i = 0; i < n; ++i)
        {
            ti = chrono::steady_clock::now();
            t[i] = chrono::duration_cast<duration>(ti - t0); // definicion de parametros del experimento como

            //agrego fun de prueba. senoidal - chirp- etc
            u[i] = seno(i, V0, w, dt);
            write(udev, ch, u[i]);

            esp[i] = espera_siguiente_intervalo(t0, i + 1, dt);
        }
        ofstream fout(nameArchive);
        for (int i = 0; i < n; i++)
        fout << t[i].count() << " " << u[i] << " " << esp[i].count() << endl;
        fout.close();
        break;}

    case 'b': //Funcion Chirp
        {
        printf("Genero señal Chirp\n");
        //defino parametros:
                       //def valor de cada intervalo.
        double w0, w1, t2, t1, tt0, C; //Parametros iniciales de la fun Chirp

        cout << "\nIngrese el nombre del archivo de resultados: ";
        cin >> nameArchive;
        nameArchive += '.txt';
        cout << "\nIngrese el numero de iteraciones: ";
        cin >> n;
        cout << "\nIngrese la duracion del intervalo en segundos: ";
        cin >> Ts; //valor del intervalo en double.
        cout << "\nIngrese el valor V0: ";
        cin >> V0;
        cout << "\nIngrese la frecuencia W0: ";
        cin >> w0;
        cout << "\nIngrese la frecuencia W1: ";
        cin >> w1;
        // cout << "\nIngrese el valor de t2: ";
        // cin >> t2;
        // cout << "\nIngrese el valor de t1: ";
        // cin >> t1;
        // cout << "\nIngrese el valor de tt0: ";
        // cin >> tt0;
        cout << "\nIngrese el valor de la constante C: ";
        cin >> C;

        duration dt(Ts);
        vector<duration> t(n);
        vector<duration> esp(n);
        vector<double> u(n);

        //Bucle controlado
        time_point t0 = std::chrono::steady_clock::now(); //instante inicial
        for (int i = 0; i < n; ++i)
        {
            ti = chrono::steady_clock::now();
            t[i] = chrono::duration_cast<duration>(ti - t0); // definicion de parametros del experimento como

            //Llamo a la Chirp
            u[i] = chirp(n, i, w0, w1, V0, C, dt);

            //escribo el resultado de la funcion a travez de la placa.
            write(udev, ch, u[i]);

            esp[i] = espera_siguiente_intervalo(t0, i + 1, dt);
        }
        ofstream fout(nameArchive);
        for (int i = 0; i < n; i++)
        fout << t[i].count() << " " << u[i] << " " << esp[i].count() << endl;
        fout.close();
        break;
    }
    case 'c': //Funcion Escalones Aleatorios.
        {
        printf("Genero señal Escalon Aleatorio\n");
        //defino parametros:

        int T1, T2; //Parametros iniciales de la fun Escalon

        cout << "\nIngrese el nombre del archivo de resultados: ";
        nameArchive += '.txt';
        cout << "\nIngrese el numero de iteraciones: ";
        cin >> n;
        cout << "\nIngrese la duracion del intervalo en segundos: ";
        cin >> Ts; //valor del intervalo en double.
        cout << "\nIngrese el valor V0: ";
        cin >> V0;
        cout << "\nIngrese el valor T1 (entero): ";
        cin >> T1;
        cout << "\nIngrese el valor T2 (entero): ";
        cin >> T2;

        duration dt(Ts);
        vector<duration> t(n);
        vector<duration> esp(n);
        vector<double> u(n);

        //Bucle controlado
        time_point t0 = std::chrono::steady_clock::now(); //instante inicial
        for (int i = 0; i < n; ++i)
        {
            ti = chrono::steady_clock::now();
            t[i] = chrono::duration_cast<duration>(ti - t0); // definicion de parametros del experimento como

            //Llamo a la fun escalones_random.
            u[i] = escalones_random(V0, T1, T2, i);
            write(udev, ch, u[i]);

            esp[i] = espera_siguiente_intervalo(t0, i + 1, dt);
        }

        ofstream fout(nameArchive);
        for (int i = 0; i < n; i++)
        fout << t[i].count() << " " << u[i] << " " << esp[i].count() << endl;
        fout.close();
        break;}

    default: //En caso de escribir algo distinto de las opciones.
        printf(" Opcion invalida, Seleccione otra opcion (1 - 2 - 3)."); 
        exit(0);
        break;
    }


    placa_end(udev);

    return 0;
}