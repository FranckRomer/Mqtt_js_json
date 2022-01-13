//    Conexion basica con el modelo de mqtt
//Librerias
#include <WiFi.h>
#include <PubSubClient.h>
#include <iostream>
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>

////////////////////////////////////////////////////////
// Configuracion Internet
const char* ssid = "RED ACCESA";
const char* password = "037E32E7";

////////////////////////////////////////////////////////
//    Configuracion MQTT
//const char* mqtt_server = "143.110.229.128"; 
const char* mqtt_server = "192.168.1.221"; 
// topic_basic = Accesa/project/client_accesa/web  // envio
// topic_basic = Accesa/project/client_accesa/esp   //  recibir
String rootTopic = "Accesa";
String project = "cia_baños";
String client_accesa = "laboratorio";

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
//   Inicio
String topicPubString = rootTopic + "/"+ project + client_accesa +"/web";
String topicSubString = rootTopic + "/"+ project + client_accesa +"/esp";
String clienteIDString = rootTopic + "_"+ project + client_accesa + "_admit" + random(1,100);

char path[] = "/";                    //no tiene otras direcciones 
const char* topic_sub = topicSubString.c_str();
const char* topic_pub = topicPubString.c_str();
const char* clienteID = clienteIDString.c_str();
///////////////////////////////////////////////////////
/*
 *  OBJETOS
*/
String json;

/*
 * VARIABLES Mqtt
*/

WiFiClient espClient;
PubSubClient client(espClient);

/*
 * VARIABLES GLOBALES
*/


/*
 * ********************************************************************
 *                                 Setup
 * ********************************************************************
*/

void setup() {

  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  client.publish(topic_pub,"Mensaje Recibido");

}

/*
 * ********************************************************************
 *                              Setup WIFI
 * ********************************************************************
*/

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

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
/*
 * ********************************************************************
 *                              Reconectado
 * ********************************************************************
*/

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clienteID)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(topic_pub, "Enviando el primer mensaje desde Cash-32");

      client.subscribe(topic_sub);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
  
      delay(5000);
    }
  }

}
/*
 * ********************************************************************
 *                              CallBack
 * ********************************************************************
*/

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived MQTT [");
  Serial.print(topic);
  Serial.print("] ");
  
  char MQTT_to_CAN[length] ;
  String payload_string = "";
  String Lampara_Encender = "";
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    Lampara_Encender = Lampara_Encender + (char)payload[i];
    
  }
  Serial.println();

  //Deserializacion de Json y envio a red CAN
  DeserializeObject(Lampara_Encender);

}


/*
 * ************************************************************************************************
 *                                          JSON
 * ************************************************************************************************
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
//    Serializacion
void SerializeObject() {
    char buffer[256];   
    StaticJsonDocument<300> doc;

    doc["id_monedero"] = "monedero_prueba";
    doc["fecha"]   = random(1,100);
    doc["hora"] = random(1,100);
    doc["monto"] = random(1,100);
    doc["cont_usuarios"] = random(1,100);
    doc["cont_empleados"] = random(1,100);
    
    serializeJson(doc, buffer);
    
    size_t n = serializeJson(doc, buffer);
    client.publish("prueba", buffer, n);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
//    Deserializacion 
void DeserializeObject(String dato_json) {   
  String envio_json_can = dato_json;
    StaticJsonDocument<300> doc;
    
    DeserializationError error = deserializeJson(doc, envio_json_can);
    if (error) { return; }
    String can        = doc["can"];
    String pin        = doc["pin"];
    String percentage = doc["percentage"];
    String rgb        = doc["rgb"];

    
    envio_json_can = "FF1"+ can + pin + percentage + "00" + rgb;
    Serial.println("------------------json desarmado------------------");
    Serial.println(envio_json_can);
    
}

/*
 * ********************************************************************
 *      Programa principal
 * ********************************************************************
*/

void loop() {
  
// Reconexion mqtt
  if (!client.connected()) {  
    reconnect();  
  }
  client.loop();
  long now = millis(); 
  String json_ouput;
  /////////////////////////////////////////////////////////////////








  /////////////////////////////////////////////////////////////////
  //  Envio MQTT
  Serial.println("ACTIVACIÓN/DESACTIVACIÓN CONFIRMADA");
  SerializeObject();
  //client.publish(topic_pub, dataSnd);
  
  // Transformacion y envio de Json
  //SerializeObject(dataSnd);

  delay(1000);
}
