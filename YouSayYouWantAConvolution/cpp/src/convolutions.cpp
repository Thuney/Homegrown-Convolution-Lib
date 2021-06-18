#include <thread>
#include <mutex>
#include <Python.h>
#include "convolutions.h"

// Helper Functions

int max(int num_1, int num_2)
{
	return (num_1 >= num_2) ? num_1 : num_2;
}

int min(int num_1, int num_2)
{
	return (num_1 <= num_2) ? num_1 : num_2;
}

void normalize_signals(int* f_len, double** f, int* g_len, double** g)
{
	// Swap the arrays if F is shorter than G
	if((*f_len) < (*g_len))
	{
		printf("Swapping signals F and G\n");
		
		int temp_len     = (*f_len);
		double* temp_ptr = (*f);

		(*f_len)  = (*g_len);
		(*f)      = (*g);

		(*g_len)  = temp_len;
		(*g)      = temp_ptr;
	}
	// F is now greater than or equal to the length of G
}

void convolve_n(const int n, const int f_len, double* f, const int g_len, double* g, double* convolved)
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
	// printf("Index: %d\n", n);
}

// Convolution Implementations

void convolve_brute(int f_len, double* f, int g_len, double* g, double* convolved)
{
	normalize_signals(&f_len, &f, &g_len, &g);

	// Calculate the number of samples that are in the output array
	const int num_convolved_samples = (f_len + g_len - 1);

	// For every sample in the output, calculate the convolution sum
	for(int n = 0; n < num_convolved_samples; n++)
	{
		convolve_n(n, f_len, f, g_len, g, convolved);
	}
}

void convolve_multi(int f_len, double* f, int g_len, double* g, double* convolved)
{
	normalize_signals(&f_len, &f, &g_len, &g);

	const int num_convolved_samples = (f_len + g_len - 1);

	std::mutex n_mutex;
	int n = 0;

	// Lambda expression for our per-sample convolution function
	// - Captures the function-scope variables pertinent to our signals
	// - Computes the value of a particular sample 'n' in the output signal
	//   - This value is the discrete linear convolution of 'f' and 'g' at 'n'
	// 
	auto convolve_threaded = [&n, &n_mutex, f_len, f, g_len, g, num_convolved_samples, &convolved]()
	{
		int local_n = 0;

		while(true)
		{
			n_mutex.lock();

			local_n = (++n);

			// printf("local_n = %d \n", local_n);

			n_mutex.unlock();

			if(local_n >= num_convolved_samples) break;

			convolve_n(local_n, f_len, f, g_len, g, convolved);
		}
	};

	// Compute the "optimal" number of threads to use
	const int NUM_THREADS = std::thread::hardware_concurrency();

	// DEBUG: Just to see, y'know, how many threads are being used
	printf("NUM_THREADS = %d \n", NUM_THREADS);

	// Allocate our array of threads
	std::thread* threads = new std::thread[NUM_THREADS]; 

	for(int thread_number = 0; thread_number < NUM_THREADS; thread_number++)
	{
		threads[thread_number] = std::thread(convolve_threaded);
	}

	for(int thread_number = 0; thread_number < NUM_THREADS; thread_number++)
	{
		if(threads[thread_number].joinable())
			threads[thread_number].join();
		printf("Processing ended on thread number: %d \n", thread_number);
	}

	delete[] threads;
}
