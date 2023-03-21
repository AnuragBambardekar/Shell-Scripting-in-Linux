# Basic Linux Commands

```man``` <br>
man - an interface to the system reference manuals

```lsb_release```
```
anuragb@anuragb:~/ECE434/Basic_Linux_Commands$ lsb_release -a
No LSB modules are available.
Distributor ID:	Ubuntu
Description:	Ubuntu 22.04.2 LTS
Release:	22.04
Codename:	jammy

```
lsb_release - print distribution-specific information

```htop``` <br>
htop - interactive process viewer <br>
htop -u anuragb [see my processes]

```pwd```  <br>
Print working Directory
```
anuragb@anuragb:~/ECE434/Basic_Linux_Commands$ pwd
/home/anuragb/ECE434/Basic_Linux_Commands
```

```ls``` <br>
ls - list directory contents

```
anuragb@anuragb:~/ECE434/Basic_Linux_Commands$ ls
Basic_Commands.md

```
ls –al // can see hidden files as well <br>
ls –lt // list my files but in a sorted fashion, in the order of time <br>
ls –lt –reverse // list my files but in a sorted fashion, in the reverse order of time <br>

```cd``` <br>
Change directory
```
anuragb@anuragb:~/ECE434$ cd Basic_Linux_Commands/
anuragb@anuragb:~/ECE434/Basic_Linux_Commands$ 
```

```ps``` <br>
List processes 
```
anuragb@anuragb:~/ECE434/Basic_Linux_Commands$ ps
    PID TTY          TIME CMD
   3964 pts/2    00:00:00 bash
   4014 pts/2    00:00:00 ps

```
ps –el <br>
ps –al // can see hidden processes as well <br>
ps –u //can see my own processes, coming from user anuragb <br>
ps –aux // show only my own processes, and not only those attached to the current terminal <br>
ps -e --forest // which processes fork to other processes <br>

a = show processes for all users <br>
u = display the process's user/owner <br>
x = also show processes not attached to a terminal <br>

```kill``` <br>
kill -9 no_of_process //kill this process

```
anuragb@anuragb:~/ECE434/in_class$ kill -l
 1) SIGHUP	 2) SIGINT	 3) SIGQUIT	 4) SIGILL	 5) SIGTRAP
 6) SIGABRT	 7) SIGBUS	 8) SIGFPE	 9) SIGKILL	10) SIGUSR1
11) SIGSEGV	12) SIGUSR2	13) SIGPIPE	14) SIGALRM	15) SIGTERM
16) SIGSTKFLT	17) SIGCHLD	18) SIGCONT	19) SIGSTOP	20) SIGTSTP
21) SIGTTIN	22) SIGTTOU	23) SIGURG	24) SIGXCPU	25) SIGXFSZ
26) SIGVTALRM	27) SIGPROF	28) SIGWINCH	29) SIGIO	30) SIGPWR
31) SIGSYS	34) SIGRTMIN	35) SIGRTMIN+1	36) SIGRTMIN+2	37) SIGRTMIN+3
38) SIGRTMIN+4	39) SIGRTMIN+5	40) SIGRTMIN+6	41) SIGRTMIN+7	42) SIGRTMIN+8
43) SIGRTMIN+9	44) SIGRTMIN+10	45) SIGRTMIN+11	46) SIGRTMIN+12	47) SIGRTMIN+13
48) SIGRTMIN+14	49) SIGRTMIN+15	50) SIGRTMAX-14	51) SIGRTMAX-13	52) SIGRTMAX-12
53) SIGRTMAX-11	54) SIGRTMAX-10	55) SIGRTMAX-9	56) SIGRTMAX-8	57) SIGRTMAX-7
58) SIGRTMAX-6	59) SIGRTMAX-5	60) SIGRTMAX-4	61) SIGRTMAX-3	62) SIGRTMAX-2
63) SIGRTMAX-1	64) SIGRTMAX	
anuragb@anuragb:~/ECE434/in_class$ 

```

```whoami``` <br>
```
anuragb@anuragb:~/ECE434/Basic_Linux_Commands$ whoami
anuragb
```

```cd /``` (brings you to the root directory – topmost directory of your system) <br>

```~/ ```is shorthand for the current user's home folder. So if your user name is "foobar" it expands to /home/foobar/

```cd ~bob``` takes you to the home of the account named bob

If you type cd followed by nothing, cd will change the working directory to your home directory. 

```cal ``` displays calendar <br>
But first, install ncal package <br>
sudo apt install ncal <br>

```ls -all```, with a single hyphen, is the same as ls -a -l -l, which is    the same as ls -a -l, which is the same as ls -al. <br>

A single - introduces short options, which are single characters, and can be combined. Two -s introduce long options, which are words (or multiple words) and can’t be combined. <br>

In the ls -a output, . represents the current directory, and .. represents the parent directory. <br>

## Create, Delete, Copy from file to file and to/from directories
```mkdir``` <br>
```rm file``` //remove a file in the working directory <br>
```rm –r dir``` //remove all files in directory dir <br>
```mv file1 file2``` // move file1 to file2 <br>
```cp file1 file2``` // copy file1 to file2 <br>
```cp –r dir1 dir2``` <br>

```touch``` (create new file) <br>
```nano file``` (edit and Ctrl-X to exit) <br>
```head file``` (//output the first part of a file, in particular the first 10 lines of text within a file) <br>
```tail file``` (//display the last part of a file, in particular the last 10 lines of text within a file) <br>
```more file``` (//output the context of the file page by page)
```grep “pattern” file``` (search for pattern in file) <br>
```grep –r “pattern” dir``` (search recursively for pattern in dir) <br>
-r (means: recursively) <br>
-n (means: display the line number) <br>

For example, suppose I want to find all of the running instances of a program called ```zsh```. I could use ps to find the list of all of my processes. However, this list could be massive, and scrolling through it all would be a pain. I could combine it with grep to search through the results of ps and display only what I am looking for: ```ps | grep zsh```

```cat > file``` //creates a new file <br>
```
To exit the prompt and write the changes to the file, hold the Ctrl key and press d 
```
```cat file``` //outputs / concatenates file contents <br>
```cat file | more``` //If file having large number of content that won’t fit in output terminal and screen scrolls up very fast, we can use parameters more <br>
```cat –n file``` //With -n option you could see the line numbers of a file in the output terminal. <br>
```cat file > test``` //We can redirect standard output of a file into a new file [**overwrites existing content**]<br>
```cat file >> test``` // **Appends** in existing file with ‘>>‘ double greater than symbol. Here, contents of test file will be appended at the end of test1 file. <br>
```cat test1 test2 test3 > test4``` //This will create a file called test4 and all output will be redirected in a newly created file <br>

```
Instead of displaying the contents of a file on the screen, cat can put them in a file.
```
```./executable_file > Testing.txt``` (write the output into Testing.txt) <br>
```cat Testing.txt``` (concatenate Testing.txt) <br>

Put executable's output in a file <br>
```
gcc test.c -o test -lm
./test > test.txt
cat test.txt
```


### Find shell
```
anuragb@anuragb:~/ECE434/Basic_Linux_Commands$ echo $SHELL
/usr/bin/bash
```
```ps –p $$``` // The $$ is the process id of the shell in which your script is running

### exec system call
The  exec() family of functions replaces the current process image with a new process image. <br>

```l - execl(), execlp(), execle()```
The const char *arg and subsequent ellipses can be thought of as  arg0,
arg1, ..., argn.  Together they describe a list of one or more pointers
to null-terminated strings that represent the argument  list  available
to  the  executed  program.   The first argument, by convention, should
point to the filename associated with the  file  being  executed.   The
list  of  arguments  must  be  terminated by a null pointer, and, since
these are variadic functions, this pointer must be cast (char *) NULL.

By contrast with the 'l' functions, the 'v' functions  (below)  specify
the command-line arguments of the executed program as a vector.

```v - execv(), execvp(), execvpe()```
The  char *const argv[] argument is an array of pointers to null-termi‐
nated strings that represent the argument list  available  to  the  new
program.   The first argument, by convention, should point to the file‐
name associated with the file being executed.  The  array  of  pointers
must be terminated by a null pointer.

```e - execle(), execvpe()```
The  environment of the caller is specified via the argument envp.  The
envp argument is an array of pointers to  null-terminated  strings  and
must be terminated by a null pointer.

All  other  exec()  functions  (which do not include 'e' in the suffix)
take the environment for the new process image from the external  vari‐
able environ in the calling process. <br>

```p - execlp(), execvp(), execvpe()```
These  functions duplicate the actions of the shell in searching for an
executable file if the specified filename does not contain a slash  (/)
character.  

The file is sought in the colon-separated list of directory pathnames specified in the PATH environment variable.  If this variable isn't  defined,  the path list defaults to a list that includes the directories returned by confstr(_CS_PATH) (which  typically  returns  the value "/bin:/usr/bin") and possibly also the current working directory. <br>

```ls``` <br>
That'd be execlp("ls", "ls", (char *)NULL); <br>

```ls -l /``` <br>
That'd be execlp("ls", "ls", "-l", "/", (char *)NULL); <br>


```echo``` - display a line of text
```
anuragb@anuragb:~/ECE434/Basic_Linux_Commands$ which echo
/usr/bin/echo
```

## Types of Linux Shell
1. Bourne Shell (sh)
2. C shell (csh)
3. TENEX C shell (tcsh)
4. Korn shell (ksh)
5. Debian Almquist shell (dash)
6. Bourne Again shell (bash)
7. Z shell (zsh)
8. friendly interactive shell (fish)

echo $SHELL -> returns path to executable file of default shell <br>
echo $0 -> returns name of current shell <br>
ps -p ```$$``` -> The dollar signs represents PID of currently running shell <br>
echo ```$$``` -> returns process ID of current shell

### Instructions to switch from default shell to another shell and back to default shell

cat /etc/shells -> view list of available shells <br>
chsh <br>
Then enter path of executable file of shell: /usr/bin/rbash <br>
Restart terminal to see changes take effect.<br>
Run ps -p ```$$``` to confirm change of shell. <br>
Switch back to default shell: ```exec $SHELL``` <br>

Another method is to run: ```exec fish``` to switch to fish. <br>

Detailed ```ps``` view <br>
```
anuragb@anuragb:~/ECE434$ ps -o ppid,pid,euser,stat,%cpu,rss,args
   PPID     PID EUSER    STAT %CPU   RSS COMMAND
  10535   10553 anuragb  Ss    0.2  5484 bash
  10553   10560 anuragb  R+    0.0  1568 ps -o ppid,pid,euser,stat,%cpu,rss,args
anuragb@anuragb:~/ECE434$ 
```
### STAT:
**First State Character** <br>
D: Uninterruptible sleep (usually IO) <br>
R: Running or runnable (or run queue) <br>
S: Interruptible Sleep (waiting for an event to complete) <br>
T: Stopped, either by a job control signal or because it is being traced <br>
Z: defunct ("zombie") process, terminated but not reaped by its parent <br>

**Second State Character** <br>
< High Priority <br>
N Low priority <br>
L has pages locked into memory (for real-time and custom IO) <br>
s is a session leader (closes all child processes on termination) <br>
L is multi-threaded (uses pthread) <br>
+ is in the foreground process group <br>


```top``` <br>
allows you to view the processes running on the machine in real time - one of the few animated built-in programs. <br>

```uptime``` <br>
shows the average number of runnable processes over several different periods of time. (1, 5 and 15 minutes) <br>

```cat /proc/cpuinfo``` <br>
To see number of cores in CPU, among many other things. <br>