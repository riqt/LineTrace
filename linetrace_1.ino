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
//Weight
#define W_VL -10.0
#define W_L -5.0
#define W_CL -1.2
#define W_C 0.0
#define W_CR 1.2
#define W_R 5.0
#define W_VR 10.0
//Speed
#define SPEED 160
#define SUB1 30
#define SUB2 50
#define SUB3 100
#define T_SPEED 255
#define B_TH 39
#define B_SPEED 0
#define COEF 10  // turn_L(R)の引数


boolean l_flag = false;
boolean r_flag = false;
boolean err_flag = false;

void setup() {
  pinMode(L_IN1, OUTPUT);
  pinMode(L_IN2, OUTPUT);
  pinMode(R_IN1, OUTPUT);
  pinMode(R_IN2, OUTPUT);
  Serial.begin(9600);

}

void loop() {
  int L = analogRead(SENSOR_L);
  int CL = analogRead(SENSOR_CL);
  int C = analogRead(SENSOR_C);
  int CR = analogRead(SENSOR_CR);
  int R = analogRead(SENSOR_R);
  
  double weight = 0;
  int counter = 0;
  int l_sum = 0;
  int r_sum = 0;

  int sub1 = 0;
  int sub2 = 0;
  int sub3 = 0;

  if(C > TH_C){
    weight += W_C;
    ++counter;
  }
  if(L > TH_L){
      weight += W_L;
      ++counter;
      ++l_sum;
      sub2 = SUB2;
  }
  if(CL > TH_CL){
      weight += W_CL;
      ++counter;
      ++l_sum;
      sub1 = SUB1;
  }
  if(CR > TH_CR){
      weight += W_CR;
      ++counter;
      ++r_sum;
      sub1 = SUB1;
  }
  if(R > TH_R){
      weight += W_R;
      ++counter;
      ++r_sum;
      sub2 = SUB2;
  }

  if(l_sum == r_sum){

  }else if(l_sum > r_sum){
    l_flag = true;
    r_flag = false;
  }else{
    l_flag = false;
    r_flag = true;
  }
  
  if(counter == 0){
    //sub3 = SUB3;
    if(l_flag == true){
      weight += W_VL;
      ++counter;
    }else if(r_flag == true){
      weight += W_VR;
      ++counter;
    }else{
      weight = 0;
      counter = 1;
    }
  }

  weight = abs(weight);
  weight /= counter;
  weight += 1;

  if(l_flag == true){
    turn_l(SPEED - sub1 - sub2, weight);
  }else if(r_flag == true){
    turn_r(SPEED - sub1 - sub2 - sub3, weight);
  }else{
    turn_r(SPEED - sub1 - sub2 - sub3, 1.0);
  }
}


void turn_l(int speed, double weight){
  int l_speed = SPEED;
  int r_speed = SPEED;
  
  if(speed*weight > T_SPEED){
    r_speed = T_SPEED;
  }else{
    r_speed = (int)speed*weight;
  }
  if(speed/weight < B_TH){
    l_speed = B_SPEED;
  }else{
    l_speed = (int)speed/weight;
  }
  
  analogWrite(L_VREF, l_speed);
  analogWrite(R_VREF, r_speed);
  digitalWrite(L_IN1, HIGH);
  digitalWrite(L_IN2, LOW);
  digitalWrite(R_IN1, HIGH);
  digitalWrite(R_IN2, LOW);

  Serial.print(speed);
  Serial.print(" ");
  Serial.print(weight);
  Serial.print(" ");
  Serial.print(l_speed);
  Serial.print(" ");
  Serial.print(r_speed);
  Serial.print(" \n");
}

void turn_r(int speed, double weight){
  int l_speed = SPEED;
  int r_speed = SPEED;
  
  if(speed*weight > T_SPEED){
    l_speed = T_SPEED;
  }else{
    l_speed = (int)speed*weight;
  }
  if(speed/weight < B_TH){
    r_speed = B_SPEED;
  }else{
    r_speed = (int)speed/weight;
  }
  
  analogWrite(L_VREF, l_speed);
  analogWrite(R_VREF, r_speed);
  digitalWrite(L_IN1, HIGH);
  digitalWrite(L_IN2, LOW);
  digitalWrite(R_IN1, HIGH);
  digitalWrite(R_IN2, LOW);

  Serial.print(speed);
  Serial.print(" ");
  Serial.print(weight);
  Serial.print(" ");
  Serial.print(l_speed);
  Serial.print(" ");
  Serial.print(r_speed);
  Serial.print(" \n");
}
