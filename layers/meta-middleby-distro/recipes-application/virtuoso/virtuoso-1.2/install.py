import os
import subprocess

def silent_delete(fn):
    try:
        os.unlink(fn)
    except:
        pass


scratch_root = '/brava/'
brava_staging = scratch_root+'brava_staging/'
print(f'Installing in {os.getcwd()}')
subprocess.call(f'rm {brava_staging}*.SUM',shell=True)
walker = os.walk(brava_staging)
for tup in walker:
    dr = tup[0]
    fns = tup[2]
    dc = {}
    for fn in fns:
        silent_delete(fn)       #Unlink the local version which will allow the parent to continue to run but the copy to proceed
subprocess.call(f'cp {brava_staging}* .',shell=True)
subprocess.call(f'rm -rf {brava_staging}* .',shell=True)
subprocess.call(f'/sbin/shutdown -r now',shell=True)
