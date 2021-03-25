#include "funciones.h"
#include <thread>
#include <random>
#include <cmath>
#include <signal.h>
#include <csignal>
#include <iostream>
// funcion de espera
duration espera_siguiente_intervalo(const time_point &t, int n, const duration &Dt){
    duration tiempo = n * Dt - (std::chrono::steady_clock::now() - t);
    if (tiempo.count() > 0)
        std::this_thread::sleep_for(tiempo);
    return tiempo;
}
double escalones_random(double V0, int T1, int T2, int i){
    static double Vout = 0;
    static int idx_cambio = 0;
    const unsigned seed = 123;
    static mt19937_64 rng(seed);
    static uniform_int_distribution<int> dist(T1, T2);

    if (i == idx_cambio)
    {
        Vout = Vout == 0 ? V0 : 0;
        idx_cambio += dist(rng);
    }
    return Vout;
}

double chirp(int n, int i, double w0, double w1, double V0, double C, const duration &Dt){
    static double w = 0;
    double V = 0;
    double tt0, t1;
    duration t2 = n*Dt;
    t1 = 0.9*t2.count();
    tt0 = 0.1*t2.count();
    duration tiempo = i * Dt;
    w = w0 +  tiempo.count() / t2.count() * (w1 - w0);
    if ( tiempo.count() <= tt0)
        V = V0 *  tiempo.count() / tt0;
    else
    {
        if ( tiempo.count() <= t1)
            V = V0;
        else
            V = V0 * (t2.count() -  tiempo.count()) / (t2.count() - t1);
    }
    return V * sin(w * tiempo.count()) + V0;
}

double seno(int i, double V0, double w, const duration &Dt){
        duration tiempo = i * Dt;
        double V = 0;
        V = V0 * sin(w * tiempo.count()) + V0;
        return V;
}


uint16_t v2counts(float V){
    float c = 999 * V + 1.664; // Calibracion de salida
    uint16_t cuentas;
    cuentas = (uint16_t) c;
    return cuentas;
}
float counts2v(signed short value){
    float c = 0.00489 * value + 0.0198; // Calibracion de entrada
    float voltage = c;
    return voltage;
}

libusb_device_handle *placa_init(){
    libusb_device_handle *udev = NULL;
    int ret;
    //unsigned char serial[9];          --------
    
    ret = libusb_init(NULL);
    if (ret < 0) {
        perror("libusb_init: Failed to initialize libusb");
        return NULL;
        // exit(1);

    }

    if ((udev = usb_device_find_USB_MCC(USB1208FS_PID, NULL))) {
        printf("USB-1208FS Device is found!\n");
        init_USB1208FS(udev);
    } else {
        printf("No device found.\n");
        return NULL;
        // exit(0);
    }

    //print out the wMaxPacketSize.  Should be 64
    printf("wMaxPacketSize = %d\n", usb_get_max_packet_size(udev,0));

    /* config mask 0x01 means all inputs */
    usbDConfigPort_USB1208FS(udev, DIO_PORTA, DIO_DIR_OUT);
    usbDConfigPort_USB1208FS(udev, DIO_PORTB, DIO_DIR_IN);
    usbDOut_USB1208FS(udev, DIO_PORTA, 0);
    usbDOut_USB1208FS(udev, DIO_PORTA, 0);

    usbBlink_USB1208FS(udev); // when the system is ok, it blinks
    
    // registro mi handler para atender la señal que recibo cuando se presiona ctrl+c
    // struct sigaction sigIntHandler;
    // sigIntHandler.sa_handler = my_handler;
    // sigemptyset(&sigIntHandler.sa_mask);
    // sigIntHandler.sa_flags = 0;
    // sigaction(SIGINT, &sigIntHandler, NULL);
    
    
    return udev;
}

int placa_end(libusb_device_handle *udev){

    // escribo 0V en cada salida anlogica para desenergizar
    usbAOut_USB1208FS(udev, 0, 0);
    usbAOut_USB1208FS(udev, 1, 0);
    // reseteo la placa
    usbReset_USB1208FS(udev);

    libusb_clear_halt(udev, LIBUSB_ENDPOINT_IN | 1);
	libusb_clear_halt(udev, LIBUSB_ENDPOINT_OUT| 2);
	libusb_clear_halt(udev, LIBUSB_ENDPOINT_IN | 3);
	libusb_clear_halt(udev, LIBUSB_ENDPOINT_IN | 4);
	libusb_clear_halt(udev, LIBUSB_ENDPOINT_IN | 5);
	for (int i = 0; i < 4; i++) {
	  libusb_release_interface(udev, i);
	}
	libusb_close(udev);
	return 0;
}
void my_handler(int sig, libusb_device_handle *udev)
{
  printf( "Se presiono ctrl+c. Antes de terminar el programa, cierro correctamente la placa de adquisicion.\n");
  placa_end(udev);
  exit(1);
}

float read(int ch, libusb_device_handle *udev){
    // if (ch <0 || ch >7){
    //     printf("Channel %d not found.\n", ch);
    //      printf("Choose a channel between 0-7.\n");
    //     exit(0);
    // }
    
    signed short svalue;
    uint8_t channel, gain;
    // int flag;
	channel = (uint8_t) (ch);
	gain =  SE_10_00V;
    // flag = fcntl(fileno(stdin), F_GETFL);
    // fcntl(0, F_SETFL, flag | O_NONBLOCK);
    
    svalue = usbAIn_USB1208FS(udev, channel, gain);
	// fcntl(fileno(stdin), F_SETFL, flag);
    return counts2v(svalue);
}

void write(libusb_device_handle *udev, int ch, float V){ // usb handler, channel, voltage in mV
    // if (ch < 0 || ch > 1){
    //     printf("Channel %d not found.\n", ch);
    //     printf("Choose a channel between 0-1.\n");
    //     placa_end(udev);
    //     exit(0);
    // }
    // printf("Analog Writing Mode. \t");
    // printf("Channel: %d \t", ch);
    // printf("Voltage: %d mV \n", mV);
    uint8_t channel = (uint8_t) ch;
    uint16_t value;
    value = v2counts(V);
    if (value < 0)
        value = 0;
    if (value > 4095)
        value = 4095;
    usbAOut_USB1208FS(udev, channel, value);
}