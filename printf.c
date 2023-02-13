#include "printf.h"
#include <stdarg.h>
#include <stdint.h>
#include "strings.h"
#include "uart.h"

int unsigned_to_base(char *buf, 
                     size_t bufsize, 
                     unsigned int val, 
                     int base, size_t 
                     min_width);
int signed_to_base(char *buf, 
                   size_t bufsize, 
                   int val, 
                   int base, 
                   size_t min_width);


#define MAX_OUTPUT_LEN 1024

unsigned int get_int_size(unsigned int val, int base){ // get the number of digits in an integer
    int count = 0;
    if(val == 0){
        return 1;
    }
    while (val != 0){ // continually cut down on val until reaching zero; the number of iterations is equal to the number of digits
        unsigned int rem = val % base; 
        val = val - rem;
        count++;
        val /= base; 
    }
    return count;
}

unsigned char get_ascii(unsigned int digit){
    unsigned char ascii = 0;
    if (digit >= 0 && digit <= 9){ // if char is 0-9
            ascii = digit + 48; // for now; implement hex later if digit between 11 and 15 or whatever
        }
    if (digit >= 10 && digit <= 15){ // if hex a-f
             ascii = digit + 87; // for now; implement hex later if digit between 11 and 15 or whatever
        }
    return ascii;
}


int unsigned_to_base(char *buf, size_t bufsize, unsigned int val, int base, size_t min_width)
{
    if (bufsize == 0){
        return min_width; // equal to chars written if there were space
    }

    unsigned int int_size = get_int_size(val, base);
    int num_zeros = 0;
    
    if(min_width > int_size){
        num_zeros = min_width - int_size;
    }

    char temp[int_size + num_zeros + 1]; // will hold the characters in reverse order
    int temp_index = 0;
    int buf_index = 0;

    
    if(val == 0){ // edge case
        temp[temp_index] = '0';
        temp_index++;
    }
    // add ascii value of each integer of val to temp
    while(val != 0){
        int digit = val % base;
        val -= digit;
        val /= base;
        temp[temp_index] = get_ascii(digit);
        temp_index++;
    }

    // add zeros to end of temp if necessary!
    for(int t = temp_index; t < temp_index + num_zeros; t++){
        temp[t] = '0';
    }
    temp_index = int_size + num_zeros - 1; // temp_index should be equal to int_size + numzeros - 1 and points to last digit in temp
   
    // what if we have a buffer space issue?
    size_t max_space = 0; // max digits of input, including the nullptr
    if (min_width >= int_size){
        max_space = min_width + 1;
    }else {
        max_space =int_size + 1;
    }
    if(max_space > bufsize){
        for(int i = 0; i < bufsize - 1; i++){ // truncate the output
            buf[i] = temp[temp_index - i];
        }
        buf[bufsize - 1] = '\0'; // store a null terminator in buf[bufsize - 1]
    }
    else{ // under assumption that buf is large enough for temp
    
    while(temp_index != 0){ 
        buf[buf_index] = temp[temp_index];
        temp_index--;
        buf_index++;
    }
    buf[buf_index] = temp[temp_index]; // make sure to write what is at temp_index = 0
    // buf_index is now pointing to the last digit in buf
    buf[buf_index + 1] = '\0'; // increment buf_index to place nullptr

    }

    return int_size + num_zeros;

}

int signed_to_base(char *buf, size_t bufsize, int val, int base, size_t min_width)
{
    if(val >= 0){ // if val is NOT neg, then treat as if unsigned
        return unsigned_to_base(buf, bufsize, val, base, min_width);
    }
    else{ // else treat as if signed
    if (bufsize == 0){
        return min_width; // equal to chars written if there were space
    }
        buf[0] = '-';  // enter ascii value for '-' char for first in buf
        // move the pointer for buf so that you don't include minus sign
        if (min_width == 0){
            min_width = 1; // prevent case where min_width is passed in as -1
        }
        return unsigned_to_base(buf + 1, bufsize - 1, val *-1, base, min_width - 1) + 1;
    }
}

int vsnprintf(char *buf, size_t bufsize, const char *format, va_list arg)
{
    char temp[1024];
    size_t tempsize = sizeof(temp);
    memset(temp, '\0', tempsize);
    memset(buf, '\0', bufsize);
    int t = 0; // temp index
    int i = 0; // format index
    while(format[i] != '\0'){ // continue iterating through format
        t = strlen(temp); // continuously update index of temp (t) to the end of temp
        if(format[i] != '%'){ 
            temp[t] = format[i]; // copy char normally
            i++;
        }
        else{ // under assumption that codes formats are correct
            i++; // check the next char for a code
            char code = format[i];
            if (code == '%'){
                temp[t] = '%';
            }
            else if (code == 'c'){
                temp[t] = va_arg(arg, int);
            }
            else if (code == 's'){
                const char *s = va_arg(arg, const char*);
                strlcat(temp, s, tempsize - strlen(temp)); // adjust tempsize to reflect number of spaces left
            }
            else if (code == 'd'){
                const int d = va_arg(arg, int);
                signed_to_base(&temp[t], tempsize - strlen(temp), d, 10, 0);
            }
            else if (code == 'x'){
                const int x = va_arg(arg, int);
                signed_to_base(&temp[t], tempsize - strlen(temp), x, 16, 0);
            }
            else if (code == '0'){
                i++; // get to next char
                unsigned int width = strtonum(&format[i], NULL); 
                int width_size = get_int_size(width, 10); // get number of digits in width to know how much to increment i
                i += width_size;
                if (format[i] == 'd'){
                const int d = va_arg(arg, int);
                signed_to_base(&temp[t], tempsize - strlen(temp), d, 10, width);
                }
                if (format[i] == 'x'){
                const int x = va_arg(arg, int);
                signed_to_base(&temp[t], tempsize - strlen(temp), x, 16, width);
                }
            }
            else if (code == 'p'){
                const int p = va_arg(arg, int);
                strlcat(&temp[t], "0x", tempsize - strlen(temp));
                t += 2; // increment t to skip over the 0x
                signed_to_base(&temp[t], tempsize - strlen(temp), p, 16, 8);
            }
        i++;
        }
        }


    strlcat(buf, temp, tempsize);
    return strlen(buf);
}

int snprintf(char *buf, size_t bufsize, const char *format, ...)
{
    va_list arg;
    va_start(arg, format);
    int res = vsnprintf(buf, bufsize, format, arg);
    va_end(arg);
    return res;

    }

int printf(const char *format, ...)
{
    char input[MAX_OUTPUT_LEN];
    va_list arg;
    va_start(arg, format);
    int res = vsnprintf(input, MAX_OUTPUT_LEN, format, arg);
    uart_putstring(input);
    va_end(arg);
    return res;
}

