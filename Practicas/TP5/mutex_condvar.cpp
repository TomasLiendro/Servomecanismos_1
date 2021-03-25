#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
// compilar con g++ -Wall -std=c++11 mutex_condvar.cpp -o mutex_condvar -lpthread -latomic

struct micro2raspi
{
    uint8_t head;
    uint16_t adc0;
    uint16_t adc1;
    uint16_t encoder;
    uint16_t i2c;
    uint8_t chksum;
};

void leer_puerto_serie();

// las variables globales son accesibles desde todos los threads
// running indica al thread si debe seguir trabajando
std::atomic<bool> running{true};

// datos ficticios que son generados por el thread y consumidos en el main
micro2raspi sensordata;
std::mutex sensordata_mtx; // para serializar el acceso a sensordata

// para que el thread pueda indicar al main que ya se tomó la primera medición
bool ready = false;
std::mutex ready_mtx;             // para serializar el acceso a ready
std::condition_variable ready_cv; // para que el main espere y el thread lo despierte

// funcion que se ejecutará en un thread seaparado
// es como si fuera el main del nuevo thread
void fun1()
{
    std::cout << "thread 1 corriendo" << std::endl;
    while (running)
    {
        leer_puerto_serie();
    }
    std::cout << "thread 1 terminando" << std::endl;
}

// main es siempre el punto de entrada del primer thread de un proceso
int main()
{
    // pongo a correr la fun1 dentro del thread1
    std::cout << "main lanzando thread1" << std::endl;
    std::thread thread1(fun1);
    //Ahora esperamos hasta que el thread avise que está ready
    {
        // lockeamos el mutex
        std::unique_lock<std::mutex> lck(ready_mtx);
        // ahora es seguro chequear si el thread ya empezó a escribir datos
        while (!ready)
            ready_cv.wait(lck); // sino, espero a que me despierte
    }
    // el main también hace sus cosas
    micro2raspi sd;
    for (int i = 0; i < 10; ++i)
    {

        {
            // la creación del unique_lock garantiza el lock del mutex
            std::unique_lock<std::mutex> guarda(sensordata_mtx);
            // ahora es seguro copiar el sensordata porque nadie puede estar modificandolo
            sd = sensordata;
            // al salir del scope, guarda se destruye, liberando el mutex
        }
        std::cout << "Medicion = " << i << std::endl;
        std::cout << "adc0 = " << sd.adc0 << std::endl;
        std::cout << "adc1 = " << sd.adc1 << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "encoder = " << sd.encoder << std::endl;
        std::cout << "i2c = " << sd.i2c << std::endl;
        std::cout << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // paramos el thread
    running = false;

    // los threads no terminan hasta que alguien les haga un join()
    thread1.join(); // el main se bloquea hasta que el thread1 termine
    // ahora que todos los threads terminaron puedo terminar el main
    return 0;
}

void leer_puerto_serie()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    static int kk = 0;
    micro2raspi sd;
    sd.head = 0xfe;
    sd.adc0 = 1000 + kk;
    sd.adc1 = 2000 + kk;
    sd.encoder = 3000 + kk;
    sd.i2c = 4000 + kk;
    kk++;

    {
        // la creación del unique_lock garantiza el lock del mutex
        std::unique_lock<std::mutex> guarda(sensordata_mtx);
        // ahora es seguro modificar sensordata porque nadi puede estar usandolo
        sensordata = sd;
        // al salir del scope, guarda se destruye liberando el mutex
    }

    // ahora tengo que avisarle al main que ya puede empezar a leer los datos
    {
        std::unique_lock<std::mutex> lck(ready_mtx);
        if (!ready)
        {
            ready = true;
            ready_cv.notify_all();
        }
    }
}