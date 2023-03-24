# A simple Named pipe Demo in Terminal

**Terminal 1:** <br>

```$ mknod new_named_pipe p``` <br>
```$ echo 123 > new_named_pipe```

Terminal 1 created a named pipe. <br>
It wrote data in it using echo. <br>
It is blocked as there is no receiving end (as pipes both named and unnamed need receiving and writing ends to it) <br>

**Terminal 2:** <br>

```$ cat new_named_pipe``` <br>
```$ 123``` <br>
```$ ```

From Terminal 2, a receiving end for the data is added. <br>
It read the data in it using cat. <br>
Since both receiving and writing ends are there for the new_named_pipe it displays the information and blocking stops. <br>