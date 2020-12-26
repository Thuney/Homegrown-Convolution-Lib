#include <thread>
#include <Python.h>
#include "convolutions.h"

int max(int num_1, int num_2)
{
	return (num_1 >= num_2) ? num_1 : num_2;
}

int min(int num_1, int num_2)
{
	return (num_1 <= num_2) ? num_1 : num_2;
}

void convolve_brute(int f_len, double* f, int g_len, double* g, double* convolved)
{
	// Swap the arrays if F is shorter than G
	if(f_len < g_len)
	{
		int temp_len     = f_len;
		double* temp_ptr = f;

		f_len = g_len;
		f     = g;

		g_len = temp_len;
		g     = temp_ptr;
	}
	// F is now greater than or equal to the length of G

	// Calculate the number of samples that are in the output array
	const int num_convolved_samples = (f_len + g_len - 1);

	// For every sample in the output, calculate the convolution sum
	for(int n = 0; n < num_convolved_samples; n++)
	{
		// Calculate the upper and lower bounds in each signal
		// for the "overlapping" region at this point in the convolution

		int f_low = max(0, n - (g_len - 1));
		int f_upp = min(n, (f_len - 1));

		int g_low = max(0, n - (f_len - 1));
		int g_upp = min(n, (g_len - 1));

		// Holds the sum of products over all of the overlapping samples
		double sample_sum = 0.0;

		// Iterate forwards through 'f' and backwards through 'g',
		// calculating the intermediate products and adding them to the convolution sum
		for(
			int f_index = f_low, g_index = g_upp; 
			f_index <= f_upp, g_index >= g_low; 
			f_index++, g_index--
		   )
		{
			double product = f[f_index] * g[g_index];
			sample_sum += product;
		}

		// Set the computed value of this sample in the output array
		convolved[n] = sample_sum;

		// Print our progress
		printf("Index: %d\n", n);
	}
}

void convolve_multi(int f_len, double* f, int g_len, double* g, double* convolved)
{
	// Swap the arrays if F is shorter than G
	if(f_len < g_len)
	{
		int temp_len     = f_len;
		double* temp_ptr = f;

		f_len = g_len;
		f     = g;

		g_len = temp_len;
		g     = temp_ptr;
	}
	// F is now greater than or equal to the length of G

	const int num_convolved_samples = (f_len + g_len - 1);

	// Lambda expression for our per-sample convolution function
	// - Captures the function-scope variables pertinent to our signals
	// - Computes the value of a particular sample 'n' in the output signal
	//   - This value is the discrete linear convolution of 'f' and 'g' at 'n'
	// 
	auto convolution_range = [&f_len, &f, &g_len, &g, &num_convolved_samples, &convolved](int n_low, int n_upp)
	{
		for(int n = n_low; n < n_upp; n++)
		{

			// Calculate the upper and lower bounds in each signal
			// for the "overlapping" region at this point in the convolution
	
			int f_low = max(0, n - (g_len - 1));
			int f_upp = min(n, (f_len - 1));

			int g_low = max(0, n - (f_len - 1));
			int g_upp = min(n, (g_len - 1));

			// Holds the sum of products over all of the overlapping samples
			double sample_sum = 0.0;

			// Iterate forwards through 'f' and backwards through 'g',
			// calculating the intermediate products and adding them to the convolution sum	
			for(
				int f_index = f_low, g_index = g_upp; 
				f_index <= f_upp, g_index >= g_low; 
				f_index++, g_index--
		   	)
			{
				double product = f[f_index] * g[g_index];
				sample_sum += product;
			}

			// Set the computed value of this sample in the output array
			convolved[n] = sample_sum;
		}
	};

	// Compute the "optimal" number of threads to use
	const int NUM_THREADS = std::thread::hardware_concurrency();

	// DEBUG: Just to see, y'know, how many threads are being used
	printf("NUM_THREADS = %d \n", NUM_THREADS);

	// Allocate our array of threads
	std::thread* threads = new std::thread[NUM_THREADS]; 

	const int samples_per_thread = (num_convolved_samples / NUM_THREADS);
	const int remainder_samples  = (num_convolved_samples % NUM_THREADS);

	printf("Samples per thread = %d \n", samples_per_thread);
	printf("Remainder samples  = %d \n", remainder_samples);

	int lower_bound = 0;

	for(int thread_number = 0; thread_number < NUM_THREADS; thread_number++)
	{
		int range = (thread_number != 0) ? (samples_per_thread) : (samples_per_thread + remainder_samples);

		printf("Lower = %d \t Upper = %d \n", lower_bound, lower_bound + range);

		threads[thread_number] = std::thread(convolution_range, lower_bound, lower_bound + range);

		lower_bound += range;
	}

	for(int thread_number = 0; thread_number < NUM_THREADS; thread_number++)
	{
		if(threads[thread_number].joinable())
			threads[thread_number].join();
	}

	/**

	// Track the sample number to be computed
	int sample_number = 0;
	// Track the thread index that we are accessing
	int thread_number = 0;

	// Start all of our threads
	for(thread_number = 0; thread_number < NUM_THREADS; thread_number++)
	{
		// Thread uses our lambda expression to compute the convolution at
		// the current sample number (which is then post-fix incremented to 
		// provide the value for the next thread to be spooled up)
		threads[thread_number] = std::thread(convolution_step, (sample_number++));
	}
	
	// Constantly join and resupply our threads with work until we're done with all samples
	// - This algorithm for supplying the threads with work is probably causing a notable 
	//   decrease in performance, as we have to wait (in order) for the threads to finish and 
	//   then supplied with work. Perhaps a 'done' notification servicing system would improve?
	//   No clue.
	while(sample_number < num_convolved_samples)
	{
		for(thread_number = 0; thread_number < NUM_THREADS; thread_number++)
		{
			threads[thread_number].join();
			if(sample_number < num_convolved_samples)
			{
				threads[thread_number] = std::thread(convolution_step, (sample_number++));
			}
		}
	}

	for(thread_number = 0; thread_number < NUM_THREADS; thread_number++)
	{
		if(threads[thread_number].joinable())
			threads[thread_number].join();
	}

	**/

	delete[] threads;
}
