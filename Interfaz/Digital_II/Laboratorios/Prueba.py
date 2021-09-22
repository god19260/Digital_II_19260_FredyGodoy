import math

R2 = 10000
Vin = 1
Vout = -100
R3 = 100000/(((-R2*Vout-100000*Vout/100)/(Vin-(2/100)*Vout))-R2-100000)
print(R3)