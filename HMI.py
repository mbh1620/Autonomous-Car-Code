# ///////// HUMAN MACHINE GRAPHICAL USER INTERFACE //////////////
# 

from Tkinter import *
import Tkinter as tk
import ttk as ttk
import smbus
import time
import os
import sys

bus = smbus.SMBus(1)

# i2c adress of arduino slave
i2c_address = 0x07
i2c_cmd_write = 0x01
i2c_cmd_read = 0x02

def ConvertStringToByte(src):
	converted = [] 
	for i in src:
		converted.append(ord(i))
	return converted


window = tk.Tk()

window.title("Human machine Interface")
window.geometry("500x850")


t = tk.Label(window, text="Human Machine Interface").grid(row=1, column=3)

global line
line = 1

#Button Functions 

def Forward():
	global line
	print line
	a = '%d.end' %line
	print a
	disp.insert(a, "F:")
	
def Backwards():
	global line
	print line
	a = '%d.end' %line
	print a
	disp.insert(a, "B:")

def Right():
	global line
	print line
	a = '%d.end' %line
	print a
	disp.insert(a, "R:")

def Left():
	global line
	print line
	a = '%d.end' %line
	print a
	disp.insert(a, "L:")

def _10():
	global line
	print line
	a = '%d.end' %line
	print a
	disp.insert(a, "10.")

def _50():
	global line
	print line
	a = '%d.end' %line
	print a
	disp.insert(a, "50.")

def _100():
	global line
	print line
	a = '%d.end' %line
	print a
	disp.insert(a, "100.")

def _45deg():
	global line
	print line
	a = '%d.end' %line
	print a
	disp.insert(a, "45.")

def _90deg():
	global line
	print line
	a = '%d.end' %line
	print a
	disp.insert(a, "90.")

def Reset():
	disp.delete('1.0', "100.0")
	global line
	line = 1

def Return():
	global line
	line = line + 1
	print line
	a = '%d.end' %line
	disp.insert(a, "\n")

def run():
        line1 = disp.get("1.0", "1.7")
        line1b = ConvertStringToByte(line1)
        print line1
        print line1b
        bus.write_i2c_block_data(i2c_address, i2c_cmd_write, line1b)




def quit1():
	os._exit(0)

	


# Button initialiations

forwardbutton = tk.Button(window, text="^\n| \n Forwards", height=5, width=7, command=Forward).grid(row=2, column=3)

rightbutton = tk.Button(window, text="--> \n Right", height=5, width=5, command=Right).grid(row=3, column=4)

leftbutton = tk.Button(window, text="<-- \n Left", height=5, width=5, command=Left).grid(row=3, column=2)

backbutton = tk.Button(window, text="|\n V \n Backwards", height=5, width=7, command=Backwards).grid(row=3, column=3)

resetbutton = tk.Button(window, text="reset", height=5, width=5, command=Reset).grid(row=10, column=2)

returnbutton = tk.Button(window, text="Return", state="normal", height=5, width=5, command=Return).grid(row=10, column=4)

runbutton = tk.Button(window, text="RUN", state="normal", height=5, width=5, command=run).grid(row=10, column=3)

quitbutton = tk.Button(window, text="quit", state="normal", height=5, width=5, command=quit1).grid(row=12, column=3)

button10 = tk.Button(window, text="10", state="normal", height=5, width=5, command=_10).grid(row=4, column=2)

button50 = tk.Button(window, text="50", state="normal", height=5, width=5, command=_50).grid(row=4, column=3) 

button100 = tk.Button(window, text="100", state="normal", height=5, width=5, command=_100).grid(row=4, column=4)

button90deg = tk.Button(window, text="90 deg", state="normal", height=5, width=5, command=_90deg).grid(row=5, column=2)

button45deg = tk.Button(window, text="45 deg", state="normal", height=5, width=5, command=_45deg).grid(row=5, column=4)

canvs = tk.Canvas(window, height=5, width=80).grid(row=5, column=1)


t = tk.Label(window, text="LOGO code").grid(row=6, column=3)
disp = tk.Text(window, height=15, width=20, state="normal")
disp.grid(row=7, column=3)




t = tk.Label(window, text="Designed by Matt").grid(row=12, column=5)



window.mainloop()








