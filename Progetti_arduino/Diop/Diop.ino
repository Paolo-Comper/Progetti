#define Echo 2 //senza punto e virgola
#define Trig 3

int Tempo;
float Distanza;
void setup() {
  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);

  digitalWrite(Trig, LOW);

  Serial.begin(9600);
}

void loop() {

  // Manda un breve segnale al Trig
  digitalWrite(Trig, HIGH);
  delay(10);
  digitalWrite(Trig, LOW);
  
  //Mette un timer
  Tempo = pulseIn(Echo, HIGH);

  //calcola la distanza dal tempo
  Distanza = (Tempo * 0.034) / 2;

  Serial.println(Distanza);

}
