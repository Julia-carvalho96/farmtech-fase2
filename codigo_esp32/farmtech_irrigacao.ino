#include <DHT.h>

#define DHTPIN 15
#define DHTTYPE DHT22

// Botões (simulam N, P, K)
#define N_BUTTON 4
#define P_BUTTON 13
#define K_BUTTON 14

// LDR (simula pH do solo)
#define PH_SENSOR 34

// LED azul (simula bomba de irrigação)
#define PUMP_PIN 5

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Usa resistores internos de pull-up (HIGH quando solto, LOW quando pressionado)
  pinMode(N_BUTTON, INPUT_PULLUP);
  pinMode(P_BUTTON, INPUT_PULLUP);
  pinMode(K_BUTTON, INPUT_PULLUP);

  pinMode(PH_SENSOR, INPUT);
  pinMode(PUMP_PIN, OUTPUT);
}

void loop() {
  // Lê os botões (LOW = pressionado)
  bool n_ok = digitalRead(N_BUTTON) == LOW;
  bool p_ok = digitalRead(P_BUTTON) == LOW;
  bool k_ok = digitalRead(K_BUTTON) == LOW;

  // Lê o valor analógico do LDR e mapeia para pH entre 0 e 14
  int ph_value_raw = analogRead(PH_SENSOR);
  float ph = map(ph_value_raw, 0, 4095, 0, 14);

  // Lê a umidade do solo (via DHT22)
float humidity = dht.readHumidity();
if (isnan(humidity)) {
  humidity = 40.0; // valor padrão de simulação
}



  // Mostra tudo no monitor serial
  Serial.println("---- Leitura Atual ----");
  Serial.print("Nitrogênio (N): "); Serial.println(n_ok ? "OK" : "BAIXO");
  Serial.print("Fósforo (P): ");   Serial.println(p_ok ? "OK" : "BAIXO");
  Serial.print("Potássio (K): "); Serial.println(k_ok ? "OK" : "BAIXO");
  Serial.print("pH do solo: ");   Serial.println(ph, 1);
  Serial.print("Umidade do solo: "); Serial.print(humidity); Serial.println("%");

  // Regras para ativar a irrigação
  if (n_ok && p_ok && k_ok && (ph >= 6.0 && ph <= 7.0) && humidity < 50) {
    digitalWrite(PUMP_PIN, HIGH);
    Serial.println("💧 Irrigação ativada!");
  } else {
    digitalWrite(PUMP_PIN, LOW);
    Serial.println("🚫 Irrigação desativada.");
  }

  Serial.println("------------------------\n");

  delay(2000); // espera 2 segundos
}
