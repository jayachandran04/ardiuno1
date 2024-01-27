String red = "";  
String green = "";  
String blue = "";  
String len = "";  
String bdh="";
float wall_size,data4,data5;
int data1,data2,data3;
void timer(int time1,int time2,int time3,float siz);
void setup()   
{
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  Serial.begin(9600);  
}  
  
void loop()   
{  
    Serial.println("Enter value for red.");  
    while (Serial.available() == 0)   
    {} //Wait for user input  }  
    red = Serial.readString(); //Reading the Input string from Serial port.  
    Serial.println("Enter value for green.");  
    while (Serial.available() == 0) {}  
    green = Serial.readString();  
    Serial.println("Enter value for blue.");  
    while (Serial.available() == 0) {}  
    blue = Serial.readString();  
    Serial.println("Enter value for length.");  
    while (Serial.available() == 0) {}  
    len = Serial.readString(); 
    Serial.println("Enter value for breadth.");  
    while(Serial.available() == 0) {}  
    bdh = Serial.readString();  
    data1=red.toInt();
    data2=green.toInt();
    data3=blue.toInt();
    data4=len.toFloat();
    data5=bdh.toFloat();
    wall_size=data4*data5;
    Serial.println("-------------------------"); //Showing the details  
    Serial.println("red:" + (String)data1);  
    Serial.println("green:" + (String)data2);  
    Serial.println("blue:" + (String)data3);  
    Serial.println("wall_size:" + (String)wall_size);  
    Serial.println("Thanks You...");  
    Serial.println("");  
    timer(data1,data2,data3,wall_size);
   // while (Serial.available() == 0) {} */
    }
    void timer(int motor1,int motor2,int motor3,float area)
    {
      //motor1 on
      digitalWrite(8,HIGH);
      delay(motor1);
      digitalWrite(8,LOW);
      delay(1000);
      Serial.println(motor1);
      //motor2 on
      digitalWrite(9,HIGH);
      delay(motor2);
      digitalWrite(9,LOW);
      delay(1000);
      Serial.println(motor2);
    //  motor3 on
      digitalWrite(10,HIGH);
      delay(motor3);
      digitalWrite(10,LOW);
      delay(1000);
      Serial.println(motor3);
      // motor4 on
      digitalWrite(11,HIGH);
      delay(area);
      digitalWrite(11,LOW);
      delay(1000);
      Serial.println(area);
      
    }
