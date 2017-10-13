import java.util.Scanner;

public class Matrix {
	
	public void onMult(int lines, int cols) {
		double pha[] = new double[lines * cols];
		double phb[] = new double[lines * cols];
		double phc[] = new double[lines * cols];
		double temp;

		for(int i = 0; i < lines; i++)
		for(int j = 0; j < lines; j++)
			pha[i * lines + j] = (double)1.0;


		
		for (int i = 0; i < cols; i++)
		for (int j = 0; j < cols; j++)
			phb[i * cols + j] = (double)(i + 1);
		
		long startTime = System.currentTimeMillis();

		for(int i = 0; i < lines; i++)
		for(int j = 0; j < cols; j++) {	
			temp = 0;
			for(int k = 0; k < lines; k++) {
				temp += pha[i * lines + k] * phb[k * cols + j];
			}
			phc[i * lines + j] = temp;
		}
		
		long diffTime = (System.currentTimeMillis() - startTime);
		System.out.println(diffTime + " ms");
	}

	public void onMultLine(int lines, int cols) {
		double pha[] = new double[lines * cols];
		double phb[] = new double[lines * cols];
		double phc[] = new double[lines * cols];
		double temp;

		for(int i = 0; i < lines; i++)
		for(int j = 0; j < lines; j++)
			pha[i * lines + j] = (double)1.0;


		
		for (int i = 0; i < cols; i++)
		for (int j = 0; j < cols; j++)
			phb[i * cols + j] = (double)(i + 1);
		
		long startTime = System.currentTimeMillis();

		for(int i = 0; i < lines; i++)
		for(int j = 0; j < cols; j++) {	
			for(int k = 0; k < lines; k++) {
				phc[i * lines + k] += pha[i * lines + j] * phb[j * cols + k];
			}
		}
		
		long diffTime = (System.currentTimeMillis() - startTime);
		System.out.println(diffTime + " ms");
	}

	public static void main(String[] args) {
		int option, lines, cols = -1;
		Scanner reader = new Scanner(System.in);
		Matrix matrix = new Matrix();

		while(true) {
			System.out.println("1. Multiplication");
			System.out.println("2. Line Multiplication");
			System.out.println("Selection: ");
			option = reader.nextInt();
			System.out.println("Lines: ");
			lines = reader.nextInt();
			System.out.println("Collumns: ");
			cols = reader.nextInt(); 
			switch (option) {
				case 1:
					matrix.onMult(lines, cols);
					break;
				case 2:
					break;
			}
		}
	}

}
