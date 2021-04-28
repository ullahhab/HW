def backward(A,B,pi,o):
    #Creating beta arrays
    betas = []
    prob = 0
    for t in range(len(o)):
        betas.append([])
        for a in range(len(pi)):
            betas[t].append(0)
    T = len(o)
    for y in range(len(pi)):
        betas[T-1][y] = 1
    for t in reversed(range(T-1)):
        for y in range(len(pi)):
            for y1 in range(len(pi)):
                    betas[t][y] += betas[t+1][y1]*B[y1][o[t+1]]*A[y][y1]
                    #debug
                    #print(betas[t][y], "adding= ", betas[t+1][y1]*B[y1][o[t+1]]*A[y][y1])
    for y in range(len(pi)):
        prob += pi[y]*B[y][o[0]]*betas[0][y]
        return prob, betas

'''
A = [[0.6,0.34],[1,0]]
B = [[0.5,0.25,0.25],[0.1,0.1,0.8]]
pi = [0.8,0.2]
o = [0,1,0,2,0,1,0]
prob, betas = backward(A,B,pi,o)
print(prob)
print(betas)
A = [[0.8, 0.1, 0.1],
     [0.4, 0.2, 0.4],
     [0, 0.3, 0.7]]
B = [[0.66, 0.34, 0],
     [0, 0, 1],
     [0.5, 0.4, 0.1]]
pi = [0.6, 0, 0.4]
o = [0,1,0,2,0,1,0]


prob, betas = backward(A,B,pi,o)
print(prob)
print(betas)
'''
