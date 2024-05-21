Indiquez ici vos nom et prénom :OUATTARA Umm-Habibah

import numpy as np

M = np.array([[0,1,0,1], [0,0,1,0], [1,0,0,1], [0,1,0,0]])
print("Matrice adjacence M\n", M)

#%% Exercice 1 et 2
def RoyWarshall(M):
    ligne, colone = M.shape
    # 1 sur la diagonale
    AC = np.eye(ligne,colone, dtype=int)

    # R(0)
    AC = np.logical_or(AC, M)
    for k in range(ligne):
        for i in range(ligne):
            for j in range(colone):
                AC[i,j] = AC[i,j] or (AC[i,k] and AC[k,j])

    return AC
print("AC:\n",RoyWarshall(M))

#%% Exercice 2
def RoyWarshall(M):
    ligne, colone = M.shape
    # 1 sur la diagonale
    AC = np.eye(ligne,colone, dtype=int)

    # R(0)
    AC = np.logical_or(AC, M)

    P = np.zeros((ligne, colone), dtype=int)
    for i in range(ligne):
        P[i] = np.where(AC[i] != 0, i+1, P[i]) #P(0)

    for k in range(ligne):
        for i in range(ligne):
            for j in range(colone):
                AC[i,j] = AC[i,j] or (AC[i,k] and AC[k,j])
                if (AC[i,j] == 0) and (AC[i,k+1] and AC[k+1,j]) == 1:
                    P[i,j] = P[k+1,j]
                else:
                    if AC[i,j] == 1:
                        P[i,j] = P[i,j]
                    else:
                        P[i,j] = 0
    return P

print("P:\n",RoyWarshall(M))

#%% Exercice 3

#%% Exercice 4

#%% Exercice 5
def Gsommets(G):
    S = list(G.keys())
    for s in G.values():
        S.extend(s)
    S = set(S)
    return list(S)

def preds(G,i):
    return [k for k in G.keys() if i in G[k]]

def noyau(G):
    def recNoyau(G, L):
        if len(G) == 0:
            return L
        else:
            L.extend([k for k in Gsommets(G) if k not in G])
            p = []
            for k in L:
                p.extend(preds(G,k))
            p = list(set(p))
            d = {}
            for k in G.keys():
                if k not in p:
                    l = [i for i in G[k] if i not in p]
                    if l!= []:
                        d[k] = l
            return recNoyau(d, L)
    return recNoyau(G, [])

G = {1:[2,3,4], 2:[3,4], 3:[4,5,6], 4:[5,6], 5:[6]}
print(noyau(G))