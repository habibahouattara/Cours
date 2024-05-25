"Indiquez ici vos noms et prénoms : POLO DITSIA DI NGOMA Phil"


#%%Prise en main
#Ce fichier est divisé en blocs de code appelés "cellules", et délimités par les lignes qui commencent par #%%
#Les touches Ctrl+Entrée permettent d'executer toute une cellule à la fois.
#Les lignes qui commencent par "#" sont considérées comme des commentaires, et ignorées quand le code est executé.
#Tutoriel officiel python : https://docs.python.org/fr/3/tutorial/index.html

#%% cette ligne est le début de la cellule suivante
1+1
print(5**2)
print("Bonjour")

#%% Type de données : Python est un language à typage dynamique fort, une variable prend le tye de la valuer qu'elle contient
a = 3.14
print("a est de type ",type(a)," et vaut ",a)
b=10
print("b est de type ",type(b)," et vaut ",b)
c = True
print("c est de type ",type(c)," et vaut ",c)
d= "Bonjour "+3*"!"
print("d est de type ",type(d)," et vaut ",d)

#%% Tuples : Une fois crée, un tuple ne peut pas être modifié, c'est un objet immuable !
M = (1 , 0.5**3, "ciao !") #tuple
print(type(M))
a,b,c = M #dépaquette un tuple
print(a,b,c)
x=M[0]
print(x) #M est un objet immuable

#%% Listes : une fois crée, une liste peut être modifiée, c'est un objet muable !
L=[]
L.append(4)
L.append(5)
print(L)
print(len(L))
L=L+[6,7]
print(L)
L.insert(1,10) #insère 10 à la place d'indice 1
print(L)
print(L[-1]) #affiche le dernier élément de L
z=L.pop() #supprime le dernier élément de L et on récupère cet élément dans z
print(z,L)
print(L[1:3]) #La technique du slicing permet d'extraire des élément d'une liste ou d'un tableau numpy, L[start:stop] : 'indice de départ de L vaut start et celui de fin vaut stop-1 avec un pas de 1 par défaut
L1=['mot', 21, 3.14, False] #liste avec éléments de type different
print(L1)
L1[0]= "phrase"
print(L1) #objet muable

#%% Deques : dec, permet d'ajouter et de supprimer très rapidement des éléments aux extrémités droite et gauche. Il s'agit d'un objet muable
from collections import deque #Il est possible d'importer des information depuis un autre fichier ou d'une bibliothèque
D=deque()
D.append(8)
D.appendleft(9)
print(D)
print(type(D))
x=D.pop()
print(D)
y=D.popleft()
print(D)
print(x,y)

#%% Une table d'hachage est une structure des données permettant de stocker des couples (clé, valeur). En Python, cette structure des données est appelée dictionnainre. Il s'agit d'un objet muable
dico_code={} #création d'un dictionnaire vide, nommé dico_code
dico_code["A"]=1
dico_code["B"]=2
print(dico_code)


#%% Tableaux numpy, la bibliothèque numpy permet d'effecuer du calcul matriciel, objet muable
import numpy as np #permet d'importer la bibliothèqyue numpy rennomée np

M=np.array([[1,2,3,4], [5,6,7,8]]) #tableau à 2 lignes et 4 colonnes
print(M)
print(np.shape(M)) #dimension du tableau M
print(M[1,2]) #Elémént de M d'indice (1,2)
N=np.ones((2,4))
print(N)
print(M+N)

#%% Fonctions
def f(x): #La fontion f(x) calcule le cube de x
    return x*x*x
print("Le cube de 3 vaut", f(3))

def somme(n): #La fonction somme(n) donne 0 si n<=0 et la somme des n premiers entiers sinon
    s=0
    for i in range(n+1) : #Boucle faisant varier i de à à n+1 exclu
        s=s+i
    return  s

print("La somme des 5 premiers entiers est :",somme(5))
print("somme(-5) vaut :", somme(-5))

def euclide(a,b): #Une fonction récursive
    if b==0: #Condition d'arrêt
        return a
    else :
        return euclide(b,a%b) #Appel récursif
print("Le pgcd entre",3, "et",6,"vaut" ,euclide(6,9))

# Si la fonction f1 est définie dans un module (programme en Python) nommé prog.py, pour l'utiliser dans un autre module il faudra importer son contenu par # >>>from prog import f1


#%% Exercice 1 :
# Graphe par matrice d'adjacence : tableau numpy
# import  numpy as np #permet d'importer la bibliothèque numpy rennomée np
#Attention :les sommets d'un graphe sont les entiers de 1 à n
M =np.array([[0, 0, 0, 1, 1], [1, 0, 0, 0, 1], [1, 0, 0, 0, 0], [0, 0, 1, 0, 0], [0, 0, 1, 0, 0]])

def SuccesseursM(M,i): #SuccesseursM renvoie la liste des successeurs de i pour un graphe G dont la matrice d'adjacence est M
    n = np.shape(M)[0] #nombre de ligne de G = nombre sommets de G
    L =[]
    for j in range(n):
        if M[(i-1),j] > 0 :
            L.append(j+1)
    return(L)
print(M)
print(SuccesseursM(M,1))
print(SuccesseursM(M,2))
print(SuccesseursM(M,3))
print(SuccesseursM(M,4))
print(SuccesseursM(M,5))

def Degré_extM(M,i):
    return(len(SuccesseursM(M,i)))

print(Degré_extM(M,1))
print(Degré_extM(M,3))

def distance(M, L):
    nbArcs = 0
    n = len(L)
    i = 0
    while i < n - 1:
        ligne = L[i]
        col = L[i+1]
        if M[ligne - 1, col - 1] == 0:
            return -1
        else:
            nbArcs += 1
            i += 1
    return nbArcs

L1 = [2,1,5,3,1]
L2 = [4,3,5,1,4]
print(distance(M, L1))
print(distance(M, L2))

def accessible(M, i, j):
    """
    accessible renvoie, s'il existe, un chemin de i vers j par la matrice M, sinon -1
    """
    def recAccessible(M, i, j, L):
        """
        recAccessible, calcule, si ils existent, tous les chemins de i vers j par la matrice d'adjacence M, et renvoie la liste de ces chemins
        sinon renvoie -1
        """
        C = L[:]
        C.append(i)
        if i == j:
            return C
        iSucc = SuccesseursM(M, i)
        if iSucc == []:
            return -1
        elif j in iSucc:
            C.append(j)
            return C
        else:
            d = {k : recAccessible(M, k, j, C) for k in iSucc if k not in C}
            val = []
            for v in d.values():
                if v != -1:
                    if type(v[0]) == list:
                        for i in v:
                            val.append(i)
                    else:
                        val.append(v)
            return val if len(val) > 0 else -1
    val = recAccessible(M, i, j, [])
    if val != -1 and type(val[0]) == list:
        np.random.shuffle(val)
        return val[0]
    else:
        return val


print("Un chemin de 2 vers 3 :", accessible(M, 2, 3))
print(accessible(M, 3, 2))


def versionNO(M):
    ligne, col = M.shape
    G = np.zeros((ligne, col), dtype=M.dtype)
    for i in range(ligne):
        for j in range(col):
            if M[i, j] == 1:
                if i != j:
                    G[j, i] = 1
                G[i, j] = 1
    return G

print("Version non orienté de M :\n", versionNO(M))

#%% Exercice 2 :
# Graphe par dictionnaire

G = {1:[2,3,5], 2:[5,9], 3:[4,5,7], 4:[7], 5:[6,8,9], 6:[7,8], 8:[9]}

def voisins(G, i):
    v = []
    for j in G.keys():
        if i in G.get(j):
            v.append(j)
    v.extend(G.get(i) if i in G else [])
    return v

def degre(G, i):
    return len(voisins(G, i))

def deMaTL(M):
    return {i+1 : [j+1 for j in range(len(M[i])) if M[i, j] > 0] for i in range(M.shape[0])}

def deTLaM(G):
    n = max(max(G.keys()), max(G.values())[0])
    M = np.zeros((n, n), dtype=int)
    for k in G.keys():
        for j in G.get(k):
            M[k - 1, j - 1] = 1
    return M


#%% Exercice 3 :
# Coloriage d'un graphe

# def nbchromatique(G):
#     l = []
#     for v in G.values():
#         l.extend(v)
#     l.extend(list(G.keys()))
#     L = list(np.unique(l))
#     sortedList = sorted(L, key=lambda x:degre(G, x))
#     c = [i for i in range(1, len(sortedList) + 1)]

#%% Exercice 4 :
# Graphe par fichier
import numpy as np
M =np.array([[0, 0, 0, 1, 1], [1, 0, 0, 0, 1], [1, 0, 0, 0, 0], [0, 0, 1, 0, 0], [0, 0, 1, 0, 0]])
print(M)
f=open('matrice.txt', 'w')
for i in range(0,M.shape[0]) :
    f.write(str(M[i]))
    f.write('\n')
f.close()
f=open('matrice.txt', 'r')
for i in range(0,M.shape[0]) :
    print(str(M[i]))
    print('\n')
f.close()

#%% Exercice 5
# Dessin de graphes non orientés
import math
import matplotlib.pyplot as plt
import numpy as np
x=[1, 3, 4]
y=[3, 1, 5]
plt.axis([-1, 5, -1, 6])
plt.plot(x,y, color='r', linestyle='-', marker='o')
plt.show()