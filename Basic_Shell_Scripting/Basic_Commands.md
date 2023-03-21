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

```pwd```
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
cd - change directory
```
anuragb@anuragb:~/ECE434$ cd Basic_Linux_Commands/
anuragb@anuragb:~/ECE434/Basic_Linux_Commands$ 
```

```ps``` <br>
ps - list processes 
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

a = show processes for all users <br>
u = display the process's user/owner <br>
x = also show processes not attached to a terminal <br>

```kill``` <br>
kill -9 no_of_process //kill this process

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
```cat file > test``` //We can redirect standard output of a file into a new file <br>
```cat file >> test``` // Appends in existing file with ‘>>‘ double greater than symbol. Here, contents of test file will be appended at the end of test1 file. <br>
```cat test1 test2 test3 > test4``` //This will create a file called test4 and all output will be redirected in a newly created file <br>

```
Instead of displaying the contents of a file on the screen, cat can put them in a file.
```
```./executable_file > Testing.txt``` (write the output into Testing.txt) <br>
```cat Testing.txt``` (concatenate Testing.txt) <br>


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
