#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*#define HOURS 24 Kan vi sagtens arbejde videre med, det ville give rigtigt god mening at have symbolske konstanter for disse to værdier. 
  #define DAYS 7*/

struct event {
    char occasion[10]; //10-tallet antallet af characters der er plads til i char-strengen
    unsigned int value; //vigigt at alle starter med 0
    //int priority; //mangler prioritering + evt andet?
    int elevtid;
}
event_default = { "", 0 }; //Forloop l�ngere nede.
typedef struct event event;

//test
const char* weekdays[] = { "Day:", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };
const char* timeOfDay[] = { "00:00","01:00","02:00","03:00","04:00","05:00","06:00","07:00",
                            "08:00","09:00","10:00","11:00","12:00","13:00","14:00","15:00",
                            "16:00","17:00","18:00","19:00","20:00","21:00","22:00","23:00" };

void printDemoSchedule(int tasks, int days, event display[tasks][days]);
void accept(int tasks, int days, event temp[tasks][days], event planned[tasks][days]); 
void sleep(int tasks, int days, event TempWeekPlan[tasks][days]);
void DoubleBooking(int tasks, int days, event firstPlan[tasks][days], event secondPlan[tasks][days], event tempPlan[tasks][days]);
void Prep_Lesson(int tasks, int days, event TempWeekPlan[tasks][days]);
void ReassignD(int tasks, int days, event tempPlan[tasks][days], event reassignedarray[tasks][days], int i, int j, event doublebookingarray[tasks][days]);
void ReassignEvent(int tasks, int days, event tempPlan[tasks][days], event reassignedarray[tasks][days]); 

int main(void) {
    int i, j;
    int tasks = 24, days = 7; //oprindeligt tasks = 4
   
    /*Jeg t�nker at vi er n�dt til at lave kalenderen dag for dag og s� hver event er koblet til en ugedag og
    ikke bare til en plads i et to-dimensionelt array, but I don't know */
    /*Vi pr�ver: F�rst med et to-dimensionelt array*/
    event SchoolPlan[tasks][days]; /* = malloc(tasks * sizeof *week);*/
    event ConstPlan[tasks][days];
    event SocialPlan[tasks][days];
    event PlannedWeek[tasks][days]; // = malloc(tasks * sizeof *nextweek); //ikke n�dvendigt med dynamisk lagerallokering her?
    event TempWeekPlan[tasks][days];
    event display[tasks][days];
    
    //Default udf�res: 
    for (j = 0; j < 7; j++) {
        for (i = 0; i < 24; i++) {
            SchoolPlan[i][j].value = event_default.value;
            strcpy(SchoolPlan[i][j].occasion, event_default.occasion);
            ConstPlan[i][j].value = event_default.value;
            strcpy(ConstPlan[i][j].occasion, event_default.occasion);
            SocialPlan[i][j].value = event_default.value;
            strcpy(SocialPlan[i][j].occasion, event_default.occasion);
            TempWeekPlan[i][j].value = event_default.value;
            strcpy(TempWeekPlan[i][j].occasion, event_default.occasion);
        }
    }

    //Schedules sleep for all days of the week
    sleep(24, 7, TempWeekPlan);

    //for (j = 0; j < 7; j++) {
        //ConstPlan[7][j].value = 1;
        //strcpy(ConstPlan[7][j].occasion, "Breakfast");
    //}
    //Slut p� de helt faste events, der g�lder for hver dag

    //Arbejde
    for (i = 16; i < 21; i++) {
        ConstPlan[i][0].value = 1;
        strcpy(ConstPlan[i][0].occasion, "Work");
    }
    for (i = 8; i < 17; i++) {
        ConstPlan[i][5].value = 1;
        strcpy(ConstPlan[i][5].occasion, "Work");
    }

    //Alle nuværende events ligges sammen i ConstPlan og der tjekkes for dobbeltbooking: 
    DoubleBooking(24, 7, ConstPlan, TempWeekPlan, ConstPlan);

    //Herefter slettes alle events fra TempWeekPlan: 
    for (j = 0; j < 7; j++) {
        for (i = 0; i < 24; i++) {
            TempWeekPlan[i][j].value = event_default.value;
            strcpy(TempWeekPlan[i][j].occasion, event_default.occasion);
        }
    }

    //Lectio-kalenderen: 
    for (j = 0; j < 5; j++) {
        for (i = 8; i < 15; i++) {
            SchoolPlan[i][j].value = 1;
            strcpy(SchoolPlan[i][j].occasion, "School");
        }
    }
    //Det dobbeltbookede test-event: 
    for (i = 18; i < 20; i++) {
        SchoolPlan[i][1].value = 1;
        strcpy(SchoolPlan[i][1].occasion, "SchoolEvent"); //Skal dette oprettes et andet sted end her??
    }
    //Transport til skole
    for (j = 0; j < 5; j++) {
        SchoolPlan[15][j].value = 1;
        strcpy(SchoolPlan[15][j].occasion, "Transport");
    }

    //Social kalender: 
    for (i = 18; i < 20; i++) {
        SocialPlan[i][1].value = 1;
        strcpy(SocialPlan[i][1].occasion, "Soccer");
    }
    for (i = 18; i < 20; i++) {
        SocialPlan[i][3].value = 1;
        strcpy(SocialPlan[i][3].occasion, "Soccer");
    }
    for (i = 12; i < 16; i++) {
        SocialPlan[i][6].value = 1;
        strcpy(SocialPlan[i][6].occasion, "Soccer");
    }

    //Der tjekkes for double-booking: 
    DoubleBooking(24, 7, ConstPlan, SchoolPlan, TempWeekPlan);
    DoubleBooking(24, 7, ConstPlan, SocialPlan, TempWeekPlan);
    DoubleBooking(24, 7, SocialPlan, SchoolPlan, TempWeekPlan);

    Prep_Lesson(24, 7, TempWeekPlan);

    //TempWeekPlan vises
    printf("TempWeekPlan: \n");
    printDemoSchedule(24, 7, TempWeekPlan);

    ReassignEvent(24, 7, TempWeekPlan, TempWeekPlan); 
   
    printf("TempWeekPlan(fra main, efter ReassignEvent): \n");
    printDemoSchedule(24, 7, TempWeekPlan);

    //TempWeekPlan bliver accepteret eller afvist, hvis accepteret bliver TempWeekPlan overført til PlannedWeek 
    accept(24, 7, TempWeekPlan, PlannedWeek);

    //PlannedWeek vises
    printf("PlannedWeek: \n");
    printDemoSchedule(24, 7, PlannedWeek); 

    //Hernæst kommer lesson preparation og deadlines funktionerne (I hvilken rækkefølge?)

    //Her bliver printDemoSchedule() og accept() kaldt flere gange
    
    
    return 0;
}

void printDemoSchedule(int tasks, int days, event display[tasks][days]){
    int i, j;

    for (i = 0; i < 8; i++) {
        printf("%-11s", weekdays[i]);
    }
    printf("\n");

    for (i = 0; i < 24; i++) { 
        printf("%-11s", timeOfDay[i]);
        for (j = 0; j < 7; j++) {
            printf("%-11s", display[i][j].occasion);
        }
        printf("\n");
    }
    printf("\n"); 
    
}

void accept(int tasks, int days, event temp[tasks][days], event planned[tasks][days]) {

    int booking = 0;
    printf("Do you accept the calender? Type 1 for yes and 2 for no: \n");
    scanf("%d", &booking);
    if (booking == 1) {
        for (int j = 0; j < 7; j++) {
            for (int i = 0; i < 24; i++) {
                planned[i][j].value = temp[i][j].value;
                strcpy(planned[i][j].occasion, temp[i][j].occasion);
            }
        }
    }
    else {
        printf("Idiot\n"); //Mere professionelt tak! Her bliver man 
        //sendes videre til ReassignEvent(); 
    }
}

void sleep(int tasks, int days, event TempWeekPlan[tasks][days]){
    int i, j, hours = 24;
    int sleep, go_to_sleep, wake_up;

    //event SleepPlan[hours][days];

    printf("Please the number representing your desired sleeping schedule:\n");
    printf("1. Same sleeping schedule througout the week\n");
    printf("2. One schedule for school days and one for the weekend\n");
    scanf(" %d", &sleep);
    
    while(sleep != 1 && sleep != 2){
        printf("Your answer isn't recognized, try again:");
        scanf(" %d", &sleep);
    }
    if(sleep == 1){ //Ens søvnskema HELE ugen
        printf("Please type the time you wish to go to sleep (which hour under 24?):\n");
        scanf(" %d", &go_to_sleep);
        printf("Please type the time you wish to wake up (which hour under 24?):\n");
        scanf(" %d", &wake_up);

        while(wake_up > 7){
            printf("Remember you have to go to school\nPlease try an earlier time to wake up:");
            scanf(" %d", &wake_up);
        }

        if(go_to_sleep > 15){ //Til hvis de går i seng inden midnat, tallet er tilfældigt
            if((wake_up + (24 - go_to_sleep)) < 8){
                printf("The doctors recommend 8 hours of sleep a day\n");
                printf("With this schedule you only get %d hours a day\n", (wake_up + (24 - go_to_sleep)));
                printf("Please reconsider this in the future\n");
            }
            for(i = go_to_sleep; i < 24; i++){
                for(j = 0; j < 7; j++){
                    strcpy(TempWeekPlan[i][j].occasion, "Sleep"); 
                    TempWeekPlan[i][j].value = 1;
                    TempWeekPlan[go_to_sleep - 1][j].value = 1;
                    //strcpy(TempWeekPlan[go_to_sleep - 1][j].occasion, "Stupid");
                }
            }
            for(i = 0; i <= wake_up; i++){
                for(j = 0; j < 7; j++){
                    strcpy(TempWeekPlan[i][j].occasion, "Sleep"); 
                    TempWeekPlan[i][j].value = 1;
                    strcpy(TempWeekPlan[wake_up][j].occasion, "Breakfast");
                    TempWeekPlan[wake_up][j].value = 1;
                }
            }       
        } else if(go_to_sleep < 15){ //Hvis de går i seng efter midnat
            if((wake_up - go_to_sleep) < 8){
                printf("The doctors recommend 8 hours of sleep a day\n");
                printf("With this schedule you only get %d hours a day\n", (wake_up - go_to_sleep));
                printf("Please reconsider this in the future\n");
            }
            for(i = go_to_sleep; i <= wake_up; i++){
                for(j = 0; j < 7; j++){
                    strcpy(TempWeekPlan[i][j].occasion, "Sleep"); 
                    TempWeekPlan[i][j].value = 1;
                    strcpy(TempWeekPlan[wake_up][j].occasion, "Breakfast");
                    TempWeekPlan[wake_up][j].value = 1;
                    if(go_to_sleep == 0){
                        if(j == 0){
                            TempWeekPlan[23][6].value = 1;
                            //strcpy(TempWeekPlan[23][6].occasion, "Stupid");
                        } else {
                        TempWeekPlan[23][j - 1].value = 1;
                        //strcpy(TempWeekPlan[23][j - 1].occasion, "Stupid");
                        }
                    } else{
                        TempWeekPlan[go_to_sleep - 1][j].value = 1;
                        //strcpy(TempWeekPlan[go_to_sleep - 1][j].occasion, "Stupid");
                    }
                }
            }
        }
    } else if(sleep == 2){ //Til hvis de har to forskellige (eller bare en søvnskema til hverdagene)
        printf("Do you wish a sleeping schedule for your school days and weekend (1) or just the school days(2)?\n");
        scanf(" %d", &sleep);
        if(sleep == 1){ //Til to forskellige søvnskemaer, 1 til hverdagene og 1 til weekenden

            printf("Please type the time you wish to go to sleep for school days (which hour under 24?):\n"); //Hverdagene
            scanf(" %d", &go_to_sleep);
            printf("Please type the time you wish to wake up on school days (which hour under 24?):\n");
            scanf(" %d", &wake_up);

            while(wake_up > 7){
                printf("Remember you have to go to school\nPlease try an earlier time to wake up:");
                scanf(" %d", &wake_up);
            }
        
            if(go_to_sleep > 15){ //Til hvis de går i seng inden midnat, tallet er tilfældigt
                if((wake_up + (24 - go_to_sleep)) < 8){
                    printf("The doctors recommend 8 hours of sleep a day\n");
                    printf("With this schedule you only get %d hours a day\n", (wake_up + (24 - go_to_sleep)));
                    printf("Please reconsider this in the future\n");
                }
                for(i = go_to_sleep; i < 24; i++){
                    for(j = 0; j < 4; j++){
                        strcpy(TempWeekPlan[i][j].occasion, "Sleep"); 
                        TempWeekPlan[i][j].value = 1;
                        TempWeekPlan[go_to_sleep - 1][j].value = 1;
                        //strcpy(TempWeekPlan[go_to_sleep - 1][j].occasion, "Stupid");
                    }
                }
                for(i = 0; i <= wake_up; i++){
                    for(j = 0; j < 5; j++){
                        strcpy(TempWeekPlan[i][j].occasion, "Sleep"); 
                        TempWeekPlan[i][j].value = 1;
                        strcpy(TempWeekPlan[wake_up][j].occasion, "Breakfast");
                        TempWeekPlan[wake_up][j].value = 1;
                    }
                }
                for(i = go_to_sleep; i < 24; i++){ //For Søndagen
                    strcpy(TempWeekPlan[i][6].occasion, "Sleep"); 
                    TempWeekPlan[i][6].value = 1;
                    TempWeekPlan[go_to_sleep - 1][6].value = 1;
                    //strcpy(TempWeekPlan[go_to_sleep - 1][6].occasion, "Stupid");
                }

            } else if(go_to_sleep < 15){
                if((wake_up - go_to_sleep) < 8){
                    printf("The doctors recommend 8 hours of sleep a day\n");
                    printf("With this schedule you only get %d hours a day\n", (wake_up - go_to_sleep));
                    printf("Please reconsider this in the future\n");
                }
                for(i = go_to_sleep; i <= wake_up; i++){
                    for(j = 0; j < 5; j++){
                        strcpy(TempWeekPlan[i][j].occasion, "Sleep"); 
                        TempWeekPlan[i][j].value = 1;
                        strcpy(TempWeekPlan[wake_up][j].occasion, "Breakfast");
                        TempWeekPlan[wake_up][j].value = 1;
                        if(go_to_sleep == 0){
                            if(j == 0){
                                TempWeekPlan[23][6].value = 1;
                                //strcpy(TempWeekPlan[23][6].occasion, "Stupid");
                            } else {
                                TempWeekPlan[23][j - 1].value = 1;
                                //strcpy(TempWeekPlan[23][j - 1].occasion, "Stupid");
                            } 
                        } else {
                            TempWeekPlan[go_to_sleep - 1][j].value = 1;
                            //strcpy(TempWeekPlan[go_to_sleep - 1][j].occasion, "Stupid");
                        }
                    }
                }
            }
            printf("Please type the time you wish to go to sleep for weekends (which hour under 24?):\n");
            scanf(" %d", &go_to_sleep); //Weekenden
            printf("Please type the time you wish to wake up on weekends (which hour under 24?):\n");
            scanf(" %d", &wake_up);

            if(go_to_sleep > 15){ //Til hvis de går i seng inden midnat, tallet er tilfældigt
                if((wake_up + (24 - go_to_sleep)) < 8){
                    printf("The doctors recommend 8 hours of sleep a day\n");
                    printf("With this schedule you only get %d hours a day\n", (wake_up + (24 - go_to_sleep)));
                    printf("Please reconsider this in the future\n");
                }
                for(i = go_to_sleep; i < 24; i++){
                    for(j = 4; j < 6; j++){
                        strcpy(TempWeekPlan[i][j].occasion, "Sleep"); 
                        TempWeekPlan[i][j].value = 1;
                        TempWeekPlan[go_to_sleep - 1][j].value = 1;
                        //strcpy(TempWeekPlan[go_to_sleep - 1][j].occasion, "Stupid");
                    }
                }
                for(i = 0; i <= wake_up; i++){
                    for(j = 5; j < 7; j++){
                        strcpy(TempWeekPlan[i][j].occasion, "Sleep"); 
                        TempWeekPlan[i][j].value = 1;
                        strcpy(TempWeekPlan[wake_up][j].occasion, "Breakfast");
                        TempWeekPlan[wake_up][j].value = 1;
                    }
                }
            } else if(go_to_sleep < 15){
                if((wake_up - go_to_sleep) < 8){
                    printf("The doctors recommend 8 hours of sleep a day\n");
                    printf("With this schedule you only get %d hours a day\n", (wake_up - go_to_sleep));
                    printf("Please reconsider this in the future\n");
                }
                for(i = go_to_sleep; i <= wake_up; i++){
                    for(j = 5; j < 7; j++){
                        strcpy(TempWeekPlan[i][j].occasion, "Sleep"); 
                        TempWeekPlan[i][j].value = 1;
                        strcpy(TempWeekPlan[wake_up][j].occasion, "Breakfast");
                        TempWeekPlan[wake_up][j].value = 1;
                        if(go_to_sleep == 0){
                            if(j == 0){
                                TempWeekPlan[23][6].value = 1;
                                //strcpy(TempWeekPlan[23][6].occasion, "Stupid");
                            } else {
                                TempWeekPlan[23][j - 1].value = 1;
                                //strcpy(TempWeekPlan[23][j - 1].occasion, "Stupid");
                            } 
                        } else {
                            TempWeekPlan[go_to_sleep - 1][j].value = 1;
                            //strcpy(TempWeekPlan[go_to_sleep - 1][j].occasion, "Stupid");
                        }
                    }
                }
            }

        } else if(sleep == 2){ //Til hvis det kun er hverdagene

            printf("Please type the time you wish to go to sleep (which hour under 24?):\n");
            scanf(" %d", &go_to_sleep);
            printf("Please type the time you wish to wake up (which hour under 24?):\n");
            scanf(" %d", &wake_up);
        
            if(go_to_sleep > 15){ //Til hvis de går i seng inden midnat, tallet er tilfældigt
                if((wake_up + (24 - go_to_sleep)) < 8){
                    printf("The doctors recommend 8 hours of sleep a day\n");
                    printf("With this schedule you only get %d hours a day\n", (wake_up + (24 - go_to_sleep)));
                    printf("Please reconsider this in the future\n");
                }
                for(i = go_to_sleep; i < 24; i++){
                    for(j = 0; j < 4; j++){
                        strcpy(TempWeekPlan[i][j].occasion, "Sleep"); 
                        TempWeekPlan[i][j].value = 1;
                        TempWeekPlan[go_to_sleep-1][j].value = 1;
                        //strcpy(TempWeekPlan[go_to_sleep - 1][j].occasion, "Stupid");
                    }
                }
                for(i = 0; i <= wake_up; i++){
                    for(j = 0; j < 5; j++){
                        strcpy(TempWeekPlan[i][j].occasion, "Sleep"); 
                        TempWeekPlan[i][j].value = 1;
                        strcpy(TempWeekPlan[wake_up][j].occasion, "Breakfast");
                        TempWeekPlan[wake_up][j].value = 1;
                    }
                }
                for(i = go_to_sleep; i < 24; i++){ //For Søndagen
                    strcpy(TempWeekPlan[i][6].occasion, "Sleep"); 
                    TempWeekPlan[i][6].value = 1;
                    TempWeekPlan[go_to_sleep - 1][6].value = 1;
                    //strcpy(TempWeekPlan[go_to_sleep - 1][6].occasion, "Stupid");
                }

            } else if(go_to_sleep < 15){
                if((wake_up - go_to_sleep) < 8){
                    printf("The doctors recommend 8 hours of sleep a day\n");
                    printf("With this schedule you only get %d hours a day\n", (wake_up - go_to_sleep));
                    printf("Please reconsider this in the future\n");
                }
                for(i = go_to_sleep; i <= wake_up; i++){
                    for(j = 0; j < 5; j++){
                        strcpy(TempWeekPlan[i][j].occasion, "Sleep"); 
                        TempWeekPlan[i][j].value = 1;
                        strcpy(TempWeekPlan[wake_up][j].occasion, "Breakfast");
                        TempWeekPlan[wake_up][j].value = 1;
                        if(go_to_sleep == 0){
                            if(j == 0){
                                TempWeekPlan[23][6].value = 1;
                                //strcpy(TempWeekPlan[23][6].occasion, "Stupid");
                            } else {
                                TempWeekPlan[23][j - 1].value = 1;
                                //strcpy(TempWeekPlan[23][j - 1].occasion, "Stupid");
                            } 
                        }else {
                            TempWeekPlan[go_to_sleep - 1][j].value = 1;
                            //strcpy(TempWeekPlan[go_to_sleep - 1][j].occasion, "Stupid");
                        }
                    }
                }
            }
        }
    }
} 

void DoubleBooking(int tasks, int days, event firstPlan[tasks][days], event secondPlan[tasks][days], event tempPlan[tasks][days]) {
    int booking, choose;//TYPE!!
    int day, j, i;

    for (j = 0; j < 7; j++) {
        for (i = 0; i < 24; i++) {
            if ((firstPlan[i][j].value == 1) && (secondPlan[i][j].value == 1)) {
                day = j + 1;
                printf("Warning! You are currently overbooked on %s at %s.\n", weekdays[day], timeOfDay[i]);
                printf("Do you want to schedule event 1: %s or event 2: %s? Type 1 or 2: ", firstPlan[i][j].occasion, secondPlan[i][j].occasion);
                scanf(" %d", &booking);
                if (booking == 1) {
                    strcpy(tempPlan[i][j].occasion, firstPlan[i][j].occasion); //Alt dette kan erstattes siden man ikke skal gøre noget i dette tilfælde
                    tempPlan[i][j].value = 1;
                    printf("Do you want to reschedule the other event %s? For yes, type 1, for no, type 2: \n", secondPlan[i][j].occasion);
                    scanf(" %d", &choose);
                    if (choose == 1) {

                        ReassignD(tasks, days, tempPlan, tempPlan, i, j, secondPlan);
                        //break; 
                    }
                }
                if (booking == 2) {
                    strcpy(tempPlan[i][j].occasion, secondPlan[i][j].occasion);
                    tempPlan[i][j].value = 1;
                    printf("Do you want to reschedule the other event %s? For yes, type 1, for no, type 2: \n", firstPlan[i][j].occasion);
                    scanf(" %d", &choose);
                    if (choose == 1) {
                        ReassignD(tasks, days, tempPlan, tempPlan, i, j, firstPlan);
                        //break; 
                    }
                }
                //else
            }
            //Resten overføres direkte til tempPlan[][]
            else if (firstPlan[i][j].value == 1) { // else if ((firstPlan[][].value == 1) && (tempPlan[][].value == 0)
                strcpy(tempPlan[i][j].occasion, firstPlan[i][j].occasion);
                tempPlan[i][j].value = firstPlan[i][j].value;
            }
            else if (secondPlan[i][j].value == 1) {
                strcpy(tempPlan[i][j].occasion, secondPlan[i][j].occasion);
                tempPlan[i][j].value = secondPlan[i][j].value;
            }
        }
    }
}

void Prep_Lesson(int tasks, int days, event TempWeekPlan[tasks][days]) {
    int j, i, prep_time, preparation, count = 0;

    printf("How many hours a day, do you wish to spend preparing for lessons?\n");
    scanf(" %d", &prep_time);

    while (prep_time >= 4) {
        printf("Are you sure you want to spend %d hours daily preparing for lessons?\n", prep_time);
        printf("Please consider a lower amount of hours a days:\n");
        scanf(" %d", &prep_time);
    }

    for (j = 0; j < 4; j++) {
        count = 0;
        for (i = 0; i < 24; i++) {
            if (TempWeekPlan[i][j].value == 0 && count != prep_time) {
                strcpy(TempWeekPlan[i][j].occasion, "LP");
                TempWeekPlan[i][j].value = 1;
                count++;
            }
        }
    }
    count = 0;
    for (i = 0; i < 24; i++) {
        if (TempWeekPlan[i][6].value == 0 && count != prep_time) {
            strcpy(TempWeekPlan[i][6].occasion, "LP");
            TempWeekPlan[i][6].value = 1;
            count++;
        }
    }
}

//modtager i og j fra DoubleBooking() + variabel der fortæller om vi er kommet fra doubleBooking() eller ej + et array fra double-booking()
void ReassignD(int tasks, int days, event tempPlan[tasks][days], event reassignedarray[tasks][days], int i, int j, int fraDoubleBooking, event doublebookingarray[tasks][days]) {
    //Vi printer først TempWeekPlan[][] for at brugeren kan se kalenderen som den ser ud nu og har et overblik over den
    printDemoSchedule(24, 7, tempPlan);   
    int endday, endhour;  
    int run = 0;
    int confirm = 0; 
    int choose = 0; 

    while (run == 0) {         
        printf("You want to reschedule the event %s.", doublebookingarray[i][j].occasion); // yes or no? 
        printf("Please enter the day you wish to reschedule the event to: For Monday, type 0, for Tuesday type 1 and so on... \n");
        scanf(" %d", &endday);  //enum type or? 
        printf("Please enter the hour you wish to reschedule the event %s to: For the hour 00:00 - 01:00 type 0 and so on\n", doublebookingarray[i][j].occasion);
        scanf(" %d", &endhour);
        printf("You have chosen to reschedule the event %s to hour %d of day %d. To confirm, enter 1. \n", doublebookingarray[i][j].occasion, endhour, endday);
        scanf(" %d", &confirm);
        if (confirm == 1) {
            if (reassignedarray[endhour][endday].value != 0) { //reassignedarray er holdeværdi, tempPlan er returarray
                printf("It seems that another event is planned for hour %d of day %d. The event is %s, how do you want to continue?\n", endhour, endday, reassignedarray[endhour][endday].occasion);
                printf("If you want to overwrite the planned event %s with the unscheduled event %s and erase the old event, type 1\n", reassignedarray[endhour][endday].occasion, doublebookingarray[i][j].occasion);
                printf("If you want to find another time for the new event %s, and keep the old event %s, type 2\n", doublebookingarray[i][j].occasion, reassignedarray[endhour][endday].occasion);
                printf("If you want to reschedule the old event %s and keep the new event %s, type 3\n", reassignedarray[endhour][endday].occasion, doublebookingarray[i][j].occasion);
                scanf("%d", &choose);
            }
            if (choose == 1) {
                //Overwrites the old event with the new one
                strcpy(tempPlan[endhour][endday].occasion, doublebookingarray[i][j].occasion);
                tempPlan[endhour][endday].value = 1;
                run = 1; //terminates the loop and the function
            }
            if (choose == 2) {
                continue; //does this work?? In theory we should just jump back to the while look here and try again, could also be break;?
            }
            if (choose == 3) {
                //Vi skal lægge værdien af det event vi overskriver et andet sted hen, inklusiv elevtid?
               
                //Vi overskriver
                strcpy(tempPlan[endhour][endday].occasion, doublebookingarray[i][j].occasion);
                tempPlan[endhour][endday].value = 1;
                //Nu skulle vi gerne fortsætte tilbage i loopet 
                printf("You have now planned the event %s for hour %d on day %d.\nNow you will be asked about rescheduling event %s", tempPlan[endhour][endday].occasion, endhour, endday, reassignedarray[endhour][endday]); 
                strcpy(doublebookingarray[i][j].occasion, reassignedarray[endhour][endday].occasion);
                doublebookingarray[i][j].value = 1;
                strcpy(reassignedarray[endhour][endday].occasion, tempPlan[endhour][endday].occasion);
                reassignedarray[i][j].value = 1;
                //i = endhour; 
                //j = endday;
           
            }
            else if (confirm != 1) {
                printf("Choice for reassignment not confirmed, try again\n");
            }
        } 
        if (reassignedarray[endhour][endday].value == 0) {
            strcpy(reassignedarray[endhour][endday].occasion, doublebookingarray[i][j].occasion);
            reassignedarray[endhour][endday].value = 1;
            run = 1; 
        }
        else if (confirm != 1) {
            printf("Reassignment not confirmed, try again\n");
        }
    } 
    //Her til slut printes det rearrangerede array og man bliver sendt tilbage til videre til doubleBooking()
    printf("TempWeekPlan som den der ud i slutningen af ReassignD(); \n"); 
    printDemoSchedule(24, 7, tempPlan); 
}   

void ReassignEvent(int tasks, int days, event tempPlan[tasks][days], event reassignedarray[tasks][days]) {
    //Funktionen kaldes fra et andet sted end doublebooking: 
    
    //Vi printer først TempWeekPlan[][] for at brugeren kan se kalenderen som den ser ud nu og har et overblik over den
    printf("Start of ReassignD, print of TempPlanWeek: "); 
    printDemoSchedule(24, 7, tempPlan);

    int endday, endhour, startday, starthour, i, j;
    int run = 0;
    int confirm = 0;
    int choose = 0;

    //er det et problem at køre et while loop her? 
    //Det er godt idet kan reschedule flere events ad gangen men skal man så ikke samtidig køre et indre while loop eller flere
    //der styrer de ting som while-looppet styrede i ReassignD()?
    while (run = 0) {
        printf("Which event do you want to reschedule?\nPlease enter the day of the event you wish to reschedule: For Monday, type 0, for Tuesday type 1 and so on... \n");
        scanf(" %d", &startday);  //enum type or? 
        printf("Please enter the hour of the event you wish to reschedule: For the hour 00:00 - 01:00 type 0 and so on\n");
        scanf(" %d", &starthour);
        printf("You want to reschedule the event %s, in hour %d of day %d. To confirm, enter 1: ", tempPlan[starthour][startday].occasion, starthour, startday);
        scanf(" %d", &confirm);
        if (confirm == 1) {
            printf("Do you want to delete the event %s or reschedule it? For deletion, type 1, for rescheduling, type 2: \n", tempPlan[starthour][startday].occasion);
            scanf(" %d", &confirm);
            if (confirm == 1) {
                strcpy(reassignedarray[starthour][startday].occasion, ""); 
                reassignedarray[starthour][startday].value = 0;
                run = 1;
            }
            if (confirm == 2) {
                printf("Please enter the day you wish to reschedule the event %s to: For Monday, type 0, for Tuesday type 1 and so on... \n", tempPlan[starthour][startday].occasion);
                scanf(" %d", &endday);  //enum type or? 
                printf("Please enter the hour you wish to reschedule the event %s to: For the hour 00:00 - 01:00 type 0 and so on\n", tempPlan[starthour][startday].occasion);
                scanf(" %d", &endhour);
                printf("You have chosen to reschedule the event %s to hour %d of day %d. To confirm, enter 1. \n", tempPlan[starthour][startday].occasion, endhour, endday);
                scanf(" %d", &confirm);
                if (reassignedarray[endhour][endday].value != 0) {
                    printf("It seems that another event is planned for hour %d of day %d. The event is %s, how do you want to continue?\n", endhour, endday, reassignedarray[endhour][endday].occasion);
                    printf("If you want to overwrite the planned event %s with the unscheduled event %s and erase the old event, type 1\n", reassignedarray[endhour][endday].occasion, tempPlan[starthour][startday].occasion);
                    printf("If you want to find another time for the new event %s, and keep the old event %s, type 2\n", tempPlan[starthour][startday].occasion, reassignedarray[endhour][endday].occasion);
                    printf("If you want to reschedule the old event %s and keep the new event %s in the chosen time-slot, type 3\n", reassignedarray[endhour][endday].occasion, tempPlan[starthour][startday].occasion);
                    scanf(" %d", &choose);
                }
                if (choose == 1) {
                    //Overwrites the old event with the new one
                    strcpy(reassignedarray[endhour][endday].occasion, tempPlan[starthour][startday].occasion);
                    reassignedarray[endhour][endday].value = 1;
                    run = 1; //terminates the loop and the function
                }
                if (choose == 2) {
                    continue; //does this work?? In theory we should just jump back to the while look here and try again, could also be break;?
                }
                if (choose == 3) {
                    //first I need to store the value of reassignedarray[endhour][endday] in another spot (in tempPlan[][])
                    for (int j = 0; j < 7; j++) {
                        for (int i = 0; i < 24, i++) {
                            if ((tempPlan[i][j].occasion == "") && (tempPlan[i][j].value == 0)) {
                                strcpy(tempPlan[i][j].occasion, reassignedarray[endhour][endday].occasion);
                                tempPlan[i][j].value = 1;

                            }

                        }

                    }
                    //Så er værdierne lagt over på en tom plads
                    //Nu lægges tempPlan[starthour][startday] værdierne over i reassignedarray[endhour][endday]

                    strcpy(reassignedarray[endhour][endday].occasion, tempPlan[starthour][startday].occasion);
                    reassignedarray[endhour][endday].value = 1;
                    
                    //fra gammel funktion: 
                    
                    //Nu skulle vi gerne fortsætte tilbage i loopet 
                    printf("You have now planned the event %s for hour %d on day %d.\nNow you will be asked about rescheduling event %s", tempPlan[endhour][endday].occasion, endhour, endday, reassignedarray[endhour][endday]);
                    strcpy(doublebookingarray[starthour][startday].occasion, reassignedarray[endhour][endday].occasion);
                    doublebookingarray[starthour][startday].value = 1;
                    strcpy(reassignedarray[endhour][endday].occasion, tempPlan[endhour][endday].occasion);
                    reassignedarray[starthour][startday].value = 1;
                    //slut på gammel funktion
                }
                else if (confirm != 1) {
                    printf("Choice for reassignment not confirmed, try again\n");
                }
            }
            else if (confirm != 1) {
                printf("Choice for reassignment not confirmed, try again\n");
            }
        }
        else if (confirm != 1) {
            printf("Do you want to reschedule other events? For yes, type 1, for no, type 2: ")
            scanf("%d", choose); 
            if (choose == 1) {
                printf("TempWeekPlan i ReassignEvent(), choose another event to reschedule; \n");
                printDemoSchedule(24, 7, reassignedarray);
                continue; 
            }
            if (choose == 2) {
                run = 1; 
            }
            printf("Event for reassignment not confirmed, try again(?)\n");
        }
    }
    //Her til slut printes reassignedarray[][].occasion værdierne og man bliver sendt videre til accept()
    printf("TempWeekPlan som den der ud i slutningen af ReassignEvent(); \n");
    printDemoSchedule(24, 7, reassignedarray);
}