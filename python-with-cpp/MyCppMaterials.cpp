/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyCppMaterials.cpp
@Time: 2025/5/6 22:42
@Desc: 
***************************/

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/stl_bind.h>

#include "CGraph.h"

PYBIND11_MAKE_OPAQUE(std::vector<int>);
PYBIND11_MAKE_OPAQUE(std::map<int, std::string>);

using namespace CGraph;
namespace py = pybind11;

struct MyPosition {
    float x = 0.0f;
    float y = 0.0f;
};


class MyCppSimpleGParam : public GParam {
public:
    int val = 0;
    std::string info;

    CVoid reset(const CStatus& curStatus) override {
        val = 0;
        info.clear();
    }
};


class MyCppFatGParam : public GParam {
public:
    std::vector<int> vec;
    std::map<int, std::string> mp;
    MyPosition position;

    CVoid reset(const CStatus& curStatus) override {
        vec.clear();
        mp.clear();
        position.x = 0.0f;
        position.y = 0.0f;
    }
};


class MyCppGNode1 : public GNode {
protected:
    CStatus run() override {
        CGRAPH_ECHO("MyCppGNode sleep for 1s.");
        CGRAPH_SLEEP_SECOND(1)
        return CStatus();
    }
};


class MyCppGNode2 : public GNode {
protected:
    CStatus init() override {
        return createGParam<MyCppFatGParam>("fat");
    }

    CStatus run() override {
        auto param = getGParamWithNoEmpty<MyCppSimpleGParam>("simple");
        {
            CGRAPH_PARAM_WRITE_CODE_BLOCK(param);
            param->val += 1;
            param->info = "cpp set info to " + std::to_string(param->val);
        }

        return CStatus();
    }
};


class MyCppGNode3 : public GNode {
protected:
    CStatus run() override {
        auto param = getGParamWithNoEmpty<MyCppFatGParam>("fat");
        for (auto x : param->vec) {
            std::cout << getName() <<  " [cpp] vec : " << x << std::endl;
        }

        for (auto iter : param->mp) {
            std::cout << getName() << " [cpp] mp : " << iter.first << ", " << iter.second << std::endl;
        }

        std::cout << getName() << " [cpp] position.x : " << param->position.x << ", position.y : " << param->position.y << std::endl;
        return CStatus();
    }
};


PYBIND11_MODULE(MyCppMaterials, m) {
    m.doc() = "PyCGraph-example demo objects";

    py::bind_vector<std::vector<int> >(m, "VectorInt");
    py::bind_map<std::map<int, std::string> >(m, "MapIntString");

    py::implicitly_convertible<py::iterable, std::vector<int>>();
    py::implicitly_convertible<py::dict, std::map<int, std::string>>();

    py::class_<MyCppGNode1, GElement, std::unique_ptr<MyCppGNode1, py::nodelete> >(m, "MyCppGNode1")
        .def(py::init<>());

    py::class_<MyCppGNode2, GElement, std::unique_ptr<MyCppGNode2, py::nodelete> >(m, "MyCppGNode2")
        .def(py::init<>());

    py::class_<MyCppGNode3, GElement, std::unique_ptr<MyCppGNode3, py::nodelete> >(m, "MyCppGNode3")
        .def(py::init<>());

    py::class_<MyPosition, std::unique_ptr<MyPosition> >(m, "MyPosition")
        .def(py::init<>())
        .def_readwrite("x", &MyPosition::x)
        .def_readwrite("y", &MyPosition::y);

    py::class_<MyCppSimpleGParam, GParam, std::unique_ptr<MyCppSimpleGParam, py::nodelete> >(m, "MyCppSimpleGParam")
        .def(py::init<>())
        .def_readwrite("val", &MyCppSimpleGParam::val)
        .def_readwrite("info", &MyCppSimpleGParam::info);

    py::class_<MyCppFatGParam, GParam, std::unique_ptr<MyCppFatGParam, py::nodelete> >(m, "MyCppFatGParam")
        .def(py::init<>())
        .def_readwrite("vec", &MyCppFatGParam::vec)
        .def_readwrite("mp", &MyCppFatGParam::mp)
        .def_readwrite("position", &MyCppFatGParam::position);

        // use def_property to support: param.mp = {1: "a", 2: "b"} in python
        /**
        .def_property("mp",
            [] (MyCppFatGParam &self) -> std::map<int, std::string>& {
                return self.mp;
            },
            [] (MyCppFatGParam &self, const py::dict &dict) {
                self.mp.clear();
                for (const auto& item : dict) {
                    auto key = item.first.cast<int>();
                    auto value = item.second.cast<std::string>();
                    self.mp[key] = value;
                }
            }
        );
        **/
}
