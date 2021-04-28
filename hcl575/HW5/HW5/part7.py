from part2 import forward
from part4 import backward





A = [[0.6, 0.4],
     [1, 0]]
B = [[0.7, 0.3, 0],
     [0.1, 0.1, 0.8]]
pi = pi = [0.7, 0.3]


O = [[1,0,0,0,1,0,1],[0,0,0,1,1,2,0],[1,1,0,1,0,1,2],[0,1,0,2,0,1,0],[2,2,0,1,1,0,1]]
i = 0
for obs in O:
    probf, alpha = forward(A,B,pi,obs)
    probb, beta = backward(A,B,pi,obs)
    for i in range(len(alpha)):
        Sum = 0
        for j in range(len(alpha[i])):
            Sum += alpha[i][j]*beta[i][j]
        print("L=",Sum,end=" ")
    print("\n")




