String inputString = "";
boolean stringComplete = false; 
String commandString = "";
boolean isConnected = false;

double period, frequency;
int high, low;

#define DUTY_CICLE 0.25

#define DIRECTION_PIN 9
#define HORIZONTAL_PIN 10
#define VERTICAL_PIN 11
#define PHEM_LEFT_PIN 12
#define PHEM_RIGHT_PIN 13


String getCommand () {
  if (inputString && inputString.length()) {
    int separatorIndex = inputString.indexOf("-");
    
    if (separatorIndex > -1) {
      return inputString.substring(0, separatorIndex);
    }

    return inputString;  
  }
  return "";
}

String getParam () {
  int separatorIndex = inputString.indexOf("-");
  
  if (separatorIndex > -1) {
    return inputString.substring(separatorIndex + 1);
  }

  return "";
}

void setSystemParams (String f) {
  frequency = f.toDouble();
  period = (1.0 / frequency) * 1000000.0;
  high = DUTY_CICLE * period;
  low = period * (1 - DUTY_CICLE);
}

void Pulse (int channel) {
  digitalWrite(channel, HIGH);
  delayMicroseconds(high);
  digitalWrite(channel, LOW);
  delayMicroseconds(low);
}

void TrainPulse (int channel, int count) {
  for (int i = 0; i < count; i++) {
    Pulse(channel);
  }
}

void ToggleDirection (String direction) {
  if (direction.equals("LEFT") || direction.equals("UP")) {
    digitalWrite(DIRECTION_PIN, HIGH);
  } else {
    digitalWrite(DIRECTION_PIN, LOW);
  }
}

void setup () {
  Serial.begin(9600);
  pinMode(DIRECTION_PIN, OUTPUT);
  pinMode(HORIZONTAL_PIN, OUTPUT);
  pinMode(VERTICAL_PIN, OUTPUT);
  pinMode(PHEM_LEFT_PIN, OUTPUT);
  pinMode(PHEM_RIGHT_PIN, OUTPUT);
}

void loop () {
  if (stringComplete)
  {
    stringComplete = false;
    
    String command = getCommand();
    String param = getParam();
    
    if (command.equals("INIT")) {
      setSystemParams(param);
      Serial.println(command + " " + frequency + " " + period + " " + high + " " + low);
    } else if (command.equals("DIRECTION")) {
      ToggleDirection(param);
      Serial.println(command + " " + param);
    } else if (command.equals("MOVE_HORIZONTAL")) {
      int steps = param.toInt();
      TrainPulse(HORIZONTAL_PIN, steps);
      Serial.println(command + " " + param);
    } else if (command.equals("MOVE_VERTICAL")) {
      int steps = param.toInt();
      TrainPulse(VERTICAL_PIN, steps);
      Serial.println(command + " " + param);
    }
    
    inputString = "";
  }
}

void serialEvent () {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    
    if (inChar != '\n') {
      inputString += inChar;  
    }
    
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
