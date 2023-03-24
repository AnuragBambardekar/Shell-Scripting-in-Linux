#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

// Program that generates two pipes in the parent and child process and tests the exchange of messages with various modes
// wait/waitpid accept a status integer that is 0-255, where 255 is the maximum.

int main() {
   int pipefds1[2], pipefds2[2];
   int returnstatus1, returnstatus2;
   pid_t pid;
   int status;
   int ret,ret2;
   char pipe1writemessage[16] = "Hello from Pipe1";
   char pipe2writemessage[16] = "Hello from Pipe2";
   char readmessage[20];
   char *ptr;   
   char str[10];

   //Pipes are unidirectional

   //Create pipe1
   returnstatus1 = pipe(pipefds1);   
   if (returnstatus1 == -1) {
      printf("Unable to create pipe 1 \n");
      perror("Pipefsd1 could not be created\n");
      return(1);
   }

   //Create pipe2
   returnstatus2 = pipe(pipefds2);
   if (returnstatus2 == -1) {
      printf("Unable to create pipe 2 \n");
      perror("Pipefsd2 could not be created\n\n");
      return(1);
   }

   //Create fork
   pid = fork();
   
   if (pid != 0)    {  // Parent process 
      close(pipefds1[0]); // Close the unwanted pipe1 read side
      close(pipefds2[1]); // Close the unwanted pipe2 write side

      //Write operation
      printf("In Parent: Writing to pipe 1 – Message is: %s\n", pipe1writemessage);
      ret = write(pipefds1[1], pipe1writemessage, strlen((pipe1writemessage)));
      // sizeof includes '\0' in length calculation
      // strlen() takes a null terminated byte string str as its argument and returns its length. The 
      // length does not include the null character
      /**
       * char a[] = "hello\0\0";
       * // sizeof(a) = 8 and strlen(a) = 5;
      */


      // ret = write(pipefds1[1],pipe1writemessage, sizeof(pipe1writemessage)); // In child  there is mismatch  in read=16, it did not match write=20
      if (ret<0) perror("In parent: Write Failed!");
      else  printf("Parent has written %d bytes\n\n",ret);
  
      //Read operation
      ret=read(pipefds2[0], readmessage, sizeof(readmessage));
      printf("In Parent: Reading from pipe 2 – Message is %s\n", readmessage);
      if (ret<0) perror("In parent: Read Failed!");
      if  (ret!=sizeof(readmessage)) printf("In parent there is mismatch  in read =%d, it did not match write=%ld\n",ret,sizeof(readmessage));
      else printf("Parent has read %d bytes\n\n",ret);  

      ret = wait(&status);
      if (ret) 
         if (WIFEXITED(status)) {
               fprintf(stderr,"Exited Naturally  with status=%d\n",WEXITSTATUS(status)); 
        }
   } 

   else { //child process
      close(pipefds1[1]); // Close the unwanted pipe1 write side
      close(pipefds2[0]); // Close the unwanted pipe2 read side
      
      //Read operation
      ret=read(pipefds1[0], readmessage, sizeof(readmessage));
      // ret=read(pipefds1[0], readmessage, strlen(readmessage));
      if (ret<0) perror("In child: Read failed!");
      else printf("In Child: Reading from pipe 1 – Message is: %s\n", readmessage);

      //Write operation
      printf("In Child: Writing to pipe 2 – Message is: %s\n", pipe2writemessage);
      write(pipefds2[1], pipe2writemessage, strlen((pipe2writemessage)));
      if (ret<0)  perror("In child: Write Failed!");
      if (ret!=sizeof(readmessage)) printf("In child  there is mismatch  in read=%d, it did not match write=%ld\n",ret,sizeof(readmessage));
         //  status=ret;
         printf("readmessage=%s\n\n",readmessage); 
         strcpy(str,"1234");
         status=strtol(str, &ptr,10);
         status=ret*atoi(str);
         // status=atoi(readmessage);
         fprintf(stderr,"CHILD STATUS = %d\n",status%255);
         exit(status%255);  
   }
   return(0);
 }
