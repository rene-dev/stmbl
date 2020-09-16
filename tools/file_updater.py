#!/usr/bin/env python
import shutil
import tempfile
import filecmp

def copyIfDifferent(src, dst_path):
    dst = None
    try:
        dst = open(dst_path, 'r+')
        src.seek(0)
        if src.read() == dst.read():
            return
        dst.seek(0)
    except:
        pass
    if dst is None:
        dst = open(dst_path, 'w')
    src.seek(0)
    shutil.copyfileobj(src, dst)

class FileUpdater:
    def __init__(self, path):
        self.filePath = path
        self.tempFile = tempfile.TemporaryFile(mode='r+')

    def __del__(self):
        if not self.tempFile.closed:
            self.close()

    def write(self, data):
        self.tempFile.write(data)

    def close(self):
        copyIfDifferent(self.tempFile, self.filePath)
        self.tempFile.close()
