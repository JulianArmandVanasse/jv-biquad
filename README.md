# jv-biquad

    Julian's Biquad Filter
 
    A variable bandpass filter plugin built from delay lines. Easily alterable
    to other filter states, and based on the Bandpass filter described in DAFX
    (Zolzer, p.43).
 
 
             ┌---- A(z) ----┐
    x[n] ----┤            (+/-) --- (*0.5) ---> y[n]
             └--------------┘
 
    Where A(z) is a second order allpass filter. The filter coefficients are
    stored in Array<float>s a and b, both processor properties. These can be
    changed and extended to permit other filter types of higher order, as
    long as a and b are the same size.
 
    This is an experiment in filter implementation more than in filter design,
    and is for educational purposes. For a long time real-time filter implement-
    -ation was a mystery and building this plugin made the process concrete.
 
    Included is the object FilterDelayLine that is used to handle reading and
    writing to a circular buffer. Two lines are used: (1) to handle feed for-
    -ward signal and (2) to handle feedback signal.
 
    Julian Armand Vanasse
    McGill University, Music Technology Area
    Fall 2020.
