# Feeder4pets
I created an automated feeder for a rabbit 😁 

## Why? you may ask 😱
Well, long story short, y have to take care of a rabbit for the month of July, and to avoid go to the house where the rabbit is in (Yes, I'm that lazy 😅), I developed a simple proyect to make a dispenser for the pet food with only cardboard and my old [Arduino Uno](https://store.arduino.cc/products/arduino-uno-rev3?srsltid=AfmBOorJnbd139dxaxaAQkf2PpH3Emm7-MJmiFvstXOTHKIlZhMDNLmq) microcontroller from school days.

So if you are in a similar position to mine, or you just like to automate things like me 😁, I will show you the process for some ideas!

## 🐢🔨 The hardware 
This simple proyect could escalate very quickly into a more complex proyect if you can't select a proper hardware for the application, I search for some ideas online and I develop my own idea, so simple that I couldn't make it fail, the concept is:

### 🎡 The MILL 
With this approach I only need to make a horizontal "_mill_" with 8 mill blades that will push the food arround, and add a hole between two of them. One day of food 1/8 rotation of the mill, easy isn't it?.

Well the **BIG PROBLEM** wasn't make it rotate, was **how to make it rotate exactly 1/8 of the perimeter**, you can do that with big servomotors with feedback, I have a humble servo [SG90 360º](https://www.micro-semiconductor.fr/datasheet/b6-SER0043.pdf) and it can't control position. So I developed an encoder for the "_mill_" similar to the [encoders of the mouse wheels](https://en.wikipedia.org/wiki/Rotary_encoder), using a fotoresitor, a cardboard piece piereced and a LED as you can see on the image below.


Finally the whole Mill system is like the following drawings.


### 📎 Circuit
I use the Arduino Uno R3 microcontroller, for the encoder as I said I use a fotoresistor and a LED, for the rotation a servo and finally to keep the time an RTC. The full list is:
- [Arduino Uno R3 microcontroller](https://store.arduino.cc/products/arduino-uno-rev3?srsltid=AfmBOorJnbd139dxaxaAQkf2PpH3Emm7-MJmiFvstXOTHKIlZhMDNLmq) - 29'30 €.
- [LDR fotoresistor](https://www.luisllamas.es/en/measure-light-level-arduino-ldr-photoresistor/) - 0'35€.
- [LED](https://learn.adafruit.com/all-about-leds/the-led-datasheet) (Yellow) - 0'16€.
- [Servo 360º](https://www.electrocomponentes.es/motores-y-servos/931-servo-mini-sg90-9g-micro-360-motor-paso-a-paso-giro-continuo.html) - 0'99€.
- [RTC3231](https://es.aliexpress.com/item/1005006127416923.html?src=google&pdp_npi=4%40dis!EUR!4.25!1.49!!!!!%40!12000035879185769!ppc!!!&snpsid=1&snps=y&snpsid=1&src=google&albch=shopping&acnt=439-079-4345&isdl=y&slnk=&plac=&mtctp=&albbt=Google_7_shopping&aff_platform=google&aff_short_key=UneMJZVf&gclsrc=aw.ds&&albagn=888888&&ds_e_adid=&ds_e_matchtype=&ds_e_device=c&ds_e_network=x&ds_e_product_group_id=&ds_e_product_id=es1005006127416923&ds_e_product_merchant_id=5551326180&ds_e_product_country=ES&ds_e_product_language=es&ds_e_product_channel=online&ds_e_product_store_id=&ds_url_v=2&albcp=20007147547&albag=&isSmbAutoCall=false&needSmbHouyi=false&gad_source=1&gad_campaignid=19998877869&gbraid=0AAAAACbpfvZNW1XJTN-0dUVVNRU8wRm1T&gclid=CjwKCAjwyb3DBhBlEiwAqZLe5OO42gJm4CRIlCElcgCkL_AS93jTn0qfm2HM43VHM9N_4EgJj30E_RoCvFsQAvD_BwE) - 1'49€.
- [bunch of cables, resistors and a protoboard](https://solectroshop.com/es/placas-de-pruebas-protoboard/256-placa-prototipo-protoboard-breadboard-400-puntos-arduino-5905323238381.html?srsltid=AfmBOorbpLdx1sRy4ew9aUN4s4LWh7R0Y5SuwKCbuFp5khx8BQtBL-sKNPI) 2'00€

For a total of less than 35€ you can make yourself a dispenser with a really simple ciruit:


## ✒️ Software
The code for this application intends to save the most energy possible to be able to execute with an external power bank and be very efficient with the energy. To do that I use interruptions using the RTC3231 and the library [_LowPower_](https://docs.arduino.cc/libraries/lowpower_lowpowerlab/) of LowPowerLab to uses as little as ~9–13 mA.

To do this we use the Low power library like:


The full code with the interruptions using the RTC can be seen in the main file "_dispenser.ino_"
