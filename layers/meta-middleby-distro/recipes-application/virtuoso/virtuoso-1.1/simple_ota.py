import os
import subprocess
import json
import sys
ota_root = "https://packages.bravacontent.com/ota/oven/virtuoso/"
local_brava_package = 'brava_package.tar.gz'
scratch_root = '/brava/'
brava_staging = scratch_root+'brava_staging/'
status_file = '/tmp/otastatus.txt'
standard_destination = '/opt/virtuoso/'

def write_ota_status(message):
    sf = open(status_file,'w')
    sf.write(message+'\n')
    print(message)
    sf.close()

def silent_delete(fn):
    try:
        os.unlink(fn)
    except:
        pass

def extract_checksum(fn):
    f = open(fn,'r')
    sum = f.read()
    f.close()
    sum = sum.split(' ')
    return sum[0]

def get_checksum(fn):
    silent_delete('check.sum')
    result = subprocess.call('sha256sum -b '+fn+' >check.sum',shell=True)
    if result!=0:
        return None
    else:
        return extract_checksum('check.sum')

def get_cohort_versions(oven_cohort):
    write_ota_status('Getting desired version information')
    silent_delete(scratch_root+'prod.json')
    get_prod = ['curl -o /brava/prod.json https://update.bravacontent.com/ota/oven/prod.json']
    result = subprocess.call(get_prod,shell=True)
    if result!=0:
        write_ota_status('Failed to fetch prod.json')
        raise
    try:
        f = open(scratch_root+'prod.json','r')
        data = f.read()
        f.close()
        j = json.loads(data)
        cohorts = j['cohorts']
        if oven_cohort=='':
            cohort_name = 'VIKING'
        else:
            cohort_name = 'VIKING'+oven_cohort
        if cohort_name in cohorts:
            cohort = cohorts[cohort_name]
        else:
            cohort = cohorts['VIKING']
        brava_package = None
        core_package = None
        if 'packages' in cohort:
            if 'ovenui' in cohort['packages']:
                brava_package = cohort['packages']['ovenui']
            if 'core' in cohort['packages']:
                core_package = cohort['packages']['core']
        if brava_package=='0000000':
            brava_package=None
        if core_package=='0000000':
            core_package=None
    except:
        write_ota_status('Failed to find cohort')
        raise
    silent_delete(scratch_root+'prod.json')
    return [brava_package,core_package]

def do_application_package(desired_brava_package,destination):
    if desired_brava_package==None:
        return False
    write_ota_status('Acquiring application data')
    os.chdir(brava_staging)
    silent_delete(local_brava_package)
    brava_image = ota_root+'virtuoso'+desired_brava_package+'.tar.gz'
    get_brava = ['curl -o '+local_brava_package+' '+brava_image]
    result = subprocess.call(get_brava,shell=True)
    if result!=0:
        raise
    write_ota_status('Unpacking application data')
    result = subprocess.call('tar -xvf '+local_brava_package,shell=True)
    write_ota_status('Checking against current version')
    walker = os.walk('.')
    installation_required = False
    for tup in walker:
        dr = tup[0]
        fns = tup[2]
        dc = {}
        for fn in fns:
            if fn[-4:]=='.SUM':
                prefix = fn[:-4]
                sum1 = get_checksum(destination+prefix)
                if sum1==None:
                    installation_required=True
                    break
                sum2 = extract_checksum(fn)
                if sum1!=sum2:
                    installation_required=True
                    break
        if installation_required:
            break
    if installation_required:
        write_ota_status('Installing')
        subprocess.call(f'python3 {brava_staging}install.py',cwd=destination,shell=True)
    else:
        write_ota_status('Application installation not required')
    return installation_required

def get_boot_partition():
    f = open('/proc/cmdline',"r")
    data = f.read()
    f.close()
    s = data.split(" ")
    for snip in s:
        if snip.find('root')==0:
            if snip.find('mmcblk0p5')>0:
                return ['mmcblk0p5','mmcblk0p6']
            if snip.find('mmcblk0p6')>0:
                return ['mmcblk0p6','mmcblk0p5']
    return None            
            
oven_cohort = 'A' 
if len(sys.argv)==2:
    oven_cohort = sys.argv[1]
silent_delete(status_file)
boot_partition = get_boot_partition()
if boot_partition==None:
    write_ota_status('Aborting, not booted to an SD card')
    exit(1)

subprocess.call("rm -rf "+scratch_root+'brava_staging',shell=True)
subprocess.call("mkdir "+scratch_root+'brava_staging',shell=True)
brava_package,core_package = get_cohort_versions(oven_cohort)
installation_required = do_application_package(brava_package,standard_destination)


