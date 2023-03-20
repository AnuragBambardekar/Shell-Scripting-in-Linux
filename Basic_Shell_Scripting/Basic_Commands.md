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


