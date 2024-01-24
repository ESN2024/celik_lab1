<h1> ESN11 - lab1 </h1>

<h2> Objectif </h2>

Nous entreprenons la programmation d'un système de base Nios II sur un FPGA DE10-Lite dans le but de superviser un ensemble de périphériques intégrés à la carte électronique. Notre objectif consiste à mettre en place un chenillard de LED déclenché par un bouton-poussoir avec une vitesse réglable par plusieurs commutateurs.

<h2> Introduction </h2>

Ce document expose la mise en œuvre d'un chenillard qui se déclenche dès l'appui sur le bouton KEY1 de la carte activé par une interruption. La vitesse du chenillard peut être ajustée à l'aide des 4 bits des premiers switchs. Le programme utilise 2 types d'interruptions :

- L'interruption qui amorce le chenillard lors de l'appui sur le bouton.
- L'interruption qui modifie la vitesse du chenillard déclenchée par le changement d'état des commutateurs.

<h2> Création d'un système NIOS II basique </h2>

<h3> Schéma fonctionnel </h3>

Nous débuterons par la création d'une représentation schématique du système final avant d'entamer la phase de mise en œuvre. Il se concentre uniquement sur les périphériques définis dans nos objectifs. Nous pouvons donc retrouver l'architecture qui englobe plusieurs modules essentiels :

![image](https://github.com/ESN2024/celik_lab1/assets/117944504/db1e5e4c-b6c7-4ffe-b261-61fbdcc5e197)

<h3> Platform Designer </h3>

Suite à la création d'un nouveau projet sur Quartus, procéder à l'ouverture du Platform Designer afin d'intégrer tous les éléments requis.

![image](https://github.com/ESN2024/celik_lab1/assets/117944504/7fe78234-9f81-4efc-a00f-7a1963b138c6)

Les composants à ajouter comprennent :

- RAM0 : Une mémoire d'une capacité de 40 Mo amplement suffisante pour cette implémentation.
- 3 PIOs : Ces derniers permettant au FPGA de communiquer avec l'extérieur en lisant des signaux d'entrées (capturer les états des boutons-poussoirs et des switchs) et en envoyant des signaux de sortie (allumage des LEDs). Ce processus remplace le polling qui scrute en permanence l'arrivée du signal recherché.
- Processeur Nios II : Un processeur soft-core configurable du FPGA présent sur la DE10 Lite. En tant que soft-core, il est implémenté en logique programmable plutôt que fabriqué sous forme de puce physique. Cette nature configurable offre une personnalisation du processeur en fonction de nos besoins spécifiques, entraînant un réel gain de performances.
  
<h3> Quartus </h3>

Suite au Platform Designer, nous allons effectuer une vérification minutieuse pour s'assurer de l'absence d'erreurs puis procéder à la génération du code VHDL. Ensuite, nous allons établir les connexions des entrées/sorties en associant les broches correspondantes sur la carte en utilisant l'outil Pin Planner de Quartus pour une configuration précise. 

![image](https://github.com/ESN2024/celik_lab1/assets/117944504/c98385b5-fb47-49f1-a9b1-4008ad86ed9a)

Après cette étape, compiler le projet Quartus afin de préparer le fichier de configuration puis programmer la carte à l'aide de l'USB-Blaster pour transférer le code vers le FPGA.

![image](https://github.com/ESN2024/celik_lab1/assets/117944504/631d0b66-4f33-4838-97f3-0a2a6b3b95b6)

<h3> Flux logiciel du NIOS II </h3>

Ouvrir un terminal NIOS II et générer le BSP en lien avec les spécifications de la carte DE-10 :

![image](https://github.com/ESN2024/celik_lab1/assets/117944504/14a01ae6-1579-4528-8990-a77549f41a49)

Générer le Makefile du projet :

![image](https://github.com/ESN2024/celik_lab1/assets/117944504/c6555cc7-8295-46f2-97b6-c1a1213479f8)

Tout est prêt, nous allons maintenant écrire notre code C qui fonctionne sur notre propre processeur. Ne pas oublier de mettre à jour le Makefile avec le code source. Le programme généré peut être chargé et exécuté dans le softcore du FPGA

<h3> Language C </h3>

![image](https://github.com/ESN2024/celik_lab1/assets/117944504/d667d4d6-8aa5-4e2e-9fb1-68ad1cfcf119)
![image](https://github.com/ESN2024/celik_lab1/assets/117944504/c56056d3-2593-4e96-8758-fa5019670edb)

<h3> Résultats </h3>

Dans le code, nous pouvons retrouver différentes versions. 

- _Blink_led.c_ : Il s'agit du tout premier programme conçu spécifiquement pour initier les utilisateurs à la manipulation de la carte. Ce code introductif a pour objectif de fournir une expérience pratique dans le processus de programmation de la carte en question.

https://github.com/ESN2024/celik_lab1/assets/117944504/e80960c5-7c2c-4be5-b077-6127342c6bd3

- _LED_chaseur.c_ : Pour mettre en œuvre le chasseur de LED, j'ai opté pour une approche simple consistant à effectuer des décalages de bits vers la gauche en boucle démarrant de la LED 0 vers la LED 7 puis des décalages vers la droite de la LED 7 vers la LED 0. À chaque étape, j'ai introduit un délai en utilisant la commande "usleep()". Cette approche me permet non seulement de réaliser le chenillard LED mais également de réguler sa vitesse pendant la phase d'interrogation.

https://github.com/ESN2024/celik_lab1/assets/117944504/be101fff-f32c-4a40-82ec-6f187470e7a2

- _LED_chaseur_interruption.c_ : En ce qui concerne la gestion par interruption, j'ai introduit différentes fonctions pour prendre en charge ces événements. La fonction "key_interrupt()" gère les boutons, tandis que "sw_interrupt()" s'occupe des commutateurs. Dans la fonction principale, j'ai initialisé les interruptions en spécifiant les masques, les détections de bord, et les enregistrements d'interruption. Pour la fonction "key_interrupt()", elle vérifie quel bouton est enfoncé, déclenchant ainsi le chenillard LED ou le réinitialisant, tout en réinitialisant les variables associées à son contrôle. Dans la fonction "sw_interrupt()", je lis d'abord le registre où sont écrits les commutateurs. Ensuite, j'appelle ma fonction "nombre_bits_a_1()", qui renvoie le nombre de bits mis à 1 dans le registre. En fonction de ce nombre, je modifie la variable "time" pour ajuster la vitesse du chenillard LED.

https://github.com/ESN2024/celik_lab1/assets/117944504/e88b1f84-cad2-4189-b592-c94441f073c7

<h3> Conclusion </h3>

Le lab1 a été une expérience réussie pour moi, dépourvue de difficultés majeures. C'était la première fois que j'avais l'opportunité de travailler sur une véritable cible. Ce premier exercice pratique a joué un rôle crucial en me permettant de reprendre en main la maîtrise des divers outils nécessaires pour manipuler à la fois l'aspect matériel et logiciel de la cible. Au cours de ce lab, j'ai pu consolider mes compétences et mettre en pratique les connaissances théoriques acquises. Cependant, le seul obstacle notable auquel j'ai été confronté était lié aux interruptions. Les avertissements générés par la syntaxe des fonctions utilisées dans le cours ont suscité quelques difficultés, retardant ainsi légèrement ma progression. La résolution de ce problème a nécessité une adaptation de la syntaxe, en utilisant une approche alternative pour définir les interruptions. En fin de compte, ce lab a été une étape constructive dans mon apprentissage, illustrant la nécessité de la compréhension approfondie des concepts et de la flexibilité dans l'approche des problèmes rencontrés lors du développement sur une cible réelle

