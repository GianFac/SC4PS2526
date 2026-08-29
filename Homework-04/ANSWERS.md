# Worksheet answers

## Part 1: Before running anything

1. _In your own words, what does an FFT tell you about a signal?_
   
   The FFT finds the various frequencies components that make up the analysed signal.
2. _What is the difference between:_
   - _sampling rate:_ the freuqnecy at which you are measuring the signal (samples per seconds)
   - _Nyquist frequency:_ the highest frequency you can extract from the signal without aliasing. It is half the sampling frequency. 
   - _frequency resolution:_ spacing of the FFT frequency bins, determines how closely you can distringuish two frequency components.
3. _Why is a uniformly spaced time grid important for the FFT routines used in
   this project?_

   This FFT program requires uniformely spaced times, otherwise the algorithm would return incorrect bin frequencies.

## Part 2: Build and run

1. _Which CSV files are created in `output/`?_
    - `coupled_oscillators_spectrum.csv`
    - `coupled_oscillators_time.csv`
    - `good_sampling_signal.csv`
    - `good_sampling_spectrum.csv`
    - `short_record_signal.csv`
    - `short_record_spectrum.csv`
    - `undersampled_signal.csv`
    - `undersampled_spectrum.csv`
2. _Which program studies pure sampling issues?_
   -  `sampling_demo` 
3. _Which program studies a mechanics problem?_
   -  `coupled_oscillators_fft`
4. _What two frequencies are reported in the well-sampled case?_
   -  `peak 1: f =   50.000 Hz`
   -  `peak 2: f =  120.000 Hz`

## Part 3: Sampling and aliasing

1. _In the `undersampled` case, the sampling frequency is_ $f_s = 128\,\mathrm{Hz}$.
   - Yes.
2. _Using_ $f_N = \dfrac{f_s}{2}$, _what is the Nyquist frequency_ $f_N$ _for this case?_
   - 64 Hz.
3. _Why can a_ $120\,\mathrm{Hz}$ _component not be reconstructed faithfully here?_
   - It is above the Nyquist frequency.
4. _The program reports an aliased peak near `8 Hz`._
   - Yes, it is the difference between the sampling frequency and the actual component. 
5. _Explain physically and mathematically why a high-frequency signal can appear
   as a false low-frequency signal after sampling._
   - Since the singal is faster than the Nyquist frequency, the FFT reconstructs the sinusoidal component at a lower frequency given by $f_{alias} = |f_{sig}-n*f_S|=|128-120|$ = 8 Hz, with $n=1$.


## Part 4: Frequency resolution

1. _The signal contains_ $50\,\mathrm{Hz}$ _and_ $55\,\mathrm{Hz}$.
   - Yes.
2. _Using_ $T = N \Delta t$, _what is the total record length_ $T$?
   - 64/512 Hz = 0.125 s 
3. _Using_ $\Delta f = \dfrac{f_s}{N} = \dfrac{1}{T}$, _what is the frequency spacing_ $\Delta f$ of the FFT bins?
   - $\Delta f = \dfrac{1}{T} = \dfrac{1}{0.125} = 8$ Hz
4. _Why does this make the two frequencies difficult to separate?_
   - The resolution, 8 Hz, is higher than the frequency difference of the componenets, 5 Hz.
5. _If you wanted to improve the separation, would you first change:_
   - the total acquisition time: it is the one defining the frequency resolution.

## Part 5: Coupled oscillators

1. _Write down the equations of motion used in the code in the form_ $m \ddot{x}_1 = \cdots$ and $m \ddot{x}_2 = \cdots$.

   $$m \ddot{x}_1 = - (k + k_c) x_1 + k_c x_2$$
   
   $$m \ddot{x}_2 = k_c x_1 - (k + k_c) x_2$$

   where:
   - $m$ is the mass of each oscillator
   - $k$ is the spring constant of the wall springs
   - $k_c$ is the coupling spring constant
   - $x_1(t)$ and $x_2(t)$ are the displacements of the two masses

2. _What physical system do these equations represent?_
   - Two equal masses connected to a wall with a spring, and with another to each other.
3. _What are the two normal modes of this system?_
   - An in-phase and an out-of-phase for the positions of the two masses.
4. Why does the FFT of $x_1(t)$ show more than one important frequency?
   - They are a combination of normal modes due to initial conditions.
5. _Why is it useful to compare the numerical FFT peaks with the theoretical
   normal-mode frequencies?_
   - Useful in checking if the simulated model is correct, and if there are other mistakes in the code.

## Part 6: Plot inspection

1. _Which plot makes aliasing easiest to see?_
   - In `sampling_spectra.png`, the undersampling spectrum.
2. _Which plot makes limited frequency resolution easiest to see?_
   - In `sampling_spectra.png`, the short-record spectrum.
3. _In the coupled-oscillator FFT plot, do the numerical peaks line up well with
   the theoretical reference frequencies?_
   - Yes, slightly less than 1 Hz and 0.8 Hz. 
4. _If they are not exactly identical, what numerical reasons could explain the
   small difference?_
   - It may depend on the tolerance of the differential equation solver, the bin spacing, and for the FFT analysis the finite time window.

## Part 7: Small code modifications

### Option C: Change the spring constants

1. Changed `wall_k = 50.0 N/m` and `coupling_k = 30.0 N/m`.
2. The two frequencies should be:
     $f_{\mathrm{in}} = \dfrac{1}{2\pi}\sqrt{\dfrac{k}{m}} = 1.12540 \quad \textrm{Hz}$
   
     $f_{\mathrm{out}} = \dfrac{1}{2\pi}\sqrt{\dfrac{k + 2 k_c}{m}} = 1.66923 \quad \textrm{Hz}$
4. `in-phase mode: expected 1.12540 Hz, measured 1.12500 Hz, amplitude 0.0498`
   
    `out-of-phase mode: expected 1.66923 Hz, measured 1.67188 Hz, amplitude 0.0411`

## Part 8: Reflection

1. I Actually never considered the effect of a too short recording window in the discrimination of nearby frequency components.
2. The division into specific folders of the various elements of a project.
3. Maybe a sort of 'tutorial' in which the student has to modify not only the fft and oscillation codes, but also the makefile.

