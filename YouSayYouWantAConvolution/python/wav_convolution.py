import numpy as np
import time

from scipy import signal

from convlib.wavlib  import get_samples_from_wav, signal_to_wav
from convlib.convlib import convolve_brute, convolve_multi, c_convolve_brute, c_convolve_multi
from convlib.plotlib import plot_convolution

input_file_name   = "sylphrena_meow_cut.wav"
impulse_file_name = "nuclear_impulse_cut.wav"

output_file_name  = "sylphrena_nuclear_impulsed_mine.wav"

x       = get_samples_from_wav("..//wavs//" + input_file_name)
impulse = get_samples_from_wav("..//wavs//" + impulse_file_name)

#
# Test our convolution implementations
#

#before = time.perf_counter()

#convolved = signal.fftconvolve(x, impulse)
#convolved = signal.convolve(x, impulse, method="direct")

#after = time.perf_counter()
#print("Reference Convolution Time = " + str(after - before))

#plot_convolution(impulse, x, convolved)

before = time.perf_counter()

#convolved = convolve_brute(x, impulse)
#convolved = convolve_multi(x, impulse)

#convolved = c_convolve_brute(x, impulse)
convolved = c_convolve_multi(x, impulse)

after = time.perf_counter()
print("C++ Multi Thread Convolution Time = " + str(after - before))

#signal_to_wav(convolved, "..//wavs//output//" + output_file_name)

plot_convolution(impulse, x, convolved)
