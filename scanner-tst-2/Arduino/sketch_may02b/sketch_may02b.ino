String inputString = "";
boolean stringComplete = false; 
String commandString = "";
boolean isConnected = false;
double period, frequency;
int high, low;

boolean read_signal = false;
unsigned long count = 0;

#define DUTY_CICLE 0.25

#define DIRECTION_PIN 12

#define SIGNAL_PIN 3

#define HORIZONTAL_PIN 5
#define VERTICAL_PIN 6
#define PHEM_LEFT_PIN 9
#define PHEM_RIGHT_PIN 10

#define END_SWITCH_UP 2
#define END_SWITCH_DOWN 4
#define END_SWITCH_LEFT 7
#define END_SWITCH_RIGHT 8



/*
 * Распарс команды
*/
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

/*
 * Распарс параметра
*/
String getParam () {
  int separatorIndex = inputString.indexOf("-");
  
  if (separatorIndex > -1) {
    return inputString.substring(separatorIndex + 1);
  }

  return "";
}

/* Определение концевого выключателя от направления
 * direction - направление UP, DOWN, LEFT, RIGHT
*/
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


/* Инициализация системы с заданными параметрами
 * f - частота в Hz
*/
void setSystemParams (String f) {
  frequency = f.toDouble();
  period = (1.0 / frequency) * 1000000.0;
  high = DUTY_CICLE * period;
  low = period * (1 - DUTY_CICLE);
}

/*Генерация одиночного импульса
 * channel - вывод двигателя
*/
void Pulse (int channel) {
  digitalWrite(channel, HIGH);
  delayMicroseconds(high);
  digitalWrite(channel, LOW);
  delayMicroseconds(low);
}

/* Генерация серии импульсов
 * channel - вывод двигателя
 * steps - кол-во шагов
 * end_switch - концевой выключатель для выбанного направления движения
*/
int TrainPulse (int channel, int steps, int end_switch) {
  int end_switch_state;
  int steps_done = 0;
  for (int i = 0; i < steps; i++) {
    Pulse(channel);
    
    if (end_switch) {
      end_switch_state = digitalRead(end_switch);  
    }
    
    steps_done += 1;
    
    if (end_switch_state > 0) {
      return steps_done;
    }
  }

  return steps_done;
}

/*Изменение направления движения для двигателей главного зеркала
 * direction - направление UP, DOWN, LEFT, RIGHT
*/
void ToggleDirection (String direction) {
  if (direction.equals("LEFT") || direction.equals("UP")) {
    digitalWrite(DIRECTION_PIN, HIGH);
  } else {
    digitalWrite(DIRECTION_PIN, LOW);
  }
}

/*
 * Центрирование системы
*/
void CentreSystem () {
  int steps_done, steps_center, end_switch;

  //Центрируем систему по горизонтали
  //Двигаем главное зеркало влево до концевика
  ToggleDirection("LEFT");
  end_switch = getEndSwitch("HORIZONTAL");
  steps_done = TrainPulse(HORIZONTAL_PIN, 5000, end_switch);
  
  //Двигаем главное зеркало вправо до концевика и получаем кол-во шагов
  ToggleDirection("RIGHT");
  end_switch = getEndSwitch("HORIZONTAL");
  steps_done = TrainPulse(HORIZONTAL_PIN, 5000, end_switch);

  //Двигаем главное зеркало влево на центр
  ToggleDirection("LEFT");
  end_switch = getEndSwitch("HORIZONTAL");
  steps_center = steps_done / 2;
  steps_done = TrainPulse(HORIZONTAL_PIN, steps_center, end_switch);

  
  //Центрируем систему по горизонтали
  //Двигаем главное зеркало влево до концевика
  ToggleDirection("UP");
  end_switch = getEndSwitch("VERTICAL");
  steps_done = TrainPulse(VERTICAL_PIN, 5000, end_switch);
  
  //Двигаем главное зеркало вправо до концевика и получаем кол-во шагов
  ToggleDirection("DOWN");
  end_switch = getEndSwitch("VERTICAL");
  steps_done = TrainPulse(VERTICAL_PIN, 5000, end_switch);

  //Двигаем главное зеркало влево на центр
  ToggleDirection("UP");
  end_switch = getEndSwitch("VERTICAL");
  steps_center = steps_done / 2;
  steps_done = TrainPulse(VERTICAL_PIN, steps_center, end_switch);
  
}

void PulseCounter () {
  if (read_signal) {
    count += 1;  
  }
}

void setup () {
  Serial.begin(9600);
  
  pinMode(SIGNAL_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(SIGNAL_PIN), PulseCounter, RISING);
//  NVIC_SET_PRIORITY(IRQ_PORTA, 0);
  
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
    } else if (command.equals("READ")) {
      if (param.equals("START")) {
        count = 0;
        read_signal = true;
        Serial.println("READ-START");
      } else if (param.equals("STOP")) {
        read_signal = false;
        char buf[50];
        sprintf(buf, "%lu", count);
        String str = String(buf);
        count = 0;
        Serial.println("READ-STOP COUNTS = " + str);
      }
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
