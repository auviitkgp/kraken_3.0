# import xml.etree.ElementTree as ET
# tree = ET.parse('scene_1.xml')
# root = tree.getroot()

# for link in root.iter('link'):

#     attribs = link.attrib

#     if attribs['name'] == 'gate_buoy_blue':

#         print "Blue Buoy Found!"

#         origin = link[0][1]

#         print origin.attrib

#         origin.set('xyz', '20 20 10')

# tree.write('scene_2.xml')

# import sys
# from PyQt4 import QtGui


# class Button(QtGui.QPushButton):
  
#     def __init__(self, title, parent):
#         super(Button, self).__init__(title, parent)
        
#         self.setAcceptDrops(True)

#     def dragEnterEvent(self, e):
      
#         if e.mimeData().hasFormat('text/plain'):
#             e.accept()
            
#         else:
#             e.ignore() 

#     def dropEvent(self, e):
        
#         self.setText(e.mimeData().text())


# class Application(QtGui.QWidget):
  
#     def __init__(self):
#         super(Application, self).__init__()
        
#         self.initUI()
        
#     def initUI(self):

#         edit = QtGui.QLineEdit('buoy', self)
#         edit.setDragEnabled(True)
#         edit.move(30, 65)

#         # construct grid

            # for i in range(4):

            #     for j in range(4):

            #         button = Button(str(i) + "," + str(j), self)

            #         button.move(190 + 100 * i, 50 + 50 * j)

#         # button = Button("Button", self)
#         # button.move(190, 65)

#         # button = Button("Button", self)
#         # button.move(190, 100)

#         # button = Button("Button", self)
#         # button.move(290, 100)
        
#         self.setWindowTitle('Simple Drag & Drop')
#         self.setGeometry(1000, 1000, 1000, 1000)
#         self.show()

# def main():
  
#     app = QtGui.QApplication([])
#     ex = Application()
#     app.exec_()  
  

# if __name__ == '__main__':
#     main() 


import sys
from PyQt4 import QtCore, QtGui


class Button(QtGui.QPushButton):
  
    def __init__(self, title, parent):
        super(Button, self).__init__(title, parent)

    def mouseMoveEvent(self, e):

        if e.buttons() != QtCore.Qt.RightButton:
            return

        mimeData = QtCore.QMimeData()

        drag = QtGui.QDrag(self)
        drag.setMimeData(mimeData)
        drag.setHotSpot(e.pos() - self.rect().topLeft())

        dropAction = drag.start(QtCore.Qt.MoveAction)

    def mousePressEvent(self, e):
      
        super(Button, self).mousePressEvent(e)
        
        if e.button() == QtCore.Qt.LeftButton:
            print 'press'


class Application(QtGui.QWidget):
  
    def __init__(self):
        super(Application, self).__init__()

        self.initUI()
        
    def initUI(self):

        self.setAcceptDrops(True)

        self.button = Button('Button', self)
        self.button.move(100, 65)

        self.setWindowTitle('Click or Move')
        self.setGeometry(300, 300, 280, 150)
        self.show()

    def dragEnterEvent(self, e):
      
        e.accept()

    def dropEvent(self, e):

        position = e.pos()  
        print e
        # e.move(position)      
        self.button.move(position)

        e.setDropAction(QtCore.Qt.MoveAction)
        e.accept()
        

def main():
  
    app = QtGui.QApplication([])
    ex = Application()
    sys.exit(app.exec_())


if __name__ == '__main__':
    main()