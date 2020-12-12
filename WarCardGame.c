/**
 
 Description:
 The WarCardGame.c creates 2 child process that are both players in a 2-player war card game tournament that lasts for 3 rounds. The processes communicate via sockets pairs to the parent process which acts as a mediator between the two. the Parent process determines when the players draw a card, by using the kill signal to switch the processes on and off. As the program executes the  parent  keeps score for the winner of each round and  decides who wins the tournament.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>


void compareSuite(char* setter, int draw_num);
  


char suite (int draw);
       
           
int main(int argc, char *argv[]){
    
  
    int sockets[2], sockets2[2];
    
    
   
    
    if(argc != 2){
        printf("Error, integer argument expected. Please enter an amount of rounds to play");// prompts user to enter a integer after ./a.out
        exit(EXIT_FAILURE);
    }
    
    /*
     Opening sockets
     */
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) < 0) {
        perror("Error opening stream socket pair");
        exit(EXIT_FAILURE);
    }
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets2) < 0) {
        perror("Error opening stream socket pair");
        exit(EXIT_FAILURE);
    }
    
    printf("\nThis is a War Card Game\n");
    
    
    
    pid_t child1_pid, child2_pid;
    
    // forking program to create 2 children

    (child1_pid=fork())&&(child2_pid=fork());
    
    if(child1_pid<0){
        
        perror("Error creating process 1");
        exit(EXIT_FAILURE);
        
    }
    
    if (child1_pid==0)
    
    {
       
        
        while(1)
        
        {
            
            char turn;
            unsigned seed;
            _Bool run =1;
            
                
            seed = time(0);
            srand(seed);
            
            printf( "Child One ");
            
            // getting randomly generated draw value for player1
            int c_draw = rand() % 19;
            
           
            int draw_suite = c_draw;
                
            // prints prompt that displays player's draw to console
            turn = suite(c_draw);
            
            if (write(sockets[0],&c_draw, sizeof(int)) < 0)
                perror("writing stream message");
            
            // sending suite value in case of a tie
            
            if (write(sockets2[0],&draw_suite, sizeof(int)) < 0)
                perror("writing stream message");

            
            
            sleep(3);
        }

                
       
       
            
    }
    
            
            
    if(child2_pid<0){
        
        perror("Error creating process 2");
        exit(EXIT_FAILURE);
        
    }
    
    else if (child2_pid==0)
        
    {
      
            
       
        while(1)
        
        {
            
            char turn;
            unsigned seed;
           
           
                
            seed = time(0);
            srand(seed);
            printf( "Child Two ");
            
            // getting randomly generated draw value for player2
            int c_draw1 = rand() % 19;
            
            
            
            int draw_suite2 = c_draw1;
            
            // prints prompt that displays player's draw to console
            
            turn = suite(c_draw1);
            
        
            
            if (write(sockets[1],&c_draw1, sizeof(int)) < 0)
                perror("writing stream message");
                
            
                //sending suite value in case of a tie
            
            if (write(sockets2[1],&draw_suite2, sizeof(int)) < 0)
                perror("writing stream message");
          
            sleep(3);
         }
            
        
    
    }
    
    

    else
        
    {
        // program comes here to begin execution, but first is pauses the children processes
        kill(child1_pid, SIGSTOP);
        kill(child2_pid, SIGSTOP);
        
        
        int rounds = atoi(argv[1]); //sets number of rounds to value of command line arguments entered by user
        int pid1 = child1_pid;
        int pid2 = child2_pid;
        
        printf("Child 1 PID: %d \n",  pid1);
        printf("Child 2 PID: %d \n",  pid2);
        
         
        printf("\nLet's Start Playing %d Rounds Of War!\n",rounds);
    
        int count = 1;
        int childone_roundswon = 0;
        int childtwo_roundswon = 0;
        int child1_draw;
        int child2_draw;
        
        char check_c1suite;
        char check_c2suite;
        
        do {
       
        
        printf("\n--------  Round: %d ---------\n", count );
        
       // PARENT: sending SIGCONT to child one
                kill(child1_pid, SIGCONT);
        sleep(2); /* pause for 2 secs */
        
        //printf("\nPARENT: sending SIGSTOP to child one \n");
        kill(child1_pid, SIGSTOP);
        sleep(2); /* pause for 2 secs */
        
        
        // PARENT: sending SIGCCONT to child 2
        sleep(1);
        kill(child2_pid, SIGCONT);
        sleep(1); /* pause for 1 sec */
        
       
                kill(child2_pid, SIGSTOP);
        sleep(1); /* pause for 1 sec */
        
        
        
        /*
        Retrieving draw values of child one
        */
        read(sockets[1], &child1_draw, sizeof(int));
           
       
            sleep(2);
            
        /*
        Retrieving draw values of child two
        */
        read(sockets[0], &child2_draw,sizeof(int));
           
            
            sleep(2);
        
        
        if (child1_draw>child2_draw){
            printf("\nChild One Wins!\n");
            childone_roundswon ++;
        }
        if (child1_draw<child2_draw){
            printf("\nChild Two Wins!\n");
            childtwo_roundswon ++;
        }
            /*
             In case of a tie the refery will check the suites of the
             players. the compareSuite function assigns a suite value to each
             players draw
             */
        if (child1_draw==child2_draw){
            printf("Checking Suite");
            
            /*
             Retrieving generated values for suite. child_draw 1 and 2  values are decided decided within the children processes
             */
            read(sockets2[1], &child1_draw, sizeof(int));
            read(sockets2[0], &child2_draw, sizeof(int));
            
            compareSuite(&check_c1suite,child1_draw);
            compareSuite(&check_c2suite,child2_draw);
            
              if((strcmp(&check_c1suite,"Clubs")) && (strcmp(&check_c2suite,"Diamonds") ||strcmp(&check_c2suite,"Hearts")|| strcmp(&check_c2suite,"Spades"))){
                
                  printf("\nChild Two Wins!\n");
                
                  childtwo_roundswon ++;
                
                
                    }
                
              if((strcmp(&check_c1suite,"Diamonds")) && (strcmp(&check_c2suite,"Clubs" ))){
                  
                  printf("\n Child One Wins!\n");
                  
                  childone_roundswon ++;
                
                
                    }
                
              if((strcmp(&check_c1suite, "Hearts")) && (strcmp(&check_c2suite,"Clubs" )||strcmp(&check_c2suite,"Diamonds" ))){
                
                    printf("\n Child One Wins!\n");
                  
                    childone_roundswon ++;
                    
                    }
               
             if(strcmp(&check_c1suite, "Spades")) {
                    printf("\n Child One Wins!\n");
                   
                    childone_roundswon ++;
                
                    }
                
                
                
             if((strcmp(&check_c2suite,"Clubs")) && (strcmp(&check_c1suite,"Diamonds") ||strcmp(&check_c1suite,"Hearts")|| strcmp(&check_c1suite,"Spades"))){
                    
                    printf("\nChild One Wins!\n");
                    
                    childone_roundswon ++;
                    
                    
                    }
            
             if((strcmp(&check_c2suite,"Diamonds")) && (strcmp(&check_c1suite,"Clubs" ))){
                    
                    printf("\n Child Two Wins!\n");
                    
                    childtwo_roundswon ++;
                    
                    
                    }
                    
             if((strcmp(&check_c2suite, "Hearts"))&& (strcmp(&check_c1suite,"Clubs" )||strcmp(&check_c1suite,"Diamonds" ))){
                    
                    printf("\n Child Two Wins!\n");
                    
                    childtwo_roundswon ++;
                        
                    }
                   
             if(strcmp(&check_c2suite, "Spades")) {
                
                    printf("\n Child Two Wins!\n");
                    
                    childtwo_roundswon ++;
                    
                    }
            
            
            
        }// closes if statement for if tie
            count++;
                
    }while(count<=rounds);
        
        
       
        
        printf("\nResults\n");
        
        printf("Child 1: %d\n",  childone_roundswon);
        printf("Child 2: %d\n",  childtwo_roundswon);
        
        if(childone_roundswon > childtwo_roundswon){
            
            printf("Child One Wins The Tournament!\n");
        }
        
        if(childone_roundswon < childtwo_roundswon){
            
            printf("Child Two Wins The Tournament!\n");
        }
        
        if(childone_roundswon == childtwo_roundswon){
            
            printf("It's A Draw, Both Players Are Winners!\n");
            
            
        }
        
        int i, child_status;
        
        //killing processes and reaping children
        printf("Killing process 1\n");
        kill(child1_pid,SIGINT);
        printf("Killing process 2\n");
        kill(child2_pid,SIGINT);
        
        
        
        //reaping children
        
        waitpid(child1_pid,&child_status,0);
        waitpid(child2_pid,&child_status,0);
        
        printf("children reaped\n");
        
        //closing sockets
        
        close(sockets[1]);
        close(sockets[0]);
        close(sockets2[1]);
        close(sockets2[0]);
        
        printf("scokets closed\n");
        
        
       
  
  }
    
    
    exit(EXIT_SUCCESS);
    
}

void compareSuite(char* setter, int draw_num){
    
    if(draw_num == 1){
        setter = "Hearts";
       
    }
    if(draw_num == 2){
        setter = "Spades";
    }
    if(draw_num == 3){
        setter = "Diamond";
    }
    if(draw_num == 4){
        setter = "Spades";
    }
    if(draw_num == 5){
        setter = "Clubs";
    }
    if(draw_num == 6){
        setter = "Diamond";
    }
    if(draw_num == 7){
        setter = "Hearts";
    }
    if(draw_num == 8){
        setter = "Clubs";
    }
    if(draw_num == 8){
        setter = "Diamonds";
    }
    if(draw_num == 10){
        setter = "Spades";
    }
}

char suite (int draw) {

   /* local variable definition */
   
    char statement;
    switch(draw) {
           
    case 1 :
            return printf("Draws 1\n") ;
            break;
    case 2 :
            return printf("Draws 2\n" );
            break;
    case 3 :
            return printf("Draws 3\n" );
            break;
    case 4 :
            return printf("Draws 4\n" );
            break;
    case 5 :
            return printf("Draws 5\n" );
            break;
    case 6 :
            return printf("Draws 6\n" );
            break;
    case 7 :
            return printf("Draws 7\n" );
            break;
    case 8 :
            return printf("Draws 8\n" );
            break;
    case 9 :
            return printf("Draws 9\n" );
            break;
    case 10 :
            return printf("Draws 10 \n" );
            break;
    case 11 :
            return printf("Draws Joker\n" );
            break;
    case 12 :
            return printf("Draws Queen \n");
            break;
    case 13 :
            return printf("Draws King \n" );
            break;
    case 14 :
            return printf("Draws Ace\n" );
            break;
    case 15 :
            return printf("Draws Spades\n" );
            break;
    case 16 :
            return printf("Draws Hearts\n " );
            break;
    case 17 :
            return printf("Draws Diamonds\n " );
            break;
    case 18 :
            return printf("Draws Clubs\n " );
            break;

    default :
             return printf(" Has chosen not to draw this turn \n" );
           
   }
     
}



