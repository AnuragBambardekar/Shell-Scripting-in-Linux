#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	  // Validate program arguments.
	  if (argc < 3) 
	  {
	    printf("Usage: %s file_in_1 file_in_2 [file_out (default:myfile.out)]\n", argv[0]); //name of the program is obtained from the argv[0] argument.
	    exit(1); //Exit with status code 1
	  }

	  // Open two files to be merged.
	  FILE *fp1 = fopen(argv[1], "r");
	  FILE *fp2 = fopen(argv[2], "r");

	  // Define third file, to be written to.
	  FILE *fp3;
	  // Open third file to store the result.
	  // If only given 2 input filenames, third filename is "myfile.out" by default.
	  // If given 3 input filenames, third filename is the final argv.
	  if (argc == 3) 
	  {
	    fp3 = fopen("myfile.out", "w");
	  } else 
	  {
	    fp3 = fopen(argv[3], "w");
	  }

	  // Ensure all files exist.
	  if (fp1 == NULL || fp2 == NULL || fp3 == NULL) //circuit breaker, if first condition equates to false, then statements within if are executed
	  {
	    printf("No such file or directory.\n");
	    exit(0); //Exit with status code 1
	  }

	  // Copy contents of first file to file3.txt.
	  char ch;
	  while ((ch = fgetc(fp1)) != EOF) // loop continues until the end-of-file
	  {
	    fputc(ch, fp3);
	  }

	  // Copy contents of second file to file3.txt.
	  while ((ch = fgetc(fp2)) != EOF) // loop continues until the end-of-file
	  {
	    fputc(ch, fp3);
	  }

	  // Close all files.
	  fclose(fp1);
	  fclose(fp2);
	  fclose(fp3);
	  return 0;
}
