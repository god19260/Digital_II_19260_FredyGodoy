/*
Electronica Digital II  
Laboratorio No.06
Autor: Fredy Godoy 19260
*/

// ------------------------------------- Variables --------------------------------------
//int ledRed = 30, ledBlue = 40, ledGreen = 39;    // LED connected to digital pin 9
int tiempo = 800;
int Sem_Flag = 0;
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
  if(digitalRead(PUSH1) == LOW or digitalRead(PUSH2) == LOW){
    Semaforo();
    Cont_1 = Cont_2 = 0;
    while (Cont_1 <= 255 && Cont_2 <= 255){
      // Acción Jugador 1
      if(digitalRead(PUSH1) == LOW){
        if(Cont_1 == 0){
          Cont_1 = 1;
          analogWrite(RED_LED, 255);
        }else if(digitalRead(PUSH1) == LOW){
          Cont_1 = Cont_1*2 ;
          analogWrite(RED_LED, 255);
        }
      }
      // Acción Jugador 2
      if(digitalRead(PUSH2) == LOW){
        if(Cont_2 == 0){
          Cont_2 = 1;
          analogWrite(BLUE_LED, 255);
        }else if(digitalRead(PUSH2) == LOW){
          Cont_2 = Cont_2*2 ;
          analogWrite(BLUE_LED, 255);
        }
      }
      
      delay(100);
      analogWrite(RED_LED, 0);  // Acción del Jugador 1
      analogWrite(BLUE_LED, 0); // Acción del Jugador 2
    }
   
    if(Cont_1 >= 255){  // Ganador Jugador 1 - Color Celeste
      analogWrite(RED_LED, 44);
      analogWrite(GREEN_LED, 199);
      analogWrite(BLUE_LED, 211);
      
    }else if(Cont_2 >= 255){ // Ganador Jugador 2 - Color Rosa
      analogWrite(RED_LED, 220);
      analogWrite(GREEN_LED, 35);
      analogWrite(BLUE_LED, 216);
    }
    
    delay(2000);
    analogWrite(RED_LED, 0);
    analogWrite(BLUE_LED, 0);
    analogWrite(GREEN_LED, 0);
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
  analogWrite(RED_LED, 150);
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

// ---------------------------------------------------------------------------------------
