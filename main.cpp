#include<iostream>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<linux/i2c.h>
#include<linux/i2c-dev.h>

#define PCF8523_ADDR 0x68

using namespace std;

int bcdToDec(char in)
{
    return (in/16)*10 + (in%16);
}

int main()
{
    int file;
    cout << "RTC PCF8523 Test application\n";
    if((file=open("/dev/i2c-1", O_RDWR)) < 0)
    {
        perror("Failed to open bus");
        return 1;
    }
    if(ioctl(file, I2C_SLAVE, PCF8523_ADDR) < 0)
    {
        perror("Failed to connect to RTC\n");
        return 1;
    }
    
    char writeBuffer[1] = {0x00};
    if(write(file, writeBuffer, 1) != 1)
    {
        perror("Failed to set read address\n");
        return 1;
    }

    int bytesToRead = 10;
    char buffer[bytesToRead];
    if(read(file, buffer, bytesToRead) != bytesToRead)
    {
        perror("Failed to read in the buffer\n");
        return 1;
    }


    bool twelve_hour_format = (buffer[0] & 0x01) == 0x01;
    bool battery_low = (buffer[2] & 0x02) == 0x02;
    cout << "Battey Status: " << (battery_low ? "Low\n" : "Good\n");

    cout << "UTC date and time\n";
    char seconds = buffer[3];
    if((seconds & 0x90) == 0x90)
    {
        cout << "clock integrity is not guaranteed; oscillator has stopped or been interrupted\n";
        seconds &= 0x7F;
    }
    char minutes = buffer[4];
    char hours = twelve_hour_format ? (buffer[5] & 0x1F) : buffer[5];


    if(twelve_hour_format)
    {
        bool pm = (buffer[5] & 0x10) == 0x10;
        cout << "12h Time (h:m:s am/pm)\t" << bcdToDec(hours) << ":" << bcdToDec(minutes) << ":" << bcdToDec(seconds) << pm ? " pm\n" : " am\n";
    }
    else
    {
        cout << "24h Time (h:m:s)\t\t" << bcdToDec(hours) << ":" << bcdToDec(minutes) << ":" << bcdToDec(seconds) << "\n";
    }

    int day = bcdToDec(buffer[6]);
    int month = bcdToDec(buffer[8]);
    int year = bcdToDec(buffer[9]);
    cout << "Date (d/M/y)\t\t\t" << day << "/" << month << "/" << year << "\n";

    char weekday = buffer[7];
    string str_weekday;
    switch(weekday)
    {
        case 0:
        str_weekday = "Sunday";
        break;
        case 1:
        str_weekday = "Monday";
        break;
        case 2:
        str_weekday = "Tuesday";
        break;
        case 3:
        str_weekday = "Wednesday";
        break;
        case 4:
        str_weekday = "Thursday";
        break;
        case 5:
        str_weekday = "Friday";
        break;
        case 6:
        str_weekday = "Saturday";
        break;
    }

    cout << "Day\t\t\t\t" << str_weekday << "\n";
    close(file);
}

