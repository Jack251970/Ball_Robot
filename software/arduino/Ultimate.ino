// #include <AFMotor.h>

// /* Running Flag */

// bool isRunning = true;

// /* Speed Encoder */

// #define WE_PIN_LEFT 18
// #define WE_PIN_RIGHT 19

// volatile int count_left = 0;
// volatile int count_right = 0;

// /* Motor */

// const int carSpeed = 100;
// const int carRunLeftRightDistance = 1;
// const int carRunForwardBackwardDistance = 1;
// char direction = 'X';

// AF_DCMotor motor_front_right(3);
// AF_DCMotor motor_front_left(4);
// AF_DCMotor motor_back_left(1);
// AF_DCMotor motor_back_right(2);

// void serialRunCar(char inChar);
// void carForward();
// void carBackward();
// void carLeft(int angle = -1);
// void carRight(int angle = -1);
// void carStop();

// /* Ultrasonic Sensor */

// const int TRIGGER_FRONT = 32;
// const int ECHO_FRONT = 30;
// long duration_front;
// int cm_front;
// bool get_ball = false;

// /* Serial */

// char readSerial();
// void writeSerial(char c);

// void setup() {
//   // Init serial
//   Serial.begin(9600);
//   // Init speed encoder
//   pinMode(WE_PIN_LEFT, INPUT_PULLUP);
//   pinMode(WE_PIN_RIGHT, INPUT_PULLUP);
//   attachInterrupt(digitalPinToInterrupt(WE_PIN_LEFT), tick_left, FALLING);
//   attachInterrupt(digitalPinToInterrupt(WE_PIN_RIGHT), tick_right, FALLING);
//   // Init ultrasonic sensor
//   pinMode(TRIGGER_FRONT, OUTPUT);
//   pinMode(ECHO_FRONT, INPUT);
// }

// void loop() {
//   // Begin & Quit Running
//   char inChar = readSerial();
//   if (inChar == 'B') {
//     isRunning = true;
//     writeSerial('Begin running');
//   }
//   else if (inChar == 'Q') {
//     isRunning = false;
//     carStop();
//     writeSerial('Quit running');
//   }
//   else {
//     if (isRunning) {
//       // If is running, check the obstacle in front of the car first
//       get_front_distance();
//       serialRunCar(inChar);
//     }
//     else {
//       writeSerial('Car is not running');
//     }
//   }
// }

// /* Serial Control */

// char readSerial() {
//   if (Serial.available()) {
//     char inChar = Serial.read();
//     inChar = toupper(inChar);
//     return inChar;
//   }
//   return ' ';
// }

// void writeSerial(char c) {
//   Serial.print(c);
// }

// /* Motor Control */

// void serialRunCar(char inChar) {
//   // Else get the direction information from raspberry pi
//   if (inChar == 'W' || inChar == 'S' || inChar == 'A' || inChar == 'D' || inChar == 'X') {
//     // Continute to run the car if the car is already running in this direction
//     if (direction == inChar && (count_left > 0 || count_right > 0)) {
//       count_left = 0;
//       count_right = 0;
//     }
//     direction = inChar;
//   }
//   else
//   {
//     writeSerial('Invalid command');
//     writeSerial(inChar);
//   }

//   // Control the car based on the direction
//   switch (direction) {
//       case 'W':
//           carForward();
//           if ((count_left + count_right) > carRunForwardBackwardDistance) {
//             count_left = 0;
//             count_right = 0;
//             direction = 'X';
//           }
//           break;
//       case 'S':
//           carBackward();
//           if ((count_left + count_right) > carRunForwardBackwardDistance) {
//             count_left = 0;
//             count_right = 0;
//             direction = 'X';
//           }
//           break;
//       case 'A':
//           carLeft();
//           if ((count_left + count_right) > carRunLeftRightDistance) {
//             count_left = 0;
//             count_right = 0;
//             direction = 'X';
//           }
//           break;
//       case 'D':
//           carRight();
//           if ((count_left + count_right) > carRunLeftRightDistance) {
//             count_left = 0;
//             count_right = 0;
//             direction = 'X';
//           }
//           break;
//       default:
//           carStop();
//           break;
//     }
// }

// void runCar(int speed_front_left, int speed_front_right, int speed_back_left, int speed_back_right) {
//   runMotor(1, speed_front_left);
//   runMotor(2, speed_front_right);
//   runMotor(3, speed_back_left);
//   runMotor(4, speed_back_right);
// }

// void runMotor(int id, int speed) {
//   AF_DCMotor* motor;
//   switch (id) {
//     case 1:
//       motor = &motor_front_right;
//       break;
//     case 2:
//       motor = &motor_front_left;
//       break;
//     case 3:
//       motor = &motor_back_left;
//       break;
//     case 4:
//       motor = &motor_back_right;
//       break;
//     default:
//       return;
//   }

//   motor->setSpeed(speed);
//   motor->run(speed > 0 ? FORWARD : BACKWARD);
// }

// void carForward() {
//   runCar(carSpeed, carSpeed, carSpeed, carSpeed);
// }

// void carBackward() {
//   runCar(-carSpeed, -carSpeed, -carSpeed, -carSpeed);
// }

// void carLeft(int angle = -1) {
//   if (angle == -1) {
//     // turn left in original point
//     runCar(carSpeed, 0, 0, carSpeed);
//   }
//   else {
//     // TODO: Implement angle turn
//     //runCar(60, carSpeed, carSpeed, carSpeed);
//   }
// }

// void carRight(int angle = -1) {
//   if (angle == -1) {
//     // turn right in original point
//     runCar(0, carSpeed, carSpeed, 0);
//   }
//   else {
//     // TODO: Implement angle turn
//     //runCar(carSpeed, carSpeed, carSpeed, 60);
//   }
// }

// void carStop() {
//   runCar(0, 0, 0, 0);
// }

// /* Speed Encoder */

// void tick_left() {
//   count_left++;
// }

// void tick_right() {
//   count_right++;
// }

// /* Ultrasonic Sensor */

// void get_front_distance() {
//   digitalWrite(TRIGGER_FRONT, LOW);
//   delayMicroseconds(2);
//   digitalWrite(TRIGGER_FRONT, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(TRIGGER_FRONT, LOW);

//   duration_front = pulseIn(ECHO_FRONT, HIGH);
//   cm_front = duration_front / 58;
// }

#include <AFMotor.h>

/* Running Flag */

bool isRunning = true;

/* Motor */

const int carSpeed = 100;
const int carRunLeftRightDistance = 1;
const int carRunForwardBackwardDistance = 1;
char direction = 'X';

AF_DCMotor motor_front_right(3);
AF_DCMotor motor_front_left(4);
AF_DCMotor motor_back_left(1);
AF_DCMotor motor_back_right(2);

void serialRunCar(char inChar);
void carForward();
void carBackward();
void carLeft(int angle = -1);
void carRight(int angle = -1);
void carStop();
void carRotate();

/* Ultrasonic Sensor */

const int TRIGGER_FRONT = 32;
const int ECHO_FRONT = 30;
long duration_front;
int cm_front;
bool get_ball = false;

/* Serial */

char readSerial();
void writeSerial(char c);

void setup() {
  // Init serial
  Serial.begin(9600);
  // Init ultrasonic sensor
  pinMode(TRIGGER_FRONT, OUTPUT);
  pinMode(ECHO_FRONT, INPUT);
  // Init random seed
  randomSeed(analogRead(0));
}

void loop() {
  // Begin & Quit Running
  char inChar = readSerial();
  if (inChar == 'B') {
    isRunning = true;
    writeSerial('Begin running');
  }
  else if (inChar == 'Q') {
    isRunning = false;
    carStop();
    writeSerial('Quit running');
  }
  else {
    if (isRunning) {
      // If is running, check the obstacle in front of the car first
      get_front_distance();
      serialRunCar(inChar);
    }
    else {
      writeSerial('Car is not running');
    }
  }
}

/* Serial Control */

char readSerial() {
  if (Serial.available()) {
    char inChar = Serial.read();
    inChar = toupper(inChar);
    return inChar;
  }
  return ' ';
}

void writeSerial(char c) {
  Serial.print(c);
}

/* Motor Control */

void serialRunCar(char inChar) {
  // Else get the direction information from raspberry pi
  if (inChar == 'W' || inChar == 'S' || inChar == 'A' || inChar == 'D' || inChar == 'X') {
    // Continute to run the car if the car is already running in this direction
    direction = inChar;
  }
  else if(inChar == 'C') {
    carRotate();
  }
  else {
    writeSerial('Invalid command');
    writeSerial(inChar);
  }

  // Control the car based on the direction
  switch (direction) {
      case 'W':
          carForward();
          break;
      case 'S':
          carBackward();
          break;
      case 'A':
          carLeft();
          break;
      case 'D':
          carRight();
          break;
      default:
          carStop();
          break;
    }
}

void runCar(int speed_front_left, int speed_front_right, int speed_back_left, int speed_back_right) {
  runMotor(1, speed_front_left);
  runMotor(2, speed_front_right);
  runMotor(3, speed_back_left);
  runMotor(4, speed_back_right);
}

void runMotor(int id, int speed) {
  AF_DCMotor* motor;
  switch (id) {
    case 1:
      motor = &motor_front_right;
      break;
    case 2:
      motor = &motor_front_left;
      break;
    case 3:
      motor = &motor_back_left;
      break;
    case 4:
      motor = &motor_back_right;
      break;
    default:
      return;
  }

  motor->setSpeed(speed);
  motor->run(speed > 0 ? FORWARD : BACKWARD);
}

void carForward() {
    runCar(carSpeed, carSpeed, carSpeed, carSpeed);
    delay(100);
    direction = 'X';
}

void carBackward() {
    runCar(-carSpeed, -carSpeed, -carSpeed, -carSpeed);
    delay(100);
    direction = 'X';
}

void carLeft(int angle = -1) {
  if (angle == -1) {
    // turn left in original point
    runCar(carSpeed, -carSpeed, -carSpeed, carSpeed);
    delay(100);
    direction = 'X';
  }
  else {
    // TODO: Implement angle turn
    //runCar(60, carSpeed, carSpeed, carSpeed);
  }
}

void carRight(int angle = -1) {
  if (angle == -1) {
    // turn right in original point
    runCar(-carSpeed, carSpeed, carSpeed, -carSpeed);
    delay(100);
    direction = 'X';
  }
  else {
    // TODO: Implement angle turn
    //runCar(carSpeed, carSpeed, carSpeed, 60);
  }
}

void carStop() {
  runCar(0, 0, 0, 0);
}

void carRotate() {
  if (random(1, 3) == 1) {
    runCar(carSpeed, -carSpeed, -carSpeed, carSpeed);
    delay(3000);
  }
  else
  {
    runCar(-carSpeed, carSpeed, carSpeed, -carSpeed);
    delay(3000);
  }
  direction = 'X';
}

/* Ultrasonic Sensor */

void get_front_distance() {
  digitalWrite(TRIGGER_FRONT, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_FRONT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_FRONT, LOW);

  duration_front = pulseIn(ECHO_FRONT, HIGH);
  cm_front = duration_front / 58;
}
