
const int ch1_in = 12;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ch1_in, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  // pulseIn measures how long the pin stays HIGHn in microseconds...
  // With this, the input signal can already be seen in the serial plotter
  Serial.println(pulseIn(ch1_in, HIGH));
}
