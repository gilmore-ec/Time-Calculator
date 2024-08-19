```
Main function to process inputs and pass to calculating, optional, and reporting functions
    Process options
        Verify option's input values are valid
    Request inputs from user
    Verify inputs are valid
    Pass data inputs to calculation functions and store the results in a variable
    Print the result to stdout


Calculating function receives the current time and adds the input value, then returns a Double representation of the resulting time
    Check if there is a specified start time
        If not, convert the system time to military time (formatted as a Double [HH.xx])
    Add the user input time value to the system time value and store the result
    Round, then return the result
```