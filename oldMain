#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Wire.h>

#define ROWS 4U
#define COLS 4U

void setup();
void loop();
void call();
void sms();
void alfakey();
void send_data(String message);
void send_sms();
void lcd_status();
void back_button();
void ok_button();
void call_button();
void sms_button();
void gsm_init();
void serialEvent();

SoftwareSerial gsm(2, 3); // RX, TX
LiquidCrystal lcd(14,15,16,17,18,19);

char hexaKeys[ROWS][COLS] =
{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {11, 10, 9, 8}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {7,6,5,4}; //connect to the column pinouts of the keypad

byte back[8] =
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

String number = " ";
String msg = " ";
String instr = " ";
String str_sms = " ";
String str1 = " ";
int ring = 0;
int i = 0, temp = 0;
int sms_flag = 0;
char sms_num [3];
int rec_read = 0;
int temp1 = 0;


//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

String ch="1,.?!@abc2def3ghi4jkl5mno6pqrs7tuv8wxyz90 ";

void setup()
{
  gsm.begin(9600);
  lcd.begin(16,2);
  lcd.createChar(1, back);
  lcd.print("Simple Mobile ");
  lcd.setCursor(0,1);
  lcd.print("System Ready..");
  delay(1000);
  gsm_init();
  lcd.clear();
  lcd.print("System Ready");
  delay(2000);
}

void loop()
{
  serialEvent();

  if(sms_flag==1)
  {
    lcd.clear();
    lcd.print("New Message");
    int ind = instr.indexOf("+CMTI: \"SM\",");
    ind += 12;
    int k = 0;
    lcd.setCursor(0,1);
    lcd.print(ind);
    while(1)
    {
      while(instr[ind]!= 0x0D)
      {
        sms_num[k++]=instr[ind++];
      }
      break;
    }
    ind=0;
    sms_flag=0;
    lcd.setCursor(0,1);
    lcd.print("Read SMS --> D");
    delay(4000);
    instr="";
    rec_read=1;
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
    lcd.setCursor(0,0);
    lcd.print("Incomming...    ");
    lcd.setCursor(0,1);
    lcd.print(number);
    instr="";
    i=0;
  }

  else
  {
  serialEvent();
  lcd.setCursor(0,0);
  lcd.print("Call --> C      ");
  lcd.setCursor(0,1);
  lcd.print("SMS  --> B   ");

  if(rec_read==1)
  {
    lcd.write(1);
    lcd.print("   ");
  }
  else
  lcd.print("     ");
  }

   char key=customKeypad.getKey();
  if(key)
  {
    if(key== 'A')
    {
      if(ring==1)
      {
      gsm.println("ATA");
      delay(5000);
      }
    }
    else if(key=='C')
    {
      call();
    }

    else if(key=='B')
    {
      sms();
    }

    else if(key == 'D'  && temp1==1)
    {
      rec_read=0;
      lcd.clear();
      lcd.print("Please wait...");
      gsm.print("AT+CMGR=");
      gsm.println(sms_num);
      int sms_read_flag=1;
      str_sms="";
      while(sms_read_flag)
      {
        while(gsm.available()>0)
        {
          char ch=gsm.read();
          str_sms+=ch;
          if(str_sms.indexOf("OK")>0)
          {
            sms_read_flag=0;
            //break;
          }
        }
      }
      int l1=str_sms.indexOf("\"\r\n");
      int l2=str_sms.indexOf("OK");
      String sms=str_sms.substring(l1+3,l2-4);
      lcd.clear();
      lcd.print(sms);
      delay(5000);
      }
      delay(1000);
    }
}

void call()
{
  number="";
  lcd.clear();
  lcd.print("After Enter No.");
  lcd.setCursor(0,1);
  lcd.print("Press C to Call");
  delay(2000);
  lcd.clear();
  lcd.print("Enter Number:");
  lcd.setCursor(0,1);
  while(1)
  {
     serialEvent();
    char key=customKeypad.getKey();
    if(key)
    {
      if(key=='C')
      {
        lcd.clear();
        lcd.print("Calling........");
        lcd.setCursor(0,1);
        lcd.print(number);
        gsm.print("ATD");
        gsm.print(number);
        gsm.println(";");
        long stime = millis()+5000;
        int ans=1;
        while(ans==1)
        {
          while(gsm.available()>0)
          {
            if(gsm.find("OK"))
            {
              lcd.clear();
              lcd.print("Ringing....");
              int l = 0;
              str1=" ";
              while(ans==1)
              {
                while(gsm.available()>0)
                {
                  char ch=gsm.read();
                  str1+=ch;
                  if(str1.indexOf("NO CARRIER")>0)
                  {
                    lcd.clear();
                    lcd.print("Call End");
                    delay(2000);
                    ans=0;
                    return;
                  }
                 }
                  char key=customKeypad.getKey();
                   if(key == 'D')
                  {
                    lcd.clear();
                    lcd.print("Call End");
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
        number+=key;
        lcd.print(key);
      }
    }
  }
}

void sms()
{
  lcd.clear();
  lcd.print("Initilising SMS");
  gsm.println("AT+CMGF=1");
  delay(400);
  lcd.clear();
  lcd.print("After Enter No.");
  lcd.setCursor(0,1);
  lcd.print("Press D        ");
  delay(2000);
  lcd.clear();
  lcd.print("Enter Rcpt No.:");
  lcd.setCursor(0,1);
  gsm.print("AT+CMGS=\"");
  while(1)
  {
    serialEvent();
    char key=customKeypad.getKey();
    if(key)
    {
      if(key=='D')
      {
        //number+='"';
        gsm.println("\"");
        break;
      }
      else
      {
        //number+=key;
        gsm.print(key);
        lcd.print(key);
      }
    }
  }
  lcd.clear();
  lcd.print("After Enter MSG ");
  lcd.setCursor(0,1);
  lcd.print("Press D to Send ");
  delay(2000);
  lcd.clear();
  lcd.print("Enter Your Msg");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  alfakey();
}

void alfakey()
{
 int x=0,y=0;
 int num=0;
  while(1)
  {
    lcd.cursor();
    char key=customKeypad.getKey();
    if(key)
    {
       if(key=='1')
       {
         num=0;
         lcd.setCursor(x,y);
         lcd.print(ch[num]);
         for(int i=0;i<3000;i++)
         {
          lcd.noCursor();
          char key=customKeypad.getKey();
          if(key=='1')
          {
           num++;
           if(num>5)
           num=0;
           lcd.setCursor(x,y);
           lcd.print(ch[num]);
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
         msg+=ch[num];
        }

       else if(key=='2')
       {
         num=6;
         lcd.setCursor(x,y);
         lcd.print(ch[num]);
         for(int i=0;i<3000;i++)
         {
          lcd.noCursor();
          char key=customKeypad.getKey();
          if(key=='2')
          {
           num++;
           if(num>9)
           num=6;
           lcd.setCursor(x,y);
           lcd.print(ch[num]);
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
         msg+=ch[num];
        }

       else if(key=='3')
       {
         num=10;
         lcd.setCursor(x,y);
         lcd.print(ch[num]);
         for(int i=0;i<3000;i++)
         {
          lcd.noCursor();
          char key=customKeypad.getKey();
          if(key=='3')
          {
           num++;
           if(num>13)
           num=10;
           lcd.setCursor(x,y);
           lcd.print(ch[num]);
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
         msg+=ch[num];
        }

       else if(key=='4')
       {
         num=14;
         lcd.setCursor(x,y);
         lcd.print(ch[num]);
         for(int i=0;i<3000;i++)
         {
          lcd.noCursor();
          char key=customKeypad.getKey();
          if(key=='4')
          {
           num++;
           if(num>17)
           num=14;
           lcd.setCursor(x,y);
           lcd.print(ch[num]);
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
         msg+=ch[num];
        }

              else if(key=='5')
       {
         num=18;
         lcd.setCursor(x,y);
         lcd.print(ch[num]);
         for(int i=0;i<3000;i++)
         {
          lcd.noCursor();
          char key=customKeypad.getKey();
          if(key=='5')
          {
           num++;
           if(num>21)
           num=18;
           lcd.setCursor(x,y);
           lcd.print(ch[num]);
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
         msg+=ch[num];
        }

        else if(key=='6')
       {
         num=22;
         lcd.setCursor(x,y);
         lcd.print(ch[num]);
         for(int i=0;i<3000;i++)
         {
          lcd.noCursor();
          char key=customKeypad.getKey();
          if(key=='6')
          {
           num++;
           if(num>25)
           num=22;
           lcd.setCursor(x,y);
           lcd.print(ch[num]);
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
         msg+=ch[num];
        }

       else if(key=='7')
       {
         num=26;
         lcd.setCursor(x,y);
         lcd.print(ch[num]);
         for(int i=0;i<3000;i++)
         {
          lcd.noCursor();
          char key=customKeypad.getKey();
          if(key=='7')
          {
           num++;
           if(num>30)
           num=26;
           lcd.setCursor(x,y);
           lcd.print(ch[num]);
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
         msg+=ch[num];
        }

       else if(key=='8')
       {
         num=31;
         lcd.setCursor(x,y);
         lcd.print(ch[num]);
         for(int i=0;i<3000;i++)
         {
          lcd.noCursor();
          char key=customKeypad.getKey();
          if(key=='8')
          {
           num++;
           if(num>34)
           num=31;
           lcd.setCursor(x,y);
           lcd.print(ch[num]);
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
         msg+=ch[num];
        }

       else if(key=='9')
       {
         num=35;
         lcd.setCursor(x,y);
         lcd.print(ch[num]);
         for(int i=0;i<3000;i++)
         {
          lcd.noCursor();
          char key=customKeypad.getKey();
          if(key=='9')
          {
           num++;
           if(num>39)
           num=35;
           lcd.setCursor(x,y);
           lcd.print(ch[num]);
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
         msg+=ch[num];
        }

        else if(key=='0')
       {
         num=40;
         lcd.setCursor(x,y);
         lcd.print(ch[num]);
         for(int i=0;i<3000;i++)
         {
          lcd.noCursor();
          char key=customKeypad.getKey();
          if(key=='0')
          {
           num++;
           if(num>41)
           num=40;
           lcd.setCursor(x,y);
           lcd.print(ch[num]);
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
         msg+=ch[num];
        }

        else if(key=='D')
        {
          lcd.clear();
          lcd.print("Sending SMS....");
         // gsm.print("AT+CMGS=");
         // gsm.print(number);
         // delay(2000);
          gsm.print(msg);
          gsm.write(26);
          delay(5000);
          lcd.clear();
          lcd.print("SMS Sent to");
          lcd.setCursor(0,1);
          lcd.print(number);
          delay(2000);
          number="";
          break;
        }
      }
    }
}

void send_data(String message)
{
  gsm.println(message);
  delay(200);
}

void send_sms()
{
  gsm.write(26);
}

void lcd_status()
{
  lcd.setCursor(2,1);
  lcd.print("Message Sent");
  delay(2000);
  //lcd.setCursor()
  //lcd.print("")
  //return;
}

void back_button()
{
  //lcd.setCursor(0,15);
}

void ok_button()
{
  lcd.setCursor(0,4);
  lcd.print("OK");
}

void call_button()
{
  lcd.setCursor(0,4);
  lcd.print("CALL");
}

void sms_button()
{
  lcd.setCursor(0,13);
  lcd.print("SMS");
}

void gsm_init()
{
  lcd.clear();
  lcd.print("Finding Module..");
  boolean at_flag=1;
  while(at_flag)
  {
    gsm.println("AT");
    while(gsm.available()>0)
    {
      if(gsm.find("OK"))
      at_flag=0;
    }

    delay(1000);
  }

  lcd.clear();
  lcd.print("Module Connected..");
  delay(1000);
  lcd.clear();
  lcd.print("Disabling ECHO");
  boolean echo_flag=1;
  while(echo_flag)
  {
    gsm.println("ATE1");
    while(gsm.available()>0)
    {
      if(gsm.find("OK"))
      echo_flag=0;
    }
    delay(1000);
  }

  lcd.clear();
  lcd.print("Echo OFF");
  delay(1000);
  lcd.clear();
  lcd.print("Finding Network..");
  boolean net_flag=1;
  while(net_flag)
  {
    gsm.println("AT+CPIN?");
    while(gsm.available()>0)
    {
      if(gsm.find("+CPIN: READY"))
      net_flag=0;
    }
    delay(1000);
  }
  lcd.clear();
  lcd.print("Network Found..");
  delay(1000);
  lcd.clear();
}

void serialEvent()
{
  while(gsm.available())
  {
    char ch=gsm.read();
    instr+=ch;
    i++;

    if(instr[i-4] == 'R' && instr[i-3] == 'I' && instr[i-2] == 'N' && instr[i-1] == 'G' )
    {
       ring=1;
    }

    if(instr.indexOf("NO CARRIER")>=0)
    {
       ring=0;
       i=0;
    }
    if(instr.indexOf("+CMTI: \"SM\"")>=0)
    {
      sms_flag=1;
    }
  }
}
