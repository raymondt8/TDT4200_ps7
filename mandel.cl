__kernel void mandelbrot(__global float *pixel, __global const int XSIZE, __global const int YSIZE ) {
    // Put actual kernel code here
    complex_t c, z, temp;
    int iter = 0;
  
    int id = get_global_id(0);
    int l_id = get_local_id(0);  
 
    c.real = (xleft + step * i);
    c.imag = (ylower + step * j);
    z = c;
    while (z.real * z.real + z.imag * z.imag < 4) {
        temp.real = z.real * z.real - z.imag * z.imag + c.real;
        temp.imag = 2 * z.real * z.imag + c.imag;
        z = temp;
        iter++;
        if(iter == MAXITER){
            break;
        }
      }
      pixel[j * XSIZE + i] = iter;

}
