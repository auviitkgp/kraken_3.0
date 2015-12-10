''' fuzzy.py
    --------

Implements fuzzy control in one degree of freedom.
'''


### Libraries
# Standard libraries
import numpy as np
from operator import add
import itertools

# Third-party libraries
import skfuzzy as fuzz
import matplotlib.pyplot as plt

# Related libraries
import pid
import visualize

class Fuzzy(pid.PID):

    """ Creates an object, to provide the pid gains corresponding to a particular error and delta_error
        This class is compatible of receiving different type of membership function for each of the variables, and
        receiving different no. of fuzzy_ subsets for each io variables each
        with all possible different range.

        Attributes :
            mf_types - List of type of membership function for i/o variables in fuzzy (format - string)
            f_ssets  - (Fuzzy subsets for each i/o variable) 3d list, each 2d list contains all the fuzzy subsets
                         and their range of a particular i/o variable
    """

    def __init__(self, mf_types, f_ssets):

        """
        Instance variables:
            error , delta_e - Input variables of fuzzy control
            mf_types , f_ssets - Membership function and fuzzy subsets for each i/o variables
            io_ranges - Range of io variables i.e, error, delta_e and control_output [not values]
        """

        self.error     = 0
        self.delta_e   = 0
        self.mf_types  = mf_types
        self.f_ssets   = f_ssets
        self.io_ranges = []

    def run(self):

        """
        Finds appropriate value of pid gains

        NO arguments :

        inputs : List to contain discrete index values of io variables in their range (step size = 1) for plotting. i.e, x axis
            inputs[0] =  ERROR AXIS          ., so stores all possible index values for error
            inputs[1] =  DEL_ERROR AXIS      .,     ,,
            inputs[2] =  CONTROL_OUTPUT AXIS .,     ,,

        b : 3d list, each layer (i.e, 2d list) contains 1d lists of y-values (in x of step size 1) of a particular fuzzy
            subset of a particular i/o variable

        muval_de, muval_e: Stores membership value of error and delta_error for each fuzzy subsets

            ERROR                  DEL_ERROR               CONTROL_OUTPUT         m_value for crisp e and delta_e values

            b[0][0] -ve Medium  || b[1][0] -ve Medium  ||  b[2][0] -ve Medium   ..        muval[0] |  muval_d[0]
            b[0][1] -ve small   || b[1][1] -ve small   ||  b[2][1] -ve small    ..        muval[1] |  muval_d[1]
            b[0][2] zero        || b[1][2] zero        ||  b[2][2] zero         ..        muval[2] |  muval_d[2]
            b[0][3] +ve small   || b[1][3] +ve small   ||  b[2][3] +ve small    ..        muval[3] |  muval_d[3]
            b[0][4] +ve Medium  || b[1][4] +ve Medium  ||  b[2][4] +_ve Medium  ..        muval[4] |  muval_d[4]

        f_mat is a 2d matrix containing rule strengths
        """
        ''' Storing the values for plotting mf ends here '''

        inputs = [ np.arange(var[0], var[1]+1, 1) for var in self.io_ranges]
        b  = []
        for i in range(3) :
            # i = 0 : for error, 1 : for delta_error and 2 : for u
            a = []
            for j in range(len(self.f_ssets[i])):
                ''' Recieve the y value for fuzzy subset[j], say for Positive small in error '''
                a.append(membership_f(self.mf_types[i], inputs[i], self.f_ssets[i][j]) )
                # print membership_f(self.mf_types[i], inputs[i], self.f_ssets[i][j])

                x = []
                ''' For the end fuzzy subset , need to get constant y-values for greater than particular variable say, for error value '''
                if j==0 or j== (len(self.f_ssets[i])-1):
                    for y in range(self.io_ranges[i][0],self.io_ranges[i][1]+1,1):
                        if y < self.f_ssets[i][j][1]:
                            x.append(1 - j/(len(self.f_ssets[i])-1))
                        elif y == self.f_ssets[i][j][1]:
                            x.append(0)
                        else:
                            x.append(j/(len(self.f_ssets[i])-1))
                    if self.mf_types[i] == 'trimf' : a[j] = np.array(map(add,a[j],x))
            b.append(a)

        ''' Storing the values for plotting mf ends here '''
        # visualize.visualize_mf(b,inputs)

        ''' Ensure that error and delta_e in their io_ranges '''
        self.error = self.io_ranges[0][0] if self.error <= self.io_ranges[0][0] else self.error
        self.error = self.io_ranges[0][1] if self.error >= self.io_ranges[0][1] else self.error
        self.delta_e = self.io_ranges[1][0] if self.delta_e <= self.io_ranges[1][0] else self.delta_e
        self.delta_e = self.io_ranges[1][1] if self.delta_e >= self.io_ranges[1][1] else self.delta_e

        ''' Fuzzify Error and delta error to obtain their membership values for corr. fuzzy subsets '''
        muval_e  = fuzzify(inputs[0], b[0], self.error)
        muval_de = fuzzify(inputs[1], b[1], self.delta_e)
        # print 'muval_e:', muval_e
        # print 'muval_de:', muval_de

        ''' Obtain the rule strength matrix'''
        f_mat = fuzzy_matrix(muval_e, muval_de)

        ''' obtain the clipped y value for each output fuzzy subsets using "output activation"
            output activation = max. value in rule strength matrix for particular types of fuzzy subsets of U'''
        output = rule_base(b, f_mat)
        aggregated = np.fmax(output[0], np.fmax(output[1],np.fmax(output[2], np.fmax(output[3], output[4]))))
        out_final  = fuzz.defuzz(inputs[2], aggregated, 'centroid')
        #print "out_final : %0.2f, Error : %0.2f Delta_e : %0.2f " % (out_final, self.error, self.delta_e)

        ''' plotting final output '''
        # visualize.visualize_output(b, inputs, output, out_final, aggregated)
        # plt.show()
        return out_final


def membership_f(mf, x, abcd ):
    """
    Returns y values corresponding to type of type of Membership fn.
    arguments:
        mf - string containing type of Membership function
        x  - x axis values
    """
    if mf == 'zmf': return fuzz.zmf(x, abcd[0], abcd[1])                                # zmf(x, a, b)
    elif mf == 'trimf'   : return fuzz.trimf(x, abcd[0:3])                              # trimf(x, abc)
    elif mf == 'dsigmf'  : return fuzz.dsigmf(x, abcd[0], abcd[1], abcd[2], abcd[3])    # dsigmf(x, b1, c1, b2, c2)
    elif mf == 'gauss2mf': return fuzz.gauss2mf(x, abcd[0], abcd[1], abcd[2], abcd[3])  # gauss2mf(x, mean1, sigma1, mean2, sigma2)
    elif mf == 'gaussmf' : return fuzz.gaussmf(x, abcd[0], abcd[1])                     # gaussmf(x, mean, sigma)
    elif mf == 'gbellmf' : return fuzz.gbellmf(x, abcd[0], abcd[1], abcd[2])            # gbellmf(x, a, b, c)
    elif mf == 'piecemf' : return fuzz.piecemf(x, abcd[0:3])                            # piecemf(x, abc)
    elif mf == 'pimf'    : return fuzz.pimf(x, abcd[0], abcd[1], abcd[2], abcd[3])      # pimf(x, a, b, c, d)
    elif mf == 'psigmf'  : return fuzz.psigmf(x, abcd[0], abcd[1], abcd[2], abcd[3])    # psigmf(x, b1, c1, b2, c2)
    elif mf == 'sigmf'   : return fuzz.sigmf(x, abcd[0], abcd[1])                       # sigmf(x, b, c)
    elif mf == 'smf'     : return fuzz.smf(x, abcd[0], abcd[1])                         # smf(x, a, b)
    elif mf == 'trapmf'  : return fuzz.trapmf(x, abcd)                                  # trapmf(x, abcd)



# def membership_f(mf, x, abc = [0,0,0], a = 1, b = 2, c = 3, d = 4, abcd = [0,0,0,0]):
#     """
#     Returns y values corresponding to type of type of Membership fn.
#     arguments:
#         mf - string containing type of Membership function
#         x  - x axis values
#         abc - list containing triangular edge point x-values
#     """
#     return {
#         'trimf'   : fuzz.trimf(x, abc),                                 # trimf(x, abc)
#         'dsigmf'  : fuzz.dsigmf(x, a, b, c, d),                         # dsigmf(x, b1, c1, b2, c2)
#         'gauss2mf': fuzz.gauss2mf(x, a, b, c, d),                       # gauss2mf(x, mean1, sigma1, mean2, sigma2)
#         'gaussmf' : fuzz.gaussmf(x, a, b),                              # gaussmf(x, mean, sigma)
#         'gbellmf' : fuzz.gbellmf(x, a, b, c),                           # gbellmf(x, a, b, c)
#         'piecemf' : fuzz.piecemf(x, abc),                               # piecemf(x, abc)
#         'pimf'    : fuzz.pimf(x, a, b, c, d),                           # pimf(x, a, b, c, d)
#         'psigmf'  : fuzz.psigmf(x, a, b, c, d),                         # psigmf(x, b1, c1, b2, c2)
#         'sigmf'   : fuzz.sigmf(x, a, b),                                # sigmf(x, b, c)
#         'smf'     : fuzz.smf(x, a, b),                                  # smf(x, a, b)
#         'trapmf'  : fuzz.trapmf(x, abcd),                               # trapmf(x, abcd)
#         'zmf'     : fuzz.zmf(x, a, b),                                  # zmf(x, a, b)
#             }[mf]

def fuzzify(Input, y, crisp_val):
    """
    Fuzzifies crisp value to obtain their membership values for corr. fuzzy subsets.
    arguments:
        Input - Range of crisp_val i.e, list of discrete x values which crisp_val can take
        y     - 2d list containing y values of each fuzzy subsets of an i/o variable
        crisp_val - value to be fuzzified
    """
    f = [fuzz.interp_membership(Input, fuzzy_sset_y, crisp_val) for fuzzy_sset_y in y ]
    return f

def fuzzy_matrix(muval_e, muval_de):
    """
    Returns 2d array of rule strengths
    arguments:
        muval_e, muval_de - 1d list of membership values to their corresponding fuzzy subsets
    """
    return np.array([ [min(a,b) for a in muval_e] for b in muval_de])

#b= y-values of trimf corresponding to each input and output variables in range var.ranges[]
def rule_base(b, f_mat):
    """
    Returns y values of output by clipping by an amount of output activations for output fuzzy subsets
    arguments:
    f_mat - rule_strength matrix
    b     - b[2] , y values of output fuzzy subsets

E / DEL_E|         NM      ||       NS        ||        Z         ||       PS        ||       PM
----------------------------------------------------------------------------------------------------------
    NM   | f_mat[0][0] NM  || f_mat[0][1] NM  ||  f_mat[0][2] NS  || f_mat[0][3] Z   || f_mat[0][4] PS
    NS   | f_mat[1][0] NM  || f_mat[1][1] NM  ||  f_mat[1][2] NS  || f_mat[1][3] PS  || f_mat[1][4] PM
    Z    | f_mat[2][0] NM  || f_mat[2][1] NS  ||  f_mat[2][2] Z   || f_mat[2][3] PS  || f_mat[2][4] PM
    PS   | f_mat[3][0] NM  || f_mat[3][1] NS  ||  f_mat[3][2] PS  || f_mat[3][3] PM  || f_mat[3][4] PM
    PM   | f_mat[4][0] NS  || f_mat[4][1] Z   ||  f_mat[4][2] PS  || f_mat[4][3] PM  || f_mat[4][4] PM

    """
    NM = max(f_mat[0][0], f_mat[0][1], f_mat[1][0], f_mat[1][1], f_mat[2][0], f_mat[3][0]) # activation
    b[2][0] = np.fmin(NM,b[2][0]) # clipping
    NS = max(f_mat[0][2], f_mat[1][2], f_mat[2][1], f_mat[3][1], f_mat[4][0]) # activation
    b[2][1] = np.fmin(NS, b[2][1])# clipping
    Z  = max(f_mat[0][3], f_mat[2][2], f_mat[4][1]) # activation
    b[2][2] = np.fmin(Z, b[2][2]) # clipping
    PS = max(f_mat[0][4], f_mat[1][3], f_mat[2][3], f_mat[3][2], f_mat[4][2]) # activation
    b[2][3] = np.fmin(PS, b[2][3])# clipping
    PM = max(f_mat[1][4], f_mat[2][4], f_mat[3][4], f_mat[3][3], f_mat[4][3], f_mat[4][4]) # activation
    b[2][4] = np.fmin(PM, b[2][4])# clipping

    return b[2]
