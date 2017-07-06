/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package matrixmult;

import matrixmult.Matrix;

/**
 *
 * @author Maciek P
 */
public class finalMatrix {
    public static Matrix fMatrix;
    public static float suma = 0;
    public static float sumakw = 0;
    
    private int ncols;
    private int nrows;
    private float _data[];
    
  public void initMatrix (int a, int b) {
     this.fMatrix = new Matrix (a,b);
      this.ncols = a;
      this.nrows = b;
      _data = new float[a*b];
    
  }
  

}

