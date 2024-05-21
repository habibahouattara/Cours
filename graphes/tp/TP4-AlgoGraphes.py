"Indiquez ici vos nom et prénom :OUATTARA Umm-Habibah"
import numpy as np
Inf = np.Inf

#%% Exercice 1
import numpy as np
inf = np.Infinity

def InitDijkstra(G,d,pi):
    for u in range(np.shape(G)[0]):
        d[u] = inf
        pi[u] = 0
    d[1] = 0

def Relacher(G,u,v,d,pi):
    if d[v] > d[u] + G[u,v]:
        d[v] = d[u] + G[u,v]
        pi[v] = u

def Dijkstra(G):
    F = []
    O = []
    d = []
    for i in range(np.shape(G)[0]):
        F[i] = False
        O[i] = False
    InitDijkstra(G,d,pi)
    for i in range(np.shape(G)[0]):
        distSomme = 0
        for u in range(np.shape(G)[0]):
            if (F[distSomme] and O[u] and F[u] == False):
                distSomme = u
            elif O[u] and F[u] == False and d[u] < d[distSomme]:
                distSomme = u
        for v in range(np.shape(G)[0]):
            if G[distSomme][v] != inf and F[v] == False:
                O[v] = True
                Relacher(G,distSomme,v,d,pi)
        F[distSomme] = True
    return (pi,d)

#%% Exercice 2

def BellmanFord(G):
    InitDijkstra(G,d,pi)
    for i in range(np.shape(G)[0]):
        for u,v in range(np.shape(G)[0]):
            Relacher(G,u,v,d,pi)

#%% Exercice 3
P = []

def PileVide(P):
    """
    Indique si la pile est vide ou non
    """
    return len(P) == []

def Empiler(P,x):
    """
    Ajoute l'élément x au sommet de la pile
    """
    P.insert(0,x)

Empiler(P,0)
Empiler(P,5)
Empiler(P,10)
Empiler(P,15)
Empiler(P,20)
print("Pile : ", P)

def Depiler(P):
    """
    Supprime le dernier élément ajouter dans la pile et le retourne
    """
    if PileVide(P):
        raise ValueError("P est vide")
    else:
        P.pop(0)
    return P

print("Pile dépilée : ", Depiler(P))

#%% Exercice 4
def SommetPile(P):
    """
    Retourne le sommet pile sans le supprimé
    """
    if not PileVide(P):
        return P[0]
    else:
        return None

def CopierColler(P):
    """
    Renvoie une copie de la pile P
    """
    Q = []
    if not PileVide(P):
        R = []
        for i in range(len(P)):
            v = SommetPile(P)
            Depiler(P)
            Empiler(R,v)
        for i in range(len(R)):
            s = SommetPile(R)
            Depiler(R)
            Empiler(P,s)
            Empiler(Q,s)
    return Q

print("Pile copié : ", CopierColler(P))

def Inverser(P):
    """
    Renvoie la pile Q contenant les éléments de P dans l'ordre inverse
    """
    S = []
    Q = []
    if not PileVide(P):
        S = CopierColler(P)
        for i in range(len(S)):
            s = SommetPile(S)
            Depiler(S)
            Empiler(Q,s)
    return Q

print("Pile inversée : ", Inverser(P))

def Rotation(P):
    """
    Place l'élément de tête en queue de pile et décale d'une position les
    autres éléments
    """
    Q = []
    S = []
    if not PileVide(P):
        s = SommetPile(P)
        Depiler(P)
        Empiler(Q,s)
        S = Inverser(P)
        for i in range(len(S)):
            v = SommetPile(S)
            Depiler(S)
            Empiler(Q,v)
    return Q

print("Rotation : ", Rotation(P))

def DernierPremier(P):
    """
    Échange l'élément de tête de la pile avec le dernier élément
    sans changer les autres éléments de place
    """
    S = []
    Q = []
    R = []
    T = []
    if not PileVide(P):
        S = CopierColler(P)
        Q = Rotation(S)
        R = Inverser(Q)
        s = SommetPile(R)
        Depiler(R)
        Empiler(T,s)
        v = SommetPile(R)
        Depiler(R)
        for i in range(len(R)):
            e = SommetPile(R)
            Depiler(R)
            Empiler(T,e)
        Empiler(T,v)
        P = CopierColler(T)
    return P

print("DernierPremier", DernierPremier(P))

#%% Exercice 5
