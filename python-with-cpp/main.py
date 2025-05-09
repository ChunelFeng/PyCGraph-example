#!/usr/bin/env python
# -*- coding: UTF-8 -*-

"""
@Project ：PyCGraph-example 
@File    ：main.py
@Author  ：Chunel
@Date    ：2025/5/5 23:42 
"""

from PyCGraph import GNode, CStatus, GPipeline
from MyCppMaterials import MyCppSimpleGParam, MyCppFatGParam, MyPosition
from MyCppMaterials import MyCppGNode1, MyCppGNode2, MyCppGNode3


class MyPyGNode1(GNode):
    def init(self) -> CStatus:
        return self.createGParam(MyCppSimpleGParam(), 'simple')

    def run(self) -> CStatus:
        fat: MyCppFatGParam = self.getGParamWithNoEmpty('fat')
        fat.vec = [1, 2]
        for i in range(0, 2):
            fat.vec.append(i + 10)

        fat.mp[1] = 'a'
        fat.mp[2] = 'b'

        fat.position.x = 3.10
        fat.position.y = 3.25
        return CStatus()


class MyPyGNode2(GNode):
    def run(self) -> CStatus:
        fat: MyCppFatGParam = self.getGParamWithNoEmpty('fat')
        fat.lock()    # lock param if needed
        for i in fat.vec:
            print(self.getName(), '[py] vec :', i)

        for k,v in fat.mp.items():
            print(self.getName(), '[py] mp :', k, v)
        fat.unlock()
        return CStatus()


class MyPyGNode3(GNode):
    def run(self) -> CStatus:
        simple: MyCppSimpleGParam = self.getGParamWithNoEmpty('simple')
        print(self.getName(), '[py] val:' , simple.val, ', info:', simple.info)
        return CStatus()


def main():
    pipeline = GPipeline()
    cpp_a, cpp_b, cpp_e = MyCppGNode1(), MyCppGNode2(), MyCppGNode3()
    py_c, py_d, py_f = MyPyGNode1(), MyPyGNode2(), MyPyGNode3()
    pipeline.registerGElement(cpp_a, set(), 'nodeA')
    pipeline.registerGElement(cpp_b, {cpp_a}, 'nodeB')
    pipeline.registerGElement(py_c, {cpp_b}, 'nodeC')
    pipeline.registerGElement(py_d, {py_c}, 'nodeD')
    pipeline.registerGElement(cpp_e, {py_d}, 'nodeE')
    pipeline.registerGElement(py_f, {cpp_e}, 'nodeF')

    pipeline.process()


if __name__ == '__main__':
    main()
