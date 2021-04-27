import sys
import itertools
comb = []
def combinations(itr, r):
     yield from itertools.product(*([itr] * r)) 
'''
for x in combinations(range(2),3):
     #print(''.join(x))
     #comb.append(''.join(x))
     comb.append(x)
     print(x)
'''

def calculatehmm(A,B,pi,o):
    comb = []
    Sum = 0
    #debug
    #print(len(pi),len(o))
    for i in combinations(range(len(pi)),len(o)):
        comb.append(i)

    #print(comb)
    for pos in comb:
        for obs in range(len(o)):
            if obs == 0:
                prod = pi[obs]*B[pos[obs]][o[obs]]
                #dubug 
                #print(pi[obs], B[pos[obs]][o[obs]])
            else:
                prod = prod*(A[pos[obs-1]][pos[obs]]*B[pos[obs]][o[obs]])
            # dubug
            #print(A[pos[obs-1]][pos[obs]],B[pos[obs]][o[obs]])
        Sum +=prod
    return Sum

def main():
    A = [[0.6,0.34],[1,0]]
    #debug 
    #print("A=",A)
    B = [[0.5,0.25,0.25],[0.1,0.1,0.8]]
    #debug 
    #print("B=",B)
    pi = [0.8,0.2]
    #debug 
    #print("pi",pi)
    o = [0,1,0,2]
    #debug 
    #print("o=",o)
    Sum = calculatehmm(A,B,pi,o)
    print(Sum)

if __name__=="__main__":
        main()








