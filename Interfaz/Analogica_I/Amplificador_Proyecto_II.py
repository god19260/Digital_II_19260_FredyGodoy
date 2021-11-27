from typing import Text
from sympy import symbols, solve
import numpy

# Configuración Polarización: Universal, de Emisor Común

# Datos:
# Transistor 2n3904
Ic = 10/1000  # 10mA
Hfe = 100 # Beta
vt = 26/1000  # 26mV Máximo voltaje térmico
re = vt/Ic # Resistencia parasita  
alpha = Hfe/(Hfe+1)
Ie = alpha*Ic
gm = Ic/vt
# Alimentación del circuito Vcc
Vcc = 10 # 10V
Frecuencia_Corte = 20
# Bocina
Potencia = 0.5
Resistencia_Bocina = 8

# Amplificador: Ganancia propuesta
Ganancia = 20
k=100
Vb = 1.7
R1 = 1000
Rc = Vcc*0.45/Ic
Re = Vcc*0.10/Ie
R2 = (Hfe+1)*(Re+re)*k*(((Vcc-Vb)/Vb)+1)/((Vcc-Vb)/Vb)
R1 = R2*(Vcc-Vb)/Vb
C = 10/(2*3.14*Frecuencia_Corte*Re)
Ganancia_Obtenida = (Rc/((vt/Ic)+Re))

# Potencia: Darlington
Ganancia_Potencia=0.60
I1 = Ic/Hfe 
Rep=2*re*Resistencia_Bocina/(Resistencia_Bocina*((1/Ganancia_Potencia)-1)-2*re)
R1p = (Vcc-1.4)/(Ic/Hfe)-Hfe*Rep

print("")
print("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-")
print("Re: " + Text(Re))
print("Rc: " + Text(Rc))
print("R1: " + Text(R1))
print("R2: " + Text(R2))
print("Capacitores: " + Text(C))
print("Etapa de potencia: ")
print("Re Potencia: " + Text(Rep))
print("R1 Potencia: " + Text(R1p))

print("Ganancia Obtenida: " + Text(Ganancia_Obtenida))
print("*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-")