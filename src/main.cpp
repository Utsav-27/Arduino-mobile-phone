////////////////////////////////////////////////////////////////////////////////////////////////////
//*******FILE NAME:   main.cpp**********************************************************************
//*******AUTHOR:      Chima Okwara (AlphaChi)*******************************************************
//*******DESC:        Main program  for Arduino Phone **********************************************
//*******LICENCE:     GPL Version 3 ****************************************************************
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <Arduino.h>
#include <phone.hpp>

void setup(void);
void loop(void);

Phone *myPhone { nullptr };



void setup()
{
  uint8_t gsmRx = 11;
  uint8_t gsmTx = 12;
  uint8_t lcdPins[] = {5, 6, 7, 8, 9, 10};
  uint8_t keypadRowsColumns [][4] =
  {
    {1, 2, 3, 4},
    {13, 14, 15, 16}
  };
  char keys[ROWS][COLUMNS] =
  {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
  };

  PhoneData pdata;
  pdata.gsmRx = gsmRx;
  pdata.gsmTx = gsmTx;
  *pdata.lcd = lcdPins;
  *pdata.keyRowCol[0] = keypadRowsColumns[0];
  *pdata.keys[0] = keys[0];


  myPhone = new Phone(pdata);
  myPhone->init();
}

void loop()
{
  using namespace phn;
  myPhone->serialEvent();
  if(smsFlag == 1)
  {
      myPhone->cls();
      myPhone->print("New Message");
      int ind = instr.indexOf("+CMTI: \"SM\",");
      ind += 12;
      int k = 0;
      myPhone->setCursor(0,1);
      myPhone->print(ind);
      while(1)
      {
        while(instr[ind]!= 0x0D)
        {
          smsNum[k++]=instr[ind++];
        }
        break;
      }
      ind=0;
      smsFlag=0;
      myPhone->setCursor(0,1);
      myPhone->print("Read SMS --> D");
      delay(4000);
      instr="";
      recRead=1;
      temp1=1;
      i=0;
  }

  if(ring == 1)
  {

      number="";
      int loc=instr.indexOf("+CLIP: \"");
      if(loc > 0)
      {
        number+=instr.substring(loc+8,loc+13+8);
      }
      myPhone->setCursor(0,0);
      myPhone->print("Incomming...    ");
      myPhone->setCursor(0,1);
      myPhone->print(number);
      instr="";
      i=0;

  }

  else
  {

    myPhone->serialEvent();
    myPhone->setCursor(0,0);
    myPhone->print("Call --> C      ");
    myPhone->setCursor(0,1);
    myPhone->print("SMS  --> B   ");

    if(recRead==1)
    {
      myPhone->write(1);
      myPhone->print("   ");
    }
    else
    myPhone->print("     ");
  }

  char key = myPhone->getKey();
  if(key)
  {

      if(key== 'A')
      {
        if(ring==1)
        {
        myPhone->gsmPrintln("ATA");
        delay(5000);
        }
      }
      else if(key=='C')
      {
        myPhone->call();
      }

      else if(key=='B')
      {
        myPhone->sms();
      }

      else if(key == 'D'  && temp1==1)
      {
        recRead=0;
        myPhone->cls();
        myPhone->print("Please wait...");
        myPhone->gsmPrint("AT+CMGR=");
        myPhone->gsmPrintln(smsNum);
        int sms_read_flag=1;
        strSms="";
        while(sms_read_flag)
        {
          while(myPhone->gsmAvail()>0)
          {
            char ch=myPhone->gsmRead();
            strSms+=ch;
            if(strSms.indexOf("OK")>0)
            {
              sms_read_flag=0;
              //break;
            }
          }
        }
        int l1=strSms.indexOf("\"\r\n");
        int l2=strSms.indexOf("OK");
        String sms =strSms.substring(l1+3,l2-4);
        myPhone->cls();
        myPhone->print(sms);
        delay(5000);
        }
        delay(1000);

  }

}
