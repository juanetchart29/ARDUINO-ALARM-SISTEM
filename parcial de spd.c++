#include <IRremote.h>
#include <LiquidCrystal.h>
#include <Servo.h>

#define ledRoja 4
#define ledVerde 2
#define servo1 3

Servo servito;

int sensorTemperatura = A0;
int receptor = 11;


int respuesta;
bool banderaApagar = true;//bandera para apagar
bool encendido = true;
int umbralTemperatura = 60;
String estacion = "Invierno";
bool incendio = false;

LiquidCrystal lcd(5, 6, 7, 8, 9, 10, 12);

void setup()
{
    pinMode(ledRoja,OUTPUT);
    pinMode(ledVerde,OUTPUT);

    Serial.begin(9600);

    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.print("Cargando Programa");
    
  	servito.attach(servo1, 400, 2600);
 	servito.write(0);
  
    IrReceiver.begin(receptor);
  
  
}

void loop()
{
	respuesta = 100;//bandera para que solo escuche los elementos presionados
  	
    if (IrReceiver.decode())
  	{	
    	respuesta = IrReceiver.decodedIRData.command;
      	Serial.println(respuesta);
    	IrReceiver.resume();
  	}

    if(encendido == true && banderaApagar == true)
    {
        escribirArriba("SISTEMA APAGADO");
        
        encendido = false;
    }

    encender(encendido,respuesta);//seteo en primavera

    if(respuesta !=100 && encendido == true && incendio == false)
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
                umbralTemperatura = cambiarUmbralTemperatura(umbralTemperatura,5);
                delay(500);
                break;
            case 0: 
          		apagarTodo(ledVerde,ledRoja);


                break;
            case 8:
                umbralTemperatura = cambiarUmbralTemperatura(umbralTemperatura,-5);
 
          		delay(500);
                break;
            
            
        }
        escribirArriba(estacion);
    }
    if (encendido == true)
    {    
        escribirAbajo(leerSensorTemp(sensorTemperatura));
        digitalWrite(ledVerde,HIGH);
		if ((leerSensorTemp(sensorTemperatura)>umbralTemperatura) || (respuesta == 5 && encendido == true))
        {
        	alarmaIncendio(ledRoja);
      		moverServito(180);
      		incendio = true;
        }
    }
  
	if(respuesta == 0)
    {
      apagarTodo(ledVerde,ledRoja);
    }

}

float leerSensorTemp(int sensorTemperatura)
{ 
  int numero = analogRead(sensorTemperatura);
  float temp = 0.4882 * numero - 49.763;
  return temp;
}


int leerSensorIr()
{
    int respuesta;
    if (IrReceiver.decode())
  {
    respuesta = IrReceiver.decodedIRData.command;
    Serial.println(respuesta);
    IrReceiver.resume();
  }else{
    respuesta = 100;
  }
  return respuesta;
}


void escribirArriba(String escribir1)
{
    lcd.clear();
    lcd.print(escribir1);
}
void escribirAbajo(int temperatura)
{
    lcd.setCursor(0, 2);
    lcd.print(temperatura); // Imprime la temperatura con un decimal
    lcd.print(" grados C");
}


void alarmaIncendio(int pinLedRoja)
{
    lcd.clear();
    escribirArriba("HAY UN INCENDIO");
    digitalWrite(pinLedRoja,HIGH);
}



int cambiarUmbralTemperatura(int umbralTemperatura,int suma)
{

    int nuevoUmbral = umbralTemperatura + suma;
    if (nuevoUmbral > -41 && nuevoUmbral < 125)
    {
        umbralTemperatura = nuevoUmbral;
    }
    escribirArriba("El humbral es :");
    escribirAbajo(umbralTemperatura); 
   
    return umbralTemperatura;
}

void moverServito(int angulo) {
  servito.write(angulo);
  delay(50);//pone un delay para el antiroboting
}

void apagarTodo(int cLedVerde , int cLedRoja)
{
  	moverServito(0);
  	digitalWrite(cLedVerde,LOW);
   	digitalWrite(cLedRoja,LOW);
	encendido = true;
  	banderaApagar = true;
  	incendio = false;
}

// funciones mejoradas:
void encender(bool CEncendido,int cRespuesta)
{
    if (encendido== false && respuesta == 2)
    {
        banderaApagar = false;
        encendido = true;
        respuesta = 16;
    }
}
// void apagar(bool cEncendido,bool cBanderaApagar)





//     if(encendido == true && banderaApagar == true)
//     {
//         escribirArriba("SISTEMA APAGADO");
        
//         encendido = false;
//     }