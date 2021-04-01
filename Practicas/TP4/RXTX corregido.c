#include <avr/io.h>
#include <avr/interrupt.h>
#include "twi_master.h"


//1. $ avr-gcc RXTX.c twi_master.c -o RXTX.elf -mmcu=atmega328p -O0 -Wall -DF_CPU=16000000
//2. $ avr-objcopy -j .text -j .data -O ihex RXTX.elf RXTX.hex
//3. $ avrdude -P /dev/ttyUSB0 -c arduino -p m328p -U flash:w:RXTX.hex -b57600

#define FOSC 16000000 //Clock speed
#define BAUD 115200
#define MYUBRR (FOSC/8/BAUD-1)
#define HEADER 0xAA

/*Registros del sensor LSM303*/
#define MAG_ADDR_W 0x3C               //con bit configurado para escritura   
#define MAG_ADDR_R 0x3D               //con bit configurado para modo lectura
#define MAG_XOUT_H 0x03               //Utilizo este por ser el primero (por orden)
#define MAG_ZOUT_H 0x05
#define MAG_YOUT_H 0x07
#define CRA_REG_M  0x00 
#define CRB_REG_M  0x01
#define MR_REG_M   0x02   

/*Configuraciones del i2c*/
twi_freq_mode_t frecuencia = TW_FREQ_100K;
bool pull_ups = false;

volatile uint16_t cont_timer = 0;   //Contador para el control del timer
volatile uint8_t dummy = 18;
volatile uint16_t indice = 1;   //indice para la transmicion
volatile uint16_t ind = 0;      //indice para el recibo
volatile uint16_t cont = 0;     //contador para dummy
volatile uint8_t checksumGlobal = 0;

volatile uint16_t adc00 = 12;
volatile uint16_t adc01 = 1050; //Auxiliares donde almaceno del ADC

volatile uint16_t PWM_val = 2000; // En el rango [0,2000], serian valor de cuentas
//Lo que hace el micro es mapear esas 2000 cuentas en el rango 1ms a 2 ms

void USART_Init();
void USART_Transmit ();
void PWM();
void ENCODER();

void ADC_Init();

typedef struct
{
    uint16_t x;
    uint16_t y;
    uint16_t z;
} mag_data_t;

mag_data_t* mag_data;

struct micro2raspi
{
   uint8_t head; //8 bits = char.
   uint16_t adc0; //16 bits = 2 char.
   uint16_t adc1;
   uint16_t encoder;
   uint16_t i2c;
   uint8_t chksum; 
   //Total 10 chars.
}m2r = {HEADER, 0x090, 0x4083, 0x0010, 0x431, 0x23};

struct raspi2micro{
    uint8_t head;
    uint16_t pwm;
    uint8_t cheksum;  
}r2m; 

void configurar_sensor(void){
    uint8_t configuracion1[2] = {CRA_REG_M, 0x0C}; //11100 en binario
    uint8_t configuracion2[2] = {CRB_REG_M, 0xE0};
    uint8_t configuracion3[2] = {MR_REG_M, 0};

    tw_master_transmit(MAG_ADDR_W, configuracion1, 2, false);        //Corroborar el false
    tw_master_transmit(MAG_ADDR_W, configuracion2, 2, false);        //Corroborar el false
    tw_master_transmit(MAG_ADDR_W, configuracion3, 2, false);        //Corroborar el false
}

void recibir_datos_mag (mag_data_t* mag_data){
    ret_code_t codigo;
    uint8_t datos[6];                                                      //variable para almacenar los datos del sensor

    datos[0] = MAG_XOUT_H;                                                //Uso datos[0] para pasar la dirección del primer registro X-Z-Y
    codigo = tw_master_transmit(MAG_ADDR_W, datos, 1, true);          //Me conecto con el sensor
    codigo = tw_master_receive(MAG_ADDR_R, datos, 6);       //Con sizeof(), le indicará cuantos registros contiguos leer

    mag_data->x = ((int16_t)datos[0]) << 8 | datos[1];        //
    mag_data->z = ((int16_t)datos[2]) << 8 | datos[3];         //Los registros de Z están antes que los de Y para el magnetrómetro
    mag_data->y = ((int16_t)datos[4]) << 8 | datos[5];
}

void configurar_timer0(){
    TCCR0A = (1 << WGM01); //Para configurar en modo CTC 
    TCCR0B = (3 << CS00);
    OCR0A = 200;//Con prescaler 1024 cada iteración es cada 100 uS
    TIMSK0 = (1 << OCIE0A); //Habilita la interrupción
}


void ADC_Init()
{
    /* Configuración de registros */
    ADMUX |= (1<<REFS0);            // REFS[1:0] = 00 --> REF = AREF ; ADLAR = 0 --> Just.derecha ; MUX[3:0] = 0000 --> CH0
    DIDR0 |= (3<<ADC0D);            // ADC0D/ADC1D = 11 --> Deshabilito la función de entrada digital de los Ch0 y Ch1
    ADCSRA |= (1<<ADEN) | (1<<ADIE) | (7 <<ADPS0); // ADEN = 1 --> habilito ADC ; ADIE = 1 --> Habilito interrupción ; ADPS[2:0] = 111 --> Prescaler en 128 --> 125 kHz
    ADCSRA |= (1 << ADSC);          //Inicio de la conversión
}

void USART_Init()
{
    /*Set baud rate */
    UBRR0H = (unsigned char) ((MYUBRR)>>8);
    UBRR0L = (unsigned char)MYUBRR;
    /* Transmit mode - Async double speed */
    UCSR0A |= (1<<U2X0);
    /*Enable receiver and transmitter */
    UCSR0B |= (1<<RXEN0) | (1<<TXEN0); 
    /*Enable Interruptions*/
    UCSR0B |= (1 << RXCIE0);
    /* Set frame format: 8 data, 1 stop bit */
    UCSR0C |= (3<<UCSZ00);    
}

void USART_Transmit ()
{
    /* Put data into buffer, sends the data */
    //Envio primer CHAR mediante el uso de su direccion de memoria.
    // UDR0 = (m2r.head); 
    m2r.adc0 = adc00; 
    m2r.adc1 = adc01;
    m2r.i2c = mag_data->x;
    /*
    m2r.encoder =  variable aux encoder
    m2r.i2c =  variable aux i2c.
    */
    UDR0 = m2r.head;
    m2r.chksum = m2r.head;
    
    UCSR0B |= (1 << UDRIE0);  //Habilito la interrupcion.
    
}

ISR(ADC_vect)
{
    if (!(ADMUX & (1 << MUX0))){       //Si MUX0 es 0 --> Salvar en adc00 (16 bits)
        adc00 = ADCW;
    }
    else{                               //else....--> Salvar en adc01   (16 bits)
        adc01 = ADCW;       
    }
    // En caso de que ADCW no funcione, probar:
    // adc00 = ADCL
    // adc00 |= (ADCH << 8)

    ADMUX ^= (1 << MUX0);   //Alterno entre CH0 y CH1
    ADCSRA |= (1 << ADSC);  //Inicio de la conversión
}

//Interrup. la transimicion
ISR(USART_UDRE_vect)
{
    /* Estructura de control para separa en paquetes la estructura a enviar */
    //indice = 1; Para empezar ya en 1 xq la primera parte ya la envie,
    if(m2r.head == HEADER)
    {
        UDR0 = *(&m2r.head+indice);         //Esto seria las diferentes partes de la informacion.
        m2r.chksum += *(&m2r.head+indice) ;  //Me cuenta la cantidad de bytes enviados, sin contarse a si mismo.
        indice++ ;
        //Finalizado el envío, se deshabilita la interrupción de buffer vacío 
        //Si llego al indice=10, significa que mande 10 "CHAR"
       
        if (indice == 10)
        {          
            m2r.chksum = 0;             
            UCSR0B &= ~(1 << UDRIE0); //Deshabilito la interrupcion que llama a la "TRANSMISION"
            indice = 1;
        }

    }
    
   /*
    UDR0 = dummy;
    cont++;
    if (cont == 100000){                                
        UCSR0B &= ~(1 << UDRIE0); //Deshabilito la interrupcion que llama a la "TRANSMISION"
        cont = 0;
    }
    */
}

    //Interr. de Recibo Informacion desde la RASPBERRY
    /*struct raspi2micro{
        uint8_t head;
        uint16_t pwm;
        uint8_t cheksum;  
    }r2m; 
    ind = indice para recibir.
    */
ISR(USART_RX_vect)
{
    //comparacion del HEAD
    *(&r2m.head + ind) = UDR0;

    if( r2m.head == HEADER) //Comparacion del HEAD y si es correcto aumento mi contador.
    {
        if(ind == 3)
        {
            if(checksumGlobal == r2m.cheksum)
            {
                OCR1B = r2m.pwm + 0x7CF;
                USART_Transmit(); 
            }

            ind = 0;
            checksumGlobal = 0;
        }
        else
        {
            checksumGlobal += *(&r2m.head + ind)  ;
            ind++;
        }
    }    
}
void PWM()
{
    TCCR1A |= (1 << COM1B1) | (0 << COM1B0); // Non inverting mode Clear OC1A/OC1B on Compare Match, set OC1A/OC1B at BOTTOM
    TCCR1A |= (1 << WGM10) | (1 << WGM11); // Fast PWM
    TCCR1B |= (1 << WGM12) | (1 << WGM13); // Fast PWM
    DDRB |= (1 << DDB2); // Set Port B2 as output
    TCCR1B |= (1 << CS11); // Set prescaler to 8
    OCR1A = 0x9C3F; //max TOP Value: 20ms; Bottom value: 0.
    OCR1B = PWM_val + 0x7CF; // 1ms -> 1999
    // OCR1B = 0xF9F; // 2ms -> 3999
}

void ENCODER()
{
    //señal A en PD2, señal B en PD3, señal Z en PD4
    //Cambio los registros necesarios.
    EICRA |= (1 << ISC00) | (1 << ISC01); // Rising edge of INT0 genera la interrupcion.
    EIMSK |= (1 << INT0); //Habilito la interrupcion en INT0.
}

ISR(TIMER0_COMPA_vect){
    cont_timer++;    //Cada interrupción incrementa el valor de i
}

ISR(INT0_vect) //Logica para la interrupcion usada en el Encoder
{// Ya entro con A=TRUE, es decir, señal ascendente y chequeo el valor de la señal B.
    
    if ((PIND &(1<<PIND4)) )
    {// Señal Z indica que completo 1 vuelta.
        m2r.encoder = 0;
    }

    if ((PIND &(1<<PIND3)) == 0  )
    { //Significa señal A adelantada a señal B. Giro horario
         m2r.encoder += 1;
    }
    else
    {//Significa señal B adelantada a señal A. Giro antihorario
        if (m2r.encoder == 0)
        {
            m2r.encoder = 2000;
        }
        else
        {
            m2r.encoder -= 1;
        }
            
    } 
}

int main()
{

    configurar_timer0();
    USART_Init();
    ADC_Init();
    ENCODER();
    PWM();
    tw_init(frecuencia, pull_ups);
    configurar_sensor();
    sei();
    
    //USART_Transmit(dummy);
    //USART_Transmit(micro2raspi.head);
    
    while(1){
        if(cont_timer == 1000){
           recibir_datos_mag(mag_data);
           cont_timer = 0; 
        }
        ;
    }
    return 0;
}

  
