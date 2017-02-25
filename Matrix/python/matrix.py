import time

def showCli():
	print("1. Multiplication")
	print("2. Line Multiplication")
	selection = input("Selection?: ")
	print(selection)
	print("Dimensions:")
	lines = input("Lines?: ")
	cols = input("Cols?: ")
	multiplyMatrices(lines, cols)

def multiplyMatrices(lines, cols):
	phc = phb = pha = [[None for i in range(lines)] for j in range(lines)]
	temp = None

	for i in range(lines):
		for j in range(lines):
			pha[i][j] = float(1.0) #float is a C double

	for i in range(cols):
		for j in range(cols):
			phb[i][j] = float(i + 1);

	time1 = time.time()

	for i in range(lines):
		for j in range(cols):
			temp = float(0)
			for k in range(lines):
				temp += pha[i][k] * phb[k][j]
			phc[i][j] = temp

	time2 = time.time()

	print("Time: " + str(time2 - time1) + " seconds")

	del pha, phb, phc

showCli()