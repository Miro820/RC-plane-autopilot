## 2. PWM to percent

Het ingelezen PWM-signaal staat uitgedrukt in µs, met een waarde tussen 1000µs en 2000µs. Deze waarde wordt hier omgerekent naar percent.

Voor deze berekening wordt een functie gebruikt, om toekomstig herhalende code te vermijden.

``pulseIn`` meet hoe lang een pin ``HIGH`` blijft in microseconden. Hiermee kan het input signaal al gezien worden in de plotter.