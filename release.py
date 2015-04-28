#!/usr/bin/env python
# cording: UTF-8

import os
import shutil
import glob

fromCurrent = 'C:\\Users\\MoNoQLoREATOR\\Documents\\CppProjects\\simplect3D_3.1.0\\'
toCurrent = 'C:\\Users\\MoNoQLoREATOR\\Downloads\\GM31_No.01_2.1.0\\'


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


fromWild = fromCurrent + 'simplect3D_project\\use_simplect3D\\data\\Lat_Miku\\*.bmp'
for fromPath in glob.glob(fromWild):
    name = get_name(fromPath)
    toPath = toCurrent + 'bin\\data\\Lat_Miku\\' + name + '.bmp'
    copy_file(fromPath, toPath)

fromWild = fromCurrent + 'simplect3D_project\\use_simplect3D\\data\\Lat_Miku\\*.png'
for fromPath in glob.glob(fromWild):
    name = get_name(fromPath)
    toPath = toCurrent + 'bin\\data\\Lat_Miku\\' + name + '.png'
    copy_file(fromPath, toPath)


fromPath = fromCurrent + 'simplect3D_project\\use_simplect3D\\data\\Lat_Miku\\Miku_newface.pmd'
toPath = toCurrent + 'bin\\data\\Lat_Miku\\Miku_newface.pmd'
copy_file(fromPath, toPath)

fromPath = fromCurrent + 'simplect3D_project\\use_simplect3D\\data\\Lat_Miku\\Miku_alpha.pmd'
toPath = toCurrent + 'bin\\data\\Lat_Miku\\Miku_alpha.pmd'
copy_file(fromPath, toPath)

fromPath = fromCurrent + 'simplect3D_project\\Release\\use_simplect3D.exe'
toPath = toCurrent + 'bin\\use_simplect3D.exe'
copy_file(fromPath, toPath)

fromPath = fromCurrent + 'simplect3D'
toPath = toCurrent + 'projects\\simplect3D'
copy_tree_after_delete(fromPath, toPath)

fromPath = fromCurrent + 'simplect3D_cpp'
toPath = toCurrent + 'projects\\simplect3D_cpp'
copy_tree_after_delete(fromPath, toPath)

fromPath = fromCurrent + 'simplect3D_project\\simplect3D_win7project0x1.sln'
toPath = toCurrent + 'projects\\simplect3D_project\\simplect3D_win7project0x1.sln'
copy_file(fromPath, toPath)

fromPath = fromCurrent + 'simplect3D_project\\use_simplect3D\\use_simplect3D.vcxproj'
toPath = toCurrent + 'projects\\simplect3D_project\\use_simplect3D\\use_simplect3D.vcxproj'
copy_file(fromPath, toPath)

fromPath = fromCurrent + 'simplect3D_project\\use_simplect3D\\use_simplect3D.vcxproj.filters'
toPath = toCurrent + 'projects\\simplect3D_project\\use_simplect3D\\use_simplect3D.vcxproj.filters'
copy_file(fromPath, toPath)

fromPath = fromCurrent + 'simplect3D_project\\use_simplect3D\\WinMain.cpp'
toPath = toCurrent + 'projects\\simplect3D_project\\use_simplect3D\\WinMain.cpp'
copy_file(fromPath, toPath)

raw_input()