#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <CL/cl.h>

/* Shorthand for less typing */
typedef unsigned char uchar;

/* Declarations of output functions */
void output(int* pixel);
void fancycolour(uchar *p, int iter);
void savebmp(char *name, uchar *buffer, int x, int y);

/* Struct for complex numbers */
typedef struct {
  float real, imag;
} complex_t;

/* Size of image, in pixels */
const int XSIZE = 2560;
const int YSIZE = 2048;

/* Max number of iterations */
const int MAXITER = 255;

/* Range in x direction */
const float xleft = -2.0;
const float xright = 1.0;
const float ycenter = 0.0;

/* Range in y direction, calculated in main
 * based on range in x direction and image size
 */
float yupper, ylower;

/* Distance between numbers */
float step;


/* Timing */
double walltime() {
    static struct timeval t;
    gettimeofday(&t, NULL);
    return (t.tv_sec + 1e-6 * t.tv_usec);
}

/* Given error code, return OpenCL error string */
const char *errorstr(cl_int err) {
	switch (err) {
	case CL_SUCCESS:                          return "Success!";
	case CL_DEVICE_NOT_FOUND:                 return "Device not found.";
	case CL_DEVICE_NOT_AVAILABLE:             return "Device not available";
	case CL_COMPILER_NOT_AVAILABLE:           return "Compiler not available";
	case CL_MEM_OBJECT_ALLOCATION_FAILURE:    return "Memory object allocation failure";
	case CL_OUT_OF_RESOURCES:                 return "Out of resources";
	case CL_OUT_OF_HOST_MEMORY:               return "Out of host memory";
	case CL_PROFILING_INFO_NOT_AVAILABLE:     return "Profiling information not available";
	case CL_MEM_COPY_OVERLAP:                 return "Memory copy overlap";
	case CL_IMAGE_FORMAT_MISMATCH:            return "Image format mismatch";
	case CL_IMAGE_FORMAT_NOT_SUPPORTED:       return "Image format not supported";
	case CL_BUILD_PROGRAM_FAILURE:            return "Program build failure";
	case CL_MAP_FAILURE:                      return "Map failure";
	case CL_INVALID_VALUE:                    return "Invalid value";
	case CL_INVALID_DEVICE_TYPE:              return "Invalid device type";
	case CL_INVALID_PLATFORM:                 return "Invalid platform";
	case CL_INVALID_DEVICE:                   return "Invalid device";
	case CL_INVALID_CONTEXT:                  return "Invalid context";
	case CL_INVALID_QUEUE_PROPERTIES:         return "Invalid queue properties";
	case CL_INVALID_COMMAND_QUEUE:            return "Invalid command queue";
	case CL_INVALID_HOST_PTR:                 return "Invalid host pointer";
	case CL_INVALID_MEM_OBJECT:               return "Invalid memory object";
	case CL_INVALID_IMAGE_SIZE:               return "Invalid image size";
	case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:  return "Invalid image format descriptor";
	case CL_INVALID_SAMPLER:                  return "Invalid sampler";
	case CL_INVALID_BINARY:                   return "Invalid binary";
	case CL_INVALID_BUILD_OPTIONS:            return "Invalid build options";
	case CL_INVALID_PROGRAM:                  return "Invalid program";
	case CL_INVALID_PROGRAM_EXECUTABLE:       return "Invalid program executable";
	case CL_INVALID_KERNEL_NAME:              return "Invalid kernel name";
	case CL_INVALID_KERNEL_DEFINITION:        return "Invalid kernel definition";
	case CL_INVALID_KERNEL:                   return "Invalid kernel";
	case CL_INVALID_ARG_INDEX:                return "Invalid argument index";
	case CL_INVALID_ARG_VALUE:                return "Invalid argument value";
	case CL_INVALID_ARG_SIZE:                 return "Invalid argument size";
	case CL_INVALID_KERNEL_ARGS:              return "Invalid kernel arguments";
	case CL_INVALID_WORK_DIMENSION:           return "Invalid work dimension";
	case CL_INVALID_WORK_GROUP_SIZE:          return "Invalid work group size";
	case CL_INVALID_WORK_ITEM_SIZE:           return "Invalid work item size";
	case CL_INVALID_GLOBAL_OFFSET:            return "Invalid global offset";
	case CL_INVALID_EVENT_WAIT_LIST:          return "Invalid event wait list";
	case CL_INVALID_EVENT:                    return "Invalid event";
	case CL_INVALID_OPERATION:                return "Invalid operation";
	case CL_INVALID_GL_OBJECT:                return "Invalid OpenGL object";
	case CL_INVALID_BUFFER_SIZE:              return "Invalid buffer size";
	case CL_INVALID_MIP_LEVEL:                return "Invalid mip-map level";
	default:                                  return "Unknown";
	}
}

/* Prints string for error code */
void clerror(char *s,cl_int err) {
	printf("%s: %s\n",s,errorstr(err));
	exit(1);
}

void error(char *s) {
	puts(s);
	exit(1);
}

/* Loads kernel source code from file */
char *load_program_source(const char *s) {
	FILE *f;
	char *t;
	size_t len;
	if(NULL==(f=fopen(s,"r"))) error("couldn't open file");
	fseek(f,0,SEEK_END);
	len=ftell(f);
	fseek(f,0,SEEK_SET);
	t=malloc(len+1);
	fread(t,len,1,f);
	t[len]=0;
	fclose(f);
	return t;
}

/* Calculate the number of iterations until divergence for each pixel.
 * If divergence never happens, return MAXITER
 */
void calculate(int* pixel) {
  for (int i = 0; i < XSIZE; i++) {
    for (int j = 0; j < YSIZE; j++) {
      complex_t c, z, temp;
      int iter = 0;
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
  }
}

/* Set up and call mandelbrot kernel */
void calculate_ocl(int* pixel /* Add any neccessary arguments */){

    // Allocate memory for result
	
	//err = clEnqueueWriteBuffer(q, pixels_device, CL_TRUE, 0, XSIZE*YSIZE*sizeof(float), pixel, 0, NULL, NULL);
	
	// Set kernel arguments
	
	
	// Set up block size

	// Launch kernel

    // Transfer results back to CPU

}

int main(int argc, char** argv) {
	cl_platform_id platform;
	cl_device_id device;
	cl_context context;
	cl_command_queue q;
	cl_program program;
	cl_kernel kernel;
	cl_int err;
	char *source;
	int i;
	
	/* Check input arguments */
    if (argc == 1) {
      puts("Usage: MANDEL n");
      puts("n decides whether image should be written to disk (1 = yes, 0 = no)");
      return 0;
    }
    
    /* Get any GPU device on the first platform */
	err = clGetPlatformIDs(1, &platform, NULL);
	if(CL_SUCCESS != err) clerror("Couldn't get platform ID", err);
	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	if(CL_SUCCESS != err) clerror("Couldn't get device ID", err);

	/* Create context on device */
	context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
	if(CL_SUCCESS != err) clerror("Couldn't get context", err);

	/* Create command queue on device */
	q = clCreateCommandQueue(context, device, 0, &err);
	if(CL_SUCCESS != err) clerror("Couldn't create command queue", err);

	/* Create a program object */
	source = load_program_source("mandel.cl");
	program = clCreateProgramWithSource(context, 1, (const char **)&source, NULL, &err);
	if(CL_SUCCESS != err) clerror("Error creating program",err);

	/* Build a program associated with a program object */
	err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
	if(CL_SUCCESS != err) {
		/* Show build log (aka compile errors) */
		static char s[1048576];
		size_t len;
		printf("Error building program: %s\n", errorstr(err));
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(s), s, &len);

		printf("Build log:\n%s\n", s);
		return 1;
	}

	/* Create kernel from built program */
	kernel = clCreateKernel(program, "mandelbrot", &err);
	if(CL_SUCCESS != err) clerror("Error creating kernel",err);
	free(source);
    
    
    /* Calculate the range in the y - axis such that we preserve the aspect ratio */
    step = (xright - xleft)/XSIZE;
    yupper = ycenter + (step * YSIZE)/2;
    ylower = ycenter - (step * YSIZE)/2;
  
    /* Global arrays for iteration counts/pixels
     * One array for the result of the CPU calculation,
     * one for the result of the GPU calculation.
     * (Both are in the host/CPU memory)
     */
    int* pixel_for_cpu = (int*) malloc(sizeof(int) * XSIZE * YSIZE);
    int* pixel_for_gpu = (int*) malloc(sizeof(int) * XSIZE * YSIZE);
  

    /* Perform calculation on CPU */
    double start_cpu = walltime();
    calculate(pixel_for_cpu);
    double end_cpu = walltime();
  
    /* Perform calculations on GPU */
    double start_gpu = walltime();
    calculate_ocl(pixel_for_gpu);
    double end_gpu = walltime();
  
    /* Compare execution times
     * The GPU time also includes the time for memory allocation and transfer
     */
    printf("CPU time: %f s\n" , (end_cpu-start_cpu));
    printf("GPU time: %f s\n" , (end_gpu-start_gpu));
  

    /* Output */
    if (strtol(argv[1], NULL, 10) != 0) {
        output(pixel_for_gpu);
    }
  
  	/* Clean up */
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(q);
    clReleaseContext(context);
  
    return 0;

}

/* Save 24 - bits bmp file, buffer must be in bmp format: upside - down */
void savebmp(char *name, uchar *buffer, int x, int y) {
  FILE *f = fopen(name, "wb");
  if (!f) {
    printf("Error writing image to disk.\n");
    return;
  }
  unsigned int size = x * y * 3 + 54;
  uchar header[54] = {'B', 'M',
                      size&255,
                      (size >> 8)&255,
                      (size >> 16)&255,
                      size >> 24,
                      0, 0, 0, 0, 54, 0, 0, 0, 40, 0, 0, 0, x&255, x >> 8, 0,
                      0, y&255, y >> 8, 0, 0, 1, 0, 24, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  fwrite(header, 1, 54, f);
  fwrite(buffer, 1, XSIZE * YSIZE * 3, f);
  fclose(f);
}

/* Given iteration number, set a colour */
void fancycolour(uchar *p, int iter) {
  if (iter == MAXITER);
  else if (iter < 8) { p[0] = 128 + iter * 16; p[1] = p[2] = 0; }
  else if (iter < 24) { p[0] = 255; p[1] = p[2] = (iter - 8) * 16; }
  else if (iter < 160) { p[0] = p[1] = 255 - (iter - 24) * 2; p[2] = 255; }
  else { p[0] = p[1] = (iter - 160) * 2; p[2] = 255 - (iter - 160) * 2; }
}

/* Create nice image from iteration counts. take care to create it upside down (bmp format) */
void output(int* pixel){
    unsigned char *buffer = (unsigned char*)calloc(XSIZE * YSIZE * 3, 1);
    for (int i = 0; i < XSIZE; i++) {
      for (int j = 0; j < YSIZE; j++) {
        int p = ((YSIZE - j - 1) * XSIZE + i) * 3;
        fancycolour(buffer + p, pixel[(i + XSIZE * j)]);
      }
    }
    /* write image to disk */
    savebmp("mandel2.bmp", buffer, XSIZE, YSIZE);
    free(buffer);
}
