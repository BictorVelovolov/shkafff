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

bool avaliable1;
bool avaliable2;
bool avaliable3;
bool avaliable4;


// Массивы в которые необходимо записать ID карт:
uint8_t uidFirstCard[4];
uint8_t uidSecondCard[4];
uint8_t uidThirdCard[4];
uint8_t uidFourthCard[4];

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
  servo3.attach(12);
  servo4.attach(13);

  //поворот сервы на 90 градусов (закрытие)
  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  servo4.write(90);


  avaliable1 = true;
  avaliable2 = true;
  avaliable3 = true;
  avaliable4 = true;
  
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
     servo1.write(30);
     Serial.println("First comp");
     delay(5000);
     //повторное сканирование для закрытия
     uint8_t unlocking;
     unlocking = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
     if (comparisonOfUid(uid, uidFirstCard, uidLength)) {
         //поворачиваем серву1 для закртытия
         servo1.write(90);
         avaliable1 = true;
         uint8_t uidFirstCard[] = { 0, 0, 0, 0 };
         Serial.println("Avaliable 1");
        }
      } else {
      // Открывает серву2 если сравнение совпадает
      if (comparisonOfUid(uid, uidSecondCard, uidLength)) {
        servo2.write(30);
        Serial.println("Second comp");
        delay(5000);
        //повторное сканирование для закрытия
        uint8_t unlocking;
        unlocking = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
        if (comparisonOfUid(uid, uidSecondCard, uidLength)) {
            //поворачиваем серву2 для закрытия
            servo2.write(90);
            avaliable2 = true;
            uint8_t uidSecondCard[] = { 0, 0, 0, 0 };
            Serial.println("Avaliable 2");
        }
      } else {
        // Открывает серву3 если сравнение совпадает
        if (comparisonOfUid(uid, uidThirdCard, uidLength)) {
          servo3.write(30);
          Serial.println("Third comp");
          delay(5000);
          //повторное сканирование для закрытия
          uint8_t unlocking;
          unlocking = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
          if (comparisonOfUid(uid, uidThirdCard, uidLength)) {
              //поворачиваем серву3 для закрытия
              servo3.write(90);
              avaliable3 = true;
              uint8_t uidThirdCard[] = { 0, 0, 0, 0 };
              Serial.println("Avaliable 3");
          }
        } else {
          // Открывает серву4 если сравнение совпадет
          if (comparisonOfUid(uid, uidFourthCard, uidLength)) {
           servo4.write(30);
           Serial.println("Fourth comp");
           delay(5000);
           //повторное сканирование для закрытия
           uint8_t unlocking;
           unlocking = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
             if (comparisonOfUid(uid, uidFourthCard, uidLength)) {
              //поворачиваем серву3 для закрытия
              servo4.write(90);
              avaliable4 = true;
              uint8_t uidFourthCard[] = { 0, 0, 0, 0};
              Serial.println("Avaliable 4");
           }       
         } else {
            //если нет совпадения
            Serial.println("No comp");
            //если свободен первый - открыть
            if (avaliable1) {
             servo1.write(30);
             Serial.println("Open 1");
             //повторное сканирование для закрытия
             delay(5000);
             uint8_t locking;
             uint8_t uid1[4];
             Serial.println("Awaiting for card again...");
             locking = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid1, &uidLength);
             nfc.PrintHex(uid1, uidLength);
             // если сравнение первой карточки со второй совпадет, то он закроет серву и запишет ID в буфер
             if (comparisonOfUid(uid1, uid, uidLength)) {
                 //поворачиваем серву1 для закрытия
                 servo1.write(90);
                 Serial.println("Lock 1");
                 uint8_t lockingbuf;
                 // костыль для записи в буфер
                 lockingbuf = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uidFirstCard, &uidLength);
                 nfc.PrintHex(uidFirstCard, uidLength);
                 avaliable1 = false;
              } 
            } else {
              //если свободен второй - открыть
              if (avaliable2) {
                servo2.write(30);
                Serial.println("Open 2");
                //повторное сканирование для закрытия
                delay(5000);
                uint8_t locking;
                uint8_t uid1[4];
                Serial.println("Awaiting for card again...");
                locking = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid1, &uidLength);
                nfc.PrintHex(uid1, uidLength);
                // если сравнение первой карточки со второй совпадет, то он закроет серву и запишет ID в буфер
                if (comparisonOfUid(uid1, uid, uidLength)) {
                    //поворот сервы2 для закрытия
                    servo2.write(90);
                    Serial.println("Lock 2");
                    uint8_t lockingbuf;
                    // костыль для записи в буфер
                    lockingbuf = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uidSecondCard, &uidLength);
                    nfc.PrintHex(uidSecondCard, uidLength);
                    avaliable2 = false;
                }
              } else {
                //если свободен третий - открыть
                if (avaliable3) {
                  servo3.write(30);
                  Serial.println("Open 3");
                  //повторное сканирование для закрытия
                  delay(5000);
                  uint8_t locking;
                  uint8_t uid1[4];
                  Serial.println("Awaiting for card again...");
                  locking = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid1, &uidLength);
                  nfc.PrintHex(uid1, uidLength);
                  // если сравнение первой карточки со второй совпадет, то он закроет серву и запишет ID в буфер                  
                  if (comparisonOfUid(uid1, uid, uidLength)) {
                      //поворачиваем серву3 для закрытия
                      servo3.write(90);
                      Serial.println("Lock 3");
                      uint8_t lockingbuf;
                      // костыль для записи в буфер
                      lockingbuf = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uidThirdCard, &uidLength);
                      nfc.PrintHex(uidThirdCard, uidLength);
                      avaliable3 = false;
                  }
                } else {
                  //если свободен четвертый - открыть
                  if (avaliable4) {
                    servo4.write(30);
                    Serial.println("Open 4");
                    //повторное сканирование для закрытия
                    delay(5000);
                    uint8_t locking;
                    uint8_t uid1[4];
                    Serial.println("Awaiting for card again...");
                    locking = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid1, &uidLength);
                    nfc.PrintHex(uid1, uidLength);
                    // если сравнение первой карточки со второй совпадет, то он закроет серву и запишет ID в буфер
                    if (comparisonOfUid(uid1, uid, uidLength)) {
                        //поворачивает серву4 для закрытия
                        servo4.write(90);
                        Serial.println("Lock 4");
                        uint8_t lockingbuf;
                        // костыль для записи в буфер
                        lockingbuf = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uidFourthCard, &uidLength);
                        nfc.PrintHex(uidFourthCard, uidLength);
                        avaliable4 = false;
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
