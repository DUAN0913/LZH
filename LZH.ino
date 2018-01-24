#include<Servo.h>
#define acidMotor 3//酸舵机
#define sodaMotor 5//碱舵机
#define alumMotor 6//明矾舵机
#define ph A1//PH，A1口
#define Turbidity A0//浑浊度，A0口
char c;//接收蓝牙信号
int acidpos = 0;//酸当前位置
int sodapos = 0;//碱当前位置
int alumpos = 0;//明矾当前位置
int i = 0;//酸位置计数
int j = 0;//碱位置计算
int k = 0;//明矾位置计算
float v;//处理浑浊度信号
float a;//浑浊度信号
float b;//PH信号
int flagAll = 0;//标识位
Servo acidServo;//酸舵机定义
Servo sodaServo;//碱舵机定义
Servo alumServo;//明矾舵机定义

void setup() {
  Serial.begin(9600);
  acidServo.attach(acidMotor);
  sodaServo.attach(sodaMotor);
  alumServo.attach(alumMotor);
  pinMode(turbidity,INPUT);
  pinMode(PH,INPUT);
}
/*下面是发送浑浊度程度*/
void turbidity(){
  Serial.print(v);
  delay(500);
  }
/*下面是发送PH值*/
void PH(){
  Serial.print(b);
  delay(500);
  }
/*急停*/
void stopp(){
  acidServo.write(0);
  sodaServo.write(0);
  alumServo.write(0);
  }
/*恢复*/
void recover(){
  for(acidpos = j;acidpos>0;acidpos--){
    acidServo.write(acidpos);
    delay(15);
    }
  for(sodapos = i;sodapos>0;sodapos--){
    sodaServo.write(sodapos);
    delay(15);
    }
  for(alumpos = k;alumpos>0;alumpos--){
    alumServo.write(alumpos);
    delay(15);
    }
   flagAll = 1;
  }
/*主程序*/
void loop() {
  c = Serial.read();
  
  a = analogRead(Turbidity);
  v = a * (5.0 / 1024.0);
  
  b = analogRead(ph);
  if(c == '5'){//蓝牙接收指令5，开始工作
    flagAll = 1;
    delay(10);
    }
  if(flagAll == 1){
    if(v>4){//如果水中浑浊度高
      do{
        alumpos = alumpos+1;
        k = k+1;
        if(alumpos == 90){alumpos = 90;}
        alumServo.write(alumpos);
        delay(15);
        }while(v<4);//直到低下来停止，下面三个同理
      for(alumpos = alumpos;alumpos>0;alumpos--){
        alumServo.write(alumpos);
        delay(15);
        }//回初始位置
      }
    if(b>3){//酸过高
      do{
        acidpos = acidpos+1;
        j = j+1;
        if(acidpos == 90){acidpos = 90;}
        acidServo.write(acidpos);
        delay(15);
        }while(b<3);
      for(acidpos = acidpos;acidpos>0;acidpos--){
        acidServo.write(acidpos);
        delay(15);
        }
      }
    if(b<2){//碱过高
      do{
        sodapos = sodapos+1;
        i = i+1;
        if(alumpos == 90){alumpos = 90;}
        sodaServo.write(sodapos);
        delay(15);
        }while(b>2);
      for(sodapos = sodapos;sodapos>0;sodapos--){
        alumServo.write(sodapos);
        delay(15);
        }
      }
    if(c == '2'){//蓝牙发送2指令，向上位机反馈PH值
      PH();
      }
    if(c == '3'){//蓝牙发送3指令，向上位机反馈浑浊度值
      turbidity();
      }
    if(c == '0'){//蓝牙发送0指令，总标识位变0
      flagAll = 0;
      }
    }
    if(flagAll == 0){//立马变0
      stopp();
      if(c == '1'){//蓝牙发送1指令，恢复初始位置
        recover();
        }
      }
}
