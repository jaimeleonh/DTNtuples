import sys
import time
from subprocess import call

if len(sys.argv)==1 :
    print ("Default case: running all")
    time.sleep(3)
    rc = call("./copyToEos_rates_noaging_norpc.sh", shell=True)
    rc = call("./copyToEos_rates_aging_norpc.sh", shell=True)
    rc = call("./copyToEos_rates_noaging_rpc.sh", shell=True)
    rc = call("./copyToEos_rates_aging_rpc.sh", shell=True)

elif len(sys.argv) == 2 : 
  if sys.argv[1] == 'noageing' or sys.argv[1] == 'noaging': 
    print ("Running noaging norpc")
    time.sleep(3)
    rc = call("./copyToEos_rates_noaging_norpc.sh", shell=True)
  elif sys.argv[1] == 'ageing' or sys.argv[1] == 'aging': 
    print ("Running aging norpc")
    time.sleep(3)
    rc = call("./copyToEos_rates_aging_norpc.sh", shell=True)
  else : 
    print ("Bad argument")
    sys.exit(1)

elif len(sys.argv) == 3 : 
  if (sys.argv[1] == 'noageing' or sys.argv[1] == 'noaging') and sys.argv[2] == 'norpc' : 
    print ("Running noaging norpc")
    time.sleep(3)
    rc = call("./copyToEos_rates_noaging_norpc.sh", shell=True)
  elif (sys.argv[1] == 'ageing' or sys.argv[1] == 'aging')  and sys.argv[2] == 'norpc': 
    print ("Running aging norpc")
    time.sleep(3)
    rc = call("./copyToEos_rates_aging_norpc.sh", shell=True)
  elif (sys.argv[1] == 'noageing' or sys.argv[1] == 'noaging') and sys.argv[2] == 'rpc' : 
    print ("Running noaging rpc")
    time.sleep(3)
    rc = call("./copyToEos_rates_noaging_rpc.sh", shell=True)
  elif (sys.argv[1] == 'ageing' or sys.argv[1] == 'aging')  and sys.argv[2] == 'rpc': 
    print ("Running aging rpc")
    time.sleep(3)
    rc = call("./copyToEos_rates_aging_rpc.sh", shell=True)
  else : 
    print ("Bad argument")
    sys.exit(1)

