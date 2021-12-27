// Test application for the array_stats syscall.

#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

struct array_stats_s {
	long min;
	long max;
	long sum;
};


// Sys-call number:
#define _ARRAY_STATS_ 549



/*
	Multiple tests with various edge cases visited 
*/
struct array_stats_s stats = {0,0,0};
long data[5] = {5 , 50 , 100,55555,390};
long size = 5;
long data2[3] = {1 , 100 , 400};
long size2 = 3;
long data3[1] = {4};
long size3 = 1;
long data4[6] = {100,1000000,34456,333,22,1};
long size4 = 6;
long data5[4] = {-1,-2,1,2};
long size5 = 4;
long data6[3] = {-1,-2,-1};
long size6 = 3;
long data7[0] = {};
long size7 = 0;
long data8[0] = {};
long size8 = -1;
long * nullP = NULL;
long sizeNullP = 2;
long * badP = (void*)1LL; // Make it long long
long sizebadP = 1;
long badSize[1] = {1};
long badSizex = 102034593;
long arrayONE[1] = {1};

static long find_max(long data[], long size);
static long find_min(long data[], long size);
static long find_sum(long data[], long size);


int main(int argc, char *argv[])
{
	// Random testing

	long correctMax = find_max(data,5);
	long correctMin = find_min(data,5);
	long correctSum = find_sum(data,5);
	printf("Correct Min (Test 1) = %ld\n",correctMin);
	printf("Correct Max (Test 1) = %ld\n",correctMax);
	printf("Correct Sum (Test 1) = %ld\n",correctSum);
	printf("Diving to kernel level\n");
	int result = syscall(_ARRAY_STATS_,&stats,data,size);
	printf("Our Min = %ld\n",stats.min);
	printf("Our Max = %ld\n",stats.max);
	printf("Our Sum = %ld\n",stats.sum);
	printf("Rising to user level w/result = %d\n\n",result);


	long correctMax2 = find_max(data2,3);
	long correctMin2 = find_min(data2,3);
	long correctSum2 = find_sum(data2,3);
	printf("Correct Min (Test 2) = %ld\n",correctMin2);
	printf("Correct Max (Test 2) = %ld\n",correctMax2);
	printf("Correct Sum (Test 2) = %ld\n",correctSum2);
	printf("Diving to kernel level\n");
	int result2 = syscall(_ARRAY_STATS_,&stats,data2,size2);
	printf("Our Min = %ld\n",stats.min);
	printf("Our Max = %ld\n",stats.max);
	printf("Our Sum = %ld\n",stats.sum);
	printf("Rising to user level w/result = %d\n\n",result2);

	long correctMax3 = find_max(data3,1);
	long correctMin3 = find_min(data3,1);
	long correctSum3 = find_sum(data3,1);
	printf("Correct Min (Test 3) = %ld\n",correctMin3);
	printf("Correct Max (Test 3) = %ld\n",correctMax3);
	printf("Correct Sum (Test 3) = %ld\n",correctSum3);
	printf("Diving to kernel level\n");
	int result3 = syscall(_ARRAY_STATS_,&stats,data3,size3);
	printf("Our Min = %ld\n",stats.min);
	printf("Our Max = %ld\n",stats.max);
	printf("Our Sum = %ld\n",stats.sum);
	printf("Rising to user level w/result = %d\n\n",result3);

	long correctMax4 = find_max(data4,6);
	long correctMin4 = find_min(data4,6);
	long correctSum4 = find_sum(data4,6);
	printf("Correct Min (Test 4) =%ld\n",correctMin4);
	printf("Correct Max (Test 4) = %ld\n",correctMax4);
	printf("Correct Sum (Test 4) = %ld\n",correctSum4);
	printf("Diving to kernel level\n");
	int result4 = syscall(_ARRAY_STATS_,&stats,data4,size4);
	printf("Our Min = %ld\n",stats.min);
	printf("Our Max = %ld\n",stats.max);
	printf("Our Sum = %ld\n",stats.sum);
	printf("Rising to user level w/result = %d\n\n",result4);

	long correctMax5 = find_max(data5,4);
	long correctMin5 = find_min(data5,4);
	long correctSum5 = find_sum(data5,4);
	printf("Correct Min (Test 5) =%ld\n",correctMin5);
	printf("Correct Max (Test 5) = %ld\n",correctMax5);
	printf("Correct Sum (Test 5) = %ld\n",correctSum5);
	printf("Diving to kernel level\n");
	int result5 = syscall(_ARRAY_STATS_,&stats,data5,size5);
	printf("Our Min = %ld\n",stats.min);
	printf("Our Max = %ld\n",stats.max);
	printf("Our Sum = %ld\n",stats.sum);
	printf("Rising to user level w/result = %d\n\n",result5);

	long correctMax6 = find_max(data6,3);
	long correctMin6 = find_min(data6,3);
	long correctSum6 = find_sum(data6,3);
	printf("Correct Min (Test 6) =%ld\n",correctMin6);
	printf("Correct Max (Test 6) = %ld\n",correctMax6);
	printf("Correct Sum (Test 6) = %ld\n",correctSum6);
	printf("Diving to kernel level\n");
	int result6 = syscall(_ARRAY_STATS_,&stats,data6,size6);
	printf("Our Min = %ld\n",stats.min);
	printf("Our Max = %ld\n",stats.max);
	printf("Our Sum = %ld\n",stats.sum);
	printf("Rising to user level w/result = %d\n\n",result6);



	// Testing for size <= 0 (RETURNS ERROR 22)
	printf("Diving to kernel level\n");
	int result7 = syscall(_ARRAY_STATS_,&stats,data7,size7);
	int errorCode = errno;
	printf("Rising to user level w/result = %d (err #%d)\n",result7,errorCode);
	assert(errorCode == 22);
	printf("Error Number: %d shows -EINVAL\n\n",errorCode);

	printf("Diving to kernel level\n");
	int result8 = syscall(_ARRAY_STATS_,&stats,data8,size8);
	int errorCode2 = errno;
	printf("Rising to user level w/result = %d (err #%d)\n",result8,errorCode2);
	assert(errorCode2 == 22);
	printf("Error Number: %d shows -EINVAL\n\n",errorCode2);

	printf("Diving to kernel level\n\n");
	int resultx = syscall(_ARRAY_STATS_,&stats,arrayONE, size8);
	int errorCodex = errno;
	printf("Rising to user level w/result = %d (err #%d)\n",resultx,errorCodex);
	assert(errorCodex == 22);
	printf("Error Number: %d shows -EINVAL\n\n",errorCodex);



	// Testing invalid/bad pointers (EFAULT (ERR 14))

	printf("Diving to kernel level\n");
	int result9 = syscall(_ARRAY_STATS_,&stats,nullP,sizeNullP);
	int errorCode3 = errno;
	printf("Rising to user level w/result = %d (err #%d)\n",result9,errorCode3);
	assert(errorCode3 == 14);
	printf("Error Number: %d shows -EFAULT\n\n",errorCode3);

	printf("Diving to kernel level\n");
	int result10 = syscall(_ARRAY_STATS_,&stats,badP,sizebadP);
	int errorCode4 = errno;
	printf("Rising to user level w/result = %d (err #%d)\n",result10,errorCode4);
	assert(errorCode4 == 14);
	printf("Error Number: %d shows -EFAULT\n\n",errorCode4);

	printf("Diving to kernel level\n");
	int result11 = syscall(_ARRAY_STATS_,&stats,badSize,badSizex);
	int errorCode5 = errno;
	printf("Rising to user level w/result = %d (err #%d)\n",result11,errorCode5);
	assert(errorCode5 == 14);
	printf("Error Number: %d shows -EFAULT\n\n",errorCode5);


	printf("Diving to kernel level\n");
	int result12 = syscall(_ARRAY_STATS_,(void*)1,arrayONE,1);
	int errorCode6 = errno;
	printf("Rising to user level w/result = %d (err #%d)\n",result12,errorCode6);
	assert(errorCode6 == 14);
	printf("Error Number: %d shows -EFAULT\n",errorCode6);




	
	
}

static long find_max(long data[], long size)
{
	long max = data[0];
	for (int i = 0; i < size; i++) {
		if (data[i] > max) {
			max = data[i];
		}
	}
	return max;
}
static long find_min(long data[], long size)
{
	long min = data[0];
	for (int i = 0; i < size; i++) {
		if (data[i] < min) {
			min = data[i];
		}
	}
	return min;
}
static long find_sum(long data[], long size)
{
	long sum = 0;
	for (int i = 0; i < size; i++) {
		sum += data[i];
	}
	return sum;
}







