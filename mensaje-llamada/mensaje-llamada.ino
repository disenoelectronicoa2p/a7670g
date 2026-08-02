/*
  Control básico del A7670G mediante switch

  Comandos desde el monitor serial:

  R = Leer mensaje
  M = Enviar mensaje
  L = Hacer llamada
  C = Colgar llamada

  Arduino Mega:
  TX1 pin 18 -> RX del A7670G
  RX1 pin 19 <- TX del A7670G
  GND        -> GND
*/

enum Estado{
  ESPERA,
  LEER_MENSAJE,
  ENVIAR_MENSAJE,
  HACER_LLAMADA,
  COLGAR_LLAMADA
};

Estado estadoActual = ESPERA;

const char numeroTelefono[] = "3207045369"; //Numero Movil
const char codigo[] = "+57";                //Codigo pais

// Texto del mensaje
const char textoMensaje[] = "Mensaje enviado desde modulo A7670G";

// Posición del mensaje almacenado que se quiere leer
const int posicionMensaje = 1;

void setup(){
  // Monitor serial
  Serial.begin(115200);

  // Comunicación con el A7670G
  Serial1.begin(115200);

  delay(2000);

  Serial.println();
  Serial.println("Control A7670G");
  mostrarMenu();
}

void loop(){
  // Leer opción ingresada por el usuario
  if (Serial.available())  {
    char opcion = Serial.read();

    // Permitir mayúsculas y minúsculas
    //opcion = toupper(opcion);

    switch (opcion)    {
      case 'R':
        estadoActual = LEER_MENSAJE;
        break;

      case 'M':
        estadoActual = ENVIAR_MENSAJE;
        break;

      case 'L':
        estadoActual = HACER_LLAMADA;
        break;

      case 'C':
        estadoActual = COLGAR_LLAMADA;
        break;

      case '\r':
      case '\n':
        // Ignorar saltos de línea
        break;

      default:
        Serial.println("Opcion no valida.");
        mostrarMenu();
        break;
    }
  }

  // Ejecutar la acción correspondiente al estado
  switch (estadoActual){
    case ESPERA:
      // No realizar ninguna operación
      break;

    case LEER_MENSAJE:
      leerMensaje();
      estadoActual = ESPERA;
      mostrarMenu();
      break;

    case ENVIAR_MENSAJE:
      enviarMensaje();
      estadoActual = ESPERA;
      mostrarMenu();
      break;

    case HACER_LLAMADA:
      hacerLlamada();
      estadoActual = ESPERA;
      mostrarMenu();
      break;

    case COLGAR_LLAMADA:
      colgarLlamada();
      estadoActual = ESPERA;
      mostrarMenu();
      break;
  }
}

/*
  Leer un mensaje almacenado en la memoria.
*/
void leerMensaje(){
    Serial.println();
    Serial.println("===== LEER MENSAJE =====");

    // Configurar modo texto
    Serial1.println("AT+CMGF=1");
    esperarRespuesta("OK", 3000);

    // Leer el mensaje almacenado en la posición 1
    Serial1.println("AT+CMGR=1");

    Serial.println("Contenido del mensaje:");
    Serial.println("------------------------------");

    unsigned long tiempo = millis();

    while (millis() - tiempo < 5000)
    {
        while (Serial1.available())
        {
            String linea = Serial1.readStringUntil('\n');
            linea.trim();

            if (linea.length() > 0)
            {
                Serial.println(linea);
            }

            // La respuesta termina cuando el módulo responde OK
            if (linea == "OK")
            {
                Serial.println("------------------------------");
                return;
            }
        }
    }

    Serial.println("Tiempo de espera agotado.");
}

/*
  Enviar un mensaje de texto.
*/
void enviarMensaje(){
  Serial.println();
  Serial.println("Enviando mensaje...");

  // Configurar modo texto
  enviarComandoAT("AT+CMGF=1");
  delay(500);

  // Enviar número de destino
  Serial1.print("AT+CMGS=\"");
  Serial1.print(codigo);
  Serial1.print(numeroTelefono);
  Serial1.println("\"");

  Serial.print("AT+CMGS=\"");
  Serial.print(codigo);
  Serial.print(numeroTelefono);
  Serial.println("\"");

  // Esperar el símbolo >
  delay(1000);

  // Enviar contenido del mensaje
  Serial1.print(textoMensaje);
  Serial.println(textoMensaje);

  // Ctrl + Z para finalizar y enviar el SMS
  Serial1.write(0x1A);

  Serial.println("Contenido del mensaje enviado al modulo.");

  // Esperar respuesta +CMGS
  delay(5000);
}

/*
  Realizar una llamada.
*/
void hacerLlamada(){
  Serial.println();
  Serial.print("Llamando a: ");
  Serial.println(numeroTelefono);

  Serial1.print("ATD");
  Serial1.print(numeroTelefono);
  Serial1.println(";");

  Serial.print("ATD");
  Serial.print(numeroTelefono);
  Serial.println(";");

  Serial.println("Presione C para colgar.");
}

/*
  Colgar la llamada.
*/
void colgarLlamada(){
  Serial.println();
  Serial.println("Colgando llamada...");

  enviarComandoAT("AT+CHUP");
}

/*
  Enviar un comando AT al A7670G.
*/
void enviarComandoAT(const char *comando){
  Serial1.println(comando);

  Serial.print("Comando enviado: ");
  Serial.println(comando);
}

/*
  Mostrar en el monitor serial las respuestas del módulo.
*/
void leerRespuestaModulo(){
  while (Serial1.available())
  {
    char caracter = Serial1.read();
    Serial.write(caracter);
  }
}

bool esperarRespuesta(String respuestaEsperada, unsigned long timeout){
    unsigned long tiempo = millis();

    while (millis() - tiempo < timeout)
    {
        if (Serial1.available())
        {
            String respuesta = Serial1.readString();

            Serial.print(respuesta);

            if (respuesta.indexOf(respuestaEsperada) != -1)
            {
                return true;
            }
        }
    }

    return false;
}

/*
  Mostrar las opciones disponibles.
*/
void mostrarMenu(){
  Serial.println();
  Serial.println("============================");
  Serial.println("Seleccione una operacion:");
  Serial.println("R = Leer mensaje");
  Serial.println("M = Enviar mensaje");
  Serial.println("L = Hacer llamada");
  Serial.println("C = Colgar llamada");
  Serial.println("============================");
  Serial.println();
}


