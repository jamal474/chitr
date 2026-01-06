#include <chitr/CTime.h>
#include <string>
#include <format>

CTime::CTime() {
    hour = 0;
    minute = 0;
    second = 0;
    milisecond = 0;
}

CTime::CTime(long long mseconds) {
    calculate(mseconds);
}

void CTime::calculate(long long mseconds) {

    milisecond = mseconds;
    hour = mseconds / 3600000;
    mseconds %= 3600000;      
    minute = mseconds / 60000;
    mseconds %= 60000;       
    second = mseconds / 1000; 
}

void CTime::setTime(long long mseconds) {
    calculate(mseconds);
}

long long CTime::getMiliseconds() const {
    return milisecond;
}

CTime::operator std::string() const {
    

    std::string timeString = "";
    std::string hourString = "", minuteString = "", secondString = "";

    if (hour == 0) {
        hourString = "";
    } else if (hour <= 9) {
        hourString = std::format("0{}",hour);
    } else {
        hourString = std::format("{}", hour);
    }

    if (minute == 0) {
        minuteString = "00";
    } else if (minute <= 9) {
        minuteString = std::format("0{}",minute);
    } else {
        minuteString = std::format("{}", minute);
    }

    if (second == 0) {
        secondString = "00";
    } else if (second <= 9) {
        secondString = std::format("0{}",second);
    } else {
        secondString = std::format("{}", second);
    }
    
    if(hourString == "") {
        timeString = minuteString + ":" + secondString;
    } else {
        timeString = hourString + ":" + minuteString + ":" + secondString;
    }
    return timeString;
}

std::string CTime::timeString(long long mseconds) {
    
    CTime tempTime(mseconds);
    return tempTime;
}