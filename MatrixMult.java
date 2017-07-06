/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package matrixmult;

import java.io.*;
import static java.lang.Math.sqrt;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.Scanner;

/**
* @author Maciek P
 */
public class MatrixMult implements Runnable {
    
    
    
    private Matrix A,B;
    float suma;  
    int iloscWatkow;
    int i1;
    int i2;
    
    
    
    public MatrixMult(int ileWatkow, int i1, int i2) throws FileNotFoundException {
    this.A = read("A.txt");
    this.B = read("B.txt");
    this.i1 = i1;
    this.i2 = i2;
    }

    public static void main(String[] args) throws FileNotFoundException, InterruptedException {
      
        MatrixMult ala = new MatrixMult(1,0,0);
        ala.start1();
        
    }

    
    protected synchronized void start1() throws FileNotFoundException, InterruptedException {
        Matrix A,B;
        A = this.A;
        B = this.B;
        System.out.println("Loaded A:");
        print(A);

        System.out.println("\nLoaded B:");
        print(B);
        
        Matrix C = new Matrix(A.rows(), B.cols());
        
        ArrayList indexy = new ArrayList <Integer> ();
        
        int numberOfThreads = 1;
        System.out.println("Number of threads = " + numberOfThreads);
        
        int liczbaElementow = A.rows();
        for (int i=0; i<=liczbaElementow; i=i+(liczbaElementow/numberOfThreads)){
        indexy.add(i);
        }

        finalMatrix f = new finalMatrix();
        f.initMatrix(A.rows(), B.cols());
        
        Runnable[] runners = new Runnable[numberOfThreads];
        Thread[] threads = new Thread[numberOfThreads];
 
        for(int i=0; i<numberOfThreads; i++) {
            runners[i] = new MatrixMult(numberOfThreads, (int)indexy.get(i), (int)indexy.get(i+1));
        }
 
        for(int i=0; i<numberOfThreads; i++) {
            threads[i] = new Thread(runners[i]);
        }
 
        for(int i=0; i<numberOfThreads; i++) {
            threads[i].start();
            threads[i].join();
    }
        double frobenius = sqrt(finalMatrix.sumakw);
        System.out.println("");
        System.out.println("Frobenius equation = " + frobenius);
    }

  

   @Override
    public void run(){

        mult(this.A,this.B);
        System.out.println("*******************");
        System.out.println("Matrix after thread");
        print(finalMatrix.fMatrix);
        System.out.println("Threads addition sum = " + finalMatrix.suma);
        System.out.println("Squares addition in threads sum = " + finalMatrix.sumakw);

        
    }

    private void mult(Matrix A, Matrix B) {
       
        float sum = 0;
        float sumakwadratow = 0;
        for (int r = this.i1; r < this.i2; r++) {
            for (int c = 0; c < B.cols(); c++) {
                float s = 0;
                
                for (int k = 0; k < A.cols(); k++) {
                    s += A.get(r,k) * B.get(k, c);
                }
                finalMatrix.fMatrix.set(r,c,s);
                sum = sum + s;
                sumakwadratow = sumakwadratow + s*s;
            }
        }
        finalMatrix.suma = finalMatrix.suma + sum;
        finalMatrix.sumakw = finalMatrix.sumakw + sumakwadratow;
    }

    protected Matrix read(String fname) throws FileNotFoundException {
        File f = new File(fname);
        Scanner scanner = new Scanner(f).useLocale(Locale.ENGLISH);

        int rows  = scanner.nextInt();
        int cols  = scanner.nextInt();
        Matrix res = new Matrix(rows,cols);

        for (int r = 0; r < res.rows(); r++) {
            for (int c = 0; c < res.cols(); c++) {
                res.set(r, c, scanner.nextFloat());
            }
        }
        return res;
    }

    protected void print(Matrix m) {
        System.out.println("[");
        for (int r = 0; r < m.rows(); r++) {

            for (int c = 0; c < m.cols(); c++) {
                System.out.print(m.get(r,c));
                System.out.print(" ");
            }

            System.out.println("");
        }
        System.out.println("]");
    }


}

 class Matrix {
        private int ncols;
        private int nrows;
        private float _data[];

        public Matrix(int r, int c) {
            this.ncols = c;
            this.nrows = r;
            _data = new float[c*r];
        }

        public float get(int r, int c) {
            return _data[r*ncols + c];
        }

        public void set(int r, int c, float v) {
            _data[r*ncols +c] = v;
        }

        public int rows() {
            return nrows;
        }

        public int cols() {
            return ncols;
        }
    }




