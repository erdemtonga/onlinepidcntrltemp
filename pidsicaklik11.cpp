#include <OneWire.h>
#include <DallasTemperature.h>

       //SICAKLIK SENSORU AYARLARI
OneWire oneWire(6);
DallasTemperature DS18B20(&oneWire);
DeviceAddress DS18B20adres;
boolean timerdurdu= false;
float data;
float sicaklik;
float referans = 38.9; // SICAKLIGIN KALMASI ISTENILEN DEGER 

       // PID TANIMLAMALARI
float hata = 0.0, onceki_hata = 0.0, toplam_hata = 0.0;
float kontrol_isareti = 0.0;
float ornekleme_zamani = 1.0; //örnekleme zamanı 1 sn (timer 1 Hz)
float Kp = 100.0, Ki = 1.0, Kd = 10.0;

void setup() {

            //TIMER INTERRUPT AYARLARI


  cli(); // stop interrupts
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1  = 0; // initialize counter value to 0
  // set compare match register for 1 Hz increments
  OCR1A = 62499; // = 16000000 / (256 * 1) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12, CS11 and CS10 bits for 256 prescaler
  TCCR1B |= (1 << CS12) | (0 << CS11) | (0 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei(); // allow interrupts

  
  Serial.begin(9600);
  pinMode(5, OUTPUT);

  DS18B20.begin();
  DS18B20.getAddress(DS18B20adres, 0);
  DS18B20.setResolution(DS18B20adres, 12);
  
}

void loop() {
  if(timerdurdu){
    digitalWrite(5,1);
    delay(1000);
    digitalWrite(5,0);
    delay(1000);
    TIMSK1 |= (1 << OCIE1A);
    timerdurdu=false;
    }
 
}




ISR(TIMER1_COMPA_vect){
    data = Serial.read();
    Serial.println(data);
    if((data == 48) || (data == 130)){
     TCCR1B=0;
     TIMSK1 |= (0 << OCIE1A);
     timerdurdu = true;
     Serial.println("manuel mod aktif");
    
  DS18B20.requestTemperatures();
  sicaklik = DS18B20.getTempC(DS18B20adres);
  Serial.print("Sicaklik: ");
  Serial.println(sicaklik);

  /* PID KONTROL */
  onceki_hata = hata;
  hata = referans - sicaklik;
  toplam_hata += hata;

  kontrol_isareti = Kp * hata + Ki * toplam_hata / ornekleme_zamani + Kd * (hata - onceki_hata) / ornekleme_zamani;
  if (kontrol_isareti > 255) kontrol_isareti = 255.0;
  if (kontrol_isareti < 0) kontrol_isareti = 0.0;
  Serial.print("Kontrol isareti: ");
  Serial.println(kontrol_isareti);
 
  /* ON/OFF KONTROL */
    if(kontrol_isareti < 60) digitalWrite(5,0);  
    else if(kontrol_isareti > 60) digitalWrite(5,1);
    
    
}
}
  
