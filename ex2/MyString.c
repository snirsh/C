#include <stdlib.h>
#include <stdio.h>

#define STR_END '\0'

/**
 * this function calculates the size of given string
 * @param str to check
 * @return unsigned it size of str
 */
unsigned int strLength(const char *str)
{
    unsigned int len = 0;
    while(str[++len] != STR_END);
    return len;
}


/**
 * Counts the amount of str1 substrings that are equal to str2.
 * In case one (or two) of the strings is empty- returns 0.
 * @str1 - the string
 * @str2 -  the substring
 * @isCycic != 0 - search also for cyclic appearnce
 * @return number appearance of str2 as substring of str1
 */
unsigned int countSubStr(const char *str1, const char *str2, int isCyclic)
{
    unsigned int str1Len = strLength(str1);
    unsigned int str2Len = strLength(str2);
    if(str1Len == 0 || str2Len == 0)
    {
        return 0;
    }
    /**
     * if cyclic is 1 we add the first (str2Len-1) chars of str1 to str1's ending
     */
    if(isCyclic)
    {
        unsigned int total = str1Len + str2Len - 1;
        char *temp = malloc(total);
        for(unsigned int i = 0; i < total; i++)
        {
            temp[i] = str1[i % str1Len];
        }
        str1 = temp;
        str1Len = total;
        free(temp);
    }
    else if(str2Len > str1Len)
    {
        return 0;
    }
    int flag = 0, matches = 0;
    /**
     * MATCHES COUNTER ALGORITHM:
     * N = str1 Length, M = str2 Length.
     *
     * every time we look at a "bucket" which will be the size of N-M
     * we take from it a portion in the length of M if it's a match we continue
     * else we go on to a new bucket and reset our flag.
     * if we finished a bucket and the flag is still 1 then we got a match and we reset the flag for the next bucket.
     * this way if there is a match we get a running time of M+N
     * otherwise if there is no match at all the running time is M*N
     * i tried to make it faster but this was the best i could've think of.
     */
    for(unsigned int i = 0; i <= str1Len - str2Len; i++)
    {
        for(unsigned int j = i; j < i + str2Len; j++)
        {
            flag = 1;
            if(str1[j] != str2[j - i])
            {
                flag = 0;
                break;
            }
        }
        if(flag == 1)
        {
            flag = 0;
            matches++;
        }
    }
    return 0;
}

int main()
{
    return 0;
}