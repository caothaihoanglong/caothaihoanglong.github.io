include <WiFi.h>
#include <BlynkSimpleEsp32.h> 
#include <WiFiClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
unsigned long time1;
int value,real_value;
const int DHTPIN=19;                                      //Khởi tạo chân 19 cho cảm biến độ ẩm, nhiệt độ
const int DHTTYPE=DHT11;
LiquidCrystal_I2C lcd(0x3F,16,2);                         //Khai báo loại lcd được dùng
char auth[] = "DBl5Tnb-YPKv8DDLTms7aIVtFjfV5-zM";
char ssid[] = "Hấu Cà Na";
char pass[] = "maytunghidi";
const int relay_pumb=33;                                  //Khởi tạo chân cho relay điều khiển máy bơm là 33
WidgetLED led(V0);                                        
BLYNK_WRITE(V0)
{
  int pinValue = param.asInt();                          //Gán giá trị value bằng với trạng thái thay đổi của nút nhấn V0 trên blynk
  digitalWrite(32,pinValue);                             //Gán giá trị value vào chân 32
  if (digitalRead(32)) {
    led.off();                                           //Tắt đèn 
    Blynk.virtualWrite(V0,pinValue);                     //Thay đổi giá trị hiển thị trên nút V0 trên blynk
  } else { 
    led.on();                                            //Bật đèn.
    Blynk.virtualWrite(V0,pinValue);                     //Thay đổi giá trị hiển thị trên nút V0 trên blynk
  }
}
DHT dht(DHTPIN, DHTTYPE);
void setup(){
  time1=millis();
  Serial.begin(9600);
  lcd.init();                                          
  lcd.backlight();                                       //Bật đèn nền lcd
  lcd.print("Nhiet do: ");
  lcd.setCursor(0,1);                                    //Chuyển vị trí hiển thị đến hàng 0, cột 1
  lcd.print("Do am: ");                                  //Hiển thị dòng chữ "ĐỘ Ẩm"
  dht.begin();                                           //Cho dht hoạt động
  Blynk.begin(auth,ssid, pass);                          //Khởi tạo blynk
  pinMode(32,OUTPUT);                                    //lấy giá trị ra ở chân 32.
  pinMode(33,OUTPUT);                                    //Lấy giá trị ra ở chân 33
  digitalWrite(32,LOW);                                  //Co chân 32 ở mức điện áp thấp
}
void loop(){
  if((unsigned long) (millis()-time1)>1000){
    Blynk.run();                                           //Chạy blynk
    for(int i=0;i<=9;i++){                                 //Dùng hàm for để đọc 10 lần giá trị cảm biến, sau đó lấy giá trị trung bình để được giá trị chính xác nhất.
    real_value+=analogRead(34);
    }                                               
    value=real_value/10;
    int percent = map(value, 4095, 1860, 0, 100);          // Set giá thang giá trị đầu và giá trị cuối để đưa giá trị về thang từ 0-100. Giá trị thực đo được là 4095-1850. 
    real_value=0;                                          //Đặt lại giá trị tổng
    if(percent<45){                                       //Nếu giá trị độ ẩm dưới 45% thì bật bơm, ko thì tắt bơm
    digitalWrite(33,HIGH);
    }
    else{
      digitalWrite(33,LOW);
    }
    int h1=dht.readHumidity();                             //Gán h1 bằng độ ẩm đo được
    int t1=dht.readTemperature();                          //Gán t1 bằng nhiệt độ đo được
    Blynk.virtualWrite(V5,h1);                             //Gán h1 bằng V5 trên blynk
    Blynk.virtualWrite(V6,t1);                             //Gán t1 bằng V6 trên blynk
    Blynk.virtualWrite(V2,percent);                        //Gán percent bằng V2 trên blynk
    lcd.setCursor(9,0);
    lcd.print(t1);
    lcd.print("doC");
    lcd.setCursor(9,1);
    lcd.print(h1);
    lcd.print("%");
    time1=millis();
  }
}
