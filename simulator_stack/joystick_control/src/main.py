from Tkinter import *


def create_callbacks(arg):
    
    def callback():
        if arg == 0:
            print 'left'
        elif arg == 1:
            print 'right'
        elif arg == 2: print 'down'
        elif arg == 3: print 'top'

    return callback

def callback(ev=None):
    print 'callback called!'

root = Tk()

frame = Frame(root, width=100, height=100)

left = Button(frame, text="left", command=create_callbacks(0))
left.grid(row=1,column=0)

down = Button(frame, text="down", command=create_callbacks(2))
down.grid(row=1,column=1)

right = Button(frame, text="right", command=create_callbacks(1))
right.grid(row=1,column=2)

top = Button(frame, text="top", command=create_callbacks(3))
top.grid(row=0,column=1)

frame.pack()

root.mainloop()
