f_ssets = [[ # error
            [-60,-60,-30],   # -ve medium
            [-60,-30 , 0],    # -ve small
            [-30 , 0 , 30],   # zero
            [ 0 , 30 , 60],   # +ve small
            [ 30 ,60 , 60], # +ve medium
           ],
            # delta_error
           [
            [-60,-60,-30],   # -ve medium
            [-60,-30 , 0],    # -ve small
            [-30 , 0 , 30],   # zero
            [ 0 , 30 , 60],   # +ve small
            [ 30 ,60 , 60], # +ve medium
           ],
            # u
           [
            [-40,-30,-15],  # -ve medium
            [-30,-15 , 0],  # -ve small
            [-15 , 0 , 15],  # zero
            [ 0 , 15 , 30], # +ve small
            [ 15 ,30 , 40], # +ve medium
           ]
          ]
# yapf: enable

io_ranges = [  # range of e
              [-180,180],
               # range of d_e
              [-180,180],
               # range of u
              [-180,180]
            ]

mf_types = ['trimf','trimf','trimf']

#-------------------------------------------------------------------------------
## fuzzy subset test case for gaussmf
# f_ssets = [[ # error
#             [-180,70], # -ve medium
#             [-50,20], # -ve small
#             [ 0 ,20], # zero
#             [50 ,20], # +ve small
#             [180 ,70], # +ve medium
#            ],
#             # delta_error
#            [
#             [-180,70], # -ve medium
#             [-50,20], # -ve small
#             [ 0 ,20], # zero
#             [50 ,20], # +ve small
#             [180 ,70], # +ve medium
#            ],
#             # u
#            [
#             [-3,2], # -ve medium
#             [-1,2], # -ve small
#             [ 0,1], # zero
#             [ 1,2], # +ve small
#             [ 3,2], # +ve medium
#            ]
#           ]
# # yapf: enable
#
# io_ranges = [  # range of e
#               [-180,180],
#                # range of d_e
#               [-180,180],
#                # range of u
#               [-10,10]
#             ]
#
# mf_types = ['gaussmf','gaussmf','gaussmf']

#---------------------------------------------------------------------------

