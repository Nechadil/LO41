#LISEZ-MOI: Gestion de la gare


L'Université de Technologie de Belfort-Montbéliard (UTBM)

Département Génie Informatique

Li Dachen



##Configuration:

1. Initialiser les nombres de train: 
Les nombres de train par défaut sont définis par les variables **_nb_TYPEDETRAIN_SENS_** avec:
```
	TYPEDETRAIN= TGV/GL/M
	SENS= oe(sens Ouest->Est)/ eo(sens Est->Ouest)
```
2. Choisir si vous voulez ajouter les nouveaux trains ou pas:
Il existe une partie de code pour créer les nouveaux trains. Une variable globale **_ajoute_** est utilisé pour gérer cette partie de code. Cette variable est initialisé à 0, qui veut dire que l'on n'ajoute pas des nouveaux trains après le lancement du programme. Si vous voulez ajouter les autres trains à part les nombres de trains qui sont initialisés au début, vous pouvez modifier sa valeur à 1.    



##Compilation(sous Linux):

```
gcc -o main main.c -lpthread
```
