#include <iostream>
#include <thread>
// compilar con g++ -Wall -std=c++11 simple_thread.cpp -o simple_thread -lpthread

// funcion que se ejecutará en un thread seaparado
// es como si fuera el main del nuevo thread
void fun1()
{
    std::cout << "thread 1 corriendo" << std::endl;
    for(int i=0; i< 10;++i)
    {
        std::cout << "1" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    std::cout << "thread 1 terminando" << std::endl;
}

// otra función que se ejecutara en un thread separado
// es como si fuera el main del nuevo thread
void fun2()
{
    std::cout << "thread 2 corriendo" << std::endl;
    for(int i=0; i< 10;++i)
    {
        std::cout << "2" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    std::cout << "thread 2 terminando" << std::endl;
}

// main es siempre el punto de entrada del primer thread de un proceso
int main()
{
    // pongo a correr la fun1 dentro del thread1
    std::cout << "main lanzando thread1" << std::endl;
    std::thread thread1(fun1);
    // pongo a correr la fun2 dentro del thread2
    std::cout << "main lanzando thread2" << std::endl;
    std::thread thread2(fun2);

    // el main también hace sus cosas
    for(int i=0; i< 10;++i)
    {
        std::cout << "0" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
 
    // los threads no terminan hasta que alguien les haga un join()
    thread1.join(); // el main se bloquea hasta que el thread1 termine
    thread2.join(); // el main se bloquea hasta que el thread2 termine
    // ahora que todos los threads terminaron puedo terminar el main
    return 0;
}