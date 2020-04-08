Proyecto para trackear personas a traves de un centro comercial
=======================


Objetivo
-------------
Poder estudiar el comportamiento de las personas en una tienda. Cuanto tiempo paso enfrente a cada producto y su tiempo en el local, 
a traves de una identificacion unica de la persona.

Método
-------------
Se estudiaron diversas formas de poder trackear a la persona, pero despues de un largo _research_, concluimos que lo 
mejor fue realizarlo a traves de los celulares de los clientes.
A traves de diferentes vulnerabilidades que diversos estudios revelaron, los protocolos de WiFi dejan mucha informacion
al descubierto. En este proyecto utilizamos esas vulnerabilidades para poder trackear las personas (celulares).

(*) Sin ir tanto a lo tecnico, basicamente, un dispositvo (RPI Zero W) en cada zona del local que enviaria un paquete 
RTS (Request To Send) con destino al celular con su respectiva MAC (Medium Access Control) y el teniendo en cuenta el 
protcolo 802.11 b/g/n/ac el celular responderia con una balisa CTS (Clear To Send) avisando de que puede usar el medio 
para trasmitir. De esta forma, se puede detectar si un celular esta en un area si el mismo responde a un RTS.
Esto seria como el "core" del metodo. Teniendo en el local un dispositivo por zona.
La Raspberry Pi Zero W, corre con una version de Kali especial para la placa que facilita el modo monitor/promiscuo. 

Asi mismo hay dos dispositivos mas en el local. Uno para detectar nuevas MACs y agregarlas a la base de datos del sistema
para su posterior rastreo (como comente recien). Este dispositivo, son 3 placas WiFi ESP8266-01, que estan programadas en Arduino,
las mismas escuchan los _prove request_ que los celulares emiten cada cierto tiempo (variando para cada version de Android)
para detectar nuevas redes y coenctarce automaticamente. Cada placa ESP8266 esta programada para escuchar en un canal 
diferente (1, 6 y 11) de esta forma escucha todo el ancho de banda de una red 2.4GHz.

El ultimo dispositivo es una placa como (*) que se encuentra en el centro del local. La misma recibe cada 5 min la tabla 
de las MACs encontradas en algun momento en el local y envia un RTS a todo el local (en vez de tener un radio acotado 
como en (*)). De esta forma lo que se consigue es alertar si una MAC que alguna vez entro, esta en el local.
Mejorando asi la efectiviadad del sistema.

Por otro lado, hay un servicio echo en Node.js, en donde escucha y envia (a traves de MQTT) todos los dispositivos del
local. Digamos que es por donde pasa toda la informacion recolectada. 
El mismo tiene dos talbas donde almacena la informacion de las MACs encontradas y en otra tabla, deja registro de los 
celulares que estaban en una zona X del local asociadando la MAC con la hora.
Ademas el mismo tiene un array de _hot_macs_ en donde registra los celulares (MACs) que estan actualmente en el local.
Despues de varios minutos de no tener "noticias" de una MAC, se descarta (por tiempo).
Este array de hot_macs, lo que permite es enviarle a los dispositivos cada cierto tiempo las MACs que hay en los locales
para que los mismos hagan el trabajo core, de RTS/CTS.

Si bien me podria seguir extendiendo en el funcionamiento, la idea es tener una idea basica del funcionamiento del mismo.

Uso del mismo
-------------
Una vez clonado el repo, seguir las siguientes instrucciones:

* Para correr el servidor, ingresen a la carpeta "api":
    * Primer deben crear una base de datos tal con el nombre igual que en config/connection.js
    * En dataFixtures/db_strucre encontraran las querys para crear las tablas.
    * Instalar [Nodejs](https://nodejs.org/es/)
    * Correr **_npm install_** para instalar todas las dependencias.
    
* El codigo de las RPi Zero, se encuentra en la carpeta "sniffer-rts-cts":
    * Para correr el codigo de las RPi de las zonas, basta con ejecutar **_python3 main.py_** e instalar las dependecias
    que requiera el script.  
    
* El codigo de las RPi Zero General del local, se encuentra en la carpeta "sniffer-rts-cts":
    * Para correr el codigo de las RPi del local, basta con ejecutar **_python3 main-old-macs.py_** e instalar las dependecias
    que requiera el script.
    
* Por ultimo el dispositivo con las placas para recolectar los probe request, con solo conectar la placa la misma empezara
  a funcionar siempre y cuando disponga de una red con SSID y CONTRASEÑA elegida y cambiadas en el codigo de arduino. 
  El codigo de las mismas se encuentra en la carpeta "arduino/WiFi_Sniffer".


Links
-------------
[kali Linux RPi Zero](https://www.offensive-security.com/kali-linux-arm-images/)

[Conectar kali Linux a una red WiFi](https://www.blackmoreops.com/2014/09/18/connect-to-wifi-network-from-command-line-in-linux/)

Biblografia
-------------
[Paper principal donde se basa el proyecto](https://arxiv.org/pdf/1703.02874.pdf)

[Mas sobre el mecanismo RTS/CTS](https://ece.umd.edu/~zcui/Publications/Mobiquitous2015.pdf)

[Fundamento de los sniffer: probe request](https://frdgr.ch/wp-content/uploads/2015/06/Freudiger15.pdf)
