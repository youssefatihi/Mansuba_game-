Dans le Makefile il y a trois exécutables:

test_set pour tester les fonctions de ensemble.c
test_1 pour tester les autres fonctions du projet
project qui exécute la totalité de l'achivement courante 


----les modes d'affichage:
affichage terminal qui affiche des images consécutives colorées sur le terminal.
le deuxième affichage est un affichage sdl

On choisit l'affichage terminal ou sdl à partir de la boucle principle, par défaut, vous trouverez l'affichage termianl sinon on met display en commentaire et on met sdl_display sans commentaire on trouve l'affichage sdl.

Après l'achievement 2 il y a juste l'affichage terminal pour la forme hexagone

----compiler le projet
make all génère tous les exécutables

on choisit le type du plateau de jeu par:

make RELATION=Numéro de la relation  (le numéro doit être 1 ou 2 ou 3):
par exemple:
make RELATION=2

make test génère les deux exécutables test_set et test_1.
---exécution project
	affichage sdl
 ./project -m (entrer le numéro) -s (entrer le numéro) -t (soit s(simple) soit c(complexe)) | ./sdl
 
 affichage terminal
   ./project -m (entrer le numéro) -s (entrer le numéro) -t (soit s(simple) soit c(complexe))

---exécution test_1 et test_set 
./test_1
./test_set# Mansuba_game-
