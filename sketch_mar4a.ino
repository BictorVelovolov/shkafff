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

bool avaliable1;
bool avaliable2;


// Массивы в которые необходимо записать ID карт:
uint8_t uidFirstCard[4];
uint8_t uidSecondCard[4];

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

  servo1.attach(10);
  servo2.attach(11);

  //поворот сервы на 90 градусов (закрытие)
  servo1.write(90);
  servo2.write(90);


  avaliable1 = true;
  avaliable2 = true;
  
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
              }
              }
            }
         }
      }
      delay(2000);
    }