# Systems-Programming

These are my codes from Systems Programming at Rutgers University.

PA1: Using mergeSort to sort a CSV file with information about movies.
    Takes in first parameter "-c" to show that the CSV files are sorted by column
      The second paramter is the name of the column to be sorted on (i.e. director_name).
      
      
PA2: Uses the code from the first assignent, but generalizes it to be able to read through a directory.
    Forks a new process for each directory or file to be sorted.
    
PA3: Using the same ideas from PA2, a multi-directory sorter is implemented using threads for each new fil or subdirectory.

PA4: Seperate from the previous assignments, this uses threads to set up a mock bank, with a server and client end.
    The bank continously prints out real time accoutn data, and the clients can perform basic account functions such as withdraw and deposit.
