SDLife
======

> Une implémentation du Jeu de la Vie en C utilisant la SDL v2


## 1. Présentation du Jeu de la Vie

### 1.1. Automates cellulaires

Le Jeu de la Vie est un jeu mathématique conçu par John Conway en 1970. C’est
le plus connu des *automates cellulaires*, des jeux de simulation sans
interaction du joueur (concept appelé par Conway des jeux à zéro joueurs)
consistant en une grille de « cellules » évoluant dans le temps entre
différents états selon un jeu spécifique de règles et constituant de nombreux
motifs multicellulaires uniques. Chacun de ces ensembles définit un automate
cellulaire particulier.

Partis de simples curiosités mathématiques, les automates cellulaires ont gagné
en popularité par leur présentation d’un comportement complexe et organique,
avec des motifs n’évoluant plus, qui se répètent ou se déplacent sur la grille.
Macroscopiquement, l’observation de l’évolution en continu d’une grille
rappelle la croissance (accélérée) de cultures de microorganismes, d’où le nom.

Le fait que des interactions complexes entre les cellules émergent directement
d’un ensemble de règles simples en apparence a été source d’études et de
réflexions, et le principe a été appliqué avec succès dans des champs
radicalement différents, tels que la finance ou la mécanique des fluides.

La nature systématique du jeu en fait un excellent exercice de développement —
ce projet n’est qu’un exemple parmi tant d’implémentation du Jeu de la Vie,
existant aussi bien en ligne que sur les bécanes de geeks passionnés.

Une propriété frappante de certains automates cellulaires, dont celui de
Conway, est leur nature Turing-complète, c’est-à-dire qu’ils sont capables
d’émuler une machine de calcul universelle, donc théoriquement d’effectuer
n’importe quel calcul mathématique.


### 1.2. Propriétés de la grille

L’évolution du jeu prend en compte les huit cellules directement voisines de
chaque cellule. Cette sélection s’appelle le voisinage de Moore de portée 1.

Les cellules voisines de la cellule centrale (représentée par un `@`) selon ce
critère sont ici représentées par les `#` :

    .....
    .###.
    .#@#.
    .###.
    .....

Le jeu est *binaire*, c’est-à-dire que chaque cellule ne peut prendre que deux
états: « vivante » ou « morte ».

La grille est *orthogonale* : les cellules sont disposées le long d’axes
perpendiculaires.

L’évolution est *totaliste extérieure*, c’est-à-dire que l’état suivant de
chaque cellule est fonction du total de cellules voisines vivantes et de l’état
actuel de la cellule. (Un automate cellulaire entièrement *totaliste*
incluerait l’état actuel de la cellule dans le total des cellules vivantes).

La grille est *isotrope*, c’est-à-dire que les cellules voisines contribuent
également à l’évolution, la localisation d’une cellule ne porte pas de
signification particulière en soi (seule le nombre de cellules vivantes et
mortes compte).

La grille est *homogène*, signifiant que la règle s’applique indistinctement
partout sur la grille de la même manière.

Les automates cellulaires exhibant ces propriétés sont qualifiés de
« similaires au Jeu de la Vie », ou *Life-like* en anglais, parfois abrégé en
*LLCA* (*Life-like cellular automata*).


#### 1.2.1. Taille de la grille

La grille théorique a une taille infinie, pour que les motifs puissent évoluer
sans limites, cependant la réalité matérielle ne permet pas la mise en place
d’une telle grille.

Les bords de la grille sont habituellement implémentés comme des murs, au-delà
desquels aucune cellule ne peut naître, et en conséquent les cellules jouxtant
un bord sont privées de trois voisines (cinq dans le cas d’une cellule dans un
coin) et leur évolution sera altérée.

Une solution à ce problème est d’utiliser une grille suffisamment grande pour
que le motif croisse sans heurter les bords.

Une implémentation courante pour imiter une grille infinie est en la faisant
boucler sur elle-même, de sorte que les motifs qui traversent un bord
reparaissent du côté opposé. Une telle grille est souvent qualifiée de
*toroïdale*, puisqu’elle se comporte comme un tore (une figure géométrique
tridimensionnelle en forme de beignet américain, ou de chambre à air de vélo).
Ce n’est cependant pas idéal, en effet les motifs peuvent traverser la grille
jusqu’à leur point de départ et interagir avec leur origine (par exemple un
planeur faisant le tour de la grille jusqu’à détruire le canon qui l’avait
émis).


### 1.3. Transitions d’états de cellule

Puisqu’il n’y a que deux états possibles pour une cellule, celle-ci ne peut
passer que par quatre transitions :

 - de vivante, elle reste vivante (survie)
 - de vivante, elle devient morte (mort)
 - de morte, elle devient vivante (naissance)
 - de morte, elle reste morte.

Chaque génération, la transition est choisie selon le nombre de voisines
vivantes. Si une cellule a trop de voisin vivants, elle meurt de surpopulation,
si trop peu, elle meurt de sous-population. Seulement si elle a un nombre
acceptable de voisines vivantes peut-elle survivre, ou naître.

La règle originelle impose qu’une cellule naisse si elle a exactement trois
voisines vivantes, et qu’elle survive si elle en a deux ou trois.


### 1.4. Motifs de cellules

La règle originale permet l’existence de motifs de natures variées. Il y a des
motifs immobiles (*natures mortes*), qui se répètent dans le temps
(*oscillateurs*), qui se déplacent sur la grille (*vaisseaux*), qui émettent
de plus petits motifs (*canons*), qui se déplacent en laissant une traînée de
débris derrière eux (*trains à vapeur*, ou *puffeurs* par anglicisme), qui se
déplacent et génèrent de plus petits vaisseaux dans leur sillage (*râteaux*),
etc.


#### 1.4.1. Motifs simples du Jeu de la Vie original

Dans les figures suivantes, des `@` sont utilisés pour représenter des cellules
vivantes et `.` des mortes.

Le *bloc*, un simple carré, la nature morte la plus simple :

    @@
    @@

La *cuve*, une croix évidée, la seule autre nature morte de quatre cellules :

    .@.
    @.@
    .@.

Le *clignotant*, un oscillateur de période 2 alternant entre des lignes
horizontale et verticale de trois cellules :

    ...
    @@@
    ...

devient

    .@.
    .@.
    .@.

Le *poteau de barbier*, un oscillateur de période 2 qui peut être étendu
indéfiniment :

    Bipoteau    Tripoteau    Quadripoteau
     ....@@      .....@@      .......@@
     ...@.@      ....@.@      ......@.@
     ......      .......      .........
     .@.@..      ..@.@..      ....@.@..
     .@@...      .......      .........    etc.
     ......      @.@....      ..@.@....
     ......      @@.....      .........
     ......      .......      @.@......
     ......      .......      @@.......

deviennent

    Bipoteau    Tripoteau    Quadripoteau
     ....@@     .....@@       .......@@
     .....@     ......@       ........@
     ..@.@.     ...@.@.       .....@.@.
     .@....     .......       .........
     .@@...     .@.@...       ...@.@...    etc.
     ......     @......       .........
     ......     @@.....       .@.@.....
     ......     .......       @........
     ......     .......       @@.......

Le *planeur*, le plus petit vaisseau, se déplace en diagonale d’une cellule
toutes les quatre générations:

    .@..    ....    ....    ....    ....
    ..@.    @.@.    ..@.    @...    ..@.
    @@@.    .@@.    @.@.    ..@@    ...@
    ....    .@..    .@@.    .@@.    .@@@

Le planeur est probablement le motif le plus important du Jeu de la Vie,
notamment dans la construction, car ils constitue un support de transmission
d’information (la présence d’un planeur peut être traitée comme un bit `1` et
son absence un bit `0`, par exemple), et il est aussi utiliser pour percuter
d’autres motifs pour déclencher des réactions intéressantes. Il existe en
réalité une traque des configurations minimales impliquant des planeurs
capables de générer des motifs particuliers.

De tous les motifs existants le planeur est également le plus reconnaissable,
en faisant l’incarnation par excellence du Jeu de la Vie.


#### 1.4.2. Considération sur la vitesse des vaisseaux

Puisque la transition ne prend en compte que les voisines directes, un vaisseau
ne peut pas se déplacer de plus d’une cellule par génération dans quelque
direction que ce soit. Cette limite est appelée `c`, en référence à la vitesse
de la lumière, la vitesse limite matérielle.

La vitesse d’un vaisseau est donc représentée comme fraction de `c` : par
exemple, comme celui-ci se déplace d’une cellule en qutre générations, la
vitesse du planeur est `c/4`.

Il n’existe pas de vaisseaux de vitesse `c` dans le Jeu, cependant certaines
variantes semblables, particulièrement les plus explosives, en présentent.

Dans d’autres automates cellulaires, non similaires au Jeu, la règle
d’évolution peut considérer les cellules plus lointaines que les voisines
directes, permettant la transmission d’information à plus d’une cellule par
génération et par là même une plus grande valeur pour `c`. Dans ces versions,
il est donc possible de trouver des vaisseaux se déplaçant de plus d’une
cellule par génération (mais leur vitesse ne dépassera jamais la valeur de
*leur* `c`).


### 1.5. Formule *rulestring*

La *rulestring* (littéralement « chaîne de règle », soit une chaîne de
caractères correspondant à une règle) est un format permettant de représenter
de manière sommaire mais complète tous les automates cellulaires similaire au
Jeu, en listant les nombres exacts de voisines vivantes dont une cellule a
besoin pour être en vie à la génération suivante. Seules les transitions
« naissance » et « survie » sont décrites ; toute configuration qui ne
correspond pas à l’un des deux états renvoie à une cellule morte. Comme il y a
moins de dix cellules voisines, ces nombres peuvent être commodément décrit par
un chiffre chacun.

Les chiffres pour les transitions « naissance » et « survie » sont énumérés en
ordre croissant, précédés de l’initiale de la transition en anglais (B for
*birth* et S pour *survival*) et les sections sont séparées par une unique
barre oblique.

Selon cette syntaxe, la règle originale s’écrit `B3/S23` : la cellule naît si
elle a trois voisines vivantes, survit avec deux ou trois, sinon elle sera
morte, que ce soit par mort ou parce qu’elle n’était déjà pas vivante
initialement.

Le format présenté ici (*B* avant *S*) est aussi appelé notation B/S, par
opposition à la notation S/B, où la section *S* précède *B*, mais cette
notation bien que similaire est tombée en désuétude.

Techniquement, pour un minimalisme total, soit les caractères *B* et *S*, soit
la barre oblique, peuvent être omis (et le programme accepte sans problème des
formules ainsi allégées), mais les conserver facilite la lisibilité et est une
matière de goût.


### 1.6. Variantes du Jeu

En cinquante ans, de nombreux automates cellulaires ont été explorés, et
certains exhibant des propriétés intéressantes ont été recensés, voire nommés,
souvent en référence à leur particularité.

Voici une liste des noms d’automates cellulaires similaires au Jeu que ce
programme reconnaît, accompagnés de leur rulestring :

 - 2x2 (B36/S125)
   Cette variante a la faculté d’évoluer en blocs de deux cellules par deux
 - 34 Life, or Life 3-4 (B34/S34)
   Nommé pour ses nombres de transition, à chaque fois trois et quatre
 - Amoeba (B357/S1358)
   De grandes zones se forment qui ressemblent à des amibes et qui peuvent
   assimiler de plus petites figures
 - Assimilation (B345/S4567)
   Une règle similaire à Diamoeba mais plus stable
 - Coagulations (B378/S235678)
   Une règle explosive qui crée des taches au cours de son expansion
 - Coral (B3/S45678)
   Crée des structures coraliennes grandissant lentement
 - Corrosion of Conformity (B3/S124)
   Une variante de Mazectric se délitant au fil du temps
 - Day & Night (B3678/S34678)
   Dans cette variante, les motifs de cellules mortes se comportent commes les
   mêmes motifs constitués de cellules vivantes (comme par inversion des
   couleurs)
 - Diamoeba (B35678/S5678)
   Développe des amibes rhombiques (en forme de losange)
 - Flakes, Life without Death, or LwoD (B3/S012345678)
   Crée de belles structures ressemblant à des flocons de neige (à noter que
   les cellules une fois nées ne meurent pas)
 - Gnarl (B1/S1)
   Commencer avec une seule cellule ; voir par soi-même.
 - HighLife, or Highlife (B36/S23)
   Une règle globaleme,nt similaire à l’originale, mais intéressante par la
   présence du *réplicateur*, un motif qui évolue en deux versions de lui-même
 - InverseLife, aka. Inverse life (B0123478/S34678)
   Une version « en négatif » de la règle originelle
 - Long Life, or Long life (B345/S5)
   Motifs avec une grande longévité
 - Maze (B3/S12345)
   Les structures s’y étendent lentement et forment des motifs labyrinthiques
 - Mazectric (B3/S1234)
   Une variante de Maze présentant des couloirs plus longs et droits
 - Move (B368/S245)
   Une règle évoluant lentement, avec de nombreux vaisseaux et puffeurs
 - Pseudo Life, or Pseudo life (B357/S238)
   Ressemble à la règle originale, mais aucun de ses motifs ne se comporte de
   la même manière
 - Replicator (B1357/S1357)
   Chaque motif forme des répliques de lui-même
 - Seeds (B2/S)
   Chaque cellule meurt systématiquement à la génération suivante. Néanmoins la
   plupart des motifs évoluent de manière explosive
 - Serviettes (B234/S)
   Produit de magnifiques géométries semblant être tirées de tapis persans
 - Stains (B3678/S235678)
   Évolue en de grandes et stables « taches d’encre »
 - WalledCities (B45678/S2345)
   Se stabilise en cités, zones de grande activité entourées d’un mur continu


## 2. Le programme

Ce programme est un simulateur d’automates cellulaires similaires au Jeu de la
Vie tels que présentés ci-dessus.


### 2.1. Exécution

#### 2.1.1. Options de ligne de commande

CE programme est invoqué depuis la ligne de commande et accepte un ensemble
d’options.

<table>
  <tr>
    <th>Catégorie</th>
    <th>Option courte (avec argument éventuel)</th>
    <th>Option longue</th>
    <th>Description</th>
    <th>Valeur par défaut</th>
    <th>Conflit avec une autre option</th>
  </tr>
  <tr>
    <td rowspan="5">Gestion de la grille</td>
    <td><code>-w LARGEUR</code></td>
    <td><code>--width</code></td>
    <td>Spécifie la largeur de la grille</td>
    <td><code>80</code></td>
    <td><code>-i</code>, <code>-S</code></td>
  </tr>
  <tr>
    <td><code>-h HAUTEUR</code></td>
    <td><code>--height</code></td>
    <td>Spécifie la hauteur de la grille</td>
    <td><code>60</code></td>
    <td><code>-i</code>, <code>-S</code></td>
  </tr>
  <tr>
    <td><code>-S TAILLE</code></td>
    <td><code>--square-size</code></td>
    <td>Spécifie que la grille est carrée et donne sa taille</td>
    <td>Aucune</td>
    <td><code>-w</code> et <code>-h</code>, car équivalent à <code>-w
TAILLE -h TAILLE</code></td>
  </tr>
  <tr>
    <td><code>-W</code></td>
    <td><code>--wrap</code></td>
    <td>Fait boucler la grille sur elle-même (cf. section 1.2.1.)</td>
    <td>Faux</td>
    <td>Aucun</td>
  </tr>
  <tr>
    <td><code>-R RÈGLE</code></td>
    <td><code>--game-rule</code></td>
    <td>Spécifie la variante du Jeu à lancer (par son nom s’il est connu, sinon
par sa *rulestring*)</td>
    <td><code>B3/S23</code></td>
    <td>Aucun</td>
  </tr>
  <tr>
    <td rowspan="4">Affichage de la grille</td>
    <td><code>-b BORDURE</code></td>
    <td><code>--border</code></td>
    <td>La taille de l’espace entre les cellules, en pixels</td>
    <td><code>1</code></td>
    <td><code>-n</code></td>
  </tr>
  <tr>
    <td><code>-n</code></td>
    <td><code>--no-border</code></td>
    <td>Supprime l’espace entre les cellules</td>
    <td>Faux</td>
    <td><code>-b</code>, parce qu’équivalent à <code>-b0</code></td>
  </tr>
  <tr>
    <td><code>-c TAILLE_CELLULE</code></td>
    <td><code>--cell-size</code></td>
    <td>Donne la taille d’une cellule, en pixels</td>
    <td><code>16</code></td>
    <td>Aucun</td>
  </tr>
  <tr>
    <td><code>-r FRÉQUENCE</code></td>
    <td><code>--update-rate</code></td>
    <td>Le nombre de générations par seconde</td>
    <td><code>25</code></td>
    <td>None</td>
  </tr>
  <tr>
    <td rowspan="4">E/S de fichier</td>
    <td><code>-f FICHIER</code></td>
    <td><code>--file</code></td>
    <td>Donne le chemin du fichier d’où lire et où écrire</td>
    <td>Aucune</td>
    <td><code>-i</code> et <code>-o</code></td>
  </tr>
  <tr>
    <td><code>-i FICHIER_ENTRÉE</code></td>
    <td><code>--input-file</code></td>
    <td>Donne le chemin vers le fichier d’où lire la configuration initiale de
la grille</td>
    <td>Aucune</td>
    <td><code>-f</code>, <code>-w</code> et <code>-h</code></td>
  </tr>
  <tr>
    <td><code>-o FICHIER_SORTIE</code></td>
    <td><code>--output-file</code></td>
    <td>Donne le chemin vers le fichier où écrire l’état de la grille</td>
    <td>Aucune</td>
    <td><code>-f</code></td>
  </tr>
  <tr>
    <td><code>-F FORMAT</code></td>
    <td><code>--format</code></td>
    <td>Spécifie le format de la représentation de la grille dans le fichier
d’entrée. Soit *plain*, *plaintext* ou *RLE* (casse indifférenciée)</td>
    <td>Aucune</td>
    <td>Aucun</td>
  </tr>
</table>

Tout argument représentant un chemin vers un fichier (pour l’une des options
`-f`, `-i` ou `-o`) peut être `-`, ce qui spécifie de lire depuis le flux
d’entrée standard ou d’écrire sur le flux de sortie standard. Il s’agit du seul
cas où l’argument de `-f` ne fait pas référence au même fichier pour l’entrée
et la sortie : `-f-` est un raccourci pour `-i- -o-`.


#### 2.1.2. Interface graphique

L’interface graphique du programme est minimaliste ; seule la grille est
affichée. Les cellules sont présentées comme des carrés, blancs pour les
cellules mortes et noir pour les vivantes. Il y a une bordure gris moyen autour
des cellules pour les distingeur visuellement. La cellule active est marquée
d’un masque gris semi-opaque, ainsi la cellule active apparaît gris foncé quand
elle est vivante et gris clair quand elle est morte.

Seules les tailles de cellule et de bordure (ou la seule présence de cette
dernière) peuvent être modifiées (cf. la section précédente) ; les couleurs ne
sont actuellement pas configurables.

Le programme dispose de deux modes d’évolution. Il peut être lancé et évoluer
en continu à la fréquence indiquée au lancement (voir ci-dessus), ou être dans
un état stoppé, donnant ainsi à l’utilisateur la possibilité de modifier
l’état des
cellules ou de dessiner des motifs complets avant que ceux-ci n’évoluent. Dans
ce mode, la grille peut toujours évoluer par étapes.


#### 2.1.3. Interaction avec le clavier et la souris

L’utilisateur peut interagir avec la fenêtre du programme au travers du clavier
aussi bien que de la souris. Déplacer la souris au-dessus de la grille change
la cellule active pour celle sous le curseur.

Le bouton gauche de la souris peut être actionné pour inverser l’état de la
cellule active (si elle est vivante, elle meurt, si déjà morte, elle naît). le
bouton peut également être mainteu pressé et la souris déplacée, pour inverser
chaque cellule que le curseur survole. Cependant, si la souris est déplacée
trop rapidement, certaines cellules peuvent être manquées : c’est une
limitation connue de l’implémentation de la régulation de la fréquence de mise
à jour du programme.

Le clavier peut également être utilisé pour changer la cellule active, en
utilisant les touches flèches. Certaines touches ou combinaisons de touches ont
aussi une action définie :

<table>
  <tr>
    <th>Touche</th>
    <th>Action</th>
  </tr>
  <tr>
    <td><code>Échap</code></td>
    <td rowspan="3">Quitte le programme</td>
  </tr>
  <tr>
    <td><code>Ctrl</code> + <code>Q</code></td>
  </tr>
  <tr>
    <td><code>Ctrl</code> + <code>W</code></td>
  </tr>
  <tr>
    <td><code>Espace</code></td>
    <td>Lance ou arrête le mode continu</td>
  </tr>
  <tr>
    <td><code>Entrée</code></td>
    <td>Quand en pause, évolue la grille d’une seule génération</td>
  </tr>
  <tr>
    <td><code>T</code></td>
    <td>Inverse la cellule active</td>
  </tr>
  <tr>
    <td><code>R</code></td>
    <td>Réinitialise la grille à la configuration dans le fichier d’entrée. Si
aucun fichier n’a été spécifié, ne fait rien</td>
  </tr>
  <tr>
    <td><code>W</code></td>
    <td>Écrit l’état actuel de la grille dans le fichier de sortie ; si le
fichier n’existe pas, le crée. Si aucun fichier spécifié, ne fait rien</td>
  </tr>
  <tr>
    <td><code>C</code></td>
    <td>Efface la grille (tue toutes les cellules)</td>
  </tr>
  <tr>
    <td><code>H</code></td>
    <td>Affiche dans la console un résumé des actions de clavier et
souris</td>
  </tr>
</table>


#### 2.1.4. Entrée/Sortie de fichiers

Le programme peut lire ou écrire vers des fichiers dont le contenu décrit une
configuration de grille (dimensions, état des cellules et parfois
*rulestring*).
Ces fichiers viennet en deux formats distincts, *texte brut* et *RLE* (pour
*run-length encoding*, codage par longueur de plage).

le format du fichier d’entrée peut être spécifié par l’option de ligne de
commande `-F`. Si l’option n’est pas donnée, le programme tente de deviner le
format du fichier. Tout d’abord à partir de son nom : si celui-ci se termine
par `.rle`, le format RLE est supposé. Au contraire, si le nom finit en
`.cells`, c’est le format texte brut qui est supposé. En tout autre cas, le
programme tente d’interpréter le contenu du fichier comme RLE, puis en cas de
non-correspondance en tant que texte brut, avant de terminer en erreur.

L’option `-F` peut également être utilisée pour forcer le format de fichier
qui serait deviné depuis le nom ou le contenu du fichier, mais il n’est pas
conseillé de nommer un fichier de motif de grille avec une extension qui ne
corresponde pas.

Le fichier d’entrée peut être fourni dans l’un des deux formats, mais le
fichier de sortie sera toujours écrit en format texte brut.


##### 2.1.4.1. Format texte brut

Le format texte brut est une représentation textuelle de la grille en un bloc
de texte rectangulaire, avec des `.` pour les cellules mortes et des `@` ou `O`
pour les vivantes. Bien que ces deux caractères soient reconnus, ils ne
devraient pas être mélangés ; si les données contiennent les deux un message
d’avertissement sera affiché sur la sortie d’erreur standard. La grille doit
être écrite intégralement, le nombre de lignes donne la hauteur de la grille et
le nombre de caractères par ligne (qui doit être constant tout au long du
fichier) en donne la largeur. Toute ligne commençant par un `!` est considérée
comme du commentaire et ignorée. Ces commentaires peuvent apparaître partout
dans le fichier (avant comme après le motif, comme au milieu) et ne sont pas
concernés par les restrictions de longueur de ligne. Cependant le point
d’interrogation doit être le premier caractère de la ligne ; le commentaire
continue jusqu’au saut de ligne suivant.


Exemple : un planeur orienté vers le bas et la gauche en texte brut

    .@.
    ! ceci est un commentaire
    @..
    @@@


##### 2.1.4.2. Format *RLE*

Le *RLE* est un format compressé où une suite contigüe de cellules dans le même
état est réduite à une seule cellule et la longueur de la suite. Ce format
attend une ligne d’en-tête donnant les dimensions de la grille et
éventuellement la *rulestring* de la règle à utiliser. Cet en-tête est
obligatoire : la configuration étant compressée, les fins de ligne mortes
peuvent être omises et les dimensions de la grille ne peuvent pas être inférés
seulement des données de la grille.

Les cellules vivantes sont notées par un `o`, les mortes par un `b`, un `$`
indique une fin de ligne (et le début de la suivante) et un `!` représente la
fin de la configuration. Les caractères d’espacement (espaces, tabulations,
retours chariot) ne sont pas significatifs (mais ne sont pas autorisés à
l’intérieur d’une contraction de série), cela permet de sauter des lignes
partout sans contrainte. Un `#` commence un commentaire, et le texte jusqu’à
la ligne suivante est ignoré. Tout ce qui vient après le `!` terminal est
considéré comme du texte commentaire et n’est pas parcouru.

Exemple : le planeur orienté sud-ouest en format *RLE*

    # Commentaire en tête de fichier
    x = 3, y = 3, rule = B3/S23
    bo$
    b$ # Commentaire sur la même ligne !
    3o!
    Ce texte après le ! final n’est pas parcouru


### 2.2. Développement

Le code est écrit en C, le fameux langage « bas niveau ». Tout est parti d’un
projet d’apprendre et me familiariser avec le langage, et au bout d’un moment
a fini par ressembler à quelque chose d’utilisable.

Je me suis efforcé de garder les fichiers d’en-tête bien documentés, au cas où
quelqu’un serait intéressé par y jeter un coup d’œil ; le formatage du code est
homogène, lisible (en tout cas en ce qui me concerne) et les parties absconses
du texte sont commentées.


#### 2.2.1. La SDL

Le programme utilise la [SDL](http://www.libsdl.org "Le site web de la SDL")
(*Simple DirectMedia Layer*), version 2 pour la partie graphique. C’est une
bibliothèque bas niveau qui fournit une API puissante mais cependant facile
d’utilisation. Elle est également populaire et supportée sur la majorité des
plateformes, ce qui améliore la portabilité du projet.


#### 2.2.2. Environnement

Les plateforms supportées sont GNU/Linux, notamment les distributions Debian,
et MS Windows.

Le code a été développé initiallement sous Ubuntu 17, puis Debian 9 ; les
distros basées sur Debian sont supportées, et les plateformes principales
GNU/Linux devraient l’être également. Sous MS Windows, le code est développé
avec Visual Studio 17 et compile de manière native, néanmoins une compilation
GNU devrait être possible avec MinGW ou Cygwin, mais je n’ai pas essayé.

Pour les plateformes *nix, le seul paquet nécessaire pour compiler les sources
est `libsdl2-dev`. Les paquets recommandés sont `make` pour fournir
l’utilitaire de compilation et `doxygen` pour le traitement de la
documentation.

Concernant Win32, les outils développeurs doivent être installés (ils viennent
avec Visual Studio) pour fournir `nmake` et le compilateur et éditeur de liens.
La SDL et Doxygen devront être installés et configurés manuellement.


#### 2.2.3. Compilation

Le code est écrit suivant le standard C11, et emploie des fonctionnalités
exclusives au C, C++ n’est pas supporté et on ne doit pas s’attendre à ce que
le code compile en C++.

Le processus de compilation est géré par `make`, avec un Makefile fourni
donnant les règles usuelles :
 - `all` pour construire l’exécutable dans le dossier `out`,
 - `clean` pour supprimer les fichiers objet du dossier `obj`,
 - `doc` pour générer la documentation dans le dossier `doc`,
 - `cleandoc` pour supprimer ce dernier,
 - `distclean` pour réinitialiser l’état du projet,
et des règles basées sur les différents fichiers pour compiler des fichiers
objet individuels.

Le développement sous GNUnux est effectué en utilisant GCC et avec MSVC sous
Windows, avec tous les avertissements activés (`-Wall -Wextra -pedantic` et
`/Wall` respectivement), afin de rendre le code le plus conforme possible au
standard. Je me suis également abstenu d’utiliser des spécificités GNU
(quelques fonctions POSIX sont bien appelées, mais aucun élément de code tels
que les pragmas ou `__attribute__` ne sont utilisés).


#### 2.2.4. Documentation

Les définitions de structures et déclarations de fonctions dans les en-têtes
sont intégralement (je l’espère ; en tout cas majoritairement) documentées dans
la syntaxe [Doxygen](http://www.doxygen.org/index.html "Site web de Doxygen").
Les fichiers de la documentation ne font pas partie du dépôt, cependant ils
peuvent être générés plutôt facilement (cf. section précédente). La
documentation résultante est au format HTML et peut être parcourue avec tout
navigateur ; le point d’entrée est le fichier `doc/html/index.html`.


## 3. Évolutions envisagées

Ce projet a été relancé après un hiatus notable, et devient de plus en plus
viable (bien que tout à fait superflu). Voici une liste d’évolutions que j’ai
en tête pour le programme, certaines de plus court terme que d’autres.

 - Atteindre une version 1.0 stable  
   C’est plus une directive spirituelle qu’un objectif concret ; il ne tient
   qu’à moi de décider quand apposer officiellement le numéro de version, mais
   j’attends d’être entièrement satisfait de l’état du projet pour cela.
 - Implémentation avec OpenGL  
   Je suis en local sur une version utilisant OpenGL pour le rendu graphique,
   cela fonctionne, mais tout juste, le code nécessite encore beaucoup
   d’affinage.
 - Amélioration de la configurabilité  
   Les couleurs de l’affichage de la grille, les caractères utilisés pour le
   format texte brut des fichiers sont fixes (et codés en dur). Un moyen de les
   configurer serait le bienvenu, mais je ne suis pas sûr de comment
   l’introduire ; je ne veux pas rendre le programme dépendant d’un fichier
   externe de configuration, et je suis réticent à l’idée d’introduire encore
   d’autres options de ligne de commande. Les macros de préprocesseur
   pourraient s’avérer une solutions, mais cela priverait de cofiguration à
   l’exécution.
 - Un meilleur format texte brut  
   Tout est dit. Tel qu’implémenté, le format texte brut est suffisant, mais
   maigre ; un support pour des commentaires, une omission des fin de ligne
   mortes seraient des ajouts judicieux.
 - Interface utilisateur  
   Dans le but de décorer l’interface, et de l’augmenter de contrôles et menus.
   Malheureusement je ne connais pas de framework GUI pour la SDL, et en
   développer un moi-même à partir de rien est un bien trop grand projet pour
   même y penser.
 - Mode d’exécution sans affichage  
   Cela introduirait une type d’exécution totallement différent (qui
   justifierait l’ajout d’une option de ligne de commande attitrée). L’idée
   serait de fournir fichiers d’entrée et de sortie et un nombre de
   générations, et d’écrire dans le fichier de sortie l’évolution après autant
   de générations de la configuration dans le fichier d’entrée. Ce serait une
   forme d’incubateur pour motifs.
 - LLCA non-totalistes ou anisotropes  
   Si je ne considère pas les AC non binaires (la grille est basée sur un
   tableau de bits, changer le nombre d’états serait un trop gros travail pour
   que j’aie l’envie de m’y atteler), ces types d’AC seraient intéressants à
   étudier. Cela demanderait de modifier le code d’évolution et calcul de la
   génération suivante, qui bénéficierait d’une refonte de toute manière.
 - Internationalisation  
   Principalement en ce qui concerne la langue de notre bon vieux Molière (on
   ne se refait pas), en tout cas pour commencer, cela affecterait les docs en
   plus du présent document. Cela impliquerait de maintenir le double de
   documentation, ce qui serait à soi tout seul un travail à plein temps.
