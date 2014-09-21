#! /usr/bin/env python
import time

primes = []

def isPrime(n):
  for i in range(2, n / 2 + 1):
    if n % i == 0:
      return False
  return True

def isPrimeFast(n):
  for i in primes:
    if i > n / 2:
      return True
    if n % i == 0:
      return False
  return True


beginTimeSeconds = time.clock()

for i in range(3, 100000, 2):
  if isPrimeFast(i):
    primes.append(i)

endTimeSeconds = time.clock()

print "{} seconds elapsed".format(endTimeSeconds - beginTimeSeconds)
