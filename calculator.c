/*
Author: Timothy Gilmore (Gilmore-EC)
Description: Pulls system time and recieves an amount of time from user and calculates the time when that amount of time has passed
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

void result(double,int,int);
double calculate(double,double,double,int);

int main(int argc, char* argv[]) {
    int s = 0;
    int p = 0;
    int c = 1;
    int h = 0;
    int d = 0;
    double input = -1;
    double start = -1;
    double precision = 0.05;
    for (int i = 1; i < argc; i++) {
            
        fprintf(stderr,"DEBUG: Argument %d: %s\n", i, argv[i]);

        if (argv[i][0] == '-') { 

            char option = argv[i][1];

            //Check for valid options:
            switch (option) {
                //Manual start-time entry:
                case 's':
                case 'S':
                    s=1;
                    break;

                //Manual rounding precision value:
                case 'p':
                case 'P':
                    p=1;
                    break;

                //Convert to military time:
                case 'c':
                case 'C':
                    c=0;
                    break;

                case 'h':
                case 'H':
                    h = 1;
                    break;

                case 'd':
                case 'D':
                    d = 1;
                    break;

                //Invalid option:
                default:
                    fprintf(stderr,"DEBUG: User entered an invalid option: %c\n\n",option);
            }
        }
        else { //Check if the argument is the additive input

            const char *str = argv[i];
            char *endptr;
    
            //Convert the input to a Double
            double num = strtod(str, &endptr);
    
            //Check if the conversion was successful
            if (endptr == str) {
                //No digits were found
                if (d) fprintf(stderr,"DEBUG: Could not convert the input to a Double.\n\n");
            } else if (*endptr == '\0') {
                //The entire input was converted
                if (d) fprintf(stderr,"DEBUG: Successfully converted the input to a Double\n\n", num);
                input = num;
            } else {
                //The input was partially converted
                if (d) fprintf(stderr,"DEBUG: Conversion partially successful. Number: %f, \n\t Unable to convert %s.\n\n", num, endptr);
                input = num;
            }
        }
        
    }

    if (d) {
        fprintf(stderr,"\nDEBUG: All program arguments processed.\n");
        fprintf(stderr,"DEBUG: User requested detailed debugging information to be displayed\n");
        fprintf(stderr,"DEBUG: Beginning input processing.\n");
    }
    if (h) {
        if (d) fprintf(stderr,"DEBUG: User requested the manual for the program.\n\n");
        printf("\n*****************************************************************************************************\n");
        printf("USER MANUAL:\n\n");
        printf("This is a simple C program that calculates end time by adding a given amount of time to a start time.\n");
        printf("\tValid program arguments:\n");
        printf("\t\t<value> : Additive input should be formatted as [HH.xx]. (Round hours to the nearest 100th)\n");
        printf("\t\t-s : Flag that you want to enter a specified start time\n");
        printf("\t\t-p : Flag that you want to enter a specified rounding value\n");
        printf("\t\t-c : Return result in military time\n");
        printf("\t\t-h : Provides the manual for the program\n");
        printf("\t\t-d : Print detailed debugging info to stderr\n");
        printf("*****************************************************************************************************\n");
    }
    if (s) {
        if (d) fprintf(stderr,"DEBUG: User flagged for manual start-time entry.\n");
        char partofday;
        printf("\nPlease enter a start time [Format: HHMM]: ");
        scanf("%lf",&start);
        printf("\nAM or PM? (a/p): ");
        scanf(" %c",&partofday);
        int temp = (int)(start/100);
        if (d) fprintf(stderr,"DEBUG: User entered custom start time: %d:%d %cm\n",temp,(int)(start-temp*100),partofday);
        start = (double)((int)start % 1200) /100;
        if (partofday=='p') start += 12;
    }
    if (p) {
        if (d) fprintf(stderr,"DEBUG: User flagged for manual precision rounding entry.\n");
        printf("\nPlease enter a rounding value: ");
        scanf("%lf",&precision);
        if (d) fprintf(stderr,"DEBUG: User entered custom rounding value: %lf\n",precision);
    }
    if (input == -1) {
        printf("\nPlease enter an amount of hours to add: ");
        scanf("%lf",&input);
        if (d) fprintf(stderr, "DEBUG: User entered additive value: %lf\n", input);
    }
    if (!c && d) fprintf(stderr,"DEBUG: User flagged for military time format.\n");;

    if (d) fprintf(stderr,"DEBUG: Input processing completed.\n");

    //Verify inputs are valid:
    while (input < 0) { //Additive Input
        if (d) fprintf(stderr, "DEBUG: Additive Input either was not entered or is invalid.\n");
        printf("\nAdditive input was either not entered, or invalid.\n\tPlease enter a valid quantity to add or '-1' to quit: ");
        scanf("%lf",&input);
        if (input == -1) {
            if (d) fprintf(stderr,"DEBUG: User quit during invalid input correction.\n");
            return(-1);
        }
    }
    while (start < 0 && start != -1) { //Custom Start Time Input
        if (d) fprintf(stderr, "DEBUG: Start-Time Input is invalid.\n");
        printf("\nStart Time input is invalid.\n\tPlease enter a positive value or '-1' for the default: ");
        scanf("%lf",&input);
        if (input == -1) {
            if (d) fprintf(stderr,"DEBUG: Resetting start-time to default value.\n");
            start = -1;
        }

    }
    while (precision < 0) { //Custom Rounding Precision Value
        if (d) fprintf(stderr, "DEBUG: Precision Value Input is invalid: %lf\n",precision);
        printf("\nRequired input is invalid.\n\tPlease enter a positive value or '-1' for the default: ");
        scanf("%lf",&input);
        if (input == -1) {
            if (d) fprintf(stderr,"DEBUG: Resetting precision to default value.\n");
            precision = 0.05;
        }

    }
    
    input = calculate(input,start,precision,d);

    if (d) fprintf(stderr, "DEBUG: Rounded input to the nearest %.2lf -> %.2lf\n", precision, input);

    printf("\nExpected out time is: ");

    result(input,c,d);

    return(0);
}

/*
Description: Takes the current time and adds the input value, then returns a Double representation of the resulting time
Arguments: input is the user-provided time, start is the start time, round is the rounding value
Returns: A Double value of the sum of input and systime
*/
double calculate(double input, double start, double precision, int d) {
    if (d) {
        fprintf(stderr,"DEBUG: Calculate args -> input = %lf, start = %lf, precision = %lf\n",input,start,precision);
    }
    //Variable Declarations:
    double sum = 0;
    double result = 0;
    int rounded = 0;
    //Check if there is a specified start time
    if (start < 0) {
        //Get system time
        //Convert system time to military
    }

    //Convert start time minutes to fractional hours
    int hours = start;
    double minutes = (start - (double)hours) * 100;
    if (d) fprintf(stderr,"DEBUG: Hours = %d, Minutes = %2.0lf\n",hours,minutes);
    start = hours + (minutes/60);

    //Add the user input time value to the system time value and store the result
    sum = input + start;
    if (d) fprintf(stderr, "DEBUG: Result before rounding is %lf\n",sum);

    //Round, then return the result
    rounded = (sum / precision) + 0.5;
    result = (double)rounded * precision;
    if (d) fprintf(stderr, "DEBUG: Result after rounding is %lf\n",result);

    return result;
}

void result(double result, int format, int d) {
    int hours = (int)result;
    if (d) fprintf(stderr,"\nDEBUG: Hours = %d\n",hours);
    int minutes = (int)((result- (double)hours)*60);
    if (d) fprintf(stderr,"DEBUG: Minutes = %d\n",minutes);

    //if format = 1, use 12-hour time format
    if (format) hours = hours%12;

    printf("%02d:%02d",hours,minutes);

    if (format) printf("%s",(result >= 12 ? " PM\n" : " AM\n"));
    printf("\n\n");
}