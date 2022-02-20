# Test performance of sort
option fail 0
option malloc 0
new
ih RAND 500000
time sort
new
ih RAND 500000
option linuxsort 1
time sort
