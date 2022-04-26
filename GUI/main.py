from PyQt5 import QtWidgets, uic
from PyQt5.QtSerialPort import QSerialPort, QSerialPortInfo
from PyQt5.QtCore import QIODevice
from PyQt5.QtCore import QElapsedTimer
from PyQt5 import QtGui
import time
from threading import *
from pyqtgraph import PlotWidget
import pyqtgraph as pg
import sys
import numpy as np

# https://stackoverflow.com/questions/35932660/qcombobox-click-event обновление по клику

app = QtWidgets.QApplication([])
ui = uic.loadUi("design.ui")
ui.setWindowTitle("LevitronGUI")
ui.setWindowIcon(QtGui.QIcon('magnet.ico'))

global k
k = 0
global p
p = 0
global i
i = 0
global d
d = 0
global state
state = True
state = not state
serial = QSerialPort()
serial.setBaudRate(115200)
portList = []
ports = QSerialPortInfo().availablePorts()
for port in ports:
    portList.append(port.portName())
ui.comL.addItems(portList)

timer = QElapsedTimer()


listX = []
for x in range(100): listX.append(x)
listY = []
for x in range(100): listY.append(0)

ui.graph.showGrid(x=True, y=True)
styles = {'color':'r', 'font-size':'20px'}
ui.graph.setLabel('left', 'Положение, см', **styles)
ui.graph.setLabel('bottom', 'Время, с', **styles)
ui.graph.setBackground(QtGui.QColor(255,255,255,25))
pen = pg.mkPen(color=(255, 0, 0))


def onRead():
    if not serial.canReadLine(): return     # выходим если нечего читать
    rx = serial.readLine()
    rxs = str(rx, 'utf-8').strip()
    # data = rxs.split(',')
    if rx:
        global listX
        global listY
        # if (timer.elapsed() / 1000) < 100:
        listY = listY[1:]
        listY.append(float(rxs))
        # listX = listX[1:]
        # listX.append(timer.elapsed()/1000)
        print("timer.elapsed()")
        ui.graph.clear()
        ui.graph.plot(listX, listY,pen = pen)

def thread():
    t1=Thread(target=onRead)
    print("fff")
    t1.start()

def onOpen():
    #timer.start()
    serial.setPortName(ui.comL.currentText())
    serial.open(QIODevice.ReadWrite)


def serialSend(data):
    txs = ""
    for val in data:
        txs += str(val)
        txs += ','
    txs = txs[:-1]
    txs += '\n'
    print(txs.encode())
    serial.write(txs.encode())

def onClose():
    serial.close()

def PIDcontrol():
    ui.KLabel.setText(str(ui.K.value()/1000))
    ui.PLabel.setText(str(ui.P.value()/1000))
    ui.ILabel.setText(str(ui.I.value()/1000))
    ui.DLabel.setText(str(ui.D.value()/1000))

def toggle(var):
    return not var
def toggleState():
    # отправить данные о регуляторе
    #добавить отправку данных о старте стопе

    global state
    state = toggle(state)
    ui.groupBox.setDisabled(state)
    ui.groupBox_2.setDisabled(state)
    ui.groupBox_3.setDisabled(state)
    if not state:
        ui.startB.setText("START")
    else:
        ui.startB.setText("STOP")
    serialSend([ui.K.value()/1000, ui.P.value()/1000, ui.I.value()/1000, ui.D.value()/1000])


def mouse_clicked(event):
    print("l")

serial.readyRead.connect(onRead)
ui.openB.clicked.connect(onOpen)
ui.closeB.clicked.connect(onClose)

ui.startB.clicked.connect(toggleState)

ui.K.valueChanged.connect(PIDcontrol)
ui.P.valueChanged.connect(PIDcontrol)
ui.I.valueChanged.connect(PIDcontrol)
ui.D.valueChanged.connect(PIDcontrol)
# Capture mouse click events
ui.graph.scene().sigMouseClicked.connect(mouse_clicked)


ui.show()
app.exec()