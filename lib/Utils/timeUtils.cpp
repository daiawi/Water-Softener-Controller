#include "timeUtils.h"

void createTimeString(uint8_t days, uint8_t hours, uint8_t minutes, uint8_t seconds, char* formattedTime){
    if (days == 0) {
        // Format as hh:mm
        formattedTime[0] = '0' + (hours / 10);     // Tens place of hours
        formattedTime[1] = '0' + (hours % 10);     // Ones place of hours
        formattedTime[2] = ':';                    // Colon separator
        formattedTime[3] = '0' + (minutes / 10);   // Tens place of minutes
        formattedTime[4] = '0' + (minutes % 10);   // Ones place of minutes
        formattedTime[5] = ':';
        formattedTime[6] = '0' + (seconds / 10);
        formattedTime[7] = '0' + (seconds % 10);
        formattedTime[8] = '\0';                   // Null-terminate the string
    } else {
        // Format as mm:ss (for times less than 1 hour)
        formattedTime[0] = '0' + (days / 10);       // Tens place of days
        formattedTime[1] = '0' + (days % 10);   // Ones place of minutes
        formattedTime[2] = ' ';                    // Colon separator
        formattedTime[3] = 'D';   // Tens place of seconds
        formattedTime[4] = 'A';   // Ones place of seconds
        formattedTime[5] = 'Y';
        formattedTime[6] = 'S';
        formattedTime[7] = '\0';                   // Null-terminate the string
    }
}

void createDateString(uint8_t year, uint8_t month, uint8_t day, char* formattedDate){
    // Format as hh:mm
    formattedDate[0] = '0' + (month / 10);     // Tens place of hours
    formattedDate[1] = '0' + (month % 10);     // Ones place of hours
    formattedDate[2] = ':';                    // Colon separator
    formattedDate[3] = '0' + (day / 10);        // Tens place of minutes
    formattedDate[4] = '0' + (day % 10);        // Ones place of minutes
    formattedDate[5] = ':';
    formattedDate[6] = '0' + (year / 10);
    formattedDate[7] = '0' + (year % 10);
    formattedDate[8] = '\0';
}

void formatTime(DateTime clock, char* formattedTime){
    uint8_t day = 0;
    uint8_t hour = clock.hour();
    uint8_t minute = clock.minute();
    uint8_t second = clock.second();
    createTimeString(day, hour, minute, second, formattedTime);
}

void formatTime(TimeSpan countdown, char* formattedTime){
    uint8_t days = countdown.days();
    uint8_t hours = countdown.hours();
    uint8_t minutes = countdown.minutes();
    uint8_t seconds = countdown.seconds();
    createTimeString(days, hours, minutes, seconds, formattedTime);
}

void formatDate(DateTime clock, char* formattedDate){
    uint8_t year = clock.year() - 2000;
    uint8_t month = clock.month();
    uint8_t day = clock.day();
    createDateString(year, month, day, formattedDate);
}