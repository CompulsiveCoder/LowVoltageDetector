int voltageSignalOutPin = 4;
int voltageSignalInPin = A2;

double singleVoltageThreshold = 2.7;

int numberOfBatteriesInSeries = 3;

double voltageThreshold = singleVoltageThreshold * numberOfBatteriesInSeries;

double voltageReading = 0;

bool isDebug = true;

void setup() {
  Serial.begin(9600);

  pinMode(voltageSignalOutPin, OUTPUT);
  pinMode(13, OUTPUT);

}

// the loop routine runs over and over again forever:
void loop() {

  takeReading();

  adjustOutputSignal();

  delay(1000);
}

void takeReading()
{
  int numReadings = 40;

  double total = 0;

  for (int i = 0; i<numReadings; i++)
  {
    total += analogRead(voltageSignalInPin);
    delay(10);
  }

  double average = total / numReadings;

  long result;

  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);

  delay(5); // Wait for Vref to settle

  ADCSRA |= _BV(ADSC);
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1126400L / result;
  result = result / 1000;
  
  voltageReading = average * (result / 1023.0) * 5;

  if (isDebug)
  {
    Serial.print("Voltage: ");
  }

  Serial.println(voltageReading);
}

void adjustOutputSignal()
{
  if (isDebug)
  {
    Serial.print("Threshold: ");
    Serial.println(voltageThreshold);
  }
  bool output = voltageReading > voltageThreshold;

  digitalWrite(voltageSignalOutPin, output);
  digitalWrite(13, output);

  if (isDebug)
  {
    Serial.print("Output: ");
    Serial.println(output);
  }

}



