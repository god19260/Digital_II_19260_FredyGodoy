/*
Electronica Digital II  
Laboratorio No.06
Autor: Fredy Godoy 19260
*/

// ------------------------------------- Variables --------------------------------------
//int ledRed = 30, ledBlue = 40, ledGreen = 39;    // LED connected to digital pin 9
int tiempo = 800;
int Sw1_Flag = 0;
int Sw2_Flag = 0;
int Cont_1, Cont_2 = 0;
// --------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------
void setup() {
  // put your setup code here, to run once:
  pinMode(PUSH1, INPUT_PULLUP); 
  pinMode(PUSH2, INPUT_PULLUP); 
}
// ----------------------------------- Loop Principal -----------------------------------
// --------------------------------------------------------------------------------------
void loop() { 
  Sw_Flags(); 
  if((digitalRead(PUSH1) == LOW or digitalRead(PUSH2) == LOW) && (Sw1_Flag == 0 && Sw2_Flag == 0)){
    Semaforo();
    Cont_1 = 0;
    Cont_2 = 0;
    // antirrebote para el inicio de la competencia
    Sw1_Flag = 1;
    Sw2_Flag = 1;
    while (Cont_1 <= 255 && Cont_2 <= 255){
      Sw_Flags();
      
      // Acción Jugador 1
      if(digitalRead(PUSH1) == LOW && Sw1_Flag == 0){
        Sw1_Flag = 1;
        if(Cont_1 == 0){
          Cont_1 = 1;
          analogWrite(RED_LED, 155);  // verde claro
          analogWrite(GREEN_LED, 255);
        }else {
          Cont_1 = Cont_1*2 ;
          analogWrite(RED_LED, 155); // verde claro
          analogWrite(GREEN_LED, 255);
        }
      }
      // Acción Jugador 2
      if(digitalRead(PUSH2) == LOW && Sw2_Flag == 0){
        Sw2_Flag = 1;
        if(Cont_2 == 0){
          Cont_2 = 1;
          analogWrite(RED_LED, 255);
        }else {
          Cont_2 = Cont_2*2 ;
          analogWrite(RED_LED, 255);
        }
      }
      
      delay(50);   // ---- corregir ----
      
      analogWrite(RED_LED, 0);  
      analogWrite(GREEN_LED, 0);
      analogWrite(BLUE_LED, 0);
       
    }
   
    if(Cont_1 >= 255){  // Ganador Jugador 1 - Color Celeste
      analogWrite(RED_LED, 10);
      analogWrite(GREEN_LED, 100);
      analogWrite(BLUE_LED, 140);
      
    }else if(Cont_2 >= 255){ // Ganador Jugador 2 - Color Rosa
      analogWrite(RED_LED, 220);
      analogWrite(GREEN_LED, 35);
      analogWrite(BLUE_LED, 216);
    }
    
    delay(2000);
    analogWrite(RED_LED, 0);
    analogWrite(BLUE_LED, 0);
    analogWrite(GREEN_LED, 0);
    
    // antirrebote para el inicio de la secuencia
    Sw1_Flag = 1;
    Sw2_Flag = 1;
  }
  
}
// --------------------------------- Fin Loop Principal ----------------------------------
// ---------------------------------------------------------------------------------------

// ------------------------------------- Funciones ---------------------------------------
// ---------------------------------------------------------------------------------------
void Semaforo(void){
  analogWrite(RED_LED, 150);
  analogWrite(BLUE_LED, 0);
  analogWrite(GREEN_LED, 0);
  delay(tiempo);
  analogWrite(RED_LED, 150);    // Amarillo
  analogWrite(BLUE_LED, 0);
  analogWrite(GREEN_LED, 150);
  delay(tiempo);
  analogWrite(RED_LED, 0);
  analogWrite(BLUE_LED, 0);
  analogWrite(GREEN_LED, 150);
  delay(300);
  analogWrite(RED_LED, 0);
  analogWrite(BLUE_LED, 0);
  analogWrite(GREEN_LED, 0);    
}

void Sw_Flags(void){
  // Actualización bandera de botones
  if(digitalRead(PUSH1) == HIGH){ // Boton 1
    Sw1_Flag = 0;
  }
  if(digitalRead(PUSH2) == HIGH){ // Boton 2
    Sw2_Flag = 0;
  }
}
// ---------------------------------------------------------------------------------------
