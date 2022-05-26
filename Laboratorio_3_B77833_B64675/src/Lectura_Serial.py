import csv
import serial

ser = serial.Serial("/tmp/ttyS1", 9600) #Comunicacion con la placa mediante el puerto ttyS1

archivo_csv= open("Lectura_Arduino.csv",'w') 
Escritura = csv.writer(archivo_csv)

while (1):
	Data = ser.readline().decode().split(' ')
    	writer.writerow(Data)

f.close()

#Fuentes importantes: https://www.pythontutorial.net/python-basics/python-write-csv-file/