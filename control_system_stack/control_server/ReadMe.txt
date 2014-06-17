arguments to be given in following format:

inputType localFlag arg1 arg2 arg3


inputType : '1' for Position (x,y,z) ; '0' for Orientation (roll,pitch,yaw)

localflag : '1' for local input : '0' for gloal input
          localFlag does not matter for inputType = 0

arg1, arg2, arg3 : 
        for inputType '0':
           arg1 = roll (will not change)
           arg2 = pitch
           arg3 = yaw
       
        for inputType '1':
           arg1 = x
           arg2 = y
           arg3 = z       (do not forget localFlag)

Bugs:
1.In /kraken/pose, yaw is sum of 'z' and 'w'.Motion stops when z + w =yaw, but 'Pause' command is not passed 


