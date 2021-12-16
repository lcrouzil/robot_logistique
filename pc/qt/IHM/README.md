# IHM Client
Le langage de programmation est c++ avec Qt en framework  
Cette partie du projet nous permet d'offrir un interface homme machine à notre client.

## Principe
Le client ce connecte à un broker mqtt avec l'appuie sur le bouton connexion.
Au moment de la connexion, le client est automatiquement abonné aux topics important et l' IHM  
met à jour les couleurs dans les étageres.  

Une fois la connexion effectué, le client pourra séléctionner une couleur et une zone de chargement  
puis enverra l'ordre par l'appuie du bouton <Envoyer ordre>.  
A l'envoie de l'ordre, le client IHM stockera l'ordre et l'affichera dans une liste, un compteur  
LCD suivra en temps réel le nombre d'ordre restant à faire.  

En cas de besoin un bouton permettra de lancer la cam afin d'effectuer un scan.  

## Fonctionnement 
### connexion 
- le client ce connecte au broker  
- le client s'abonne automatiquement aux topics nécessaire  

### ordre
- le client envoie un ordre  
- le broker recoit l'ordre et attend le retour du robot  
- le bouton du robot est appuyé et il ce trouve dans la zone de dépot  
- le robot envoie l'info au broker puis le broker a l'IHM  
- l'IHM repond en envoyant l'id + ordre au broker qui transmet au robot  
- a chaque déplacement validé le robot renvoie son status puis recupère la suite des ordres  
- une fois la zone de reception atteinte le robot s'arrete et attend l'appuie bouton  

### chargement et analyse
#### cas 1 - balle chargée et bonne couleur
- si la balle est chargée et que le bouton robot est appuyé, une demande de scan est envoyé vers la cam  
- le deplacement ce poursuit jusqu'a la zone d'analyse  
- la balle est detecter par la cam qui renvoie l'information de la couleur  
- mise a jour de l'affichage  
- la balle est de la bonne couleur le robot ce dirige vers sa zone de dépot (la 1 ici)  
- arrivé en zone de dépot le robot s'arrete et ne redemarre pas tant qu'il est chargé, qu'il ne possède pas d'ordre et que le bouton est appuyé  

#### cas 2 - balle chargée et mauvaise couleur
- si la balle est chargée et que le bouton robot est appuyé, une demande de scan est envoyé vers la cam
le deplacement ce poursuit jusqu'a la zone d'analyse
- la balle est detecter par la cam qui renvoie l'information de la couleur
- mise a jour de l'affichage 
-la balle est de la mauvaise couleur le robot retourne en zone de reception precedente 
- dépose la balle et attend l'appuie du bouton 
- voir cas 3 - balle non chargé 


#### cas 3 - balle non chargé 
- si la balle n' est pas chargée et que le bouton robot est appuyé, aucun demande de scan est envoyé vers la cam. 
- le deplacement ce poursuit jusqu'a la zone d'analyse 
- le robot retourne en zone de reception 
- vérification des couleurs stocké dans les etageres 
- - si connue le robot ce deplace directement à la bonne étagere 
- - si non connue le robot ce deplace à l'étage supérieur en modulo 4




