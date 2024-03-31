#include <NewPing.h>  //Download this library by going to <Tools> & then <Manage Libraries>
#include <Servo.h>    //Download this library by going to <Tools> & then <Manage Libraries>
Servo out1; //roll
Servo out2; //pitch
int trig1 = 2;
int echo1 = 2;
int trig2 = A1;
int echo2 = A1;
int trig3 = 4;
int echo3 = 4;
int trig4 = 5;
int echo4 = 5;
int MAX_DISTANCE = 400;               
NewPing sonar1(trig1,echo1,MAX_DISTANCE);
NewPing sonar2(trig2,echo2,MAX_DISTANCE);
NewPing sonar3(trig3,echo3,MAX_DISTANCE);
NewPing sonar4(trig4,echo4,MAX_DISTANCE);
float dist1,dist2,dist3,dist4; 
unsigned long counter_1, counter_2, counter_3, current_count;
byte last_CH1_state, last_CH2_state, last_CH3_state;
int input_PITCH;    //input on D9 of arduino
int input_ROLL;     //input on D8 of arduino
int prev_time = 0; 
void setup(){    
 // Serial.begin(115200);
PCICR |= (1 << PCIE0);                                                   
PCMSK0 |= (1 << PCINT0);  
PCMSK0 |= (1 << PCINT1);                                              
PCMSK0 |= (1 << PCINT2);
  out1.attach(13); //roll
  out2.attach(11); //pitch
pinMode(A0,INPUT);
}

void loop(){
  int a=160;
  int b=160;
  int c=160;
  int d=160;
  int e=160;

if(input_ROLL>1600){
d=210;  
}
else if(input_ROLL<1400){
c=210;  
}
if(input_PITCH>1600){
a=210;  
}
else if(input_PITCH<1400){
b=210;  
}

dist1 = sonar1.ping_cm();
dist2 = sonar2.ping_cm();
dist3 = sonar3.ping_cm();
dist4 = sonar4.ping_cm();
 
byte roll = map(input_ROLL, 1108, 1856, 55, 125); //maping roll
byte pitch = map(input_PITCH, 1180, 1856, 55, 125); //maping pitch

 byte antifront = map(dist1, 6, 210, 55, 60); //force to back
 byte antiback = map(dist2, 6, 210, 125, 120); //force to front
 byte antileft = map(dist3, 6, 210, 125, 120); //force to right
 byte antiright = map(dist4, 6, 210, 55, 60); //force to left  
int val = pulseIn(A0,HIGH);
if(val < 1500){

//    For Pitch..............
if(dist1<a && dist1>0 && (dist2>b || dist2==0)){
  out2.write(antifront);
  b=210;
}
else if((dist1>=a || dist1==0) && (dist2>=b || dist2==0)){
  out2.write(pitch);
}
else if(dist2<b && dist2>0 && (dist1>a || dist1==0)){
  out2.write(antiback);
  a=210;
}
else if(dist1<a && dist2<b && dist1>0 && dist2>0){
  out2.write(92);
  c=210;
  d=210;
  e=210;
    if(dist3>c || dist3==0){
    out1.write(55);
   }
   else if(dist4>d || dist4==0){
    out1.write(125);
   }
}


//    For Roll..............
if(dist3<c && dist3>0 && (dist4>d || dist4==0)){
  out1.write(antileft);
  d=210;
}
else if(dist3>=c || dist3==0 && dist4>=d || dist4==0){
  out1.write(roll);
}
else if(dist4<d && dist4>0 && (dist3>c || dist3==0)){
  out1.write(antiright);
  c=210;
}
else if(dist3<c && dist4<d && dist3>0 && dist4>0){
  out1.write(92);
  a=210;
  b=210;
  e=210;  
  if(dist1>a || dist1 ==0){
    out2.write(125);
  }
  else if(dist2>b || dist2 ==0){
    out2.write(55);
  }
}





int FB = (dist1<a && dist2<b && dist1>0 && dist2>0); 
int LR = (dist3<c && dist4<d && dist3>0 && dist4>0); 

if(FB && LR){
  out1.write(92);
  out2.write(92); 
}



}

else if(val >= 1500){
  out1.write(roll);
  out2.write(pitch);
}



int f_clear = (dist1>a || dist1==0);  //front no obstacle
int b_clear = (dist2>b || dist2==0);   //back no obstacle
int l_clear = (dist3>c || dist3==0);   //left no obstacle
int r_clear = (dist4>d || dist4==0);  //right no obstacle

if(f_clear && b_clear && l_clear && r_clear  ){
   if(val<1500){
  out1.write(roll);
  out2.write(pitch);  
   }
}


} //Here the main loop ends


//   The Below Codes Reads the PWM value of Receiver
ISR(PCINT0_vect){
current_count = micros();
if(PINB & B00000001){                              
    if(last_CH1_state == 0){                         
      last_CH1_state = 1;                           
      counter_1 = current_count;                     
  }
}
else if(last_CH1_state == 1){                        
last_CH1_state = 0;                             
input_ROLL = current_count - counter_1;  
}
if(PINB & B00000010 ){                                                                          
    if(last_CH2_state == 0){                                               
      last_CH2_state = 1;                                                   
      counter_2 = current_count;                                             
  }
}
else if(last_CH2_state == 1){                                           
last_CH2_state = 0;                                                     
input_PITCH = current_count - counter_2;                             
} 
if(PINB & B00000100 ){                                                                   
   if(last_CH3_state == 0){                                             
      last_CH3_state = 1;                                                  
      counter_3 = current_count;                                               
  }
}
else if(last_CH3_state == 1){                                             
last_CH3_state = 0;                                                                              
} 
}