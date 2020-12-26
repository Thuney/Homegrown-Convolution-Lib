from ctypes import *

so_file = "../cpp/build/lib/convlib.so"
c_conv  = CDLL(so_file)

def square_list(given_list):
    
    n = len(given_list)
    c_arr_in  = (c_double * n)(*given_list)
    c_arr_out = (c_double * n)()

    c_conv.square_list(c_int(n), c_arr_in, c_arr_out)
    return c_arr_out[:]

print(square_list([1, 2, 3, 4, 5]))
