/******************************************************************
 * time_branches.c
 *
 * Time loops containing various branch patterns.
 *
 * (C) 2016 Zachary Kurmas
 ******************************************************************/

#include "timingos.h"
#include "util.h"
#include "vga.h"

// The length of the branch pattern. (The total stack size must be < 65536)
#define SIZE 4096 

const int WARM_UP = 500; // Number of "warm up" iterations
const int ITERS = 5000;  // The number of sample measurements taken

typedef enum {
  ALWAYS, NEVER, RANDOM
} pattern_type;

terminal_section_t debug;

///////////////////////////////////////////////////////////////////
//
// Time a loop containing the given branch pattern
//
///////////////////////////////////////////////////////////////////
unsigned long time_branch(char pattern[], int pattern_length) {

  // Intel CPUs count the cycles as they go by.
  // rdtsc() returns the "number" of the current cycle.
  uint32_t start = rdtsc(); 

  // We need to give the if/else statement something "useful"
  // to do so that the compiler doesn't optimize the branch away.
  // Make sure both the "if" and "else" parts of the code 
  // take the same number of cycles.
  long sum = 433;
  for (int i = 0; i < pattern_length; i++) {
    if (pattern[i] %2 == 0) {
      sum *= 5910;
      sum += 17;
    } else {
      sum *= 4317;
      sum += 19;
    }
  }
  uint32_t end = rdtsc();

  // Returns the # of cycles elapsed since the first call to rdtsc()
  // The counter may wrap around during the test; but, fortunately,
  // the math works out such that the difference is still the elapsed
  // time.
  return end - start; 
} // end time_branch



///////////////////////////////////////////////////////////////////
//
// Repeat time_branches ITER times and report the results
//
/////////////////////////////////////////////////////////////////

void repeat_measurements (pattern_type pattern_id, 
			  terminal_section_t* head, 
			  terminal_section_t* body) {

  // TODO: Set up an array to hold the branch pattern.
  // (Notice that we aren't timing this part.)
  char pattern[SIZE];

  
  for(int i = 0; i < SIZE; i++){
	if(pattern_id == ALWAYS){
		pattern[i] = 1;
	} else if(pattern_id == NEVER){
		pattern[i] = 0;
	} else {
		pattern[i] = random()%2;
	}

  }
  // TODO: "Warm up" the branch table (call time_branch without collecting data)
  for(int i =0; i < WARM_UP; i++){
  	time_branch(pattern, SIZE);
  }
  // Repeatedly time the loop and keep statistics

  unsigned long long sum = 0;
  unsigned long long sum_sq = 0;
  unsigned long long max = 0;
  unsigned long long variance = 0;
  unsigned long long min = (unsigned long) -1;

  for (int i = 1; i <= ITERS; i++) {
    unsigned long long value  =time_branch(pattern, SIZE);
   // TODO Collect data
   //if(i > (ITERS / 2){
   sum += value;

   sum_sq += value * value;

   variance = ( (sum_sq - ((sum * sum)) / i) )/(i);
   
   if(value > max){
	max = value;
   }
	
   if(value < min){
	min = value;
   }
	
    // Put the last line of output in the head
    terminal_section_t* output = (i == ITERS) ? head : body;

    terminal_write_unsigned(output, i, ": cur=");
    terminal_write_unsigned(output, value, ", avg=");
    terminal_write_unsigned(output, (sum / i), ", min=");
    terminal_write_unsigned(output, min, ", max=");
    terminal_write_unsigned(output, max, ", var=");
    terminal_write_unsigned(output, variance, "\n");
    //}
  } // end for
} // end time_many_branches
  


/////////////////////////////////////////////////////////////////
//
// This is the entry point for our "timingOS"
//
///////////////////////////////////////////////////////////////////
void bp_kernel_main() {
  randomize(0);
  
  /* Initialize terminal interface.  This function must always be
     called.  It sets up the text buffer. 
  */
  terminal_initialize();  

  /* Now, set up the individual text areas.  The code here sets up 
     two text areas:  The first four lines of the screen are labeled
     "head", the remainder of the screen is labeled "body"
  */
  terminal_section_t head, body;
  int debug_window_height = 1;
  terminal_section_initialize(&head, 0, 4, 0, VGA_WIDTH-1);
  terminal_section_initialize(&body, 5, VGA_HEIGHT-debug_window_height -1, 
			      0, VGA_WIDTH-1);
  terminal_section_initialize(&debug, VGA_HEIGHT-debug_window_height, 
			      VGA_HEIGHT-1,
			      0, VGA_WIDTH-1);
  terminal_write_string(&head, "Branch Timing:\n");
  terminal_write_string(&body, "Data Section:\n");

  terminal_write_string(&head, "Always: ");
  repeat_measurements(ALWAYS, &head, &body);

  terminal_write_string(&head, " Never: ");
  repeat_measurements(NEVER, &head, &body);

  terminal_write_string(&head, "Random: ");
  repeat_measurements(RANDOM, &head, &body);
  
}

