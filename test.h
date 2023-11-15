#include<stdio.h>
#include<assert.h>
#include<stdlib.h>
#include "srec.h"


typedef enum{
    TEST_PASS=0,
    TEST_NOT_PASS
}TEST_STATUS;

TEST_STATUS convert_hex_Test(){
  
    // Test case 1: Valid hex characters
    assert(convert_hex('0') == 0);
    assert(convert_hex('A') == 10);
    assert(convert_hex('a') == 10);
    assert(convert_hex('F') == 15);

    // Test case 2: Invalid hex character
    assert(convert_hex('G') == HEX_INVALID);
}