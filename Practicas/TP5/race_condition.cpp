#include <iostream>
#include <thread>
// compilar con g++ -Wall -std=c++11 race_condition.cpp -o race_condition -lpthread

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
bool running = true;
// datos ficticios que son generados por el thread y consumidos en el main
micro2raspi sensordata;

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
        std::cout << "Medicion = " << i << std::endl;
        std::cout << "adc0 = " << sensordata.adc0 << std::endl;
        std::cout << "adc1 = " << sensordata.adc1 << std::endl;
        std::cout << "encoder = " << sensordata.encoder << std::endl;
        std::cout << "i2c = " << sensordata.i2c << std::endl;
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
   sensordata.head=0xfe;
   sensordata.adc0 = 1000 + kk;
   sensordata.adc1 = 2000 + kk;
   sensordata.encoder = 3000 + kk;
   sensordata.i2c = 4000 + kk;
   kk++;
}