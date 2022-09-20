#ifndef ____ARDUINO____PHONE____
#define ____ARDUINO____PHONE____
#define ROWS 4U
#define COLUMNS 4U


#include <Arduino.h>
#include <Keypad.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <Wire.h>


struct PhoneData
{
  uint8_t
    gsmRx,
    gsmTx,
    *lcd[6],
    *keyRowCol[2][4];

  char *keys[ROWS] [COLUMNS];
};


class Phone
{
public:
  //Member Attribtes:
  static uint8_t smsFlag;
  static uint8_t
   ring,
  i, temp, temp1,  recRead;
  static char smsNum[3];
  static String number, msg, instr, strSms, str1;

  //Member Methods:
  Phone () = default;
  bool init();
  explicit Phone (PhoneData&);
  void serialEvent();
  void sms();
  void call();
  void alphaNumericKeyboard();
  void sendData(String message);
  void sendSms();
  void lcdStatus();
  void backButton();
  void okButton();
  void callButton();
  void smsButton();
  void cls() { lcd->clear(); }
  void setCursor(const uint8_t &r, const uint8_t &c) { lcd->setCursor(r, c); }

  template <typename T>
    void write(T val) { lcd->write(val); }

  char getKey() { return (keypad->getKey()); }

  template <typename T>
    void print(T val) { lcd->print(val); }

  template <typename T>
    void gsmPrintln(T val) { gsm->println(val); }

    template <typename T>
      void gsmPrint(T val) { gsm->print(val); }

    uint8_t gsmAvail() { return (gsm->available()); }

    char gsmRead() { return (gsm->read()); }




private:
  //Member Attributes:
  SoftwareSerial *gsm { nullptr };
  LiquidCrystal *lcd { nullptr };
  Keypad *keypad { nullptr };
  String characters = "1,.?!@abc2def3ghi4jkl5mno6pqrs7tuv8wxyz90 ";
  uint8_t
    back[8] =
    {
      0b00000,
      0b00000,
      0b11111,
      0b10101,
      0b11011,
      0b11111,
      0b00000,
      0b00000
    };
  //Member Methods:

};




#endif
