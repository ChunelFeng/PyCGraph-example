"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: setup
@Time: 2025/5/6 22:45
@Desc:
"""

import glob
from setuptools import setup, Extension
import pybind11


_include_dirs = [pybind11.get_include(),
                 '../CGraph/src/']
_sources = ["MyCppMaterials.cpp"] + glob.glob("../CGraph/src/**/*.cpp", recursive=True)
_extra_compile_args = ["-pthread",
                       "-std=c++11",
                       '-fvisibility=hidden']
_ext_modules = [
    Extension(
        name="MyCppMaterials",
        sources=_sources,
        extra_compile_args=_extra_compile_args,
        include_dirs=_include_dirs,
    ),
]

setup(
    name="MyCppMaterials",
    version="1.0.0",
    author="Chunel",
    author_email="chunel@foxmail.com",
    license="MIT",
    ext_modules=_ext_modules,
    zip_safe=False,
)
