


def fasthmm(A,B,pi,o):
    alphas = []
    palpha = 0
    for obs in range(len(o)):
        nalpha = 0
        for states in range(len(pi)):
            if obs == 0:
                palpha += pi[states]*B[states][o[obs]]
            elif obs==len(o)-1:
                for s in range(len(pi)):
                    nalpha += palpha*B[states][o[obs]]*A[s][states]

            else:
                for s in range(len(pi)):
                    nalpha += palpha*B[states][o[obs]]*A[s][states]
        if obs ==0:
            alphas.append(palpha)
        elif obs == len(o)-1:
            alphas.append(nalpha)
            print(alphas)
            return nalpha
        else:
            #palpha = nalphia
            palpha = nalpha
            alphas.append(nalpha)




if __name__=="__main__":
    A = [[0.6,0.34],[1,0]]
    B = [[0.5,0.25,0.25],[0.1,0.1,0.8]]
    pi = [0.8,0.2]
    o = list([0,1,0,2,0,1,0])

    ll = fasthmm(A,B,pi,o)

    A = [[0.8,0.1,0.1],[0.4,0.2,0.4],[0,0.3,0.7]]
    B = [[0.66,0.34,0],[0,0,1],[0.5,0.4,0.1]]
    pi = [0.6,0,0.4]

    ll = fasthmm(A,B,pi,o)
    
