# Linky_recepteur
récepteur 2.4 ghz pour compteur linky

Ce projet permet de relier la sortie TIC d'un compteur linky sur son système domotique, sans fil.
La transmission radio s'effectue avec des modules eByte E01 2G4M13S (NRF24L01).

Principe de fonctionnement du récepteur :
Des paquets de données sont reçus par le récepteur radio.
Un automate reconstitue chaque ligne d'information du LINKY.
Lorsqu'une ligne est complète, le CRC de la ligne est calculé et comparé à celui transmis dans la ligne.
Si le CRC est bon, alors la ligne est envoyée sur le port série de l'arduino NANO et la LED d'erreur est éteinte.
Si le CRC est faut, alors la trame est jetée et une LED est allumée pour signaler l'erreur.
Cette LED permet de visualiser si la transmission est perturbée ou pas. Elle ne doit pas s'allumer ou très peu.

Voir aussi le projet Linky_emetteur.