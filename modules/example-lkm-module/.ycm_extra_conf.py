import subprocess

kernel_release = subprocess.check_output(['uname', '-r']).rstrip()

def FlagsForFile( filename, **kwargs ):
        return { 'flags': [
            '-I/usr/include',
            '-I/usr/local/include',
            '-I/usr/src/linux-headers-' + kernel_release + '/arch/x86/include',
            '-I/usr/src/linux-headers-' + kernel_release + '/arch/x86/include/uapi',
            '-I/usr/src/linux-headers-' + kernel_release + '/include',
            '-D__KERNEL__',
            '-std=gnu99',
            '-xc'
        ]}
