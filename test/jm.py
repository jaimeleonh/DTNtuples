#!/bin/env python
import os, sys
from pprint import pprint
import time


labels = ['ch0', 'ch1', 'ch2', 'ch3', 'ch4', 'ch5', 'ch6', 'ch7','t0', 't1', 't2', 't3','t4', 't5', 't6', 't7', 'shift', 'wheel', 'sector', 'station' ]
lines = []
#with open(os.path.expandvars('${DTUPY_ETC}/../../pruebasj/hitsinput.txt'), 'r') as f:
with open(os.path.expandvars('input.txt'), 'r') as f:
  lines = f.readlines()
lines = [ line.strip().split(',') for line in lines ]
def str2num( text ):
  return int(text) if not '.' in text else float(text)
lines = [ {labels[i]:str2num(line[i].strip()) for i in range(len(labels))} for line in lines  if len(line) == len(labels)]
#lines = lines[:-1] if len(lines)%2 == 1 else lines
print(len(lines))

for i in range(len(lines)):
   line = lines[i]
   if line.get('t0') != -1 :
    print ('0 ' + '0 ' + str(line.get('ch0')) + ' ' + str(line.get('t0')))
   if line.get('t1') != -1 :
    print ('0 ' + '1 ' + str(line.get('ch1')) + ' ' + str(line.get('t1')))
   if line.get('t2') != -1 :
    print ('0 ' + '2 ' + str(line.get('ch2')) + ' ' + str(line.get('t2')))
   if line.get('t3') != -1 :
    print ('0 ' + '3 ' + str(line.get('ch3')) + ' ' + str(line.get('t3')))
   if line.get('t4') != -1 :
    print ('2 ' + '0 ' + str(line.get('ch4')) + ' ' + str(line.get('t4')))
   if line.get('t5') != -1 :
    print ('2 ' + '1 ' + str(line.get('ch5')) + ' ' + str(line.get('t5')))
   if line.get('t6') != -1 :
    print ('2 ' + '2 ' + str(line.get('ch6')) + ' ' + str(line.get('t6')))
   if line.get('t7') != -1 :
    print ('2 ' + '3 ' + str(line.get('ch7')) + ' ' + str(line.get('t7')))
