
const int ch3_in = 12;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ch3_in, INPUT);
}

void loop() {
  // pulseIn measures how long the pin stays HIGHn in microseconds...
  // With this, the input signal can already be seen in the serial plotter
  int ch3_in_pwm = pulseIn(ch3_in, HIGH);
  // the pulse witdh of a PWM servo signal has a range of about 1000µs - 2000µs
  // convert to percentage + print
  Serial.println(pwm_to_percent(ch3_in_pwm));
}


float pwm_to_percent(int pwm_signal) {
  float percentage = ((pwm_signal - 1000)*100)/1000;
  return percentage;
}