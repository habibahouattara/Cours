"Indiquez ici vos nom et prénom :OUATTARA Umm-Habibah"

import numpy as np

G = (11,[(3,4),(6,8),(1,2),(5,6),(9,10),(7,8),(4,5),(4,6),(11,10),(4,6),(2,8),(12,10)])

def Racine(x,pi):
    i = x;
    while pi[i] > -1:
        i = pi[i]
    return i

def Composantes_connexes(G):
    n,p = G
    pi = []
    for i in range(n):
        pi.append(-1)
    for i in range(n):
        r1 = Racine(p[i][0],pi)
        r2 = Racine(p[i][1],pi)
        if r1 != r2:
            pi[r2] = r1
    return pi

print("composantes connexes de G", Composantes_connexes(G))