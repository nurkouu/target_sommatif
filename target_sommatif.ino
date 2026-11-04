#include <Arduino.h>

#include <MicroOscSlip.h>
MicroOscSlip<128> monOsc(&Serial);

#include <Bounce2.h>
Bounce2::Button bouton;

#include <Chrono.h>
Chrono chronoPot;

void setup()
{
    Serial.begin(115200);
    bouton.attach(8, INPUT_PULLUP);
    bouton.setPressedState(LOW);
    pinMode(5,OUTPUT); //initialiser la DEL du bouton branché dans la broche 5
}


// FONCTION APPELÉE LORSQU’UN MESSAGE OSC EST REÇU
void traiterMessageOsc(MicroOscMessage & message) {
  // SI L’ADRESSE DU MESSAGE EST "/ready" ET QUE LE MESSAGE CONTIENT UN ARGUMENT ENTIER ("i") :
   if (message.checkOscAddressAndTypeTags("/ready", "i")) {  

      int valeur = message.nextAsInt();  // RÉCUPÉRER L’ENTIER ("i")

      digitalWrite(5, valeur); // ASSIGNER LA VALEUR A LA DEL
 } 
}




void loop()
{
    monOsc.onOscMessageReceived(traiterMessageOsc); // ACQUISITION DES MESSAGES
    bouton.update();
    if (bouton.pressed()) { // Bouton vient d’être appuyé
        monOsc.sendInt("/bang", 1);
    }


    if (chronoPot.hasPassed(25))
    {                        // SI LE CHRONO DÉPASSE 20 MILLISECONDES
        chronoPot.restart(); // REPARTIR LE CHRONO

        // LE 3 CI-BAS CORRESPOND À LA BROCHE ANALOGIQUE A1
        int valeur = analogRead(1); // LECTURE DE LA TENSION DE LA BROCHE A1 ENTRE 0 ET 1023

        monOsc.sendInt("/turn", valeur); // ENVOYER LA VALEUR
    }
}

