#include <stdio.h>

int main(){
    unsigned char k = 75;
    printf("%x\n",k);
    for(int i=0;i<8;i++){
        printf("%d  ",(k>>(7-i))&0x01);
    }
    return 0;
}