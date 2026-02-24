# Projet final C — Space Invaders (SDL2) - Laura ZHUANG

## 🎮 Commandes
- Le joueur peut bouger en utilisant les flèches directionnelles
- Il peut tirer un seul missile à la fois (comme dans le jeu originel) avec espace
- Pour naviguer dans les menus, il faut appuyer sur les touches directionnelles et une fois la bonne option sélectionnée il faut appuyer sur entrée
- Il y a aussi l'option d'appuyer sur les nombres du clavier correspondant aux différentes options (je suis sur qwerty je ne sais pas si ça fonctionne en azerty)

# 💬 Commentaire sur les fonctionnalitées attendues :

## 🟢 Niveau 1
- Normalement tout fonctionne pour le niveau 1

---

## 🟡 Niveau 2
- Les ennemis tirent à intervalle régulier et il y a un seul tir ennemi à la fois, mais l'ennemi qui tire est tiré au sort.
- Les coeurs tombent des cadavres des ennemis tués avec une chance de 10%
- La vie du joueur est cap à 5 pv MAX

---

## 🟠 Niveau 3

- J'ai pas fait les ennemis qui tirent plus souvent.
- La vitesse n'est pas paramétrable (mais elle est progressive, quand les ennemis touchent le mur et descendent ils vont plus vite même si c'est peu perceptible)
- Les ennemis rapides sont en jaune et sont plus petits que les ennemis normaux, les ennemis avec plus de points de vie sont en orange (quand ils ont encoroe 2 pv, quand ils passent à 1 pv ils redeviennent rouges)

---

## 🔴 Niveau 4

- La sauvegarde ne sauvegarde que le niveau auquel le joueur est (mais la progression interne dans le niveau est supprimée), ainsi que le nombre de pv du joueur.
- Lorsque le joueur appuie sur Save Game, il n'y a aucun message affiché mais c'est sauvegardé quand même
- Si on essaie de load game alors qu'aucune game est save, le jeu s'arrête et un message d'erreur s'affiche
- Le jeu est très dur à finir (parce qu'il n'y a pas de vitesse max des ennemis et que la vitesse augmente très vite vers la fin) mais il est finissable (je l'ai fini à un pixel près)
---

## Instruction de compilation

Dans le terminal :  
`make`  
`./space-invaders`

---

## Le mot de la fin

J'ai beaucoup aimé ce projet, ainsi que tous les cours sur le C (même si j'ai pas toujours tout compris, le projet m'a beaucoup aidé à comprendre certaines subtilités). Merci pour ce demi-semestre !


