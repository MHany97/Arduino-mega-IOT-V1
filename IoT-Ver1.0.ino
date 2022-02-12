#include <SoftwareSerial.h>
#include <FilterDerivative.h>
#include <FilterOnePole.h>
#include <Filters.h>
#include <FilterTwoPole.h>
#include <FloatDefine.h>
#include <RunningStatistics.h>

  //Flow function and variables
    volatile int NbTopsFan1; //measuring the rising edges of the signal for main flow
    volatile int NbTopsFan2; //measuring the rising edges of the signal for sensor outlet 1
    volatile int NbTopsFan3; //measuring the rising edges of the signal for sensor outlet 2
    int Calc1; //calculation of flow rate of main flow
    int Calc2;//calculation of flow rate of first outlet
    int Calc3;//calculation of flow rate of second outlet
    
    // PWM out signals :
    
    int MainFlowSignal;
    int FirstOutSignal;
    int SecondOutSignal;
    int MainFlowOPpin=13;//output pins of flow
    int FirstOutOPpin=12;
    int SecondOutOPpin=11;  
    int mainflow = 2; //The pin location of the mainflow sensor to get the reading
    int firstout = 3;//The pin location of the out1 sensor
    int secout = 18;//The pin location of the out2 sensor
    
    // functions  interupt calls in flow Sensor
    void rpm1 () 
                  {NbTopsFan1++;}     
    void rpm2 () //This is the function that the interupt calls
                  { NbTopsFan2++;}
    void rpm3 () //This is the function that the interupt calls
                  { NbTopsFan3++;}       

    float PresVal;
    int RPM_Val;


  //-----------------------------------------------------------------------------------------------------------
  // Voltage Readings
                  /*readings pins of analog signal
                   * A0 = Va
                   * A1 = Vb
                   * A2 = Vc
                   */
 //Variables for Voltage Sensors
      double sensorValueA=0;
      double sensorValueB=0;
      double sensorValueC=0;
      double sensorValue1A=0;
      int crosscountA=0;
      int climbhillA=0;
      double sensorValue1B=0;
      int crosscountB=0;
      int climbhillB=0;
      double sensorValue1C=0;
      int crosscountC=0;
      int climbhillC=0;
      double VmaxDA=0;
      double VeffDA;
      double VmaxDB=0;
      double VeffDB;
      double VmaxDC=0;
      double VeffDC;
      double VeffA;
      double VeffB;
      double VeffC;
  void MeasureA(){
          sensorValue1A=sensorValueA; // mea+sure sensor 
      delay(100);
      sensorValueA = analogRead(A0);
      if (sensorValueA>sensorValue1A && sensorValueA>511){
        climbhillA=1;
        VmaxDA=sensorValueA;
        }
      if (sensorValueA<sensorValue1A && climbhillA==1){
        climbhillA=0;
        VmaxDA=sensorValue1A;
        VeffDA=VmaxDA/sqrt(2);
        VeffA=(((((VeffDA-420.76)/-90.24)*-210.2)+210.2 )/260)*230;
        VmaxDA=0;
    
    }
  }
  void MeasureB(){
          sensorValue1B=sensorValueB;
      delay(100);
      sensorValueB = analogRead(A1);
      if (sensorValueB>sensorValue1B && sensorValueB>511){
        climbhillB=1;
        VmaxDB=sensorValueB;
        }
      if (sensorValueB<sensorValue1B && climbhillB==1){
        climbhillB=0;
        VmaxDB=sensorValue1B;
        VeffDB=VmaxDB/sqrt(2);
        VeffB=(((((VeffDB-420.76)/-90.24)*-210.2)+210.2 )/260)*230;
        VmaxDB=0;
    
    }
  }
  void MeasureC(){
          sensorValue1C=sensorValueA;
      delay(100);
      sensorValueC = analogRead(A2);
      if (sensorValueC>sensorValue1C && sensorValueC>511){
        climbhillC=1;
        VmaxDC=sensorValueC;
        }
      if (sensorValueC<sensorValue1C && climbhillC==1){
        climbhillC=0;
        VmaxDC=sensorValue1C;
        VeffDC=VmaxDC/sqrt(2);
        VeffC=(((((VeffDC-420.76)/-90.24)*-210.2)+210.2 )/260)*230;
        VmaxDC=0;
    
    }
  }      
  
//----------------------------------------------------------------------------------------

         // Current sensor variable and functions
            


//----------------------------------------------------------------------------------------
  //IoT Intilization
      String Name="Family"; //put your wifi name and password
      String Password="26532700ah";
      String DomainName="immense-shore-15063.herokuapp.com";
      String request;

    SoftwareSerial wifi(1,0);// TX RX for serial communication witb esp module

          // sensors Variables  for IoT
                
                int Voltage1 = 220;
                int Voltage2 = 223;
                int Voltage3 = 203;
                float Current1 = 1.4;
                float Current2 = 1.3;
                float Current3 = 1.1;
                int Flow1 = 14;
                int Flow2 = 15;
                int Flow3 = 12;
                float Pressure1= 1.85;
                float Pressure2 = 1;
                int RPM = 2200;
                
                String Vol1;
                String Vol2;
                String Vol3;
                String cur1;
                String cur2;
                String cur3;
                String Flw1;
                String Flw2;
                String Flw3;
                String Pres1;
                String Pres2;
                String rpm;
                String Signals;
                int lengthOfRequest;
        
              
               void Transfere(){
                Flow1 = Calc1;
                Flow2 = Calc2;
                Flow3 = Calc3;
                Voltage1 = float(VeffA);
                Voltage2 = float(VeffB);
                Voltage3 = float(VeffC);
                Pressure1 = PresVal;
                Pressure2 = RPM_Val;

                
                Vol1 = String(Voltage1);
                Vol2 ='/'+String(Voltage2);
                Vol3 ='/'+String(Voltage3);
                cur1 ='/'+String(Current1); 
                cur2 ='/'+String(Current2);
                cur3 ='/'+String(Current3); 
                Flw1 ='/'+String(Flow1);
                Flw2 ='/'+String(Flow2);
                Flw3 ='/'+String(Flow3);  
                Pres1 ='/'+String(Pressure1); 
                Pres2 ='/'+String(Pressure2);
                rpm ='/'+String(RPM)+'/';
                
                Signals = Vol1+cur1+Vol2+cur2+Vol3+cur3+Flw1+Flw2+Flw3+Pres1+Pres2;
                    }


    void connectToWifi(){   // function of ESP commands to connect to network
          wifi.println("AT+RST");
          delay(4000);
          wifi.println("AT+CWQAP");
          delay(100);
          wifi.println("AT+CWMODE=1");
          delay(100);
          wifi.println("AT+CIPMUX=1");
          delay(100);
          wifi.println("AT+CWJAP=\""+Name+"\",\""+Password+"\"");
          delay(5000);
         
              }
    void sendToServer(){
      
     
          request = "GET /create/"+Signals+" HTTP/1.1\r\nHost: "+DomainName+"\r\n\r\n";
          lengthOfRequest = request.length();
          wifi.println("AT+CIPSTART=1,\"TCP\",\"" +DomainName + "\",80");
           delay(8000);
          wifi.println("AT+CIPSEND=1,"+String(lengthOfRequest));
           delay(6000);
          wifi.println(request);
           delay(3000);  
          wifi.println("AT+CIPCLOSE=1");  
           delay(2000);  
         }




  

void setup() {
  // IOs of Pins

  pinMode(mainflow, INPUT);
  pinMode(firstout, INPUT);
  pinMode(secout, INPUT);
  pinMode(MainFlowOPpin, OUTPUT);
  pinMode(FirstOutOPpin, OUTPUT);
  pinMode(SecondOutOPpin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2) , rpm1, RISING); 
  attachInterrupt(digitalPinToInterrupt(3) , rpm2, RISING);
  attachInterrupt(digitalPinToInterrupt(18) , rpm3, RISING); 
  Serial.begin(9600);

  uint32_t baud = 115200;
  wifi.begin(baud); 
  connectToWifi();
}

void loop() {
  // put your main code here, to run repeatedly:
//Flow Sensors reading 
    NbTopsFan1 = 0; //Set NbTops to 0 ready for calculations
    NbTopsFan2 = 0; //Set NbTops to 0 ready for calculations
    NbTopsFan3 = 0; //Set NbTops to 0 ready for calculations
    sei(); //Enables interrupts
    delay (1000); //Wait 1 second
    cli(); //Disable interrupts
    Calc1 = (NbTopsFan1 * 60 / 5.5); //readings of sensors L/Hour
    Calc2 = (NbTopsFan2 * 60 / 5.5);
    Calc3 = (NbTopsFan3 * 60 / 5.5);
    MainFlowSignal = Calc1 * (255.0/665.0);
    FirstOutSignal = Calc2 * (255.0/665.0);
    SecondOutSignal = Calc3 * (255.0/665.0);
    Calc1=Calc1/60;// L/hour to L/Min
    Calc2=Calc2/60;
    Calc3=Calc3/60;
//Output Signal to RC Circuit

    analogWrite(MainFlowOPpin, MainFlowOPpin);//pin13
    analogWrite(FirstOutOPpin, FirstOutSignal);//pin 12 

// Voltage Sensors Loop

    delay(100);
    MeasureA();
    MeasureB();
    MeasureA();
    
    Transfere();
    sendToServer();



}
