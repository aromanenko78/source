#! /usr/bin/env python
import functools
import pickle
import os
import shutil

class Config:
  def __init__(self):
    self.source = ''
    self.destination = ''

  def __repr__(self):
    return "Config\n  source={}\n  destination={}".format(self.source, 
        self.destination)

  def destinationManifest(self):
    return os.path.join(self.destination, "manifest.txt")

  def destinationStore(self):
    return os.path.join(self.destination, "store")

@functools.total_ordering
class FileInfo:
  def __init__(self, fullPath, rootPath="", lastModified=0.0):
    fullPath = fullPath.strip()

    if rootPath != "":
      self.fullPath = os.path.relpath(fullPath, rootPath)
    else:
      self.fullPath = fullPath

    if (lastModified == 0.0):
      self.lastModified = float(self._readLastModifiedTime(fullPath))
    else:
      self.lastModified = float(lastModified)

  def _readLastModifiedTime(self, fullPath):
    try:
      return os.stat(fullPath).st_mtime
    except OSError, e:
      return 0

  def __repr__(self):
    return self.fullPath

  def __eq__(self, other):
    return (self.fullPath, self.lastModified) == \
           (other.fullPath, other.lastModified)

  def __lt__(self, other):
    return (self.fullPath, self.lastModified) < \
           (other.fullPath, other.lastModified)

  def __hash__(self):
    return self.fullPath.__hash__() ^ self.lastModified.__hash__()

def loadConfig():
  with open('./backup.config') as configFile:
    config = pickle.load(configFile)
  return config

def getFileListFromDisk(rootPath):
  rootPath = os.path.abspath(rootPath)
  return [FileInfo(os.path.join(current, f), rootPath=rootPath) 
      for (current, dirs, files) in os.walk(rootPath)
      for f in files]

def writeManifestFile(fileList, manifest):
  with open(manifest, "w") as manifestFile:
    manifestFile.writelines( \
        ["{} {}\n".format(f.lastModified, f.fullPath) for f in fileList])

def parseManifestLine(line):
  split = line.split(" ", 2)
  return (split[0], split[1])

def readManifestFile(manifest):
  if os.path.exists(manifest):
    with open(manifest) as manifestFile:
      return [FileInfo(f, lastModified=t) for (t, f) in
          [parseManifestLine(line) for line in manifestFile.readlines()]]
  else:
    return []

def deleteBackupFile(file):
  # TODO: implement recycle bin
  os.remove(file)

def copyNewFiles(files, sourceRoot, destinationRoot):
  # TODO: handle errors
  for f in files:
    source = os.path.join(sourceRoot, f)
    destination = os.path.join(destinationRoot, f)

    print "storing", source
    dir = os.path.dirname(destination)

    if !os.path.exists(dir):
      os.makedirs(dir)

    if os.path.exists(destinationFile):
      print "removing", destination
      deleteBackupFile(destinationFile)

    shutil.copy(sourceFile, destinationFile)

def deleteOldFiles(files, destinationRoot):
  # TODO: handle errors
  for f in files:
    destination = os.path.join(destinationRoot, f)
    print "removing", destination
    deleteBackupFile(destination)

def main():
  config = loadConfig()

  print config

  oldFiles = readManifestFile(config.destinationManifest())
  newManifest = getFileListFromDisk(config.source)

  filesToCopy = set(newManifest) - set(oldManifest)
  filesToDelete = set(oldManifest) - set(newManifest)

  print "files to copy", filesToCopy
  copyNewFiles(filesToCopy, config.source, config.destinationStore())

  print "file to delete", filesToDelete
  deleteOldFiles(filesToDelete, config.destinationStore())

  # TODO: handle errors and only write files that were processed successfully
  writeManifestFile(newManifest, config.destinationManifest())


  # TODO
  # - inclusions and exclusions in config
  # - recycle bin
  # - delays
  # - error report

if __name__ == "__main__":
    main()