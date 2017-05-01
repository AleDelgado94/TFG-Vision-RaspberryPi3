#!/usr/bin/env python

from __future__ import print_function


import numpy as np
import cv2
# relative module
#import video

# built-in module
import sys


face_cascade = cv2.CascadeClassifier('/home/pi/opencv-3.2.0/data/haarcascades/haarcascade_frontalface_alt.xml')
eyes_cascade = cv2.CascadeClassifier('/home/pi/opencv-3.2.0/data/haarcascades/haarcascade_eye.xml')
smile_cascade = cv2.CascadeClassifier('/home/pi/opencv-3.2.0/data/haarcascades/haarcascade_smile.xml')
cap = cv2.VideoCapture(0);

#img = cv2.imread('/home/aledelgado/Escritorio/20170311_121944.jpg')





while(True):
    #leemos un frame y lo guardamos
    ret, img = cap.read()
    
    

#convertimos la imagen a blanco y negro
    gray = cv2.cvtColor(img, cv2.COLOR_RGB2HSV)

#buscamos las coordenadas de los rostros (si los hay) y
#guardamos su posicion
    faces = face_cascade.detectMultiScale(gray, 1.3, 5)
    eyes = eyes_cascade.detectMultiScale(gray, 1.3, 5)
    smile = smile_cascade.detectMultiScale(gray, 1.3, 5)

#Dibujamos un rectangulo en las coordenadas de cada rostro
    for (x,y,w,h) in faces:
        cv2.rectangle(img,(x,y),(x+w,y+h),(125,255,0),2)
    for (x,y,w,h) in eyes:
        cv2.rectangle(img, (x,y),(x+w,y+h),(125,255,0),2)
    for (x,y,w,h) in smile:
        cv2.rectangle(img, (x,y),(x+w,y+h),(125,255,0),2)

#Mostramos la imagen
	cv2.imshow('img',img)

#con la tecla 'q' salimos del program
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
cap.release()
cv2-destroyAllWindows()
