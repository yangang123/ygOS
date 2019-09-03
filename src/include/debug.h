#ifndef _DEBUG_H
#define _DEBUG_H

#include <stdio.h>

#ifdef DEBUG_ON
#define DEBUG(...)	    printf("function:%s\tline:%03d\t",__FUNCTION__,__LINE__); printf(__VA_ARGS__)
#define DEBUG_LR(...)	printf("function:%s\tline:%03d\t",__FUNCTION__,__LINE__); printf(__VA_ARGS__);printf("\n")
#define DEBUG_COMMON(...) printf(__VA_ARGS__)
#else 
#define DEBUG(...)	
#define DEBUG_LR(...)	
#define DEBUG_COMMON(...) 
#endif 

#endif /* _DEBUG_H */
