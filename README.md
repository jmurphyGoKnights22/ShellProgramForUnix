### WARNING: Any students who are looking to copy this code, don't bother. The professor has been made aware of this public repo and you will be penalized for copying the code.

## You have been warned.

# ShellProgramForUnix

shell program project:
  - tokenize input
  - commands to implement:
    1) movetodir X
       program should have an internal variable called “currentdir”. This command checks whether the specified directory exists and it is a directory. If it exists, the currentdir variable should be assigned this value. If the specified directory does not exist, the command should write an error message and the currentdir variable should not be changed.
       Note: do not use the chdir() function call here!
    2) whereami
        prints the value of currentdir
    3) history [-c]
        Without the parameter, prints out the recently typed commands (with their arguments), in reverse order, with numbers. For instance, it can print:
 
        0: history
        1: movetodir /etc
        2: movetodir /eetc

        If “-c” is passed, it clears the list of recently typed commands. You will need to implement an internal data structure to keep the list of commands. The data structure should allow you to grow the data structure to an arbitrary length. 
        You will need to save the history data structure to an external file (when exiting), and load it (when starting the shell). 
    4) byebye
        Terminates the mysh shell (and saves the history to a file)
    5) replay N
        Re-executes the command labeled with number N in the history
    6) start program [parameters]
        The argument “program” is the program to execute. If the argument starts with a “/” (such as /usr/bin/xterm, the shell should interpret it as a full path. Otherwise, the program will be interpreted as a relative path starting from the current directory. 

        The program will be executed with the optional “parameters”. It uses fork() + exec() to start the program with the corresponding parameters, and waits until the program terminates (use the waitpid() call). 
        For instance

            start /usr/bin/xterm –bg green

        would bring up a terminal with a green background. The prompt would not return until the terminal is closed.

        Display an error message if the specified program cannot be found or cannot be executed.
    7) background program [parameters]
        Similar to the run command, but it immediately prints the PID of the program it started and returns the prompt.
    8) dalek PID
        Immediately terminate the program with the specific PID (presumably started from this command line interpreter). Use the kill() function call to send a SIGKILL signal to the program. Display success or failure.

    EXTRA CREDIT :
       repeat n command …

        Interprets n as the number of times the command must be run, command as the full path to the program to execute, and the others as parameters.  The command should start the specified number of program instances, print the PIDs of the created processes and then return to the prompt, without waiting for the processes to terminate. For instance:
            repeat 4 /usr/bin/xterm 
        would bring up 4 terminals and print out something like:
        PIDs: 31012, 31013, 31014, 31015.

       dalekall

        Which immediately terminates all the programs previously started by the mysh shell which had not been previously terminated by it, or by exterminate. It should output something like this:

        Exterminating 3 processes: 16000 31012 31013

  - What to submit:
    • The code as a single .c file.
    • If you implemented the extra credit part: a text file describing the syntax of the implementation, and example of use.
