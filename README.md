# ARDUINO-ALARM-SISTEM

## Descripción
Este proyecto consiste en construir un sistema de control de temperatura utilizando un control remoto infrarrojo (IR), una pantalla LCD y varios componentes. El sistema permite a los usuarios controlar el umbral de temperatura, monitorear las lecturas de temperatura y responder a posibles peligros de incendio.

## Componentes Utilizados
- Placa Arduino
- Módulo receptor de IR
- Control remoto IR
- Pantalla de cristal líquido (LCD)
- Sensor de temperatura
- Motor servo
- Protoboard
- LEDs rojos y verdes
- Resistencias y cables

## Instalación y Configuración

1. Conecta los componentes de la siguiente manera:
   - Conecta el módulo receptor de IR a la placa Arduino.
   - Conecta la pantalla LCD a la placa Arduino.
   - Conecta el sensor de temperatura a la placa Arduino.
   - Conecta el motor servo a la placa Arduino.
   - Conecta los LEDs rojos y verdes a la placa Arduino.

![Proyecto armado](/src/procesos.png)

2. Instala las bibliotecas necesarias:
   - Biblioteca IRremote para la funcionalidad del control remoto IR.
   - Biblioteca LiquidCrystal para la funcionalidad de la pantalla LCD.
   - Biblioteca Servo para el control del motor servo.

## Uso

El sistema puede ser controlado utilizando el control remoto IR. Presiona los botones correspondientes en el control remoto para realizar diversas acciones:

- Botón FUNC/STOP: Enciende el sistema y lo establece en el modo "Primavera".
- Botón >||: Activa la alarma de incendio manualmente y hace que el motor servo se mueva a 180 grados.
- Botón VOL-: Ajusta el umbral de temperatura en -5 grados.
- Botón VOL+: Ajusta el umbral de temperatura en +5 grados.
- Botón 4: Establece el sistema en el modo "Primavera".
- Botón 5: Establece el sistema en el modo "Verano".
- Botón 6: Establece el sistema en el modo "Otoño".
- Botón 7: Establece el sistema en el modo "Invierno".
- Botón ON/OFF: Apaga el sistema.

La pantalla LCD mostrará la estación actual y la lectura de temperatura.

Si la temperatura supera el umbral o se activa la alarma de incendio manualmente, el LED rojo se encenderá, el motor servo se moverá a 180 grados y la pantalla LCD mostrará "HAY UN INCENDIO".

Para restablecer el sistema, presiona el botón "ON/OFF" en el control remoto IR. Esto apagará todos los componentes y reiniciará el sistema.

## Explicación detallada del código

### Definiciones y Setup
## ------Explicacion detallada del codigo-------

### Definiciones y Setup:
```c++
//librerias
#include <IRremote.h>
#include <LiquidCrystal.h>
#include <Servo.h>

//pines
#define ledRoja 4
#define ledVerde 2
#define servo1 3

//PANTALLA LCD
LiquidCrystal lcd(5, 6, 7, 8, 9, 10, 12);

// SERVO
Servo servito;

//PINES PAR SETUP
int sensorTemperatura = A0;
int receptor = 11;

//VARIABLES
int respuesta;
bool apagado = true;
bool encendido = true;
int umbralTemperatura = 60;
String estacion = "Invierno";
bool incendio = false; 

void setup()
{
    //LEDS
    pinMode(ledRoja,OUTPUT);
    pinMode(ledVerde,OUTPUT);

    //CONSOLA
    Serial.begin(9600);

    //CONFIGURACION LCD
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.print("Cargando Programa");
    
    //CONFIGURACION SERVO
  	servito.attach(servo1, 400, 2600);
 	servito.write(0);
    
    //CONFIGURACION CONTROLL
    IrReceiver.begin(receptor);

}
```

## Funcion Loop
```c++

void loop()
{
	respuesta = 100;//bandera para que solo escuche los elementos presionados
  	
    if (IrReceiver.decode())// escucho los botones del control remoto
  	{	
    	respuesta = IrReceiver.decodedIRData.command;
      	Serial.println(respuesta);
    	IrReceiver.resume();
  	}

    if(encendido == true && banderaApagar == true)//cuando esta apagado muestra por lcd 1 sola vez
    {
        escribirArriba("SISTEMA APAGADO");
        
        encendido = false;
    }

    encender(encendido,respuesta);//seteo en el encendido en primavera

    if(respuesta !=100 && encendido == true && incendio == false)// si no hay incendio y si esta encendido
    {
    	
        switch (respuesta)
        {
            case 16:
                estacion = "Primavera";
                break;
            case 17:
                estacion = "Verano";
                break;
            case 18:
                estacion = "Otono";
                break;
            case 20:
                estacion = "Invierno";
                break;  
            case 10:
                umbralTemperatura = cambiarUmbralTemperatura(umbralTemperatura,5);//cambio el umbral en 5
                delay(500);
                break;
            case 0: 
          		apagarTodo(ledVerde,ledRoja);//vuelve a apagado
                break;
            case 8:
                umbralTemperatura = cambiarUmbralTemperatura(umbralTemperatura,-5);
          		delay(500);
                break;
            
            
        }
        escribirArriba(estacion);// escribo la estacion designada en el switch
    }
    if (encendido == true)si esta encendido muestra la temperatura y prende un led verde
    {    
        escribirAbajo(leerSensorTemp(sensorTemperatura));
        digitalWrite(ledVerde,HIGH);
        
        //si la temp supera el humbral o se preciona la tecla >||
        //hay incendio  
		if ((leerSensorTemp(sensorTemperatura)>umbralTemperatura) || (respuesta == 5 && encendido == true))
        {
        	alarmaIncendio(ledRoja);
      		moverServito(180);
      		incendio = true;
        }
    }
    // en cualquier momento en el que este si se preciona el botonn de apagado se reinicia todo
	if(respuesta == 0)
    {
      apagarTodo(ledVerde,ledRoja);
    }
}
```
