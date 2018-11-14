#include <Servo.h>
Servo s; //������ת����
int trig=4; //�����ź�
int echo=8; //�����ź�
int HWCG; //���⴫��
int PL=7;//����
int yy=10;//����
int l,r;
int pos=90; //��ʼ���Ƕ�
 float a[6]={};  //�洢��õľ���ֵ
 float dist_last;  //���������������ֵ
unsigned int S; //����洢
unsigned int NS; //��ǰ����
void setup() {
 Serial.begin(9600); //���ò�����
   pinMode(trig,OUTPUT); //��������ģʽ
   pinMode(echo,INPUT);
   pinMode(5,OUTPUT);//������
   pinMode(0,OUTPUT);
   pinMode(7,OUTPUT);//���� ��ת LOW��ƽΪ����
   pinMode(10,OUTPUT);//���� ��ת LOW��ƽΪ����
   pinMode(12,OUTPUT);//���� �·��ϰ��� LOW��ƽΪ����
   pinMode(11,INPUT); //11Ϊ������봫��������
   pinMode(13,OUTPUT); //13Ϊled����
 digitalWrite(12,HIGH);
 s.attach(9); //��������������
 s.write(pos); //��ʼ������Ƕ�
 delay(2000); //������ʱ

}
void loop() { //������
 NS=range(); //ִ�в�ຯ��
 HWCG=digitalRead(11); //��ȡ��������Ϣ
 if(HWCG==0){ //����ģ���߼� ���ϰ��Ϳ���12���������Լ���
  digitalWrite(12,LOW);
  digitalWrite(5,HIGH);
  delay(600);
  HWCG=digitalRead(11); //��ȡ��������Ϣ
  if(HWCG==1){
  digitalWrite(12,HIGH);
  }
}
if(NS<130&&NS>80){ //������80-130֮�� ִ���߼�
 digitalWrite(5,HIGH);
timeGapless(); //���Ҳ�ಢ��������Ч��
 digitalWrite(5,LOW);
 delay(1000);

}
else if(NS<80){ //�����ڴ���80 ִ���߼�
  digitalWrite(5,HIGH);
  digitalWrite(13,HIGH);
timeGapless();
}
 else if(NS>130){ //������80���� ִ���߼�
 digitalWrite(5,LOW);
 digitalWrite(13,LOW);
}
}


void timeGapless(){ //���Ҳ��ģ��
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
int range(){ //��ຯ��
    read_dist(4,8);  //��ֵ
    read_dist_avg();  //ȡƽ��ֵ
    return dist_last; 
    //Serial.println(S);  TEST
}
void read_dist(int TrigPin,int EchoPin)
 {
    unsigned int s;
    for(s=0; s<6; s++)
    {
      /*
       * �öν�������ѡģ��SRO4�������й�
       * ������鿴����ĵ�
       * *****************************
       * Ҳ����ѡ���Ѿ���װ�øô����Ŀ�SRO4
       * *****************************
* �����ڸ������ 20�ȵĿ����еĴ����ٶȴ�ԼΪ343��/�룬��34,300����/�롣
  ���ߣ�������һ�µ�λ���㣬34,300����1,000,000����/΢�롣
  ��Ϊ��0.0343����/΢��
  �ٻ�һ���Ƕȣ�1/0.0343 ΢��/����

  ����29.15 ΢��/���ס�s=v*t(·��=�ٶ�Xʱ��),�����ٶ�v=s/t=1/29
  ��Ȼ��s������2����s������룬����v=s/2/t=s/2t=1/��2X29��=1/58
       */

       //����һ��10us�ĸ�����ȥ����TrigPin
       digitalWrite(TrigPin, LOW);
       delayMicroseconds(2);
       digitalWrite(TrigPin, HIGH);
       delayMicroseconds(10);
       digitalWrite(TrigPin, LOW);
       //��������ȣ����������
       a[s] = pulseIn(EchoPin, HIGH) / 58.00;
       delay(10);  
    }
 }



 //��6������ֵ�ٴ���(ƽ����
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
 void xuanzhuan(int TargetPos){ //��ת����
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
