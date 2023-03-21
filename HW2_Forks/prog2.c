#include <stdio.h>
#include <unistd.h>

int i,j = 0; //Global variables

// a computationally heavy body in the for loop
// can alternatively use factorial also
int odd(int j) {
    if (j % 2 == 0) {
        return j + 1;
    } else {
        return j;
    }
}

main()
{
    i++;
    printf("%d\n",i); // prints 1

    int p1 = fork(); //shell forks a child
    i+=8;
    for(j=0; j<10000; j++) i+=odd(j);
    // if (p1==0)
    //     printf("hello_child1 %d\n",i);
    // else    
    //     printf("hello_parent1 %d\n",i);
    printf("%d\n",i);

    int p3 = fork();
    i+=13;
    // if (p3==0)
    //     printf("hello_child2 %d\n",i);
    // else    
    //     printf("hello_parent2 %d\n",i);
    printf("%d\n",i);

    int p2 = fork();
    i+=19;
    // if (p2==0)
    //     printf("hello_child3 %d\n",i);
    // else    
    //     printf("hello_parent3 %d\n",i);
    printf("%d\n",i);
}


// 1. Please explain 
// what you would “expect” to see on your screen at run-time of the program
/**
 * 
*/
