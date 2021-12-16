#IHM Client
Le langage de programmation est c++ avec Qt en framework./
Cette partie du projet nous permet d'offrir un interface homme machine à notre client.

## Principe
Le client ce connecte à un broker mqtt avec l'appuie sur le bouton connexion.
Au moment de la connexion, le client est automatiquement abonné aux topics important et l' IHM/ 
met à jour les couleurs dans les étageres./

Une fois la connexion effectué, le client pourra séléctionner une couleur et une zone de chargement/
puis enverra l'ordre par l'appuie du bouton <Envoyer ordre>./
A l'envoie de l'ordre, le client IHM stockera l'ordre et l'affichera dans une liste, un compteur/
LCD suivra en temps réel le nombre d'ordre restant à faire./

En cas de besoin un bouton permettra de lancer la cam afin d'effectuer un scan.


