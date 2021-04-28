def forward(A,B,pi,o,p=False):
    alpha = []
    prob = 0
    for i in range(len(o)):
        alpha.append([])
        for j in range(len(pi)):
            alpha[i].append(0)
    #print(alpha)
    for y in range(len(pi)):
        alpha[0][y] = pi[y]*B[y][o[0]]
    for s in range(1, len(o)):
        for y in range(len(pi)):
            for yo in range(len(pi)):
                alpha[s][y] += alpha[s-1][yo] * A[yo][y] * B[y][o[s]]
    for s in range(len(pi)):
        prob += alpha[len(o)-1][s]
    if p:
        print(alpha)
    return prob, alpha

        


'''
if __name__=="__main__":
    A = [[0.6,0.34],[1,0]]
    B = [[0.5,0.25,0.25],[0.1,0.1,0.8]]
    pi = [0.8,0.2]
    o = list([0,1,0,2,0,1,0])

    #ll = forward(A,B,pi,o,p=True)

    A = [[0.8,0.1,0.1],[0.4,0.2,0.4],[0,0.3,0.7]]
    B = [[0.66,0.34,0],[0,0,1],[0.5,0.4,0.1]]
    pi = [0.6,0,0.4]

    #ll = forward(A,B,pi,o,p=True)

    A = [[1/2,1/2],[0,1]]
    B = [[1/2,0,1/2],[0,1,0]]
    pi = [1,0]

    o = [0,2,1]
    #ll = forward(A,B,pi,o,p=True)

    A = [[0.6,0.34],[1,0]]
    B = [[0.7, 0.3, 0],
     [0.1, 0.1, 0.8]]
    pi = [0.7,0.3]


    #o1
    o = list([1,0,0,0,1,0,1])
    ll = forward(A,B,pi,o)
    print(ll)
    #o2
    o = list([0,0,0,1,1,2,0])
    ll = forward(A,B,pi,o)
    print(ll)
    #o3
    o = list([1,1,0,1,0,1,2])
    ll = forward(A,B,pi,o)
    print(ll)
    #o4
    o = list([0,1,0,2,0,1,0])
    ll = forward(A,B,pi,o)
    print(ll)
    #o5
    o = list([2,2,0,1,1,0,1])
    ll = forward(A,B,pi,o)
    print(ll)
    #forward(A,B,pi,o)
    '''
