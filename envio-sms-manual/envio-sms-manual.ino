
String comando = "";

void setup() {

  Serial.begin(115200);
  Serial1.begin(115200);

  Serial.println();
  Serial.println("====================================");
  Serial.println(" Enviar mensaje desde el A7670G");
  Serial.println("====================================");
  Serial.println();

}

void loop() {

  // ---------- PC -> A7670G ----------
  while (Serial.available()) {
    char c = Serial.read();

    // Fin de línea
    if (c == '\n' || c == '\r') {
      comando.trim();
      if (comando.length() > 0) {
        if (comando.equalsIgnoreCase("0x1A")) {
          Serial.println("");
          Serial.println("[Enviando Ctrl+Z]");
          Serial1.write(0x1A);
        }
        else {
          Serial1.print(comando);
          Serial1.print("\r");
        }
      }
      comando = "";
    }
    else {
      comando += c;
    }

  }

  // ---------- A7670G -> PC ----------
  while (Serial1.available()) {
    Serial.write(Serial1.read());
  }
}

