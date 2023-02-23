#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Relays
#define RelayPin1 2 //D1
#define RelayPin2 16  //D2


int toggleState_1 = 1; //Define integer to remember the toggle state for relay 1
int toggleState_2 = 1; //Define integer to remember the toggle state for relay 2


const char* ssid = "A33"; //WiFI Name
const char* password = "123456789"; //WiFi Password
const char* mqttServer = "test.mosquitto.org";


#define sub1 "switch1"
#define sub2 "switch2"


#define pub1 "switch1_status"
#define pub2 "switch2_status"


WiFiClient espClient;
PubSubClient client(espClient);


void setup_wifi() {
 delay(10);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 }
 Serial.println("");
 Serial.println("WiFi connected");
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());
}



void reconnect() {

  while (!client.connected()) {
    Serial.println("Connecting to MQTT broker...");
    if (client.connect("NodeMCU")) {
      Serial.println("Connected to MQTT broker");
       client.subscribe(sub1);
      client.subscribe(sub2);
    } else {
      Serial.print("Failed with state ");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

    if (strstr(topic, sub1))
  {
      for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();

     if ((char)payload[0] == '0') {
      digitalWrite(RelayPin1, LOW);  
      toggleState_1 = 0;
      client.publish(pub1, "0");
    } else {
      digitalWrite(RelayPin1, HIGH); 
      toggleState_1 = 1;
      client.publish(pub1, "1");
    }    
  }

    else if ( strstr(topic, sub2))
  {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
  
    if ((char)payload[0] == '1') {
      digitalWrite(RelayPin2, LOW);   // Turn the LED on (Note that LOW is the voltage level
      toggleState_2 = 0;
      client.publish(pub2, "0");
    } else {
      digitalWrite(RelayPin2, HIGH);  // Turn the LED off by making the voltage HIGH
      toggleState_2 = 1;
      client.publish(pub2, "1");
    }
  }  
    else
  {
    Serial.println("unsubscribed topic");
  }
}

void setup() {
  Serial.begin(115200);
    
  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);


 //During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, HIGH);
  digitalWrite(RelayPin2, LOW);

  setup_wifi();
   client.setServer(mqttServer, 1883);
   client.setCallback(callback);
}


void loop() {
  if (!client.connected()) {
 
    reconnect();
  }
  

  
  client.loop();
}