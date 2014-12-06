posDict = {}

import parseXML

posDict = parseXML.parseSimX()

from PyQt4 import QtGui, QtCore

class MyFrame(QtGui.QGraphicsView):
    def __init__( self, parent = None ):
        super(MyFrame, self).__init__(parent)

        self.setScene(QtGui.QGraphicsScene())
        self.setGeometry(100, 100, 500, 500)

        # add some items
        x = 0
        y = 0
        w = 45
        h = 45
        pen = QtGui.QPen(QtGui.QColor(QtCore.Qt.green))
        brush = QtGui.QBrush(pen.color().darker(150))

        item = self.scene().addEllipse(x, y, w, h, pen, brush)
        item.setFlag(QtGui.QGraphicsItem.ItemIsMovable)

if ( __name__ == '__main__' ):
    app = QtGui.QApplication([])
    f = MyFrame()
    f.show()
    app.exec_()