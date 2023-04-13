// Run the file
// ./signal_handler4 0 6 "anuragB"
// ./signal_handler4 1 6 "anuragB"
// ./signal_handler4

// If the executable runs without arguments then a default string 
// is passed to sigqueue.
// If the executable runs with arguments then the first 0 or 1 
// will determine if sigqueue will send a load that is string or integer

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <features.h>
#include <signal.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <stddef.h>

using namespace std;
int signum;
pid_t pid, p, par_pid, cid;
int i,status;

//sighandler_t *oldHandler;
void (*previous_handler)(int);

union sigval payload;

void sig_handler(int signum) {
    fprintf(stderr,"SIG_HANDLER: Received Signal: %d in proc pid=%d\n",signum,getpid());
}

void sig_usr1(int signum,siginfo_t *info, void *ucontext) {
    int c;
    char* ptr_val;
    int int_val = -500000;
    ptr_val = NULL;
    ptr_val = (char *)malloc(50*sizeof(char));

    fprintf(stderr, "SIG_USR1: Received Signal: %d in proc pid=%d from proc=%lu\n", signum, getpid(), (unsigned long)info->si_pid);

    int_val = info->si_value.sival_int;
    
    if ((int_val > -100000)&&(int_val < 100000)) {
        fprintf(stderr, "CASE INTEGER: Received Signal: %d int_val: %d from proc=%lu\n", signum, int_val,(unsigned long)info->si_pid);
    }
    else if (info->si_value.sival_ptr!=NULL) {
        ptr_val = (char *)info->si_value.sival_ptr;
        fprintf(stderr, "CASE STRING: sigusr1 Recv Signal: %d ptr_val= %s from proc= %lu \n", signum, ptr_val, (unsigned long)info->si_pid);
    }
}

int main(int argc, char *argv[]) {
    int exit_status;
    i=0;
    sigset_t newmask;
    sigset_t oldmask;
    sigset_t waiting_mask, look_for_these_child, look_for_these_parent,hblock,hblock1;
    siginfo_t extra;
    ucontext_t *ucontext;

    struct timespec to;
    to.tv_sec = 200;
    to.tv_nsec = 0;

    struct sigaction new_action, new_action1, old_action, old_action1;
    const char* myString = "Hello how are you today?"; 
    //myString = (char*) malloc((strlen("Hello how are you today?")+1)*sizeof(char));

    memset(&new_action,0,sizeof(new_action));
    memset(&new_action1,0,sizeof(new_action1));

    new_action.sa_handler = sig_handler;
    sigemptyset(&hblock);
    // sigaddset(&hblock,SIGINT);
    // sigaddset(&hblock,SIGILL);
    sigaddset(&hblock,SIGQUIT);
    // sigaddset(&hblock,SIGFPE);
    // sigaddset(&hblock,SIGTSTP);
    new_action.sa_flags = 0;
    new_action.sa_mask = hblock;

    new_action1.sa_sigaction = sig_usr1;
    sigemptyset(&new_action1.sa_mask);
    new_action1.sa_flags = SA_SIGINFO;

    sigaction(SIGINT, NULL, &old_action);
    if (old_action.sa_handler!=SIG_IGN)
    sigaction(SIGINT,&new_action, NULL);
    sigaction(SIGILL, NULL, &old_action);
    if (old_action.sa_handler!=SIG_IGN)
    sigaction(SIGILL,&new_action, NULL);
    sigaction(SIGSTOP, NULL, &old_action);
    if (old_action.sa_handler!=SIG_IGN)
    sigaction(SIGSTOP,&new_action, NULL);
    sigaction(SIGQUIT, NULL, &old_action);
    if (old_action.sa_handler!=SIG_IGN)
    sigaction(SIGQUIT,&new_action, NULL);
    sigaction(SIGFPE, NULL, &old_action);
    if (old_action.sa_handler!=SIG_IGN)
    sigaction(SIGFPE,&new_action, NULL);
    sigaction(SIGTSTP,&new_action, NULL);
    sigaction(SIGCONT,&new_action, NULL);

    // sigaction(SIGUSR1, NULL, &old_action1);
    // if (old_action1.sa_handler!=SIG_IGN)

    sigaction(SIGUSR1,&new_action1, NULL); 
    // sigemptyset(&newmask); //No signals
    // sigemptyset(&oldmask); //No signals
    sigprocmask(SIG_SETMASK,&hblock,NULL);

    sigdelset(&hblock, SIGQUIT);
    sigaddset(&hblock,SIGFPE);
    sigaddset(&hblock, SIGILL);
    if (sigprocmask(SIG_BLOCK, &hblock, &oldmask)<0)
    fprintf(stderr,"SIG_BLOCK error");
    sleep(5);
    p = fork();

    if (p) {
        sigaddset(&hblock, SIGQUIT);
        sigprocmask(SIG_BLOCK,&hblock,0);
        fprintf(stderr,"Parent Process %d blocks signal SIQUIT\n",getpid());
        // sleep(30);
        
        memset(&payload,0,sizeof(payload));

        if (argc==1) {
            payload.sival_ptr = (void*)myString;
            // strcpy(payload.sival_ptr,myString);
            // payload.sival_ptr = myString; 
            //if (argc==1) payload.sival_int=25;
        }

        if ((argc>1)&&(atoi(argv[1])==1)) 
            payload.sival_ptr = argv[3]; 
        if ((argc>1)&&(atoi(argv[1])==0)) 
            payload.sival_int = atoi(argv[2]);

        if (sigqueue(p,SIGUSR1, payload)>=0) {
            if ((argc>1)&&(atoi(argv[1])==0)) 
            fprintf(stderr,"SIGQUEUE sent successfully: payload_int = %d\n",payload.sival_int);
        if ((argc>1)&&(atoi(argv[1])==1)) 
            fprintf(stderr,"SIGQUEUE sent successfully: payload_ptr = %s\n",(char *)payload.sival_ptr); 
        if (argc==1) 
            fprintf(stderr,"SIGQUEUE sent successfully: payload_ptr = %s\n",(char *)payload.sival_ptr);
        }

        kill(p,SIGUSR1);
        sleep(5);
        kill(p,SIGCONT);
        sleep(5);
        kill(p, SIGFPE);
        sleep(5);
        kill(p, SIGILL);
        sleep(5);
        raise(SIGQUIT);
        sleep(10);
        sigpending(&waiting_mask);
        if (sigismember(&waiting_mask,SIGINT))
            fprintf(stderr,"User tried to kill parent\n");
        if (sigismember(&waiting_mask,SIGQUIT)) 
            fprintf(stderr,"SIGQUIT was send while parent blocked\n");
        sleep(10);
        wait(&status);
        if ( WIFEXITED(status) ) 
        { 
            exit_status = WEXITSTATUS(status); 
            fprintf(stderr,"Exit status of the child was %d\n", exit_status); 
        } 

        fprintf(stderr,"Child process is done with status=%d\n",status);
        fprintf(stderr,"Parent process = %d is blocking signals: SIGINT, SIGILL, SIGQUIT \n", getpid());
        fprintf(stderr,"Check to see if there are pending signals now: signals that were sent while being blocked\n"); 

        sigpending(&waiting_mask);
        sleep(10);
        if (sigismember(&waiting_mask,SIGINT)) 
            fprintf(stderr,"User tried to kill parent with SIGINT\n");
        if (sigismember(&waiting_mask,SIGQUIT)) 
            fprintf(stderr,"SIGQUIT was send while parent blocked\n"); 
        if (sigismember(&waiting_mask,SIGILL)) 
            fprintf(stderr,"User tried to SIGILL parent\n");
        fprintf(stderr,"Check if we only block SIGABRT now\n");
        sigemptyset(&look_for_these_parent);
        sigaddset(&look_for_these_parent, SIGABRT);

        sigprocmask(SIG_BLOCK,&look_for_these_parent,&new_action.sa_mask);
 
        sigpending(&waiting_mask);
        sleep(5);

        if (sigismember(&waiting_mask,SIGINT)) 
            fprintf(stderr,"User tried to kill parent with SIGINT\n");
        if (sigismember(&waiting_mask,SIGQUIT)) 
            fprintf(stderr,"SIGQUIT was send while parent blocked\n"); 
        if (sigismember(&waiting_mask,SIGILL)) 
            fprintf(stderr,"User tried to SIGILL parent\n");
        if (sigismember(&waiting_mask,SIGABRT)) 
            fprintf(stderr,"User tried to kill parent with SIGABRT\n");
        
        fprintf(stderr,"Now we have unblocked everything\n");
        sigdelset(&new_action.sa_mask,SIGINT);

        sigprocmask(SIG_UNBLOCK,&look_for_these_parent,&new_action.sa_mask);
 
        sleep(10);
        sigpending(&waiting_mask);
        sleep(5);
        if (sigismember(&waiting_mask,SIGINT)) 
        fprintf(stderr,"User tried to kill parent with SIGINT\n");
        if (sigismember(&waiting_mask,SIGQUIT)) 
        fprintf(stderr,"SIGQUIT was send while parent blocked\n"); 
        if (sigismember(&waiting_mask,SIGILL)) 
        fprintf(stderr,"User tried to SIGILL parent\n");
        if (sigismember(&waiting_mask,SIGABRT)) 
        fprintf(stderr,"User tried to kill parent with SIGABRT\n");
        exit(0);
    }

    if (!p) {
        int sigreceived;

        sigaddset(&hblock, SIGFPE);
        sigprocmask(SIG_BLOCK,&hblock,&oldmask);
        fprintf(stderr,"Child Process %d blocks signal SIGFPE\n",getpid());
        
        sigemptyset (&look_for_these_child);
        sigaddset(&look_for_these_child, SIGSEGV);
        sigaddset(&look_for_these_child, SIGTSTP);
        sigprocmask(SIG_BLOCK,&look_for_these_child,NULL);

        fprintf(stderr,"Child Process %d blocks signal SIGSEGV\n",getpid());
        sleep(10);
        kill(getppid(), SIGQUIT);
        sleep(3);
        kill(getppid(), SIGFPE);
        sleep(3);
        kill(getppid(), SIGTSTP);
        sleep(3);
        raise(SIGFPE);
        sleep(10);

        fprintf(stderr,"Check for pending signals: which signals are sent while they were blocked in main\n");

        sigpending(&waiting_mask);
        if (sigismember(&waiting_mask,SIGINT)) 
            fprintf(stderr,"User tried to kill child process by sending SIGINT\n");
        if (sigismember(&waiting_mask,SIGFPE)) 
            fprintf(stderr,"SIGFPE was send while child blocked it\n");

        fprintf(stderr,"Waiting to see if any of the signals of look_for these child which are blocked, are being sent to the process: SIGSEGV\n"); 
        sigreceived = 
        sigwaitinfo(&look_for_these_child,&extra); // blocking wait ....

        // sigreceived = sigtimedwait(&look_for_these_child,&extra,&to);
        if (sigreceived<0) {
            fprintf(stderr,"faulty execution of sigwaitinfo\n");
        }
        else {
            // WHEN YOU COMPILE THE PROGRAM YOU WILL NOTICE THAT STRUCT EXTRA GIVES YOU NULL FOR PAYLOAD 
            // THIS IS CORRECT FOR SIGNAL SIGSEGV because this signal is NOT ASSOCIATED TO A HANDLER THAT USES SIG_INFO
            // SO IN THIS CASE EXTRA STRUCT DOES NOT OBTAIN A PAYLOAD FROM SIGINFO. ONLY SIGUSR is ASSOCIATED WITH A HANDLER
            // THAT USES SIGINFO. YOU CAN MAKE CHANGES TO THE PROGRAM AND MODIFY THAT TO BLOCK A SIGNAL ASSOCIATED WITH ANOTHER
            // HANDLER THAT USES SIG_INFO STRUCT (THE STRUCT OF THE OS). Only then will you get payload from having called sigwaitinfo. 

            if ((argc>1)&&(atoi(argv[1])==0)) 
                fprintf(stderr,"RECEIVED SIGNAL %d: with payload_int = %d\n",sigreceived, extra.si_value.sival_int);
                    if ((argc>1)&&(atoi(argv[1])==1)) 
                fprintf(stderr,"RECEIVED SIGNAL %d with payload_ptr = %s\n",sigreceived, (char *)extra.si_value.sival_ptr);
                    if (argc==1) fprintf(stderr,"RECEIVED SIGNAL %d: with payload_ptr = %s\n",sigreceived, (char *)extra.si_value.sival_ptr);
        }

        sleep(30);
        if (sigprocmask(SIG_SETMASK, &oldmask,NULL)<0)
            fprintf(stderr,"SIG_SETMASK error\n");
            fprintf(stderr,"SIGINT, SIGILL, SIGSTOP unblocked\n");
            sleep(20);
            exit(5);
        }

         /*
        if (sigprocmask(SIG_SETMASK, &oldmask,NULL)<0)
            fprintf(stderr,"SIG_SETMASK error\n");
            
            fprintf(stderr,"SIGINT, SIGILL, SIGSTOP 
            unblocked\n");
            sleep(5);
        */
        
        return (0);
    }
