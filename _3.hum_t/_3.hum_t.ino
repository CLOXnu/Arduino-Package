#include <dht11.h>

int pinDigitron = 2; //管脚D2连接到数码管的A脚，D3连B... D9连h
int pinWei = 10;  
int count =0;
int pin9status=0;
int countstatus=0;
int readstatus=0;
dht11 DHT11;

void setup() {
  for(int x=0; x<4; x++) {
    if(x==2)
      pinMode(pinWei+x, INPUT); //设置各脚为输出状态
    else
    {
      pinMode(pinWei+x, OUTPUT); //设置各脚为输出状态
      digitalWrite(pinWei+x, LOW);//
    }
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
    {0,1,1,0,1,1,1},//H=19
    {1,0,0,1,1,1,0},//c=20
  };
  
  if ( digit > 20 ) return;
  for (unsigned int x=0; x<7; x++)
    digitalWrite( pinDigitron + x, !abcdefg[digit][x] );
}

//在指定的位，显示指定的数字
void display(unsigned int wei, unsigned int digit) {
  #define BLANK 17
  
  for(int x=0; x<4; x++) {
     if ( x != wei- 1&&x!=2)
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

void count2()
{
  if(readstatus==0)
  {
    DHT11.read(12);
  }
  readstatus++;
  if(readstatus==100)
    readstatus=0;
  int temperature=DHT11.temperature;
  int humidity=DHT11.humidity;
  pin9status=digitalRead(9);
  if(pin9status==HIGH&&countstatus==0)
  {
    count=!count;
    countstatus=1;
  }
  if(pin9status==LOW&&countstatus==1)
    countstatus=0;
  if(count)
  {
    display(1,temperature/10);
    display(2,temperature%10);
    display(4,20);
  }
  else
  {
    display(1,humidity/10);
    display(2,humidity%10);
    display(4,19);
  }
}

void count3()
{
  pin9status=digitalRead(9);
  if(pin9status==HIGH)
  {
    count++;
    delay(1);
  }
  if(count>=10000)
    count=0;
  display(1,count/1000);
  display(2,(count%1000)/100);
  display(3,(count%100)/10);
  display(4,count%10);
}

void loop() { 
  //count1();
  count2();
  //count3();
   /*display(1, 4);//在第1位显示数字3
   display(2, 7);//在第2位显示数字2
   display(3, 8);//在第3位显示数字 b
   display(4, 6);//在第4位显示数字 F*/
}
