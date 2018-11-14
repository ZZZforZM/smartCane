#include <Servo.h>
Servo s; //超声波转向舵机
int trig=4; //发射信号
int echo=8; //接收信号
int HWCG; //红外传感
int PL=7;//语音
int yy=10;//语音
int l,r;
int pos=90; //初始化角度
 float a[6]={};  //存储测得的距离值
 float dist_last;  //处理过后的理想距离值
unsigned int S; //距离存储
unsigned int NS; //当前距离
void setup() {
 Serial.begin(9600); //设置波特率
   pinMode(trig,OUTPUT); //设置引脚模式
   pinMode(echo,INPUT);
   pinMode(5,OUTPUT);//震动引脚
   pinMode(0,OUTPUT);
   pinMode(7,OUTPUT);//语音 左转 LOW电平为开启
   pinMode(10,OUTPUT);//语音 右转 LOW电平为开启
   pinMode(12,OUTPUT);//语音 下方障碍物 LOW电平为开启
   pinMode(11,INPUT); //11为红外距离传感器引脚
   pinMode(13,OUTPUT); //13为led引脚
 digitalWrite(12,HIGH);
 s.attach(9); //定义舵机所用引脚
 s.write(pos); //初始化舵机角度
 delay(2000); //开机延时

}
void loop() { //主函数
 NS=range(); //执行测距函数
 HWCG=digitalRead(11); //读取红外测距信息
 if(HWCG==0){ //红外模块逻辑 有障碍就开启12引脚语音以及震动
  digitalWrite(12,LOW);
  digitalWrite(5,HIGH);
  delay(600);
  HWCG=digitalRead(11); //读取红外测距信息
  if(HWCG==1){
  digitalWrite(12,HIGH);
  }
}
if(NS<130&&NS>80){ //距离在80-130之间 执行逻辑
 digitalWrite(5,HIGH);
timeGapless(); //左右测距并返回语音效果
 digitalWrite(5,LOW);
 delay(1000);

}
else if(NS<80){ //距离在大于80 执行逻辑
  digitalWrite(5,HIGH);
  digitalWrite(13,HIGH);
timeGapless();
}
 else if(NS>130){ //距离在80以内 执行逻辑
 digitalWrite(5,LOW);
 digitalWrite(13,LOW);
}
}


void timeGapless(){ //左右测距模块
 xuanzhuan(145);
delay(500);
 l= range();
xuanzhuan(35);
delay(500);
 r= range();
if(l>r){
  digitalWrite(PL,LOW);
  delay(1200);
     xuanzhuan(90);
  delay(500);
  digitalWrite(PL,HIGH);
   }
   else{
  digitalWrite(yy,LOW);
  delay(1200);
     xuanzhuan(90);
  delay(500);
  digitalWrite(yy,HIGH);
   }

}
int range(){ //测距函数
    read_dist(4,8);  //读值
    read_dist_avg();  //取平均值
    return dist_last; 
    //Serial.println(S);  TEST
}
void read_dist(int TrigPin,int EchoPin)
 {
    unsigned int s;
    for(s=0; s<6; s++)
    {
      /*
       * 该段仅仅与所选模块SRO4的特性有关
       * 具体请查看相关文档
       * *****************************
       * 也可以选用已经封装好该代码块的库SRO4
       * *****************************
* 声音在干燥、摄氏 20度的空气中的传播速度大约为343米/秒，合34,300厘米/秒。
  或者，我们作一下单位换算，34,300除以1,000,000厘米/微秒。
  即为：0.0343厘米/微秒
  再换一个角度，1/0.0343 微秒/厘米

  即：29.15 微秒/厘米。s=v*t(路程=速度X时间),所以速度v=s/t=1/29
  当然这s是来回2倍的s所测距离，所以v=s/2/t=s/2t=1/（2X29）=1/58
       */

       //产生一个10us的高脉冲去触发TrigPin
       digitalWrite(TrigPin, LOW);
       delayMicroseconds(2);
       digitalWrite(TrigPin, HIGH);
       delayMicroseconds(10);
       digitalWrite(TrigPin, LOW);
       //检测脉冲宽度，并计算距离
       a[s] = pulseIn(EchoPin, HIGH) / 58.00;
       delay(10);  
    }
 }



 //对6个距离值再处理(平均）
 void read_dist_avg()
 {
    unsigned int i;
    float dist_deal;
    dist_deal=0;

    for(i=1;i<5;i++)
    {
      dist_deal+=a[i];
    }
    dist_last=dist_deal*100/400;
 }
 void xuanzhuan(int TargetPos){ //旋转函数
  if(pos<TargetPos){
       for(;pos<TargetPos;pos +=1){
          s.write(pos);
          delay(10);
      }

  }else{
    for(;pos>TargetPos;pos -=1){
          s.write(pos);
           delay(10);
     }
   
  }
 }
