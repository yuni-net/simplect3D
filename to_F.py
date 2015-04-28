#!/usr/bin/env python
# cording: UTF-8

import os
import shutil
import glob

fromCurrent = 'C:\\Users\\MoNoQLoREATOR\\Documents\\CppProjects\\simplect3D_3.1.0\\'
toCurrent = 'F:\\simplect3D_3.1.0\\'


def get_name(path):
    slash = path.rfind('\\')
    dot = path.rfind('.')
    return path[slash+1: dot]


def makesure_dir(path):
    if os.path.exists(path) == True:
        return
    os.makedirs(path)


def copy_file(fromPath, toPath):
    slash = toPath.rfind('\\')
    directory = toPath[:slash]
    makesure_dir(directory)
    shutil.copyfile(fromPath, toPath)


def copy_tree(fromPath, toPath):
    slash = toPath.rfind('\\')
    currentDir = toPath[:slash]
    makesure_dir(currentDir)
    shutil.copytree(fromPath, toPath)


def delete_tree_makesure(path):
    if os.path.exists(path) == False:
        return
    shutil.rmtree(path)


def copy_tree_after_delete(fromPath, toPath):
    delete_tree_makesure(toPath)
    copy_tree(fromPath, toPath)


print 'processing'
print 'please wait...'

fromPath = fromCurrent + 'memo.txt'
toPath = toCurrent + 'memo.txt'
copy_file(fromPath, toPath)

fromPath = fromCurrent + 'release.py'
toPath = toCurrent + 'release.py'
copy_file(fromPath, toPath)

fromPath = fromCurrent + 'to_F.py'
toPath = toCurrent + 'to_F.py'
copy_file(fromPath, toPath)

fromPath = fromCurrent + 'simplect3D'
toPath = toCurrent + 'simplect3D'
copy_tree_after_delete(fromPath, toPath)

fromPath = fromCurrent + 'simplect3D_cpp'
toPath = toCurrent + 'simplect3D_cpp'
copy_tree_after_delete(fromPath, toPath)

fromPath = fromCurrent + 'simplect3D_project\simplect3D_win7project0x1.sln'
toPath = toCurrent + 'simplect3D_project\simplect3D_win7project0x1.sln'
copy_file(fromPath, toPath)

fromPath = fromCurrent + 'simplect3D_project\use_simplect3D\use_simplect3D.vcxproj'
toPath = toCurrent + 'simplect3D_project\use_simplect3D\use_simplect3D.vcxproj'
copy_file(fromPath, toPath)

fromPath = fromCurrent + 'simplect3D_project\use_simplect3D\use_simplect3D.vcxproj.filters'
toPath = toCurrent + 'simplect3D_project\use_simplect3D\use_simplect3D.vcxproj.filters'
copy_file(fromPath, toPath)

fromPath = fromCurrent + 'simplect3D_project\use_simplect3D\data\Lat_Miku'
toPath = toCurrent + 'simplect3D_project\use_simplect3D\data\Lat_Miku'
copy_tree_after_delete(fromPath, toPath)

fromPath = fromCurrent + 'simplect3D_project\use_simplect3D\WinMain.cpp'
toPath = toCurrent + 'simplect3D_project\use_simplect3D\WinMain.cpp'
copy_file(fromPath, toPath);

print 'finished!'
raw_input()