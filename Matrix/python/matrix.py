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
	

showCli()