#!/usr/bin/env python
# cording: Shift_JIS

import os
import shutil
import glob
import re


def get_name(path):
    slash = path.rfind('\\')
    dot = path.rfind('.')
    return path[slash+1: dot]


def makesure_dir(path):
    if os.path.exists(path) == True:
        return
    os.makedirs(path)


def copy_file(from_path, to_path):
    slash = to_path.rfind('\\')
    directory = to_path[:slash]
    makesure_dir(directory)
    shutil.copyfile(from_path, to_path)


def copy_tree(from_path, to_path):
    slash = to_path.rfind('\\')
    current_dir = to_path[:slash]
    makesure_dir(current_dir)
    shutil.copytree(from_path, to_path)


def delete_tree_makesure(path):
    if os.path.exists(path) == False:
        return
    shutil.rmtree(path)


def copy_tree_after_delete(from_path, to_path):
    delete_tree_makesure(to_path)
    copy_tree(from_path, to_path)


def is_match_ignore_list(path, ignore_list):
    for pattern in ignore_list:
        if pattern.match(path):
            return True
    return False


# copy whole of dir/file in 'from_dir' to 'to_dir'
# but dir/file in 'ignore_list' is not copied
# if 'to_dir' is already exists, it'll be deleted before this process
# from_dir, to_dir must be absolute path
# ignore_list must be done re.compile and absolute path
def copy_tree_ignore(from_dir, to_dir, ignore_list):
    delete_tree_makesure(to_dir)
    for dir_path,dir_name,file_names in os.walk(from_dir):
        for file_name in file_names:
            from_path_abs = dir_path + '\\' + file_name
            if is_match_ignore_list(from_path_abs, ignore_list):
                continue

            dir_path_related = '\\' + os.path.relpath(dir_path, from_dir)
            if dir_path_related == '\\.':
                dir_path_related = ''
            to_path_abs = to_dir + dir_path_related + '\\' + file_name

            copy_file(from_path_abs, to_path_abs)


ignore_list = []
ignore_list.append(re.compile(r'C:\\Users\\MoNoQLoREATOR\\Documents\\projects\\cpp\\simplect3D\\simplect3D_project\\Debug\\.*'))
ignore_list.append(re.compile(r'C:\\Users\\MoNoQLoREATOR\\Documents\\projects\\cpp\\simplect3D\\simplect3D_project\\Release\\.*'))
ignore_list.append(re.compile(r'C:\\Users\\MoNoQLoREATOR\\Documents\\projects\\cpp\\simplect3D\\simplect3D_project\\simplect3D_win7project0x1\.sdf'))

from_dir = r'C:\Users\MoNoQLoREATOR\Documents\projects\cpp\simplect3D'
to_dir = r'C:\Users\MoNoQLoREATOR\Downloads\simplect3D'

print 'copying. please wait...\n'
copy_tree_ignore(from_dir, to_dir, ignore_list)

print 'finished!'
raw_input()