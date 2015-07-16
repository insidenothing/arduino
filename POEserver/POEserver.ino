/*
  Web Server

 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe

 */

#include <SPI.h>
#include <Ethernet.h>

// adding in TempHumid 
//KY015 DHT11 Temperature and humidity sensor 
int DHpin = 8;
byte dat [5];
byte read_data () {
  byte data;
  for (int i = 0; i < 8; i ++) {
    if (digitalRead (DHpin) == LOW) {
      while (digitalRead (DHpin) == LOW); // wait for 50us
      delayMicroseconds (30); // determine the duration of the high level to determine the data is '0 'or '1'
      if (digitalRead (DHpin) == HIGH)
        data |= (1 << (7-i)); // high front and low in the post
      while (digitalRead (DHpin) == HIGH); // data '1 ', wait for the next one receiver
     }
  }
return data;
}
// end temphumid

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(10, 1, 10, 1);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

// adding in TempHumid 
void start_test () {
  digitalWrite (DHpin, LOW); // bus down, send start signal
  delay (30); // delay greater than 18ms, so DHT11 start signal can be detected
 
  digitalWrite (DHpin, HIGH);
  delayMicroseconds (40); // Wait for DHT11 response
 
  pinMode (DHpin, INPUT);
  while (digitalRead (DHpin) == HIGH);
  delayMicroseconds (80); // DHT11 response, pulled the bus 80us
  if (digitalRead (DHpin) == LOW);
  delayMicroseconds (80); // DHT11 80us after the bus pulled to start sending data
 
  for (int i = 0; i < 4; i ++) // receive temperature and humidity data, the parity bit is not considered
    dat[i] = read_data ();
 
  pinMode (DHpin, OUTPUT);
  digitalWrite (DHpin, HIGH); // send data once after releasing the bus, wait for the host to open the next Start signal
}
// end temphumid

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  // adding in TempHumid
  pinMode (DHpin, OUTPUT);
  // end temphumid
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    // adding in TempHumid
    start_test ();
    // end TempHumid
    Serial.println("connect");
    // Turn on Backlight
    Serial.write(25); 
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          // send a tone
          Serial.write(209);
          Serial.write(220);
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 60");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          

            int sensorReading = analogRead(0);
            if (sensorReading > 1000){
              client.print("Patrick is at his Desk A0>1000<br />"); 
            }else{
              client.print("Patrick is away from his DeskA0>1000<br />");
            }
            
           int sensorReading5 = analogRead(5);
            if (sensorReading5 < 200){
              client.print("Lights are detected as being on A5>200<br />"); 
            }else{
              client.print("Lights are detected as being off A5<200<br />");
            }
            
          // adding in TempHumid
          client.print ("Current humdity =");
          client.print (dat [0], DEC); // display the humidity-bit integer;
          client.print ('.');
          client.print (dat [1], DEC); // display the humidity decimal places;
          client.println ('%');
          client.print("<br />");
          client.print ("Current temperature =");
          client.print (dat [2], DEC); // display the temperature of integer bits;
          client.print ('.');
          client.print (dat [3], DEC); // display the temperature of decimal places;
          client.println ('C');
          // end TempHumid
          client.println("<br />");
          client.println("<br />Debug:<br/>");
          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            client.print("analog input ");
            client.print(analogChannel);
           client.print(" is ");
            client.print(sensorReading);
            client.println("<br />");
          }
          
          client.println("<br />");
          client.print("<small>1 minute auto refresh</small>");
          client.println("<br />");
          client.print("<small>Arduino POE Ethernet <br> Simplytronics Wide Angle PIR <br> Parallax 4x30 LCD <br> KY015 DHT11 Temperature and humidity sensor</small>");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("data sent");
    // turn off backlight after 3s delay
    delay(30);
    Serial.write(18);
    Serial.write(21);
  }
}

