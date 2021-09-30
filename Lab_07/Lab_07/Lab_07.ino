#include <SPI.h>
#include <SD.h>

File myFile;
File root;
String Numero;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.setModule(0);
  Serial.println("Inicio");

  // --- Config Pines --- 
  pinMode(PA_3, OUTPUT); // CS pin modulo SD
  Serial.println("Seleccione 1:Mario, 2:Guitarra, 3:Einstein");
  if (!SD.begin(PA_3)) {
    Serial.println("initialization failed!");
    return;
  }
  
  root = SD.open("/");
  
  printDirectory(root, 0);
 
  // --- Primer imagen ---  
  //Lectura_SD("Mario_2.txt");
  

}

void loop() {
  // put your main code here, to run repeatedly: 
  if (Serial.available()){
    Numero = Serial.read();
    if (Numero == "49"){
      Lectura_SD("Mario.txt");
    } else if(Numero == "50"){
      Lectura_SD("Guitarra.txt");
    } else if(Numero == "51"){
      Lectura_SD("Einstein.txt");
    } else if(Numero == "52"){
      root = SD.open("/");
      printDirectory(root, 0);
    }
        
  }
}

// ---------------- Funciones ----------------
void printDirectory(File dir, int numTabs) {
   Serial.println("");
   Serial.println(""); 
   while(true) {
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       //printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
   Serial.println("");
   Serial.println("");
}

void Lectura_SD(String nombre){
  char Nombre_Archivo[nombre.length()+1];
  nombre.toCharArray(Nombre_Archivo,sizeof(Nombre_Archivo));
  // re-open the file for reading:
  myFile = SD.open(Nombre_Archivo);
  if (myFile) {
    Serial.println("");
    Serial.println("");
    Serial.println("Imagen: "+nombre);
    Serial.println("");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("");
    Serial.println("");
    Serial.println("");
    Serial.println("No se encontró "+nombre);
  }
}
