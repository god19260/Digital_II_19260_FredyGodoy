#define rojo4 PB_2
#define verde4 PE_0
#define rojo3 PF_0
#define verde3 PB_7
#define rojo2 PB_6
#define verde2 PA_4
#define rojo1 PA_3
#define verde1 PA_2
#define park4 PB_4
#define park3 PA_5
#define park2 PA_6
#define park1 PA_7
int  P4;
int  P3;
int  P2;
int  P1;

int  P8;
int  P7;
int  P6;
int  P5;
int slavepark = 0;
//tivamaestra
void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(115200);
  // put your setup code here, to run once:
 pinMode(rojo4, OUTPUT);
 pinMode(rojo3, OUTPUT);
 pinMode(rojo2, OUTPUT);
 pinMode(rojo1, OUTPUT);
 pinMode(verde4, OUTPUT);
 pinMode(verde3, OUTPUT);
 pinMode(verde2, OUTPUT);
 pinMode(verde1, OUTPUT);
 pinMode(park4, INPUT_PULLUP);
 pinMode(park3, INPUT_PULLUP);
 pinMode(park2, INPUT_PULLUP);
 pinMode(park1, INPUT_PULLUP);
 digitalWrite(rojo4, LOW);
 digitalWrite(rojo3, LOW);
 digitalWrite(rojo2, LOW);
 digitalWrite(rojo1, LOW);
 digitalWrite(verde4, LOW);
 digitalWrite(verde3, LOW);
 digitalWrite(verde2, LOW);
 digitalWrite(verde1, LOW);
}

void loop() {
  if (Serial2.available()){   
    P4= Serial2.read();
    delay(10);
    P3= Serial2.read();
    delay(10);
    P2= Serial2.read();
    delay(10);
    P1= Serial2.read();
    Serial2.flush();
  }

  delay(250);
  // put your main code here, to run repeatedly: 
/*  digitalWrite(rojo4, HIGH);
  delay(500);
  digitalWrite(rojo4, LOW);
  delay(500);
  digitalWrite(verde4, HIGH);
  delay(500);
  digitalWrite(verde4, LOW);
  delay(500);
  digitalWrite(rojo3, HIGH);
  delay(500);
  digitalWrite(rojo3, LOW);
  delay(500);
  digitalWrite(verde3, HIGH);
  delay(500);
  digitalWrite(verde3, LOW);
  delay(500);
  digitalWrite(rojo2, HIGH);
  delay(500);
  digitalWrite(rojo2, LOW);
  delay(500);
  digitalWrite(verde2, HIGH);
  delay(500);
  digitalWrite(verde2, LOW);
  delay(500);
  digitalWrite(rojo1, HIGH);
  delay(500);
  digitalWrite(rojo1, LOW);
  delay(500);
  digitalWrite(verde1, HIGH);
  delay(500);
  digitalWrite(verde1, LOW);
  delay(500);
  */
  if (digitalRead(park4) == HIGH){
    digitalWrite(rojo4, LOW);
    digitalWrite(verde4, HIGH);
    P8=1;
  }
  else { 
    digitalWrite(rojo4, HIGH);
    digitalWrite(verde4, LOW);
    P8=0;
  }
  
  if (digitalRead(park3) == HIGH){
    digitalWrite(rojo3, LOW);
    digitalWrite(verde3, HIGH);
    P7=1;
  }
  else { 
    digitalWrite(rojo3, HIGH);
    digitalWrite(verde3, LOW);
    P7=0;
  }

  if (digitalRead(park2) == HIGH){
    digitalWrite(rojo2, LOW);
    digitalWrite(verde2, HIGH);
    P6=1;
  }
  else { 
    digitalWrite(rojo2, HIGH);
    digitalWrite(verde2, LOW);
    P6=0;
  }
  if (digitalRead(park1) == HIGH){
    digitalWrite(rojo1, LOW);
    digitalWrite(verde1, HIGH);
    P5=1;
  }
  else { 
    digitalWrite(rojo1, HIGH);
    digitalWrite(verde1, LOW);
    P5=0;
  }
  if(P1 == 49){
    Serial3.print(1);
    Serial.print(1);
  }else if(P1 == 48){
    Serial3.print(0);
    Serial.print(0);
  } 
  if(P2 == 49){
    Serial3.print(1);
    Serial.print(1);
  }else if(P2 == 48){
    Serial3.print(0);
    Serial.print(0);
  }
  if(P3 == 49){
    Serial3.print(1);
    Serial.print(1);
  }else if(P3 == 48){
    Serial3.print(0);
    Serial.print(0);
  }
  if(P4 == 49){
    Serial3.print(1);
    Serial.print(1);
  }else if(P4 == 48){
    Serial3.print(0);
    Serial.print(0);
  }
  Serial3.print(P5);
  Serial3.print(P6);
  Serial3.print(P7);
  Serial3.print(P8);


  Serial.print(P5);
  Serial.print(P6);
  Serial.print(P7);
  Serial.print(P8);
  Serial.println("");
  Serial.println("");
}
