//Motor Driver L : Digital Pin
#define L_IN1 4
#define L_IN2 5
#define L_VREF 6
//Motor Driver R : Digital Pin
#define R_IN1 7
#define R_IN2 8
#define R_VREF 9
//Sensor : Analog In
#define SENSOR_L A3
#define SENSOR_CL A0
#define SENSOR_C A1
#define SENSOR_CR A2
#define SENSOR_R A4
//Sensor Thereshold
#define TH_L 980 
#define TH_CL 980
#define TH_C 980
#define TH_CR 980
#define TH_R 980

//Speed
#define SPEED 160
#define C_SPEED 100
#define COEF_2 0.5
#define COEF_3 0.25
#define COEF_4 0.0

boolean l_flag = false;
boolean r_flag = false;

void setup() {
  pinMode(L_IN1, OUTPUT);
  pinMode(L_IN2, OUTPUT);
  pinMode(R_IN1, OUTPUT);
  pinMode(R_IN2, OUTPUT);
  Serial.begin(9600);

  byte state;
}

void loop() {
  int L = analogRead(SENSOR_L);
  int CL = analogRead(SENSOR_CL);
  int C = analogRead(SENSOR_C);
  int CR = analogRead(SENSOR_CR);
  int R = analogRead(SENSOR_R);

  byte state = 0;
  int l_speed = 0;
  int r_speed = 0;

  if(L <= TH_L && CL <= TH_CL && C <= TH_C && CR <= TH_CR && R <= TH_R){
    state = 0;
  } else if(L <= TH_L && CL <= TH_CL && C > TH_C && CR <= TH_CR && R <= TH_R){
    state = 1;
  } else if(L <= TH_L && CL > TH_CL && C <= TH_C && CR <= TH_CR && R <= TH_R){
    state = 1;
    l_flag = true;
    r_flag = false;
  } else if(L <= TH_L && CL <= TH_CL && C <= TH_C && CR > TH_CR && R <= TH_R){
    state = 1;
    l_flag = false;
    r_flag = true;
  } else if(L <= TH_L && CL > TH_CL && C > TH_C && CR <= TH_CR){  // Rによらない
    state = 2;
    l_flag = true;
    r_flag = false;
  } else if(CL <= TH_CL && C > TH_C && CR > TH_CR && R <= TH_R){  // Lによらない
    state = 2;
    l_flag = false;
    r_flag = true;
  } else if(L > TH_L && CL > TH_CL && C <= TH_C && CR <= TH_CR){  // Rによらない
    state = 3;
    l_flag = true;
    r_flag = false;
  } else if(CL <= TH_CL && C <= TH_C && CR > TH_CR && R > TH_R){  // Lによらない
    state = 3;
    l_flag = false;
    r_flag = true;
  } else if(L > TH_L && CL <= TH_CL && C <= TH_C && CR <= TH_CR && R <= TH_R){
    state = 4;
    l_flag = true;
    r_flag = false;
  } else if(L <= TH_L && CL <= TH_CL && C <= TH_C && CR <= TH_CR && R > TH_R){
    state = 4;
    l_flag = false;
    r_flag = true;
  } else {
    state = 5;
  }

  switch(state){
    // B = 1, W = 0, X = {0,1}
    // コースアウト : (0,0,0,0,0)
    case 0:
      break;

    // 車体がラインと平行 : (0,0,1,0,0), (0,0,0,1,0), (0,1,0,0,0)
    case 1:
      turn(SPEED, SPEED);
      break;
      
    // ラインに対してやや斜め : (X,0,1,1,0), (0,1,1,0,X)
    case 2:
    if (l_flag){
      turn(COEF_2*C_SPEED, C_SPEED);
    } else if (r_flag) {
      turn(C_SPEED, COEF_2*C_SPEED);
    }
      break;

    // ラインに対して斜め : (X,0,0,1,1), (1,1,0,0,X)
    case 3:
    if (l_flag){
      turn(COEF_3*C_SPEED, C_SPEED);
    } else if (r_flag) {
      turn(C_SPEED, COEF_3*C_SPEED);
    }
      break;

    // ラインに対してかなり斜め : (1,0,0,0,0), (0,0,0,0,1)
    case 4:
    if (l_flag){
      turn(COEF_4*C_SPEED, C_SPEED);
    } else if (r_flag) {
      turn(C_SPEED, COEF_4*C_SPEED);
    }
      break;

    // 想定されない挙動
    case 5:
      break;

    default:
      break;
  }
  
}

void back(int speed){
  analogWrite(L_VREF, speed);
  analogWrite(R_VREF, speed);
  digitalWrite(L_IN1, LOW);
  digitalWrite(L_IN2, HIGH);
  digitalWrite(R_IN1, LOW);
  digitalWrite(R_IN2, HIGH);
}
void turn(int l_speed, int r_speed){
  analogWrite(L_VREF, l_speed);
  analogWrite(R_VREF, r_speed);
  digitalWrite(L_IN1, HIGH);
  digitalWrite(L_IN2, LOW);
  digitalWrite(R_IN1, HIGH);
  digitalWrite(R_IN2, LOW);
}
