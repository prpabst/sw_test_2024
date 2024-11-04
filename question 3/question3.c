/******************************************************************************

Author: Paulo Ricardo Pabst
Date:	03/11/2024
Question 3

*******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>


typedef struct {
    int32_t size;
    int32_t *buff;
} buffer_t;


int32_t computeAvgMinMaxEvenArray(buffer_t *_in, buffer_t *_even, \
                                int32_t *_avg, int32_t *_min, int32_t *_max
                                ) {

    int32_t ii, jj;
    int32_t val, sum;

    // Verify all pointers.
    if(_in == NULL || _even == NULL || _avg == NULL \
        ||  _min == NULL || _max == NULL) {
        
        return(-1);
    }
    
    *_min = INT32_MAX;
    *_max = INT32_MIN;
    _even->size = 0;
    sum = 0;
    
    for(ii=0; ii<_in->size; ii++) {
        
        val = _in->buff[ii];
        *_min = val < *_min ? val : *_min;
        *_max = val > *_max ? val : *_max;
        sum += val;
        
        if(!(val & 1)) {
            
            _even->buff[jj] = val;
            _even->size++;
            jj++;
        }
    }
    *_avg = sum / _in->size;
    return(0);
}


int main()
{
    int32_t in_buff[30];
    int32_t even_buff[30];
    int32_t avg, min, max;
    buffer_t in;
    buffer_t even;
    int ret;
    
    srand(time(NULL));
    for(int ii=0; ii<30; ii++) {
        
        in_buff[ii] = rand() % 100;
        even_buff[0];
    }
    in.size = 30;
    in.buff = in_buff;
    even.size = 0;
    even.buff = even_buff;

    ret = computeAvgMinMaxEvenArray(&in, &even, &avg, &min, &max);
    if(ret >= 0) {
    
		printf("Original buffer data:\r\n");
        for(int ii=0; ii<in.size; ii++) {
            printf("%02d ", in.buff[ii]);
        }
        printf("\r\n");
    
		printf("Even values only:\r\n");
        for(int ii=0; ii<even.size; ii++) {
            
            printf("%02d ", even.buff[ii]);
        }
        printf("\r\n");
        
        printf("avg: %02d\tmin: %02d\tmax: %02d\r\n", avg, min, max);
    }
    
    return (ret);
}