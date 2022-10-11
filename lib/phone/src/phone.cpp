///////////////////////////////////////////////////////////////////////////////////////////////////
//*******FILE NAME:   phone.cpp*********************************************************************
//*******AUTHOR:      Chima Okwara (AlphaChi)*******************************************************
//*******DESC:        Library for Arduino Phone ****************************************************
//*******LICENCE:     GPL Version 3 ****************************************************************
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Arduino.h>
#include "phone.hpp"

Phone::Phone(PhoneData &data)
{
  lcd = new LiquidCrystal (*data.lcd[0], *data.lcd[1], *data.lcd[2], *data.lcd[3], *data.lcd[4], *data.lcd[5]);
  gsm = new SoftwareSerial (data.gsmRx, data.gsmTx);

  keypad = new Keypad (makeKeymap(data.keys), *data.keyRowCol[0], *data.keyRowCol[1], ROWS, COLUMNS);

}

Phone::~Phone()
{
  delete lcd;
  delete gsm;
  delete keypad;
  lcd = nullptr;
  gsm = nullptr;
  keypad = nullptr;
}

bool Phone::init()
{
    using namespace phn;
    gsm->begin(9600);
    lcd->begin(16,2);
    lcd->createChar(1, back);
    lcd->print("***Eichen(R)***");
    lcd->setCursor(0,1);
    delay(1000);
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print("  unofone 1.0   ");
    delay(1000);
    lcd->clear();

    lcd->setCursor(0, 0);
    lcd->print("Connecting");
    delay(500);
    lcd->setCursor(0, 1);
    for(phn::i = 0; phn::i < 3; ++phn::i)
    {
      lcd->print(".");
      delay(500);
    }
    i = 0;

    boolean at_flag=1;
    while(at_flag)
    {
      gsm->println("AT");
      while(gsm->available()>0)
      {
        if(gsm->find("OK"))
        at_flag=0;
      }

      delay(1000);
    }

    lcd->clear();
    lcd->print("Module Connected..");
    delay(1000);
    lcd->clear();
    lcd->print("Disabling ECHO");
    boolean echo_flag=1;
    while(echo_flag)
    {
      gsm->println("ATE1");
      while(gsm->available()>0)
      {
        if(gsm->find("OK"))
        echo_flag=0;
      }
      delay(1000);
    }

    lcd->clear();
    lcd->print("Echo OFF");
    delay(1000);
    lcd->clear();
    lcd->print("Finding Network..");
    boolean net_flag=1;
    while(net_flag)
    {
      gsm->println("AT+CPIN?");
      while(gsm->available()>0)
      {
        if(gsm->find("+CPIN: READY"))
        net_flag=0;
      }
      delay(1000);
    }
    lcd->clear();
    lcd->print("Network Found..");
    delay(1000);
    lcd->clear();
    lcd->clear();
    lcd->print("System Ready");
    delay(2000);
    return 1;
 }

void Phone::serialEvent()
{
  using namespace phn;
  while(gsm->available())
  {
    char ch = gsm->read();
    phn::instr += ch;
    phn::i++;

    if(phn::instr[i-4] == 'R' && phn::instr[i-3] == 'I' && phn::instr[i-2] == 'N' && phn::instr[i-1] == 'G' )
    {
       phn::ring = 1;
    }

    if(phn::instr.indexOf("NO CARRIER") >= 0)
    {
       phn::ring = 0;
       phn::i = 0;
    }
    if(phn::instr.indexOf("+CMTI: \"SM\"") >= 0)
    {
      phn::smsFlag=1;
    }
  }
}

void Phone::sms()
{
    lcd->clear();
    lcd->print("Initilising SMS");
    gsm->println("AT+CMGF=1");
    delay(400);
    lcd->clear();
    lcd->print("After Enter No.");
    lcd->setCursor(0,1);
    lcd->print("Press D        ");
    delay(2000);
    lcd->clear();
    lcd->print("Enter Rcpt No.:");
    lcd->setCursor(0,1);
    gsm->print("AT+CMGS=\"");
    while(1)
    {
      serialEvent();
      char key = keypad->getKey();
      if(key)
      {
        if(key=='D')
        {
          //number+='"';
          gsm->println("\"");
          break;
        }
        else
        {
          //number+=key;
          gsm->print(key);
          lcd->print(key);
        }
      }
    }
    lcd->clear();
    lcd->print("After typing phn::msg");
    lcd->setCursor(0,1);
    lcd->print("Press D to Send ");
    delay(2000);
    lcd->clear();
    lcd->print("Enter Your phn::msg");
    delay(1000);
    lcd->clear();
    lcd->setCursor(0,0);
    alphaNumericKeyboard();

}

void Phone::call()
{

    phn::number="";
    lcd->clear();
    lcd->print("After Enter No.");
    lcd->setCursor(0,1);
    lcd->print("Press C to Call");
    delay(2000);
    lcd->clear();
    lcd->print("Enter Number:");
    lcd->setCursor(0,1);
    while(1)
    {
       serialEvent();
      char key=keypad->getKey();
      if(key)
      {
        if(key=='C')
        {
          lcd->clear();
          lcd->print("Calling........");
          lcd->setCursor(0,1);
          lcd->print(phn::number);
          gsm->print("ATD");
          gsm->print(phn::number);
          gsm->println(";");
          int ans=1;
          while(ans==1)
          {
            while(gsm->available()>0)
            {
              if(gsm->find("OK"))
              {
                lcd->clear();
                lcd->print("Ringing....");
                phn::str1=" ";
                while(ans==1)
                {
                  while(gsm->available()>0)
                  {
                    char ch=gsm->read();
                    phn::str1+=ch;
                    if(phn::str1.indexOf("NO CARRIER")>0)
                    {
                      lcd->clear();
                      lcd->print("Call End");
                      delay(2000);
                      ans=0;
                      return;
                    }
                   }
                    char key=keypad->getKey();
                     if(key == 'D')
                    {
                      lcd->clear();
                      lcd->print("Call End");
                      delay(2000);
                      ans=0;
                      return;
                    }
                     if(ans==0)
                     break;
                  }
                }
              }
          }
        }
        else
        {
          phn::number+=key;
          lcd->print(key);
        }
      }
    }

}
void Phone::alphaNumericKeyboard()
{

   uint8_t x=0,y=0;
   uint8_t num=0;
    while(1)
    {
      lcd->cursor();
      char key=keypad->getKey();
      if(key)
      {
         if(key=='1')
         {
           num=0;
           lcd->setCursor(x,y);
           lcd->print(characters[num]);
           for(int i=0;i<3000;i++)
           {
            lcd->noCursor();
            char key=keypad->getKey();
            if(key=='1')
            {
             num++;
             if(num>5)
             num=0;
             lcd->setCursor(x,y);
             lcd->print(characters[num]);
             i=0;
             delay(200);
            }
           }
           x++;
           if(x>15)
           {
             x=0;
             y++;
             y%=2;
           }
           phn::msg+=characters[num];
          }

         else if(key=='2')
         {
           num=6;
           lcd->setCursor(x,y);
           lcd->print(characters[num]);
           for(int i=0;i<3000;i++)
           {
            lcd->noCursor();
            char key=keypad->getKey();
            if(key=='2')
            {
             num++;
             if(num>9)
             num=6;
             lcd->setCursor(x,y);
             lcd->print(characters[num]);
             i=0;
             delay(200);
            }
           }
           x++;
            if(x>15)
           {
             x=0;
             y++;
             y%=2;
           }
           phn::msg+=characters[num];
          }

         else if(key=='3')
         {
           num=10;
           lcd->setCursor(x,y);
           lcd->print(characters[num]);
           for(int i=0;i<3000;i++)
           {
            lcd->noCursor();
            char key=keypad->getKey();
            if(key=='3')
            {
             num++;
             if(num>13)
             num=10;
             lcd->setCursor(x,y);
             lcd->print(characters[num]);
             i=0;
             delay(200);
            }
           }
           x++;
            if(x>15)
           {
             x=0;
             y++;
             y%=2;
           }
           phn::msg+=characters[num];
          }

         else if(key=='4')
         {
           num=14;
           lcd->setCursor(x,y);
           lcd->print(characters[num]);
           for(int i=0;i<3000;i++)
           {
            lcd->noCursor();
            char key=keypad->getKey();
            if(key=='4')
            {
             num++;
             if(num>17)
             num=14;
             lcd->setCursor(x,y);
             lcd->print(characters[num]);
             i=0;
             delay(200);
            }
           }
           x++;
            if(x>15)
           {
             x=0;
             y++;
             y%=2;
           }
           phn::msg+=characters[num];
          }

                else if(key=='5')
         {
           num=18;
           lcd->setCursor(x,y);
           lcd->print(characters[num]);
           for(int i=0;i<3000;i++)
           {
            lcd->noCursor();
            char key=keypad->getKey();
            if(key=='5')
            {
             num++;
             if(num>21)
             num=18;
             lcd->setCursor(x,y);
             lcd->print(characters[num]);
             i=0;
             delay(200);
            }
           }
           x++;
            if(x>15)
           {
             x=0;
             y++;
             y%=2;
           }
           phn::msg+=characters[num];
          }

          else if(key=='6')
         {
           num=22;
           lcd->setCursor(x,y);
           lcd->print(characters[num]);
           for(int i=0;i<3000;i++)
           {
            lcd->noCursor();
            char key=keypad->getKey();
            if(key=='6')
            {
             num++;
             if(num>25)
             num=22;
             lcd->setCursor(x,y);
             lcd->print(characters[num]);
             i=0;
             delay(200);
            }
           }
           x++;
            if(x>15)
           {
             x=0;
             y++;
             y%=2;
           }
           phn::msg+=characters[num];
          }

         else if(key=='7')
         {
           num=26;
           lcd->setCursor(x,y);
           lcd->print(characters[num]);
           for(int i=0;i<3000;i++)
           {
            lcd->noCursor();
            char key=keypad->getKey();
            if(key=='7')
            {
             num++;
             if(num>30)
             num=26;
             lcd->setCursor(x,y);
             lcd->print(characters[num]);
             i=0;
             delay(200);
            }
           }
           x++;
            if(x>15)
           {
             x=0;
             y++;
             y%=2;
           }
           phn::msg+=characters[num];
          }

         else if(key=='8')
         {
           num=31;
           lcd->setCursor(x,y);
           lcd->print(characters[num]);
           for(int i=0;i<3000;i++)
           {
            lcd->noCursor();
            char key=keypad->getKey();
            if(key=='8')
            {
             num++;
             if(num>34)
             num=31;
             lcd->setCursor(x,y);
             lcd->print(characters[num]);
             i=0;
             delay(200);
            }
           }
           x++;
            if(x>15)
           {
             x=0;
             y++;
             y%=2;
           }
           phn::msg+=characters[num];
          }

         else if(key=='9')
         {
           num=35;
           lcd->setCursor(x,y);
           lcd->print(characters[num]);
           for(int i=0;i<3000;i++)
           {
            lcd->noCursor();
            char key=keypad->getKey();
            if(key=='9')
            {
             num++;
             if(num>39)
             num=35;
             lcd->setCursor(x,y);
             lcd->print(characters[num]);
             i=0;
             delay(200);
            }
           }
           x++;
            if(x>15)
           {
             x=0;
             y++;
             y%=2;
           }
           phn::msg+=characters[num];
          }

          else if(key=='0')
         {
           num=40;
           lcd->setCursor(x,y);
           lcd->print(characters[num]);
           for(int i=0;i<3000;i++)
           {
            lcd->noCursor();
            char key=keypad->getKey();
            if(key=='0')
            {
             num++;
             if(num>41)
             num=40;
             lcd->setCursor(x,y);
             lcd->print(characters[num]);
             i=0;
             delay(200);
            }
           }
           x++;
            if(x>15)
           {
             x=0;
             y++;
             y%=2;
           }
           phn::msg+=characters[num];
          }

          else if(key=='D')
          {
            lcd->clear();
            lcd->print("Sending SMS....");
           // gsm->print("AT+CMGS=");
           // gsm->print(number);
           // delay(2000);
            gsm->print(phn::msg);
            gsm->write(26);
            delay(5000);
            lcd->clear();
            lcd->print("SMS Sent to");
            lcd->setCursor(0,1);
            lcd->print(phn::number);
            delay(2000);
            phn::number="";
            break;
          }
        }
      }

}

inline void Phone::sendData(String msg)
{
    gsm->println(phn::msg);
    delay(200);
}


void Phone::sendSms()
{
    lcd->clear();
    lcd->print("Initilising SMS");
    gsm->println("AT+CMGF=1");
    delay(400);
    lcd->clear();
    lcd->print("Enter Phone No.");
    lcd->setCursor(0,1);
    lcd->print("  then press D  ");
    delay(2000);
    lcd->clear();
    lcd->setCursor(0,1);
    gsm->print("AT+CMGS=\"");
    while(1)
    {
      serialEvent();
      char key=keypad->getKey();
      if(key)
      {
        if(key=='D')
        {
          //number+='"';
          gsm->println("\"");
          break;
        }
        else
        {
          //number+=key;
          gsm->print(key);
          lcd->print(key);
        }
      }
    }
    lcd->clear();
    lcd->print("Type your SMS...");
    lcd->setCursor(0,1);
    lcd->print("Press D to Send ");
    delay(2000);
    delay(1000);
    lcd->clear();
    lcd->setCursor(0,0);
    alphaNumericKeyboard();

}

inline void Phone::lcdStatus()
{

    lcd->setCursor(2,1);
    lcd->print("Message Sent");
    delay(2000);
}

void Phone::backButton()
{

    //lcd->setCursor(0,15);

}

void Phone::okButton()
{
    lcd->setCursor(0,4);
    lcd->print("OK");
}

void Phone::callButton()
{
    lcd->setCursor(0,4);
    lcd->print("CALL");
}


void Phone::smsButton()
{
  lcd->setCursor(0,13);
  lcd->print("SMS");
}
