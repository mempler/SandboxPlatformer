import subprocess
import os

workingDir = 'game/resources'
destDir = 'build/release'

def ascp():
    if os.name == 'nt':
        return 'ASCP.exe'
    else:
        return 'ASCP'

def process_asset(src: str, dest: str):
    if not os.path.exists(os.path.join(
            destDir, dest)):
        os.makedirs(os.path.join(
            destDir, dest))

    subprocess.call(['./tools/bin/' + ascp(),
                     '+output', os.path.join(destDir, dest),
                     '+file', os.path.join(workingDir, src),
                     '+metadata', './tools/ASCP/metadata'])


f = open(workingDir + '/files.meta', 'r')
for ln in f.readlines():
    if ln.startswith('#'):
        continue

    if ln == "":
        continue

    x = ln.split('->')

    inputFile = x[0].strip()
    dest = x[1].strip()

    process_asset(inputFile, dest)
    print(ln)
