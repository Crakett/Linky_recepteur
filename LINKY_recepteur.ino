#include <SPI.h>
#include <RF24.h>

#define pinCE   9             // On associe la broche "CE" du NRF24L01 à la sortie digitale D7 de l'arduino
#define pinCSN  10             // On associe la broche "CSN" du NRF24L01 à la sortie digitale D8 de l'arduino
#define tunnel  "LINKY"       // On définit le "nom de tunnel" (5 caractères) à travers lequel on va recevoir les données de l'émetteur

RF24 radio(pinCE, pinCSN);    // Instanciation du NRF24L01

const byte adresse[6] = tunnel;       // Mise au format "byte array" du nom du tunnel
char message[32];                     // Avec cette librairie, on est "limité" à 32 caractères par message


char buffin[128];
int indexBuffer = 0;

void setup() {
  delay(3000);
  
  // Initialisation du port série (pour afficher les infos reçues, sur le "Moniteur Série" de l'IDE Arduino)
  Serial.begin(9600,SERIAL_7E1);
  //Serial.begin(9600);
  
  // Partie NRF24
  radio.begin();                      // Initialisation du module NRF24
  radio.setDataRate(RF24_1MBPS);    // en remplaçant « xxx » par RF24_250KBPS, RF24_1MBPS, ou encore, RF24_2MBPS
  radio.setChannel(100);              // en remplaçant « x » par une valeur comprise entre 0 et 125
  radio.openReadingPipe(0, adresse);  // Ouverture du tunnel en LECTURE, avec le "nom" qu'on lui a donné
  radio.setPALevel(RF24_PA_HIGH);  // en remplaçant « xxx » par RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, ou RF24_PA_MAX
  radio.startListening();             // Démarrage de l'écoute du NRF24 (signifiant qu'on va recevoir, et non émettre quoi que ce soit, ici)
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
}

void loop() 
{
 
  // On vérifie à chaque boucle si un message est arrivé
  if (radio.available()) {
    radio.read(&message, sizeof(message));     // Si un message vient d'arriver, on le charge dans la variable "message"

    TraiteMessage();

  }
}


void TraiteMessage()
{  
  int i;
  char in;
  String trame;

  for(i = 0 ; i < sizeof(message) ; i++)
  {
    in = message[i];
    if(in == 0) break;

    switch(in)  
    {
      case 0x0a:
          indexBuffer = 0;
          break;
          
      case 0x0d:
          buffin[indexBuffer] = 0;
          if(buffin[indexBuffer-1] == ckecksum(buffin,indexBuffer-1))
          {
            digitalWrite(5, LOW);
            Serial.println(buffin);         
          }
          else
          {
            digitalWrite(5, HIGH);
            //Serial.print("     CHEKSUM TRAME    ");Serial.println(buffin[indexBuffer-1]);
            //Serial.print("     CHEKSUM CALCULE  ");Serial.println(ckecksum(buffin,indexBuffer-1));            
          }
          break;
          
      case 0x02:
      case 0x03:
          break;

      default:
          if (indexBuffer < sizeof(buffin)-2)   buffin[indexBuffer++] = in;
          break;
      }
      
    }
      
}


// Calcul checksum  ----------------------------------------------------
char ckecksum(char *buff, int len)
{
int i;
char sum = 0;

    for (i=0; i<(len); i++) sum = sum + buff[i];
    sum = (sum & 0x3F) + 0x20;

    return(sum);
}
