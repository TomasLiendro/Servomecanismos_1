#ifndef FUNCIONES_H
#define FUNCIONES_H
#include <chrono>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdint.h>
#include "pmd.h"
#include "usb-1208FS.h"
#include <vector>
#include <fstream>

using duration = std::chrono::duration<double>;
using time_point = std::chrono::steady_clock::time_point;
using namespace std;

duration espera_siguiente_intervalo(const time_point &t0, int n, const duration &dt);
double escalones_random(double V0, int T1, int T2, int i);
double chirp(int n, int i, double w0, double w1, double V0, double C, const duration &Dt);
double seno(int i, double V0, double w, const duration &Dt);
uint16_t v2counts(float V);
float counts2v(uint16_t value);
libusb_device_handle * placa_init();
int placa_end(libusb_device_handle *udev);
void my_handler(int sig, libusb_device_handle *udev);
float read(int ch, libusb_device_handle *udev);
void write(libusb_device_handle *udev, int ch, float mV);
#endif

