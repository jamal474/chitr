#pragma once
#include <string>

class CTime {

private:
    void calculate(long long ); 

    long long hour;
    long long minute;
    long long second;
    long long milisecond;
    
public:
    CTime();
    CTime(long long );
    void setTime(long long );
    long long getMiliseconds() const;
    operator std::string() const;
    static std::string timeString(long long);
};