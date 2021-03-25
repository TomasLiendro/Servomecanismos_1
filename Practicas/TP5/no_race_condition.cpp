#include <iostream>
#include <thread>
#include <atomic>
// compilar con g++ -Wall -std=c++11 no_race_condition.cpp -o no_race_condition -lpthread -latomic

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
std::atomic<micro2raspi> sensordata;

// funcion que se ejecutará en un thread seaparado
// es como si fuera el main del nuevo thread
void fun1()
{
    std::cout << "thread 1 corriendo" << std::endl;
    while(running)
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

    // el main también hace sus cosas
    for (int i = 0; i < 10; ++i)
    {
        micro2raspi sd = sensordata; // esta operacion es atomica (nadie mas puede acceder a sensordata hasta que termine)
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
    running=false;

    // los threads no terminan hasta que alguien les haga un join()
    thread1.join(); // el main se bloquea hasta que el thread1 termine
    // ahora que todos los threads terminaron puedo terminar el main
    return 0;
}


void leer_puerto_serie()
{
   std::this_thread::sleep_for(std::chrono::milliseconds(10));
   static int kk=0;
   micro2raspi sd;
   sd.head=0xfe;
   sd.adc0 = 1000 + kk;
   sd.adc1 = 2000 + kk;
   sd.encoder = 3000 + kk;
   sd.i2c = 4000 + kk;
   kk++;
   sensordata = sd; // esta operacion es atomica (nadie mas puede acceder a sensordata hasta que termine)
}