#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*#define HOURS 24 Kan vi sagtens arbejde videre med, det ville give rigtigt god mening at have symbolske konstanter for disse to værdier. 
  #define DAYS 7*/

struct event {
    char occasion[10]; //er 10-tallet antallet af characters der er plads til i char-strengen
    unsigned int value; //vigigt at alle starter med 0
    //int priority; //mangler prioritering + evt andet?
    int elevtid;
}
event_default = { "", 0 }; //Forloop l�ngere nede. Elevtid laves ikke til default idet det er un�dvendigt
typedef struct event event;

void printDemoSchedule(int tasks, int days, event display[tasks][days]);
void accept(int tasks, int days, event temp[tasks][days], event planned[tasks][days]); 
void sleep(int tasks, int days, event TempWeekPlan[tasks][days]);


int main(void) {
    int i, j;
    int tasks = 24, days = 7; //oprindeligt tasks = 4
   
    /*Jeg t�nker at vi er n�dt til at lave kalenderen dag for dag og s� hver event er koblet til en ugedag og
    ikke bare til en plads i et to-dimensionelt array, but I don't know */
    /*Vi pr�ver:
    F�rst med et to-dimensionelt array*/
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
    for (j = 0; j < 7; j++) {
        ConstPlan[7][j].value = 1;
        strcpy(ConstPlan[7][j].occasion, "Breakfast");
    }
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

    //De tre arrays af structs l�gges sammen over i TempWeekPlan og der tjekkes for dobbeltbooking:
    int booking; //TYPE!!
    for (j = 0; j < 7; j++) {
        for (i = 0; i < 24; i++) {
            if ((ConstPlan[i][j].value == 1) && (SchoolPlan[i][j].value == 1)) {
                printf("Warning! You are currently overbooked. Do you want to schedule event 1: %s or event 2: %s? Type 1 or 2: ",
                       ConstPlan[i][j].occasion, SchoolPlan[i][j].occasion); //tidspunkt og dag i ugen
                scanf("%d", &booking); 
                if (booking == 1) {
                    strcpy(TempWeekPlan[i][j].occasion, ConstPlan[i][j].occasion); 
                    TempWeekPlan[i][j].value = 1;
                }
                if (booking == 2) {
                    strcpy(TempWeekPlan[i][j].occasion, SchoolPlan[i][j].occasion);
                    TempWeekPlan[i][j].value = 1;
                }
                //else
            }
            if ((ConstPlan[i][j].value == 1) && (SocialPlan[i][j].value == 1)) {
                printf("Warning! You are currently overbooked. Do you want to schedule event 1: %s or event 2: %s? Type 1 or 2: ",
                    ConstPlan[i][j].occasion, SocialPlan[i][j].occasion); //tidspunkt og dag i ugen
                scanf("%d", &booking);
                if (booking == 1) {
                    strcpy(TempWeekPlan[i][j].occasion, ConstPlan[i][j].occasion);
                    TempWeekPlan[i][j].value = 1;
                }
                if (booking == 2) {
                    strcpy(TempWeekPlan[i][j].occasion, SocialPlan[i][j].occasion);
                    TempWeekPlan[i][j].value = 1;
                }
                //else
            }
            if ((SocialPlan[i][j].value == 1) && (SchoolPlan[i][j].value == 1)) {
                printf("Warning! You are currently overbooked. Do you want to schedule event 1: %s or event 2: %s? Type 1 or 2: ",
                    SocialPlan[i][j].occasion, SchoolPlan[i][j].occasion); //tidspunkt og dag i ugen
                scanf("%d", &booking);
                if (booking == 1) {
                    strcpy(TempWeekPlan[i][j].occasion, SocialPlan[i][j].occasion);
                    TempWeekPlan[i][j].value = 1;
                }
                if (booking == 2) {
                    strcpy(TempWeekPlan[i][j].occasion, SchoolPlan[i][j].occasion);
                    TempWeekPlan[i][j].value = 1;
                }
                //else
            }
            //Vi l�gger resten direkte over i TempWeekPlan[][]
            else if (SchoolPlan[i][j].value == 1) {
                strcpy(TempWeekPlan[i][j].occasion, SchoolPlan[i][j].occasion);
                TempWeekPlan[i][j].value = SchoolPlan[i][j].value;
            }
            else if (ConstPlan[i][j].value == 1) {
                strcpy(TempWeekPlan[i][j].occasion, ConstPlan[i][j].occasion);
                TempWeekPlan[i][j].value = ConstPlan[i][j].value;
            }
            else if (SocialPlan[i][j].value == 1) {
                strcpy(TempWeekPlan[i][j].occasion, SocialPlan[i][j].occasion);
                TempWeekPlan[i][j].value = SocialPlan[i][j].value;
            }    
        }
    }

    printf("TempWeekPlan: \n");
    printDemoSchedule(24, 7, TempWeekPlan);

    accept(24, 7, TempWeekPlan, PlannedWeek);

    printf("PlannedWeek: \n");
    printDemoSchedule(24, 7, PlannedWeek); 

    return 0; //fordi det er main, det skal ud i vores program, der er main i dette program en void-funktion (?)
}

void printDemoSchedule(int tasks, int days, event display[tasks][days]){
    int i, j;
    for (i = 0; i < 24; i++) {
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
        printf("Idiot\n"); //Mere professionelt tak!
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

        if(go_to_sleep > 15){ //Til hvis de går i seng inden midnat, tallet er tilfældigt
            for(i = go_to_sleep; i < 24; i++){
                for(j = 0; j < 7; j++){
                    strcpy(TempWeekPlan[i][j].occasion, "Sleep"); 
                    TempWeekPlan[i][j].value = 1;
                    printf(" %s", TempWeekPlan[i][j].occasion);
                } printf("\n");
            }
            for(i = 0; i < wake_up; i++){
                for(j = 0; j < 7; j++){
                    strcpy(TempWeekPlan[i][j].occasion, "Sleep"); 
                    TempWeekPlan[i][j].value = 1;
                    printf(" %s", TempWeekPlan[i][j].occasion);
                } printf("\n");
            }
        } else if(go_to_sleep < 15){ //Hvis de går i seng efter midnat
            for(i = go_to_sleep; i < wake_up; i++){
                for(j = 0; j < 7; j++){
                    strcpy(TempWeekPlan[i][j].occasion, "Sleep"); 
                    TempWeekPlan[i][j].value = 1;
                    printf(" %s", TempWeekPlan[i][j].occasion);
                } printf("\n");
            }
        }
        printf("Thank you\n");
    } else if(sleep == 2){ //Til hvis de har to forskellige (eller bare en søvnskema til hverdagene)
        printf("Do you wish a sleeping schedule for your school days and weekend (1) or just the school days(2)?\n");
        scanf(" %d", &sleep);
        if(sleep == 1){ //Til to forskellige søvnskemaer, 1 til hverdagene og 1 til weekenden

            printf("Please type the time you wish to go to sleep for school days (which hour under 24?):\n"); //Hverdagene
            scanf(" %d", &go_to_sleep);
            printf("Please type the time you wish to wake up on school days (which hour under 24?):\n");
            scanf(" %d", &wake_up);
        
            if(go_to_sleep > 15){ //Til hvis de går i seng inden midnat, tallet er tilfældigt
                for(i = go_to_sleep; i < 24; i++){
                    for(j = 0; j < 5; j++){
                        strcpy(TempWeekPlan[i][j].occasion, "Sleep"); 
                        TempWeekPlan[i][j].value = 1;
                        printf(" %s", TempWeekPlan[i][j].occasion);
                    } printf("\n");
                }
                for(i = 0; i < wake_up; i++){
                    for(j = 0; j < 5; j++){
                        strcpy(TempWeekPlan[i][j].occasion, "Sleep"); 
                        TempWeekPlan[i][j].value = 1;
                        printf(" %s", TempWeekPlan[i][j].occasion);
                    } printf("\n");
                }
                for(i = go_to_sleep; i < 24; i++){ //For Søndagen
                    strcpy(TempWeekPlan[i][6].occasion, "Sleep"); 
                    TempWeekPlan[i][6].value = 1;
                    printf(" %s", TempWeekPlan[i][6].occasion);
                } printf("\n");
                for(i = 0; i < wake_up; i++){
                        strcpy(TempWeekPlan[i][6].occasion, "Sleep"); 
                        TempWeekPlan[i][6].value = 1;
                        printf(" %s", TempWeekPlan[i][6].occasion);
                } printf("\n");

            } else if(go_to_sleep < 15){
                for(i = go_to_sleep; i < wake_up; i++){
                    for(j = 0; j < 5; j++){
                        strcpy(TempWeekPlan[i][j].occasion, "Sleep"); 
                        TempWeekPlan[i][j].value = 1;
                        printf(" %s", TempWeekPlan[i][j].occasion);
                    } printf("\n");
                }
                for(i = go_to_sleep; i < wake_up; i++){
                    strcpy(TempWeekPlan[i][6].occasion, "Sleep"); 
                    TempWeekPlan[i][6].value = 1;
                    printf(" %s", TempWeekPlan[i][6].occasion);
                } printf("\n");
            }
            printf("Please type the time you wish to go to sleep for weekends (which hour under 24?):\n");
            scanf(" %d", &go_to_sleep); //Weekenden
            printf("Please type the time you wish to wake up on weekends (which hour under 24?):\n");
            scanf(" %d", &wake_up);

            if(go_to_sleep > 15){ //Til hvis de går i seng inden midnat, tallet er tilfældigt
                for(i = go_to_sleep; i < 24; i++){
                    for(j = 4; j < 6; j++){
                        strcpy(TempWeekPlan[i][j].occasion, "Sleep"); 
                        TempWeekPlan[i][j].value = 1;
                        printf(" %s", TempWeekPlan[i][j].occasion);
                    } printf("\n");
                }
                for(i = 0; i < wake_up; i++){
                    for(j = 5; j < 7; j++){
                        strcpy(TempWeekPlan[i][j].occasion, "Sleep"); 
                        TempWeekPlan[i][j].value = 1;
                        printf(" %s", TempWeekPlan[i][j].occasion);
                    } printf("\n");
                }
            } else if(go_to_sleep < 15){
                for(i = go_to_sleep; i < wake_up; i++){
                    for(j = 5; j < 7; j++){
                        strcpy(TempWeekPlan[i][j].occasion, "Sleep"); 
                        TempWeekPlan[i][j].value = 1;
                        printf(" %s", TempWeekPlan[i][j].occasion);
                    } printf("\n");
                }
            }

            printf("1");
        } else if(sleep == 2){ //Til hvis det kun er hverdagene

            printf("Please type the time you wish to go to sleep (which hour under 24?):\n");
            scanf(" %d", &go_to_sleep);
            printf("Please type the time you wish to wake up (which hour under 24?):\n");
            scanf(" %d", &wake_up);
        
            if(go_to_sleep > 15){ //Til hvis de går i seng inden midnat, tallet er tilfældigt
                for(i = go_to_sleep; i < 24; i++){
                    for(j = 0; j < 4; j++){
                        strcpy(TempWeekPlan[i][j].occasion, "Sleep"); 
                        TempWeekPlan[i][j].value = 1;
                        printf(" %s", TempWeekPlan[i][j].occasion);
                    } printf("\n");
                }
                for(i = 0; i < wake_up; i++){
                    for(j = 0; j < 5; j++){
                        strcpy(TempWeekPlan[i][j].occasion, "Sleep"); 
                        TempWeekPlan[i][j].value = 1;
                        printf(" %s", TempWeekPlan[i][j].occasion);
                    } printf("\n");
                }
                for(i = go_to_sleep; i < 24; i++){ //For Søndagen
                    strcpy(TempWeekPlan[i][6].occasion, "Sleep"); 
                    TempWeekPlan[i][6].value = 1;
                    printf(" %s", TempWeekPlan[i][6].occasion);
                } printf("\n");

            } else if(go_to_sleep < 15){
                for(i = go_to_sleep; i < wake_up; i++){
                    for(j = 0; j < 5; j++){
                        strcpy(TempWeekPlan[i][j].occasion, "Sleep"); 
                        TempWeekPlan[i][j].value = 1;
                        printf(" %s", TempWeekPlan[i][j].occasion);
                    } printf("\n");
                }
            }
            printf("2");
        }
        printf("Thank you\n");
    }
}