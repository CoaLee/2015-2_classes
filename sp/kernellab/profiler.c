/*****************************************************************************
 *
 * M1522.000800
 * SYSTEM PROGRAMMING
 * 
 * Lab2. Kernel Lab
 *
 * profiler.c
 *  - fork a binary & profile it.
 *
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "chardev.h"

/*****************************************************************************
 * MsrInOut Command Arrays
 */

void mystrcpy(char*, const char*);

struct MsrInOut msr_init[] = {
  { MSR_WRITE, PERF_GLOBAL_CTRL, 0x00, 0x00 },  // disable 4 PMCs & 3 FFCs
  { MSR_WRITE, PMC0, 0x00, 0x00 },              // PMC0 initialization
  { MSR_WRITE, PMC1, 0x00, 0x00 },              // PMC1 initialization
  { MSR_WRITE, PMC2, 0x00, 0x00 },              // PMC2 initialization
  { MSR_WRITE, PMC3, 0x00, 0x00 },              // PMC2 initialization
  { MSR_WRITE, FFC0, 0x00, 0x00 },              // PMC2 initialization
  { MSR_WRITE, FFC1, 0x00, 0x00 },              // PMC2 initialization
  { MSR_WRITE, FFC2, 0x00, 0x00 },              // PMC2 initialization
  { MSR_STOP, 0x00, 0x00 }
};

struct MsrInOut msr_set_start[] = {
	{ MSR_WRITE, PERF_GLOBAL_CTRL, 0x00, 0x00 },       
	{ MSR_WRITE, PMC0, 0x00, 0x00 },        
	{ MSR_WRITE, PMC1, 0x00, 0x00 },        
	{ MSR_WRITE, PMC2, 0x00, 0x00 },        
	{ MSR_WRITE, PMC3, 0x00, 0x00 },        
	{ MSR_WRITE, FFC0, 0x00, 0x00 },       
	{ MSR_WRITE, FFC1, 0x00, 0x00 },       
	{ MSR_WRITE, FFC2, 0x00, 0x00 },       
	{ MSR_WRITE, PERF_EVT_SEL0, 0x004101c2, 0x00 }, 
	{ MSR_WRITE, PERF_EVT_SEL1, 0x0041010e, 0x00 }, 
	{ MSR_WRITE, PERF_EVT_SEL2, 0x01c1010e, 0x00 }, 
	{ MSR_WRITE, PERF_EVT_SEL3, 0x004101a2, 0x00 }, 
	{ MSR_WRITE, FIXED_CTR_CTRL, 0x222, 0x00 },      
	{ MSR_WRITE, PERF_GLOBAL_CTRL, 0x0f, 0x07 },     
	{ MSR_STOP, 0x00, 0x00 }
};

struct MsrInOut msr_stop_read[] = {
  { MSR_WRITE, PERF_GLOBAL_CTRL, 0x00, 0x00 },  // disable 4 PMCs
  { MSR_WRITE, FIXED_CTR_CTRL, 0x00, 0x00 },    // clean up FFC ctrls
  { MSR_READ, PMC0, 0x00 },
  { MSR_READ, PMC1, 0x00 },
  { MSR_READ, PMC2, 0x00 },
  { MSR_READ, PMC3, 0x00 },
  { MSR_READ, FFC0, 0x00 },
  { MSR_READ, FFC1, 0x00 },
  { MSR_READ, FFC2, 0x00 },
  { MSR_STOP, 0x00, 0x00 }
};

struct MsrInOut msr_rdtsc[] = {
  { MSR_RDTSC, 0x00, 0x00 },
  { MSR_STOP, 0x00, 0x00 }
};

/*****************************************************************************
 * Process tree variable
 */

struct PtreeInfo ptree;

/*****************************************************************************
 * Functions
 */

static int load_device() {
  int fd;
  fd = open("/dev/"DEVICE_NAME, 0);
  if (fd < 0) {
    perror("Failed to open /dev/" DEVICE_NAME);
    exit(1);
  }
  return fd;
}

static void close_device(int fd) {
  int e;
  e = close(fd);
  if (e < 0) {
    perror("Failed to close fd");
    exit(1);
  }
}

void print_header(char *name) {
  printf("-----------------------------------------------------------\n");
  printf("TARGET : %s\n", name);
  printf("\n");
}

void print_ptree(void) {
  int i, j, k;
  int cnt = (int)ptree.names[0][0];
  printf("- Process Tree Information\n\n");

  for(i=cnt-1,j=0,k=0; i>0; i--){
    for(;j<k;j++){
	   printf("  ");
		if(k-1==j) printf("ㄴ");
    }
	 printf("%s\n", ptree.names[i]);
	 j=0;
	 k++;
  }
}

void print_profiling(long long nr_inst, long long st_cy, long long cy) {
  double d_nr_inst, d_st_cy, d_cy;

  printf("\n- Process Mornitoring Information\n\n");
  printf("inst retired :   %20llu\n", nr_inst);
  printf("stalled cycles : %20llu\n", st_cy);
  printf("core cycles :    %20llu\n\n", cy);

  d_nr_inst = (double) nr_inst;
  d_st_cy = (double) st_cy;
  d_cy = (double) cy;

  printf("stall rate :     %20lf %%\n", 100 * d_st_cy/d_cy);
  printf("throughput :     %20lf inst/cycles\n", d_nr_inst/d_cy);
  printf("-----------------------------------------------------------\n");
}

int getPtree(int fd, int pid) {
  int ret_val;
  
  ret_val = ioctl(fd, IOCTL_GET_PTREE, &ptree);

  if(ret_val<0){
    printf("ioctl_get_ptree failed:%d\n", ret_val);
	 exit(-1);
  }

  return -1;
}

int getProfiling(int fd, char **argv) {
  int pid;
  ioctl(fd, IOCTL_MSR_CMDS, (long long)msr_init);
  ioctl(fd, IOCTL_MSR_CMDS, (long long)msr_set_start);
  
  if((pid=fork())==0) {
    execvp(argv[1], argv);
	 return -1;
  } else {
    waitpid(-1, NULL, 0);
  }

  ioctl(fd, IOCTL_MSR_CMDS, (long long)msr_stop_read);

  return 0;
}

int main(int argc, char* argv[]) {
  int pid;
  int fd;
  char binarypath[20]={'/','.'};

  if (argc <= 1 || argc > 2) {
    printf("Usage : %s {binary}\n", argv[0]);
    return 0;
  }
 
  fd = load_device();

  print_header(argv[1]);

  pid=fork();
  if(pid==0) {
	 getPtree(fd, pid);
    print_ptree();

	 return 0;
  }
  waitpid(-1,NULL,0);  
  
  ioctl(fd, IOCTL_MSR_CMDS, (long long)msr_init);
  ioctl(fd, IOCTL_MSR_CMDS, (long long)msr_set_start);
  
  if((pid=fork())==0){
	 mystrcpy(&binarypath[2], argv[1]);
	 execvp(binarypath, argv);
	 return -1;
  } else { 
    waitpid(-1,NULL,0);
  }

  ioctl(fd, IOCTL_MSR_CMDS, (long long)msr_stop_read);
  print_profiling(msr_stop_read[6].value, msr_stop_read[4].value, msr_stop_read[7].value);
 

  close_device(fd);

  return 0;
}

void mystrcpy(char *dest, const char *src){
  int i;

  for(i=0; src[i]!='\0'; i++) dest[i]=src[i];
  
  return;
}
