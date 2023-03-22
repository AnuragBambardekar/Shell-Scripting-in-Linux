#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <features.h> 

// Testing Options of waitpid()
// Killing processes from another terminal

// ./revpr5_WNOHANG 9
// exited naturally, status = 9

// ./revpr5_WNOHANG 6 /bin/ls ls ./
// exited naturally, status = 0

// ./revpr5_WNOHANG 1
// killed

// i modified the wait.h and waitstatus.h file
 
int main(int argc, char *argv[])  { 
        int i, status,parent_PID; 
        pid_t pid, cpid, w; 
        unsigned long j; 
        cpid = fork(); 
  
        if (cpid<0)   {  // error occured 
            fprintf(stderr,"Fork Failed"); 
            return 1; 
        } 
        else if (cpid ==0)   {  
            //child process 
            fprintf(stderr,"Child PID is %ld\n", (long) getpid()); 
            if (argc ==2) { 
                 
                for (j=0; j<5000;j++) 
                    j+=1; 
                fprintf(stderr,"j =%lu \n",j); 
              
                sleep(50); 
 
                fprintf(stderr,"Child with pid=%d tests the flags of waitpid in parent: who finishes first?\n",getpid()); 
  
                exit(atoi(argv[1])); 
            } 
 
            if (argc > 2)  { 
                fprintf(stderr,"argv[2]=%s\n",argv[2]); 
                fprintf(stderr,"argv[3]=%s\n",argv[3]); 
                fprintf(stderr,"argv[4]=%s\n",argv[4]); 
            if (execlp(argv[2],argv[3],argv[4],NULL)<0) 
                fprintf(stderr,"EXECLP must have errors: Any news from the child?\n"); 
            } 
        }
        else {    
            //parent process 
            fprintf(stderr,"Parent PID is %ld\n",(long)getpid()); 
            parent_PID = getpid(); 
                 
            // fprintf(stderr,"Let the parent process sleep for sometime - so that it can accept signals\n"); 
            // sleep (30); // if commented, child process output inter-mingled with parent output
         
            do { 
                /* waitpid returns  -1 if unsuccessful, or  0 if no child changed status or w=child pid when child changed status */ 
                
                /**
                 * According to the POSIX standard, the WCONTINUED flag is optional and may not be supported by all operating systems. 
                 * If your operating system does not support the WCONTINUED flag, you can try using the WUNTRACED flag instead, which is required by the 
                 * POSIX standard and should be supported by all operating systems that implement the waitpid() function.
                */
                w = waitpid(cpid, &status, WUNTRACED);  // WCONTINUED
                // w = waitpid(cpid, &status, WNOHANG);                         
 
                fprintf(stderr,"I am parent with pid = %d and my child has pid =%d  and status=%d  and w=%d --- waitpid flags?\n", getpid(),cpid, status, w); 
 
                if (w==-1) { 
                    fprintf(stderr,"Flag WNOHANG has caused abrupt exit of the parent?\n"); 
                    exit(0); 
                } 
                          
                if  (w) { 
                    if (WIFEXITED(status)) { 
                        fprintf(stderr,"exited naturally, status = %d\n", WEXITSTATUS(status)); 
                    }  
                    else if (WIFSIGNALED(status)) { 
                        fprintf(stderr,"killed by signal: %d\n",WTERMSIG(status)); 
                    }  
                    else if (WIFSTOPPED(status)) { 
                        fprintf(stderr,"stopped by signal %d\n",WSTOPSIG(status)); 
                    }  
                    else if (WIFCONTINUED(status)) { 
                        fprintf(stderr,"continued\n"); 
                    } 
                } 
 
            }  while ((!w)||((!WIFEXITED(status)) && (!WIFSIGNALED(status)))); 
                
            fprintf(stderr,"Has the parent reached this state prematurely?\n"); 
            exit(1); 
        } 
} 