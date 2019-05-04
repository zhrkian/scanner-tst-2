String inputString = "";
boolean stringComplete = false; 
String commandString = "";
boolean isConnected = false;

double period, frequency;
int high, low;

#define DUTY_CICLE 0.25

#define DIRECTION_PIN 12

#define HORIZONTAL_PIN 5
#define VERTICAL_PIN 6
#define PHEM_LEFT_PIN 9
#define PHEM_RIGHT_PIN 10

#define END_SWITCH_UP 2
#define END_SWITCH_DOWN 4
#define END_SWITCH_LEFT 7
#define END_SWITCH_RIGHT 8

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

int getEndSwitch (String direction) {
  int direction_state = digitalRead(DIRECTION_PIN);
  if (direction.equals("VERTICAL")) {
    if (direction_state > 0) {
      return END_SWITCH_UP;
    }
    return END_SWITCH_DOWN;
  }
  
  if (direction_state > 0) {
    return END_SWITCH_LEFT;
  }
  return END_SWITCH_RIGHT;
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

int TrainPulse (int channel, int count, int end_switch) {
  int end_switch_state;
  int steps_done = 0;
  for (int i = 0; i < count; i++) {
    Pulse(channel);
    end_switch_state = digitalRead(end_switch);
    steps_done += 1;
    if (end_switch_state > 0) {
      return steps_done;
    }
  }

  return steps_done;
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
  pinMode(END_SWITCH_UP, INPUT);
  pinMode(END_SWITCH_DOWN, INPUT);
  pinMode(END_SWITCH_LEFT, INPUT);
  pinMode(END_SWITCH_RIGHT, INPUT);
  
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
      int end_switch = getEndSwitch("HORIZONTAL");
      int steps_done = TrainPulse(HORIZONTAL_PIN, steps, end_switch);
      Serial.println("COMMAND = " + command + " PARAM = " + param + " STEPS_DONE = " + steps_done + " END_SWITCH = " + end_switch);
    } else if (command.equals("MOVE_VERTICAL")) {
      int steps = param.toInt();
      int end_switch = getEndSwitch("VERTICAL");
      int steps_done = TrainPulse(VERTICAL_PIN, steps, end_switch);
      Serial.println("COMMAND = " + command + " PARAM = " + param + " STEPS_DONE = " + steps_done + " END_SWITCH = " + end_switch + " HIGHT = " + HIGH + " LOW = " + LOW);
    }
    
    inputString = "";
  }
}

///Test

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
