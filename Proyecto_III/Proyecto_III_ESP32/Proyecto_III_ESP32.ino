#include <WiFi.h>
#include <WebServer.h>

/* Put your SSID & Password */
const char* ssid = "Fredy";  // Enter SSID here
const char* password = "123";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(189,168,1,1);
IPAddress gateway(189,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);
uint8_t D_c = 9;
uint8_t D_d = 8;
uint8_t D_e = 7;
uint8_t D_b = 2;
uint8_t D_a = 3;
uint8_t D_f = 1;
uint8_t D_g = 0;

int LED1status = LOW;
int LED2status = LOW;
int LED3status = LOW;
int LED4status = LOW;
int LED5status = LOW;
int LED6status = LOW;
int LED7status = LOW;
int LED8status = LOW;
int Lectura_P1 = 0;
int Lectura_P2 = 0;
int Lectura_P3 = 0;
int Lectura_P4 = 0;
int Lectura_P5 = 0;
int Lectura_P6 = 0;
int Lectura_P7 = 0;
int Lectura_P8 = 0;
int  P_Disponibles = 10;
void setup() {
  pinMode(D_c, OUTPUT);
  pinMode(D_d, OUTPUT);
  pinMode(D_e, OUTPUT);
  pinMode(D_b, OUTPUT);
  pinMode(D_a, OUTPUT);
  pinMode(D_f, OUTPUT);
  pinMode(D_g, OUTPUT);
  Serial.begin(115200);
  Serial1.begin(115200);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);
  server.on("/parqueos", handle_parqueos);
  server.onNotFound(handle_NotFound);
  
  server.begin();
  //Serial.println("HTTP server started");
  P_Disponibles = 0;
}
void loop() {
  if(Serial.available()){
    //Serial.println("Lectura");
    
    Lectura_P1 = Serial.read();
    Lectura_P2 = Serial.read();
    Lectura_P3 = Serial.read();
    Lectura_P4 = Serial.read();
    Lectura_P5 = Serial.read();
    Lectura_P6 = Serial.read();
    Lectura_P7 = Serial.read();
    Lectura_P8 = Serial.read();
    Serial.flush();  
  }

  if(Lectura_P1 == '1'){
    LED1status = HIGH;
  } else if(Lectura_P1 == '0'){
    LED1status = LOW;
  }
  if(Lectura_P2 == '1'){
    LED2status = HIGH;
  } else if(Lectura_P2 == '0'){
    LED2status = LOW;
  }
  if(Lectura_P3 == '1'){
    LED3status = HIGH;
  } else if(Lectura_P3 == '0'){
    LED3status = LOW;
  }
  if(Lectura_P4 == '1'){
    LED4status = HIGH;
  } else if(Lectura_P4 == '0'){
    LED4status = LOW;
  }
  if(Lectura_P5 == '1'){
    LED5status = HIGH;
  } else if(Lectura_P5 == '0'){
    LED5status = LOW;
  }
  if(Lectura_P6 == '1'){
    LED6status = HIGH;
  } else if(Lectura_P6 == '0'){
    LED6status = LOW;
  }
  if(Lectura_P7 == '1'){
    LED7status = HIGH;
  } else if(Lectura_P7 == '0'){
    LED7status = LOW;
  }
  if(Lectura_P8 == '1'){
    LED8status = HIGH;
  } else if(Lectura_P8 == '0'){
    LED8status = LOW;
  }

  // -- Parqueos disponibles --
  P_Disponibles = 0;
  if(LED1status==HIGH){
    P_Disponibles++;
  }
  if(LED2status==HIGH){
    P_Disponibles++;
  }
  if(LED3status==HIGH){
    P_Disponibles++;
  }
  if(LED4status==HIGH){
    P_Disponibles++;
  }
  if(LED5status==HIGH){
    P_Disponibles++;
  }
  if(LED6status==HIGH){
    P_Disponibles++;
  }
  if(LED7status==HIGH){
    P_Disponibles++;
  }
  if(LED8status==HIGH){
    P_Disponibles++;
  }
  // --- Conexion con servidor --- 
  server.begin();
  delay(1500);
  Display(P_Disponibles);
  
  server.handleClient();
  server.close();
}

void handle_OnConnect() {
  //Serial.println("GPIO4 Status: OFF | GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(P_Disponibles,LED1status,LED2status,LED3status,LED4status,LED5status,LED6status,LED7status,LED8status)); 
}

void handle_parqueos() {
  //Serial.println("GPIO4 Status: ON");
  server.send(200, "text/html", SendHTML(P_Disponibles,LED1status,LED2status,LED3status,LED4status,LED5status,LED6status,LED7status,LED8status)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t Disponibles,uint8_t led1stat,uint8_t led2stat,uint8_t led3stat,uint8_t led4stat,uint8_t led5stat,uint8_t led6stat,uint8_t led7stat,uint8_t led8stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<META http-equiv=refresh content = 1>\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin-top: 50px ;} h3 {color: #444444;margin-bottom: 10px;}\n";
  
  ptr +=".button {display: block;width: 130px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #00FF00;}\n";
  ptr +=".button-Informacion {background-color: #0D9FC4;}\n";
//ptr +=".button-on:active {background-color: #2980b9;}\n";
  ptr +=".button-off {background-color: #FF0000;}\n";
//ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>Parqueo-Matic</h1>\n";
  ptr +="<h3>Fredy Godoy 19260</h3>\n";
  ptr +="<h3>Alejandro Rodriguez 19131</h3>\n";
  
  //Parqueos disponibles
  if(Disponibles == 1){
    ptr +="<a class=\"button button-Informacion\" href=\"/parqueos\">Parqueos Disponibles: 1</a>\n"; 
  }else if(Disponibles == 2){
    ptr +="<a class=\"button button-Informacion\" href=\"/parqueos\">Parqueos Disponibles: 2</a>\n";
  }else if(Disponibles == 3){
    ptr +="<a class=\"button button-Informacion\" href=\"/parqueos\">Parqueos Disponibles: 3</a>\n";
  }else if(Disponibles == 4){
    ptr +="<a class=\"button button-Informacion\" href=\"/parqueos\">Parqueos Disponibles: 4</a>\n";
  }else if(Disponibles == 5){
    ptr +="<a class=\"button button-Informacion\" href=\"/parqueos\">Parqueos Disponibles: 5</a>\n";
  }else if(Disponibles == 6){
    ptr +="<a class=\"button button-Informacion\" href=\"/parqueos\">Parqueos Disponibles: 6</a>\n";
  }else if(Disponibles == 7){
    ptr +="<a class=\"button button-Informacion\" href=\"/parqueos\">Parqueos Disponibles: 7</a>\n";
  }else if(Disponibles == 8){
    ptr +="<a class=\"button button-Informacion\" href=\"/parqueos\">Parqueos Disponibles: 8</a>\n";
  }else if(Disponibles == 0){
    ptr +="<a class=\"button button-Informacion\" href=\"/parqueos\">Parqueos Disponibles: 0</a>\n";
  }
  
  
  //Parqueo 1
  if(led1stat){
    ptr +="<a class=\"button button-on\" href=\"/parqueos\">Parqueo 1</a>\n";
    }
  else{
    ptr +="<a class=\"button button-off\" href=\"/parqueos\">Parqueo 1</a>\n";
    }
  // Parqueo 2
  if(led2stat){
    ptr +="<a class=\"button button-on\" href=\"/parqueos\">Parqueo 2</a>\n";
    }
  else{
    ptr +="<a class=\"button button-off\" href=\"/parqueos\">Parqueo 2</a>\n";
    }
  // Parqueo 3
  if(led3stat){
    ptr +="<a class=\"button button-on\" href=\"/parqueos\">Parqueo 3</a>\n";
    }
  else{
    ptr +="<a class=\"button button-off\" href=\"/parqueos\">Parqueo 3</a>\n";
    }
  // Parqueo 4
  if(led4stat){
    ptr +="<a class=\"button button-on\" href=\"/parqueos\">Parqueo 4</a>\n";
    }
  else{
    ptr +="<a class=\"button button-off\" href=\"/parqueos\">Parqueo 4</a>\n";
    }
  // Parqueo 5
  if(led5stat){
    ptr +="<a class=\"button button-on\" href=\"/parqueos\">Parqueo 5</a>\n";
    }
  else{
    ptr +="<a class=\"button button-off\" href=\"/parqueos\">Parqueo 5</a>\n";
    }
  // Parqueo 6
  if(led6stat){
    ptr +="<a class=\"button button-on\" href=\"/parqueos\">Parqueo 6</a>\n";
    }
  else{
    ptr +="<a class=\"button button-off\" href=\"/parqueos\">Parqueo 6</a>\n";
    }
  // Parqueo 7
  if(led7stat){
    ptr +="<a class=\"button button-on\" href=\"/parqueos\">Parqueo 7</a>\n";
    }
  else{
    ptr +="<a class=\"button button-off\" href=\"/parqueos\">Parqueo 7</a>\n";
    }
  // Parqueo 8
  if(led8stat){
    ptr +="<a class=\"button button-on\" href=\"/parqueos\">Parqueo 8</a>\n";
    }
  else{
    ptr +="<a class=\"button button-off\" href=\"/parqueos\">Parqueo 8</a>\n";
    }
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}

// --- Funciones ---
void Display(int numero){
  if(numero == 1){
    digitalWrite(D_c,HIGH);
    digitalWrite(D_d,LOW);
    digitalWrite(D_e,LOW);
    digitalWrite(D_b,HIGH);
    digitalWrite(D_a,LOW);
    digitalWrite(D_f,LOW);
    digitalWrite(D_g,LOW);
  }else if(numero == 2){
    digitalWrite(D_c,LOW);
    digitalWrite(D_d,HIGH);
    digitalWrite(D_e,HIGH);
    digitalWrite(D_b,HIGH);
    digitalWrite(D_a,HIGH);
    digitalWrite(D_f,LOW);
    digitalWrite(D_g,HIGH);
  }else if(numero == 3){
    digitalWrite(D_c,HIGH);
    digitalWrite(D_d,HIGH);
    digitalWrite(D_e,LOW);
    digitalWrite(D_b,HIGH);
    digitalWrite(D_a,HIGH);
    digitalWrite(D_f,LOW);
    digitalWrite(D_g,HIGH);
  }else if(numero == 4){
    digitalWrite(D_c,HIGH);
    digitalWrite(D_d,LOW);
    digitalWrite(D_e,LOW);
    digitalWrite(D_b,HIGH);
    digitalWrite(D_a,LOW);
    digitalWrite(D_f,HIGH);
    digitalWrite(D_g,HIGH);
  }else if(numero == 5){
    digitalWrite(D_c,HIGH);
    digitalWrite(D_d,HIGH);
    digitalWrite(D_e,LOW);
    digitalWrite(D_b,LOW);
    digitalWrite(D_a,HIGH);
    digitalWrite(D_f,HIGH);
    digitalWrite(D_g,HIGH);
  }else if(numero == 6){
    digitalWrite(D_c,HIGH);
    digitalWrite(D_d,HIGH);
    digitalWrite(D_e,HIGH);
    digitalWrite(D_b,LOW);
    digitalWrite(D_a,HIGH);
    digitalWrite(D_f,HIGH);
    digitalWrite(D_g,HIGH);
  }else if(numero == 7){
    digitalWrite(D_c,HIGH);
    digitalWrite(D_d,LOW);
    digitalWrite(D_e,LOW);
    digitalWrite(D_b,HIGH);
    digitalWrite(D_a,HIGH);
    digitalWrite(D_f,LOW);
    digitalWrite(D_g,LOW);
  }else if(numero == 8){
    digitalWrite(D_c,HIGH);
    digitalWrite(D_d,HIGH);
    digitalWrite(D_e,HIGH);
    digitalWrite(D_b,HIGH);
    digitalWrite(D_a,HIGH);
    digitalWrite(D_f,HIGH);
    digitalWrite(D_g,HIGH);
  }else if(numero == 9){
    digitalWrite(D_c,HIGH);
    digitalWrite(D_d,HIGH);
    digitalWrite(D_e,LOW);
    digitalWrite(D_b,HIGH);
    digitalWrite(D_a,HIGH);
    digitalWrite(D_f,HIGH);
    digitalWrite(D_g,HIGH);
  }else if(numero == 0){
    digitalWrite(D_c,HIGH);
    digitalWrite(D_d,HIGH);
    digitalWrite(D_e,HIGH);
    digitalWrite(D_b,HIGH);
    digitalWrite(D_a,HIGH);
    digitalWrite(D_f,HIGH);
    digitalWrite(D_g,LOW);
  }
}
