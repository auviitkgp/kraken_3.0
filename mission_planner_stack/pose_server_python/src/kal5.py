# Fill in the matrices P, F, H, R and I at the bottom
#
# This question requires NO CODING, just fill in the 
# matrices where indicated. Please do not delete or modify
# any provided code OR comments. Good luck!

from math import *

class matrix:
    
    # implements basic operations of a matrix class
    
    def __init__(self, value):
        self.value = value
        self.dimx = len(value)
        self.dimy = len(value[0])
        if value == [[]]:
            self.dimx = 0
    
    def zero(self, dimx, dimy):
        # check if valid dimensions
        if dimx < 1 or dimy < 1:
            raise ValueError, "Invalid size of matrix"
        else:
            self.dimx = dimx
            self.dimy = dimy
            self.value = [[0 for row in range(dimy)] for col in range(dimx)]
    
    def identity(self, dim):
        # check if valid dimension
        if dim < 1:
            raise ValueError, "Invalid size of matrix"
        else:
            self.dimx = dim
            self.dimy = dim
            self.value = [[0 for row in range(dim)] for col in range(dim)]
            for i in range(dim):
                self.value[i][i] = 1
    
    def show(self):
        for i in range(self.dimx):
            print self.value[i]
        print ' '
    
    def __add__(self, other):
        # check if correct dimensions
        if self.dimx != other.dimx or self.dimy != other.dimy:
            raise ValueError, "Matrices must be of equal dimensions to add"
        else:
            # add if correct dimensions
            res = matrix([[]])
            res.zero(self.dimx, self.dimy)
            for i in range(self.dimx):
                for j in range(self.dimy):
                    res.value[i][j] = self.value[i][j] + other.value[i][j]
            return res
    
    def __sub__(self, other):
        # check if correct dimensions
        if self.dimx != other.dimx or self.dimy != other.dimy:
            raise ValueError, "Matrices must be of equal dimensions to subtract"
        else:
            # subtract if correct dimensions
            res = matrix([[]])
            res.zero(self.dimx, self.dimy)
            for i in range(self.dimx):
                for j in range(self.dimy):
                    res.value[i][j] = self.value[i][j] - other.value[i][j]
            return res
    
    def __mul__(self, other):
        # check if correct dimensions
        if self.dimy != other.dimx:
            raise ValueError, "Matrices must be m*n and n*p to multiply"
        else:
            # subtract if correct dimensions
            res = matrix([[]])
            res.zero(self.dimx, other.dimy)
            for i in range(self.dimx):
                for j in range(other.dimy):
                    for k in range(self.dimy):
                        res.value[i][j] += self.value[i][k] * other.value[k][j]
            return res
    
    def transpose(self):
        # compute transpose
        res = matrix([[]])
        res.zero(self.dimy, self.dimx)
        for i in range(self.dimx):
            for j in range(self.dimy):
                res.value[j][i] = self.value[i][j]
        return res
    
    # Thanks to Ernesto P. Adorio for use of Cholesky and CholeskyInverse functions
    
    def Cholesky(self, ztol=1.0e-5):
        # Computes the upper triangular Cholesky factorization of
        # a positive definite matrix.
        res = matrix([[]])
        res.zero(self.dimx, self.dimx)
        
        for i in range(self.dimx):
            S = sum([(res.value[k][i])**2 for k in range(i)])
            d = self.value[i][i] - S
            if abs(d) < ztol:
                res.value[i][i] = 0.0
            else:
                if d < 0.0:
                    raise ValueError, "Matrix not positive-definite"
                res.value[i][i] = sqrt(d)
            for j in range(i+1, self.dimx):
                S = sum([res.value[k][i] * res.value[k][j] for k in range(self.dimx)])
                if abs(S) < ztol:
                    S = 0.0
                res.value[i][j] = (self.value[i][j] - S)/res.value[i][i]
        return res
    
    def CholeskyInverse(self):
        # Computes inverse of matrix given its Cholesky upper Triangular
        # decomposition of matrix.
        res = matrix([[]])
        res.zero(self.dimx, self.dimx)
        
        # Backward step for inverse.
        for j in reversed(range(self.dimx)):
            tjj = self.value[j][j]
            S = sum([self.value[j][k]*res.value[j][k] for k in range(j+1, self.dimx)])
            res.value[j][j] = 1.0/tjj**2 - S/tjj
            for i in reversed(range(j)):
                res.value[j][i] = res.value[i][j] = -sum([self.value[i][k]*res.value[k][j] for k in range(i+1, self.dimx)])/self.value[i][i]
        return res
    
    def inverse(self):
        aux = self.Cholesky()
        res = aux.CholeskyInverse()
        return res
    
    def __repr__(self):
        return repr(self.value)


########################################

def filter(x, P):
    for n in range(len(measurements)):
        
       # prediction
        x = (F * x) + B*u
        P = F * P * F.transpose()
        print 'predicted:'
        x.show()
        
        # measurement update
        Z = matrix([measurements[n]])
        y = Z.transpose() - (H * x)
        S = H * P * H.transpose() + R
        K = P * H.transpose() * S.inverse()
        x = x + (K * y)
        P = (I - (K * H)) * P
        print 'measure :' 
        x.show()

    print 'x= '
    x.show()
    print 'P= '
    P.show()

########################################

print "### 6-dimensional example ###"

measurements = [[5., 5. ,5.], [5., 6. ,5.], [5., 7. ,5.], [5., 8. ,5.], [5., 9. ,5.], [5., 10. ,5.]]
initial_vxyz = [5., 4., 5.]


dt = 1.


# x : [x, y, z, vx, vy, vz]
x = matrix([ 
             [0.]             , 
             [0.]             , 
             [0.]             ,
             [initial_vxyz[0]], 
             [initial_vxyz[1]],
             [initial_vxyz[2]] 
           ]) # initial state (location and velocity)


F = matrix([
            [1., 0., 0., dt, 0. ,0.], 
            [0., 1., 0., 0., dt ,0.], 
            [0., 0., 1., 0. ,0. ,dt], 
            [0., 0., 0., 1. ,0., 0.],
            [0., 0., 0., 0. ,1., 0.],
            [0., 0., 0., 0. ,0., 1.],
           ])# next state function

u = matrix([
            [0.],
            [1.], 
            [0.]
           ]) # external motion.. acceleration here. ax, ay, az

B = matrix([
            [0.5*(dt**2), 0.         , 0.         ],
            [0.         , 0.5*(dt**2), 0.         ],
            [0.         , 0.         , 0.5*(dt**2)],
            [dt         , 0.         , 0.         ],
            [0.         , dt         , 0.         ],
            [0.         , 0.         , dt         ]
           ]) # control input function

P = matrix([
            [1.5  , 0.   , 0.    , 0.  ,0.  ,0. ], 
            [0.   , 1.5  , 0.    , 0.  ,0.  ,0. ], 
            [0.   , 0.   , 1.5   , 0.  ,0.  ,0. ],
            [0.   , 0.   , 0.    , 0.2 ,0.  ,0. ],
            [0.   , 0.   , 0.    , 0.  ,0.2 ,0. ], 
            [0.   , 0.   , 0.    , 0.  ,0.  ,0.2]
           ])# initial uncertainty

H = matrix([
            [0. , 0. , 0. , 1. ,0. ,0.],
            [0. , 0. , 0. , 0. ,1. ,0.],
            [0. , 0. , 0. , 0. ,0. ,1.]
           ]) # mapping function

R = matrix([[.1 , 0., 0.], 
            [ 0.,.1 , 0.],
            [ 0., 0., .1],
           ])# measurement uncertainty

I = matrix([
            [1. , 0. , 0. , 0  ,0. ,0.],
            [0. , 1. , 0. , 0  ,0. ,0.],
            [0. , 0. , 1. , 0  ,0. ,0.],
            [0. , 0. , 0. , 1. ,0. ,0.],
            [0. , 0. , 0. , 0. ,1. ,0.],
            [0. , 0. , 0. , 0. ,0. ,1.]
           ]) # Identity matrix

filter(x, P)
