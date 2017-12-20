# PXG Software Radio Streaming Format
## What is the PXG format?

The PXG format is a standard for streaming digitised radio and audio data and
associated metadata.

## What is PXGF?

The standard extension used on PXG format data files is .pxgf

## What is the PXG format reference library?

As the name suggests, the PXG format reference library is a reference
implementation to read and write files and TCP or UDP data streams in the
PXG format. This implementation is in C++.

## What is the PXG format useful for?

Data in the PXG format is used to stream digitised data from analogue-to-digital converters connected to radio receivers and ultimately to antennas,
capturing the sampled data and the information required to plot it in the
time or frequency domain.

It can also be used to stream or record audio data.

## What are the benefits of this format over others?

The format is very loosely based on other formats for digitised data recording,
and was designed to be
  * simple,
  * support both big- and little-endian data to reduce the overhead of endian
  conversions,
  * easy to extend, by defining new chunk types,
  * allows software to easily synchronise mid-stream to a stream

Currently the standard and this implementation support sampled data in real
and complex (I + Q), 16 bit signed integer, and 32 bit single precision IEEE
floating point formats, with associate time stamps at nanosecond resolution.

Other chunks allow the bandwidth, sample rate, centre frequency (for IF data),
and scaling information.

## What platforms are supported?

It has been successfully compiled and used on Windows using Visual Studio 2015
(VS 14.0), on Linux (gcc 4.7.3 and 5.4, clang 3.7) and FreeBSD (clang 3.4.1 and
4.0).

It should compile using any C++ compiler with basic C++11 support.

On all of the above platforms this library supports reading and writing of
file-based streams and TCP and UDP socket based streams.

## Are other implementations available?

At the time of writing, this was the only known publicly available, open-source
implementation.  At least one other closed-source C++ implementation and
a closed-source Java implementation exist.

## How do I use it?

Your best bet is to have a look at the samples for reading and writing streams
in the samples directory.

## License

The library is licensed under the Apache License, Version 2.0.  This means the
library can easily be used in commercial products.
