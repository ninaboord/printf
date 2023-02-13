#include "strings.h"

// copy contents from src to dst one byte at a time
void *memcpy(void *dst, const void *src, size_t n)
{

    char *d = dst;
    const char *s = src;
    while (n--) {
        *d++ = *s++;
    }
    return dst;
}

// set a particular space in memory to a specific value (for debugging purpouses to avoid confusing garbage values)
void *memset(void *dst, int val, size_t n)
{
    unsigned char least_sig_bit = 255&val; // do this in order to get only last 8 bits of val
    char *d = dst;
    while(n--){
        *d++ = least_sig_bit;
    }
    return dst;
}


size_t strlen(const char *str)
{
    /* Implementation a gift to you from lab3 */
    size_t n = 0;
    while (str[n] != '\0') {
        n++;
    }
    return n;
}

int strcmp(const char *s1, const char *s2)
{
while(*s1 == *s2 && *s1 != '\0' && *s2 != '\0'){ // while string chars are exactly the same
    s1++;
    s2++;
}
return *s1 - *s2;

}

// concatinates two strings given a dstsize limit. if concatination is larger than dst size, truncate output. this prevents unwanted memory overwriting.
size_t strlcat(char *dst, const char *src, size_t dstsize)
{
    unsigned int valid = 0;
    unsigned int length_src = strlen(src);

    for (int i = 0; i < dstsize; i++){ // check if dstsize is correct and that dst is a proper string (exists a nullptr in dst)
        if (dst[i] == '\0'){
            valid = 1;
        }
    }

    if (valid == 0){
        return (dstsize + length_src);
    }
 
    unsigned int start_concat_index = strlen(dst);
    unsigned int space_left = dstsize - start_concat_index - 1; // this is the number of spaces left in dst
    dst += start_concat_index; // start off dst after any chars already in dst
    if (length_src > space_left){ // if length of src string is too large for dst
        dst = memcpy(dst, src, space_left - 1); // leave room for nullptr at end
        dst += space_left; // increment dst so you can add nullptr
    } else {
        dst = memcpy(dst, src, length_src); // if you have enough room, just copy over src to dst
        dst += length_src;
    }

    *dst = '\0';
    return (start_concat_index + length_src);

// i ended up coding this function in two different ways. they both work though! I kept this just in case I ran into a bug with strlcat
    // unsigned int start_concat_index = strlen(dst);
    // unsigned int curr_src_index = 0;
    // unsigned int curr_dst_index = start_concat_index;

    // while ((curr_dst_index < (dstsize - 1)) && (*src != '\0')){
    //     dst[curr_dst_index] = *src;
    //     curr_dst_index++;
    //     src++;
    // }
    // dst[curr_dst_index] = '\0'; // set end of dst to nullptr to create a string

}

// returns 1 if a string is a hex number and 0 if it is a decimal number
unsigned int isHex(const char *str){
    unsigned int hex = 0;
    if(*str == 48){ // if first char is a 0
        str++;
        if (*str == 120){ // if next char is an x
            hex = 1;
        }
    }

    return hex;
}

// converts a string into a number and changes endptr to point to the char in the string where strtonum stopped (either the nullptr or first invalid character)
unsigned int strtonum(const char *str, const char **endptr)
{
    unsigned int n = 0;
    unsigned int value = 0;
    if (isHex(str) == 1){
        str += 2; // skip the 0x part of the string
        while ((*str >= 48 && *str <= 57) || (*str >= 65 && *str <= 70) || (*str >= 97 && *str <= 102)){ // while char is a valid hex char
            if (*str >= 48 && *str <= 57){ // if char is 0-9
                value = *str - 48;
            }

            if (*str >= 65 && *str <= 70){ // if char is A-F
                value = *str - 55;
            }

            if (*str >= 97 && *str <= 102){ // if char a-f 
                value = *str - 87;
            }

            n *= 16; // this is how you add to n so that you get the number you want
            n += value;
            str++;

        }
    }

    else { // if decimal
    
    while (*str >= 48 && *str <= 57){ // if char is 0-9
        value = *str - 48;
        n *= 10; // same strategy of getting n as with hex but with base-10
        n += value;
        str++;
    }

    }
    if (endptr != NULL){
        *endptr = &(*str); // the address of str should be stored where endptr is pointing
    }
    
    return n;


    }

