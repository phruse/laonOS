#  SPDX-License-Identifier: MIT

import sys
import os

defineText = '#define OS_KERNEL_SIZE'

try:
    fileSize = os.path.getsize(sys.argv[1])
except OSError:
    fileSize = 0

fileData = ''

with open(sys.argv[2], 'r') as defineFile:
    for line in defineFile.readlines():
        if line[:min(len(line), len(defineText))] == defineText:
            fileData += \
                line.replace(line, defineText + ' ' + str(fileSize) + '\n')
        else:
            fileData += line

with open(sys.argv[2], 'w') as file:
    file.write(fileData)
