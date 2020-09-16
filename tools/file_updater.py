#!/usr/bin/env python
import shutil
import tempfile
import filecmp

def copyIfDifferent(src, dst_path):
    dst = None
    try:
        dst = open(dst_path, 'r+b')
        src.seek(0)
        if src.read() == dst.read():
            return
        dst.seek(0)
    except:
        pass
    if dst is None:
        dst = open(dst_path, 'w+b')
    src.seek(0)
    shutil.copyfileobj(src, dst)

class FileUpdater:
    def __init__(self, path):
        self.filePath = path
        self.tempFile = tempfile.TemporaryFile(mode='r+b')

    def __del__(self):
        if not self.tempFile.closed:
            self.close()

    def write(self, str):
        self.tempFile.write(str)

    def close(self):
        copyIfDifferent(self.tempFile, self.filePath)
        self.tempFile.close()
