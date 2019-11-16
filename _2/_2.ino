#include <MsTimer2.h>     //定时器库的头文件

int pinDigitron = 2; //管脚D2连接到数码管的A脚，D3连B... D9连h
int pinWei = 10;  
int count =0;
int pin9status=0;
int countstatus=0;

int aimscore[11]={0,10,30,60,100,150,210,280,360,450,550};
int score=0,level=0,eyestatus=0,times=0,part=0,second=5;


void setup() {
  for(int x=0; x<4; x++) {
     pinMode(pinWei+x, OUTPUT); //设置各脚为输出状态
     digitalWrite(pinWei+x, LOW);//
   }
     
  for(int x=0; x<8; x++) {
    if(x==7)
      pinMode(pinDigitron+x, INPUT); //设置各脚为输出状态
    else
      pinMode(pinDigitron+x, OUTPUT); //设置各脚为输出状态
  }  
}

//在数码管中显示数字的函数
void displayDigit(unsigned int digit) {
  //定义一个数组表：不同数字的abcdefg各段的取值
  unsigned int abcdefg[][7] = {
    {1,1,1,1,1,1,0},  //0
    {0,1,1,0,0,0,0},  //1
    {1,1,0,1,1,0,1},  //2
    {1,1,1,1,0,0,1},  //3
    {0,1,1,0,0,1,1},  //4
    {1,0,1,1,0,1,1},  //5
    {1,0,1,1,1,1,1},  //6
    {1,1,1,0,0,0,0},  //7
    {1,1,1,1,1,1,1},  //8
    {1,1,1,1,0,1,1},  //9
    {1,1,1,0,1,1,1},  //A
    {0,0,1,1,1,1,1},  //b
    {1,0,0,1,1,1,0},  //C
    {0,1,1,1,1,0,1},  //d
    {1,0,0,1,1,1,1},  //E
    {1,0,0,0,1,1,1},  //F
    {0,0,0,0,0,0,0},  //DOT = 16
    {0,0,0,0,0,0,0},  //BLANK = 17
    {0,0,0,0,0,0,1},  //MINUS= 18
  };
  
  if ( digit > 18 ) return;
  for (unsigned int x=0; x<7; x++)
    digitalWrite( pinDigitron + x, !abcdefg[digit][x] );
}

//在指定的位，显示指定的数字
void display(unsigned int wei, unsigned int digit) {
  #define BLANK 17
  
  for(int x=0; x<4; x++) {
     if ( x != wei- 1)
       digitalWrite(pinWei + x, LOW);
  }
  
  displayDigit(BLANK);//擦除
  digitalWrite(pinWei + wei- 1 , HIGH);//位选
  delay(1);

  displayDigit(digit);//显示数字
  delay(5);
}

void count1()
{
  pin9status=digitalRead(9);
  if(pin9status==HIGH&&countstatus==0)
  {
    count++;
    countstatus=1;
  }
  if(pin9status==LOW&&countstatus==1)
    countstatus=0;
  if(count>=10000)
    count=0;
  display(1,count/1000);
  display(2,(count%1000)/100);
  display(3,(count%100)/10);
  display(4,count%10);
}

void eyeoff()
{
  eyestatus=!eyestatus;
  times++;
}
void timecounter()
{
  second--;
}

void games()
{
  if(part==0)
  {
    display(1,8);
    display(2,8);
    display(3,8);
    display(4,8);
    if(digitalRead(9)==HIGH)
      part++;
  }
  else if(part==1)
  {
    level++;
    display(2,17);
    display(3,17);
    display(4,17);
    display(1,level);
    delay(2000);
    times=0;
    eyestatus=1;
    MsTimer2::set(600, eyeoff);
    MsTimer2::start(); 
    while(1)
    {
      if(eyestatus)
      {
        display(1,level);
        display(2,aimscore[level]/100);
        display(3,aimscore[level]/10);
        display(4,0);
      }
      else
      {
        display(1,17);
        display(2,17);
        display(3,17);
        display(4,17);
      }
      if(times==6)
      {
        MsTimer2::stop();
        break;
      }
    }
    part++;
  }
  else if(part==2)
  {
    int pin9=0;
    second=5;
    countstatus=0;
    MsTimer2::set(1000, timecounter); 
    MsTimer2::start(); 
    while(1)
    {
      if(second==0)
      {
        MsTimer2::stop();
        part++;
        break;
      }
      pin9=digitalRead(9);
      if(pin9==HIGH&&countstatus==0)
      {
        score++;
        countstatus=1;
      }
      if(pin9==LOW&&countstatus==1)
        countstatus=0;
      display(1,second);
      display(2,score/100);
      display(3,score/10);
      display(4,score%10);
    }
  }
  else if(part==3)
  {
    if(score>=aimscore[level])
    {
      eyestatus=1;
      times=0;
      MsTimer2::set(600, eyeoff); 
      MsTimer2::start();
      while(1)
      {
        if(eyestatus)
        {
          display(1,level);
          display(2,score/100);
          display(3,score/10);
          display(4,score%10);
        }
        else
        {
          display(1,17);
          display(2,17);
          display(3,17);
          display(4,17);
        }
        if(times==8)
        {
          MsTimer2::stop();
          break;
        }
      }
      if(level==10)
      {
        while(1)
        {
          display(1,12);
          display(2,score/100);
          display(3,score/10);
          display(4,score%10);
        }
      }
      else
        part=1;
    }
    else
    {
      while(1)
      {
        display(1,18);
        display(2,score/100);
        display(3,score/10);
        display(4,score%10);
      }
    }
  }
}

void loop() { 
  //count1();
  games();
   /*display(1, 4);//在第1位显示数字3
   display(2, 7);//在第2位显示数字2
   display(3, 8);//在第3位显示数字 b
   display(4, 6);//在第4位显示数字 F*/
}
