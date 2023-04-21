#include <Wire.h>
#include <SPI.h>
#include <Servo.h>
#include "Adafruit_PN532.h"

#define PN532_IRQ   9
 
// создаём объект для работы со сканером и передаём ему два этих параметра
// первый — номер пина прерывания
// вторым — число 100
Adafruit_PN532 nfc(PN532_IRQ, 100);


Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;
Servo servo7;
Servo servo8;
Servo servo9;
Servo servo10;

bool avaliable1;
bool avaliable2;
bool avaliable3;
bool avaliable4;
bool avaliable5;
bool avaliable6;
bool avaliable7;
bool avaliable8;
bool avaliable9;
bool avaliable10;


// Массивы в которые необходимо записать ID карт:
uint8_t uidFirstCard[4];
uint8_t uidSecondCard[4];
uint8_t uidThirdCard[4];
uint8_t uidFourthCard[4];
uint8_t uidFifthCard[4];
uint8_t uidSixthCard[4];
uint8_t uidSeventhCard[4];
uint8_t uidEighthCard[4];
uint8_t uidNinethCard[4];
uint8_t uidTenthCard[4];

uint8_t uidMaster1[4];

 
// булеан который сравнивает два переданных ID
// при совпадении возвращает значение true
// и значение false если ID разные
boolean comparisonOfUid(uint8_t uidRead[4], uint8_t uidComp[4], uint8_t uidLen) {
  for (uint8_t i = 0; i < uidLen; i++) {
    if (uidRead[i] != uidComp[i]) {
      return false;
    }
    if (i == (uidLen)-0x01) {
      return true;
    }
  }
}
 
void setup() {

  // инициализация монитора порта
  Serial.begin(115200);
  // инициализация RFID/NFC сканера
  nfc.begin();
  int versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    while (1) {
      Serial.print("Not detected NFC/RFID reader");
      delay(500);
    }
  }
  Serial.println("NFC/RFID reader detected");
  nfc.SAMConfig();
  Serial.println("Awaiting for a card...");

  servo1.attach(3);
  servo2.attach(4);
  servo3.attach(5);
  servo3.attach(6);
  servo3.attach(7);
  servo3.attach(8);
  servo3.attach(10);
  servo3.attach(11);
  servo3.attach(12);
  servo3.attach(13);

  //поворот сервы на 90 градусов (закрытие)
  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  servo4.write(90);
  servo5.write(90);
  servo6.write(90);
  servo7.write(90);
  servo8.write(90);
  servo9.write(90);
  servo10.write(90);


  avaliable1 = true;
  avaliable2 = true;  
  avaliable3 = true;
  avaliable4 = true;
  avaliable5 = true;
  avaliable6 = true;
  avaliable7 = true;
  avaliable8 = true;
  avaliable9 = true;
  avaliable10 = true;
  
}

uint8_t openingfunc(Servo servonum,String print1,uint8_t uidnum[4], uint8_t uidLengthnum,uint8_t uidCardnum[4],String print2) {
    //поворачиваем серву для открытия
    servonum.write(30);
     Serial.println(print1);
     delay(2000);
     //повторное сканирование для закрытия
     uint8_t unlocking;
     unlocking = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uidnum, &uidLengthnum);
     if (comparisonOfUid(uidnum, uidCardnum, uidLengthnum)) {
         //поворачиваем серву для закрытия
         servonum.write(90);
         uint8_t uidCardnum[] = { 0, 0, 0, 0 };
         Serial.println(print2);
         return uidCardnum;
     }

}

uint8_t lockingfunc(Servo servonum,String print1,uint8_t uidnum[4], uint8_t uidLengthnum,uint8_t uidCardnum[4],String print2) {
     //поворачиваем серву для открытия
     servonum.write(30);
      Serial.println(print1);
      //повторное сканирование для закрытия
      delay(2000);
      uint8_t locking;
      uint8_t uid1[4];
      Serial.println("Awaiting for card again...");
      locking = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid1, &uidLengthnum);
      nfc.PrintHex(uid1, uidLengthnum);
      // если сравнение первой карточки со второй совпадет, то закроет серву и запишет ID в буфер
        if (comparisonOfUid(uid1, uidnum, uidLengthnum)) {
          //поворачиваем серву для закрытия
          servonum.write(90);
          Serial.println(print2);
          uint8_t lockingbuf;
          // костыль для записи в буфер
          lockingbuf = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uidCardnum, &uidLengthnum);
          nfc.PrintHex(uidCardnum, uidLengthnum);
          return uidCardnum;
       } 
}
 
void loop() {
  uint8_t success;
  // буфер для хранения ID карты
  uint8_t uid[4];
  // размер буфера
  uint8_t uidLength;
  // скан новых меток
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  // если найдена карта
  if (success) {
    // выводит в консоль данные
    nfc.PrintHex(uid, uidLength);
    // Открывает серву1 если сравнение совпадает
    if (comparisonOfUid(uid, uidFirstCard, uidLength)) {
     openingfunc(servo1, "First comp, Open 1", uid, uidLength, uidFirstCard, "Avaliable 1");
     avaliable1 = true;
      } else {
      // Открывает серву2 если сравнение совпадает
      if (comparisonOfUid(uid, uidSecondCard, uidLength)) {
        openingfunc(servo2, "Second comp, Open 2", uid, uidLength, uidSecondCard, "Avaliable 2");
        avaliable2 = true;
      } else { 
      // Открывает серву3 если сравнение совпадет
      if (comparisonOfUid(uid, uidThirdCard, uidLength)) {
        openingfunc(servo3, "Third comp, Open 3", uid, uidLength, uidThirdCard, "Avaliable 3");
        avaliable3 = true;
      } else {
      // Открывает серву4 если сравнение совпадет
      if (comparisonOfUid(uid, uidFourthCard, uidLength)) {
        openingfunc(servo4, "Fourth comp, Open 4", uid, uidLength, uidFourthCard, "Avaliable 4");
        avaliable4 = true;
      } else {
      // Открывает серву5 если сравнение совпадет
      if (comparisonOfUid(uid, uidFifthCard, uidLength)) {
        openingfunc(servo5, "Fifth comp, Open 5", uid, uidLength, uidFifthCard, "Avaliable 5");
        avaliable5 = true;
      } else {
      // Открывает серву6 если сравнение совпадет
      if (comparisonOfUid(uid, uidSixthCard, uidLength)) {
        openingfunc(servo6, "Sixth comp, Open 6", uid, uidLength, uidSixthCard, "Avaliable 6");
        avaliable6 = true;
      } else {
      // Открывает серву7 если сравнение совпадет
      if (comparisonOfUid(uid, uidSeventhCard, uidLength)) {
        openingfunc(servo7, "Seventh comp, Open 7", uid, uidLength, uidSeventhCard, "Avaliable 7");
        avaliable7 = true;
      } else {
      // Открывает серву8 если сравнение совпадет
      if (comparisonOfUid(uid, uidEighthCard, uidLength)) {
        openingfunc(servo8, "Eighth comp, Open 8", uid, uidLength, uidEighthCard, "Avaliable 8");
        avaliable8 = true;
      } else {
      // Открывает серву9 если сравнение совпадет
      if (comparisonOfUid(uid, uidNinethCard, uidLength)) {
        openingfunc(servo9, "Nineth comp, Open 9", uid, uidLength, uidNinethCard, "Avaliable 9");
        avaliable9 = true;
      } else {
      // Открывает серву10 если сравнение совпадет
      if (comparisonOfUid(uid, uidTenthCard, uidLength)) {
        openingfunc(servo10, "Tenth comp, Open 10", uid, uidLength, uidTenthCard, "Avaliable 10");
        avaliable10 = true;
      } else {
            //если нет совпадения
            Serial.println("No comp");
            //если свободен первый - открыть
            if (avaliable1) {
             lockingfunc(servo1, "Open 1", uid, uidLength, uidFirstCard, "Lock 1");
             avaliable1 = false;
            } else {
            //если свободен второй - открыть
            if (avaliable2) {
              lockingfunc(servo2, "Open 2", uid, uidLength, uidSecondCard, "Lock 2");
              avaliable2 = false;
             } else {
            if (avaliable3) {
              lockingfunc(servo3, "Open 3", uid, uidLength, uidThirdCard, "Lock 3");
              avaliable3 = false;
             } else {
            if (avaliable4) {
              lockingfunc(servo4, "Open 4", uid, uidLength, uidFourthCard, "Lock 4");
              avaliable4 = false;
             } else {
            if (avaliable5) {
              lockingfunc(servo5, "Open 5", uid, uidLength, uidFifthCard, "Lock 5");
              avaliable5 = false;
             } else {
            if (avaliable6) {
              lockingfunc(servo6, "Open 6", uid, uidLength, uidSixthCard, "Lock 6");
              avaliable6 = false;
             } else {
            if (avaliable7) {
              lockingfunc(servo7, "Open 7", uid, uidLength, uidSeventhCard, "Lock 7");
              avaliable7 = false;
             } else {
            if (avaliable8) {
              lockingfunc(servo8, "Open 8", uid, uidLength, uidEighthCard, "Lock 8");
              avaliable8 = false;
             } else {
            if (avaliable9) {
              lockingfunc(servo9, "Open 9", uid, uidLength, uidNinethCard, "Lock 9");
              avaliable9 = false;
             } else {
            if (avaliable10) {
              lockingfunc(servo10, "Open 10", uid, uidLength, uidTenthCard, "Lock 10");
              avaliable10 = false;
             }
                    }
                   }
                  }
                 }
                }
               }
              }             
             }
            }
           }
          }
         }
        }
       }
      }
     }
    }
   }
  }
  delay(2000);
 }
}