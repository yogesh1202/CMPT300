#include <linux/kernel.h>
#include <linux/syscalls.h>




struct array_stats_s
{
	long min;
	long max;
	long sum;
	
};

SYSCALL_DEFINE3(array_stats,                  
/* syscall name */
struct array_stats_s*, stats, /* where to write stats */
long*, data,                  /* data to process */
long, size)
{	
	/*
		In the Guide_Syscall.pdf, we were advised to create local variables
		that have the same type as the data in the user's array (when we use copy_from_user)
		copy_from_user(to , from , n)
	*/
	int i = 0;
	struct array_stats_s localStats;
	long localData = 0;
	localStats.min = 0x7FFFFFFF;
	localStats.max = -0x7FFFFFFF;
	localStats.sum = 0;
	// If size <= 0 return -EINVAL
	if (size <= 0)
	{
		return -EINVAL;
	}

	// Copy one value at a time
	for (; i < size; i++)
	{
		/*
		Returns -EFAULT if unable to access stats or data
		copy_from_user(dest,src,size)
		*/

		if (copy_from_user(&localData, &data[i],sizeof(data[i])))
		{
			return -EFAULT;
		}


		// Find max
		if (localStats.max < localData)
		{
			localStats.max = localData;
			
		}
		//printk("MAX %ld\n",localStats -> max);

		// Find min
		if (localStats.min > localData)
		{
			localStats.min = localData;
			
		}
		//printk("MIN %ld\n",localStats -> min);

		// Result
		localStats.sum = localStats.sum + localData;

	}

		/*
			From Guide_Syscall.pdf, one of the hints was to manipulate local variables
			inside the syscall, and then use copy_to_user()
			copy_to_user(destination,src,size)
		*/
		// Check if the copy to user is unable to access stats by returning -EFAULT
		if (copy_to_user(stats,&localStats,sizeof(localStats)))
		{
			return -EFAULT;
		}

		return 0;


}
