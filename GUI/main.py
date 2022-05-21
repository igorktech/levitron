from PyQt5 import QtWidgets, uic
from PyQt5.QtSerialPort import QSerialPort, QSerialPortInfo
from PyQt5.QtCore import QIODevice
from PyQt5.QtCore import QElapsedTimer
from PyQt5 import QtGui
from PyQt5.QtCore import Qt
from PyQt5.QtCore import QAbstractTableModel
from PyQt5.QtWidgets import QApplication, QWidget, QInputDialog, QLineEdit, QFileDialog
from PyQt5.QtGui import QIcon
from PyQt5.QtWidgets import QMainWindow, QApplication, QWidget, QAction, QTableWidget, QTableWidgetItem, QVBoxLayout
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import pyqtSlot
from PyQt5.QtWidgets import QWidget, QShortcut, QApplication, QMessageBox
from PyQt5.QtWidgets import QMainWindow, QAction, qApp, QApplication
from PyQt5.QtGui import QKeySequence
from PyQt5.QtGui import QIcon, QPixmap
import time
from threading import *
from pyqtgraph import PlotWidget
import pyqtgraph as pg
import pandas as pd
import openpyxl
from PyQt5.QtWidgets import QMainWindow, QLabel, QGridLayout, QWidget
from PyQt5.QtWidgets import QPushButton
from PyQt5.QtWidgets import QMessageBox
import sys
import numpy as np

app = QtWidgets.QApplication([])
ui = uic.loadUi("design.ui")
ui.setWindowTitle("LevitronGUI")
ui.setWindowIcon(QtGui.QIcon('magnet.ico'))
# open qss file
Theme = open("themes/py_dracula_theme_light.qss", 'r')

with Theme:
    qss = Theme.read()
    app.setStyleSheet(qss)


class TableModel(QAbstractTableModel):

    def __init__(self, data):
        super(TableModel, self).__init__()
        self._data = data

    def data(self, index, role):
        if role == Qt.DisplayRole:
            value = self._data.iloc[index.row(), index.column()]
            return str(value)

    def rowCount(self, index):
        return self._data.shape[0]

    def columnCount(self, index):
        return self._data.shape[1]

    def headerData(self, section, orientation, role):
        # section is the index of the column/row.
        if role == Qt.DisplayRole:
            if orientation == Qt.Horizontal:
                return str(self._data.columns[section])

            if orientation == Qt.Vertical:
                return str(self._data.index[section])


class TableView(QWidget):

    def __init__(self, data):
        super().__init__()
        self.table = QtWidgets.QTableView()
        self.setWindowTitle('Table View')
        self.model = TableModel(data)
        self.table.setModel(self.model)
        layout = QVBoxLayout()
        layout.addWidget(self.table)
        self.setLayout(layout)
        self.resize(400, 300)


class App(QWidget):

    def __init__(self):
        super().__init__()
        self.title = 'File dialog'
        self.left = 10
        self.top = 10
        self.width = 640
        self.height = 480
        self.initUI()

    def initUI(self):
        self.setWindowTitle(self.title)
        self.setGeometry(self.left, self.top, self.width, self.height)

        # self.openFileNameDialog()
        # self.openFileNamesDialog()
        # self.saveFileDialog()

        # self.show()

    def openFileNameDialog(self):
        options = QFileDialog.Options()
        options |= QFileDialog.DontUseNativeDialog
        fileName, _ = QFileDialog.getOpenFileName(self, "Open File", "",
                                                  "Excel Files (*.xlsx)", options=options)
        if fileName:
            print(fileName)
            # df.to_excel(fileName, sheet_name='output_data')
            ret = QMessageBox.question(self, 'WARNING', "Overwrite current data?",
                                       QMessageBox.Ok | QMessageBox.Cancel, QMessageBox.Cancel)

            if ret == QMessageBox.Ok:
                file = pd.read_excel(fileName, sheet_name='output_data', index_col=0)
                return file
            # if ret == QMessageBox.Cancel:

            # print(fileName)

    # def openFileNamesDialog(self):
    #     options = QFileDialog.Options()
    #     options |= QFileDialog.DontUseNativeDialog
    #     files, _ = QFileDialog.getOpenFileNames(self, "QFileDialog.getOpenFileNames()", "",
    #                                             "All Files (*);;Python Files (*.py)", options=options)
    #     if files:
    #         print(files)

    def saveFileDialog(self, df):
        options = QFileDialog.Options()
        options |= QFileDialog.DontUseNativeDialog
        fileName, _ = QFileDialog.getSaveFileName(self, 'Save File', "",
                                                  "Excel Files (*.xlsx)", options=options)
        if fileName:
            print(fileName)
            print(_)
            fileName += ".xlsx"
            df.to_excel(fileName, sheet_name='output_data')
            print(fileName)

    def saveFilePictureDialog(self):
        options = QFileDialog.Options()
        options |= QFileDialog.DontUseNativeDialog
        fileName, _ = QFileDialog.getSaveFileName(self, 'Save File', "",
                                                  "Image Files (*.png)", options=options)
        if fileName:
            return fileName


global time_sec
time_sec = 0
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

global data
data = pd.DataFrame([], index=['OUTPUT', 'INPUT', 'TIME'])

ui.graph.showGrid(x=True, y=True)
styles = {'color': 'r', 'font-size': '20px'}
ui.graph.setLabel('left', 'Положение, В', **styles)
ui.graph.setLabel('bottom', 'Время, с', **styles)
ui.graph.setBackground(QtGui.QColor(255, 255, 255, 25))
pen_out = pg.mkPen(color=(255, 0, 0))
pen_input = pg.mkPen(color=(205, 25, 200))


def onRead():
    if not serial.canReadLine(): return  # выходим если нечего читать
    rx = serial.readLine()

    rxs = str(rx, 'utf-8').strip()
    frame = rxs.split(',')  # returns in out
    # print(frame)
    num = [5 / 1024, 5 / 1024]
    frame = np.multiply(list(map(float, frame)), num).tolist()

    if frame:
        global time_sec

        frame.append(time.time() - time_sec)
        global data
        # print(str(len(data.columns)))

        data.insert(loc=len(data.columns), column=str(len(data.columns)), value=frame)

        ui.graph.clear()
        ui.graph.plot(list(data.loc['TIME']), list(data.loc['OUTPUT']), name='OUTPUT', pen=pen_out)
        ui.graph.plot(list(data.loc['TIME']), list(data.loc['INPUT']), name='INPUT', pen=pen_input)
        # ui.graph.scene()


def thread():
    t1 = Thread(target=onRead)
    print("fff")
    t1.start()


def onOpen():
    # timer.start()
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
    ui.KLabel.setText(str(ui.K.value() / 10000))
    ui.PLabel.setText(str(ui.P.value() / 10000))
    ui.ILabel.setText(str(ui.I.value() / 10000))
    ui.DLabel.setText(str(ui.D.value() / 10000))


def PIDcontrolLabel():
    if ui.KLabel.text():
        ui.K.setValue(int(float(ui.KLabel.text()) * 10000))
    if ui.PLabel.text():
        ui.P.setValue(int(float(ui.PLabel.text()) * 10000))
    if ui.ILabel.text():
        ui.I.setValue(int(float(ui.ILabel.text()) * 10000))
    if ui.DLabel.text():
        ui.D.setValue(int(float(ui.DLabel.text()) * 10000))


def toggle(var):
    return not var


def toggleState():
    # отправить данные о регуляторе
    # добавить отправку данных о старте стопе

    global state
    state = toggle(state)
    ui.PIDGroupBox.setDisabled(state)
    # ui.groupBox_2.setDisabled(state)
    ui.DataGroupBox.setDisabled(state)
    if not state:
        ui.startB.setText("START")
        onClose()
    else:
        ui.startB.setText("STOP")
        ui.graph.clear()
        global data
        data = pd.DataFrame([], index=['OUTPUT', 'INPUT', 'TIME'])
        onOpen()
        serialSend([ui.K.value() / 1000, ui.P.value() / 1000, ui.I.value() / 1000, ui.D.value() / 1000])
        global time_sec
        time_sec = time.time()


def call_save():
    global data
    ex = App()
    ex.saveFileDialog(data)
    # ex.show()


def call_open():
    global data
    ex = App()

    data = ex.openFileNameDialog()

    if not data.empty:
        ui.graph.clear()
        ui.graph.plot(list(data.loc['TIME']), list(data.loc['OUTPUT']), name='OUTPUT', pen=pen_out)
        ui.graph.plot(list(data.loc['TIME']), list(data.loc['INPUT']), name='INPUT', pen=pen_input)


def call_show_table():  # TODO
    global data
    table = TableView(data)
    table.show()
    table.exec()


def call_save_chart():
    p = ui.graph.grab()
    ex = App()
    name = ex.saveFilePictureDialog()
    p.save(name, 'png')


def mouse_clicked(event):
    print("l")


serial.readyRead.connect(onRead)
ui.startB.clicked.connect(toggleState)

ui.saveTableB.clicked.connect(call_save)
ui.showTableB.clicked.connect(call_show_table)
ui.openChartB.clicked.connect(call_open)
ui.saveChartB.clicked.connect(call_save_chart)
ui.K.valueChanged.connect(PIDcontrol)
ui.P.valueChanged.connect(PIDcontrol)
ui.I.valueChanged.connect(PIDcontrol)
ui.D.valueChanged.connect(PIDcontrol)
ui.KLabel.editingFinished.connect(PIDcontrolLabel)
ui.PLabel.editingFinished.connect(PIDcontrolLabel)
ui.ILabel.editingFinished.connect(PIDcontrolLabel)
ui.DLabel.editingFinished.connect(PIDcontrolLabel)

# Capture mouse click events
ui.graph.scene().sigMouseClicked.connect(mouse_clicked)

ui.show()
app.exec()

# def main(args):
#     app=QtWidgets.QApplication(args)
#     window=MainWindow()
#     window.show()
#     sys.exit(app.exec_())
#     #app.exec_()
# #     app = QApplication(args)
# #     table = TableView(data, 4, 3)
# #     table.show()
# #     sys.exit(app.exec_())
# #
# #
# if __name__ == "__main__":
#     main(sys.argv)
# app=QtWidgets.QApplication(sys.argv)
# window=MainWindow()
# window.show()
# app.exec_()

# if __name__ == '__main__':
# app = QApplication(sys.argv)
# ex = App()
# ex.show()
# sys.exit(app.exec_())
