import threading
from ctypes import *

so_file = "../cpp/build/lib/convlib.so"
c_conv  = CDLL(so_file)

def c_convolve_brute(f, g):

    f_len = len(f)
    g_len = len(g)

    num_convolved_samples = (f_len + g_len - 1)

    c_arr_f = (c_double * f_len)(*f)
    c_arr_g = (c_double * g_len)(*g)

    c_arr_convolved = (c_double * num_convolved_samples)()

    c_conv.convolve_brute(c_int(f_len), c_arr_f, c_int(g_len), c_arr_g, c_arr_convolved)

    return c_arr_convolved[:]

def c_convolve_multi(f, g):

    f_len = len(f)
    g_len = len(g)

    num_convolved_samples = (f_len + g_len - 1)

    c_arr_f = (c_double * f_len)(*f)
    c_arr_g = (c_double * g_len)(*g)

    c_arr_convolved = (c_double * num_convolved_samples)()

    c_conv.convolve_multi(c_int(f_len), c_arr_f, c_int(g_len), c_arr_g, c_arr_convolved)

    return c_arr_convolved[:]

def convolve_brute(f, g):
    
    convolved = []
    
    num_f_samples = len(f)
    num_g_samples = len(g)
    
    num_convolved_samples = (num_f_samples + num_g_samples - 1)
    
    for n in range(num_convolved_samples):
        
        f_low = max(0, n - (num_g_samples - 1))
        f_upp = min(n, (num_f_samples - 1))
       
        g_low = max(0, n - (num_f_samples - 1))
        g_upp = min(n, (num_g_samples - 1))

        f_range = f[f_low : f_upp + 1]
        g_range = g[g_low : g_upp + 1]
        g_range = g_range[::-1]

        sample_sum = 0

        for i in range(len(f_range)):
            product = (f_range[i] * g_range[i])
            sample_sum = sample_sum + product

        convolved.append(sample_sum)

        print(str(n) + "/" + str(num_convolved_samples))

    return convolved

def convolve_multi(f, g):
    
    num_f_samples = len(f)
    num_g_samples = len(g)
    
    num_convolved_samples = (num_f_samples + num_g_samples - 1) 
    convolved = [0] * num_convolved_samples

    def convolution_step(sample_number):
        
        f_low = max(0, sample_number - (num_g_samples - 1))
        f_upp = min(sample_number, (num_f_samples - 1))
       
        g_low = max(0, sample_number - (num_f_samples - 1))
        g_upp = min(sample_number, (num_g_samples - 1))

        sample_sum = 0

        for i in range(f_upp - f_low):
            f_val = f[f_low + i]
            g_val = g[g_low + i]

            product = (f_val * g_val)
            sample_sum = sample_sum + product

        convolved[sample_number] = sample_sum
 
        print(str(n) + "/" + str(num_convolved_samples))

    threads = []

    for n in range(num_convolved_samples):

        sample_thread = threading.Thread(target=convolution_step, args=(n, ))
        threads.append(sample_thread)
        sample_thread.start()

    for thread in range(len(threads)):
        thread.join()

    return convolved
