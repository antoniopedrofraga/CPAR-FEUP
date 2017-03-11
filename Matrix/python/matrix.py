import time
import numpy as np

def showCli():
	while 1:
		print("1. Multiplication")
		print("2. Line Multiplication")
		selection = input("Selection?: ")
		print("Dimensions:")
		lines = input("Lines?: ")
		cols = input("Cols?: ")
		if selection == 1:
			onMult(lines, cols);
		elif selection == 2:
			onMultLine(lines, cols);

def onMult(lines, cols):
	phc = phb = pha = np.array([None for i in range(lines * lines)])
	temp = None

	for i in range(lines):
		for j in range(lines):
			pha[i * lines + j] = float(1.0) #float is a C double

	for i in range(cols):
		for j in range(cols):
			phb[i * cols + j] = float(i + 1);

	time1 = time.time()

	for i in range(lines):
		for j in range(cols):
			temp = float(0)
			for k in range(lines):
				temp += pha[i * lines + k] * phb[k * cols + j];
			phc[i * lines + j] = temp

	time2 = time.time()
	
	print("Time: " + str(time2 - time1) + " seconds")

	del pha, phb, phc

def onMultLine(lines, cols):
	phc = phb = pha = np.array([None for i in range(lines * lines)])
	temp = None

	for i in range(lines):
		for j in range(lines):
			pha[i * lines + j] = float(1.0) #float is a C double

	for i in range(cols):
		for j in range(cols):
			phb[i * cols + j] = float(i + 1);

	time1 = time.time()

	for i in range(lines):
		for j in range(cols):
			for k in range(lines):
				phc[i * lines + k] += pha[i * lines + j] * phb[j * cols + k];

	time2 = time.time()
	
	print("Time: " + str(time2 - time1) + " seconds")

	del pha, phb, phc

showCli()