#include "project1.h"
//#include <cstddef>
//#include <cstdlib>
#include <stdlib.h>  // For atoi and exit
#include <stdio.h>
#include <sys/_types/_key_t.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number_of_processes>\n", argv[0]);
        return 1;
    }

    int num_processes = atoi(argv[1]);
    if (num_processes <= 0) {
        printf("Invalid number of processes: %d\n", num_processes);
        return 1;
    }


    struct timespec start, end;
    long long seconds, nano_seconds;


    /* ToDo1
     * create and attach shared memory region 1 to store the matrix
     */
     key_t key = ftok("project1.c",99);
     if(key ==-1){
         perror("key making failed");
         exit(EXIT_FAILURE);
     }

     size_t shared_memo_size = ROWS_COUNT * COLUMN_COUNT * sizeof(int);

     int shared_memo_id = shmget(key, shared_memo_size, 0666|IPC_CREAT);
     if(shared_memo_id ==-1){
         perror("shared memory making failed");
         exit(EXIT_FAILURE);
     }

     int (*matrix)[COLUMN_COUNT] = (int (*)[COLUMN_COUNT]) shmat(shared_memo_id, NULL, 0);
     if (matrix == (void *)-1) {
         perror("matrix attach failed");
         exit(EXIT_FAILURE);
     }
    /* ToDo2
     * create and attach shared memory region 2 to store the partial sums that
     * are returned by children processes.
     */
     key_t key2 = ftok("child_proc.c",100);
     if(key ==-1){
         perror("key making for partial sums failed");
         exit(EXIT_FAILURE);
     }

     size_t shared_memo_size2 = num_processes * sizeof(int);

     int shared_memo_id2 = shmget(key2, shared_memo_size2, 0666|IPC_CREAT);
     if(shared_memo_id2 ==-1){
         perror("shared memory making for partial sums failed");
         exit(EXIT_FAILURE);
     }

     int *partial_sums = (int *)shmat(shared_memo_id2, NULL, 0);
      if (partial_sums == (void *)-1) {
          perror("partial sums attach failed");
          exit(EXIT_FAILURE);
      }
    /* ToDo3
     * Initialize the shared memory region 1 to hold
     * the matrix of size ROW_COUNT x COLUMN_COUNT.
     * Value of each element is set to i+j, where i is index of row
     * and j is index of column.
    */
    for(int i = 0; i < ROWS_COUNT; i++){
        for(int j = 0; j <COLUMN_COUNT; j++){
            matrix[i][j] = i+j;
        }
    }

    clock_gettime(CLOCK_REALTIME, &start);


    /* ToDo4:
     * 1) Create num_processes count of children processes. (using fork)
     * 2) Specify for each process the start_row, end_row, and
     * index in shared memory region 2 where a child process is supposed to store its
     * calculated partial sum.
     *
     * Hints:
     *
     * Google "man fork" and "man execlp"
     *
    */

    for (int i = 0; i < num_processes; i++) {
           pid_t pid = fork(); // Create a new process

           if (pid < 0) {
               perror("fork failed");
               exit(EXIT_FAILURE); // Handle fork error
           } else if (pid == 0) {
               // Child process
               int rows_per_process = ROWS_COUNT / num_processes; // Evenly distribute rows
               int start_row = i * rows_per_process;
               int end_row = (i == num_processes - 1) ? (ROWS_COUNT - 1) : (start_row + rows_per_process - 1);

               // Convert arguments to strings for execlp
                char start_row_str[10];
                char end_row_str[10];
                char shmid_matrix_str[10];
                char shmid_partial_sum_str[10];
                char partial_sum_index_str[10];

               snprintf(start_row_str, sizeof(start_row_str), "%d", start_row);
               snprintf(end_row_str, sizeof(end_row_str), "%d", end_row);
               snprintf(shmid_matrix_str, sizeof(shmid_matrix_str), "%d", shared_memo_id);
               snprintf(shmid_partial_sum_str, sizeof(shmid_partial_sum_str), "%d", shared_memo_id2);
               snprintf(partial_sum_index_str, sizeof(partial_sum_index_str), "%d", i);

               // Execute the child process
               execlp("./child", "child",
                      start_row_str,
                      end_row_str,
                      shmid_matrix_str,
                      shmid_partial_sum_str,
                      partial_sum_index_str,
                      (char *) NULL);

               perror("execlp failed"); // If execlp fails
               exit(EXIT_FAILURE);
           }
       }



    /* ToDo5
     * Wait for all children processes to complete
    */


    for (int i = 0; i < num_processes; i++) {
        wait(NULL);
    }

    /* ToDo6:
     * Sum all the partial-sums stored in sharred memory region 2 (by children processes)
     * and print the sum.
    */
    int final_sum = 0;
    for(int  i = 0; i < num_processes; i++){
        final_sum += partial_sums[i];
    }
    printf("sum of all partial sums = %d\n", final_sum);



    clock_gettime(CLOCK_REALTIME, &end);
    // Calculate execution time
    seconds=end.tv_sec - start.tv_sec;
    nano_seconds=end.tv_nsec - start.tv_nsec;
    long double executio_time=(long double)seconds+((long double)nano_seconds/1000000000);
    printf("Execution Time with %d process(es): %Lf\n",num_processes,executio_time);


    /* ToDo7:
     * Detach and destroy both shared memory regions.
     * Hint:
     * Google "man shmdt" and "man shmctl"
    */
    if (shmdt(matrix) == -1) {
        perror("matrix detach failed");
        exit(EXIT_FAILURE);
    }

    if (shmdt(partial_sums) == -1) {
        perror("partial sums detach failed");
        exit(EXIT_FAILURE);
    }

    if (shmctl(shared_memo_id, IPC_RMID, NULL) == -1) {
         perror("shmctl for matrix failed");
         exit(EXIT_FAILURE);
     }

     if (shmctl(shared_memo_id2, IPC_RMID, NULL) == -1) {
         perror("shmctl for partial sums failed");
         exit(EXIT_FAILURE);
     }
     printf("memorry is free and all is good, bye bye \n");
    return 0;
}
