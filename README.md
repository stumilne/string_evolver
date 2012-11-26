String evolver
==============
Author: Stuart Milne

Date : November 2012

## Description
String evolver utilises an implementation of a genetic algorithm to evolve strings from a random seed population. It supports strings of arbitary length with characters comprised from the printable ascii character set (ascii code 34 - 126).

3 crossover operations are supported: single, multiple and cut and splice crossover.

Simple single character mutation is supported.

## Usage
The compiled executable can be invoked from the command line with two optional parameters.
    
    evolver <String to evolve> <Size of population to utilise>

### Example usage
    string_evolver "Evolve towards this string" 100000

The above example will attempt to evolve the supplied string with a population of 100,000.