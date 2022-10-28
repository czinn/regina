
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#include "../pybind11/pybind11.h"
#include "../pybind11/operators.h"
#include "../pybind11/stl.h"
#include "maths/perm.h"
#include "../constarray.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::Perm;
using regina::python::ConstArray;

namespace {
    ConstArray<decltype(Perm<8>::S8), int>
        Perm8_S8_arr(Perm<8>::S8, 40320);
    ConstArray<decltype(Perm<8>::orderedS8), int>
        Perm8_orderedS8_arr(Perm<8>::orderedS8, 40320);

    template <int k>
    struct Perm8_contract {
        template <class C, typename... options>
        static void add_bindings(pybind11::class_<C, options...>& c) {
            c.def_static("contract", &Perm<8>::contract<k>);
            Perm8_contract<k+1>::add_bindings(c);
        }
    };

    template <>
    struct Perm8_contract<16> {
        template <class C, typename... options>
        static void add_bindings(pybind11::class_<C, options...>& c) {
            c.def_static("contract", &Perm<8>::contract<16>);
        }
    };
}

void addPerm8(pybind11::module_& m) {
    decltype(Perm8_S8_arr)::wrapClass(m, "ConstArray_Perm8_S8");
    decltype(Perm8_orderedS8_arr)::wrapClass(m, "ConstArray_Perm8_orderedS8");

    auto c = pybind11::class_<Perm<8>>(m, "Perm8")
        .def(pybind11::init<>())
        .def(pybind11::init<int, int>())
        .def(pybind11::init<int, int, int, int, int, int, int, int>())
        .def(pybind11::init<int, int, int, int, int, int, int, int,
                            int, int, int, int, int, int, int, int>())
        .def(pybind11::init<const std::array<int, 8>&>())
        .def(pybind11::init<const Perm<8>&>())
        .def_static("precompute", &Perm<8>::precompute)
        .def("permCode1", &Perm<8>::permCode1)
        .def("permCode2", &Perm<8>::permCode2)
        .def("setPermCode1", &Perm<8>::setPermCode1)
        .def("setPermCode2", &Perm<8>::setPermCode2)
        .def_static("fromPermCode1", &Perm<8>::fromPermCode1)
        .def_static("fromPermCode2", &Perm<8>::fromPermCode2)
        .def_static("isPermCode1", &Perm<8>::isPermCode1)
        .def_static("isPermCode2", &Perm<8>::isPermCode2)
        .def("imagePack", &Perm<8>::imagePack)
        .def_static("fromImagePack", &Perm<8>::fromImagePack)
        .def_static("isImagePack", &Perm<8>::isImagePack)
        .def(pybind11::self * pybind11::self)
        .def("cachedComp", overload_cast<const Perm<8>&>(
            &Perm<8>::cachedComp, pybind11::const_))
        .def("cachedComp", overload_cast<const Perm<8>&, const Perm<8>&>(
            &Perm<8>::cachedComp, pybind11::const_))
        .def("inverse", &Perm<8>::inverse)
        .def("pow", &Perm<8>::pow)
        .def("cachedPow", &Perm<8>::cachedPow)
        .def("order", &Perm<8>::order)
        .def("reverse", &Perm<8>::reverse)
        .def("sign", &Perm<8>::sign)
        .def("__getitem__", &Perm<8>::operator[])
        .def("pre", &Perm<8>::pre)
        .def("compareWith", &Perm<8>::compareWith)
        .def("isIdentity", &Perm<8>::isIdentity)
        .def("inc", [](Perm<8>& p) {
            return p++;
        })
        .def(pybind11::self < pybind11::self)
        .def_static("rot", &Perm<8>::rot)
        .def_static("rand", (Perm<8> (*)(bool))(&Perm<8>::rand),
            pybind11::arg("even") = false)
        .def("trunc", &Perm<8>::trunc)
        .def("clear", &Perm<8>::clear)
        .def("S8Index", (int (Perm<8>::*)() const) &Perm<8>::S8Index)
        .def("SnIndex", &Perm<8>::SnIndex)
        .def("orderedS8Index", &Perm<8>::orderedS8Index)
        .def("orderedSnIndex", &Perm<8>::orderedS8Index)
        .def("isConjugacyMinimal", &Perm<8>::isConjugacyMinimal)
        .def_static("extend", &Perm<8>::extend<2>)
        .def_static("extend", &Perm<8>::extend<3>)
        .def_static("extend", &Perm<8>::extend<4>)
        .def_static("extend", &Perm<8>::extend<5>)
        .def_readonly_static("codeType", &Perm<8>::codeType)
        .def_readonly_static("imageBits", &Perm<8>::imageBits)
        .def_readonly_static("imageMask", &Perm<8>::imageMask)
        .def_readonly_static("nPerms", &Perm<8>::nPerms)
        .def_readonly_static("nPerms_1", &Perm<8>::nPerms_1)
        .def_readonly_static("S8", &Perm8_S8_arr)
        .def_readonly_static("Sn", &Perm8_S8_arr)
        .def_readonly_static("orderedS8", &Perm8_orderedS8_arr)
        .def_readonly_static("orderedSn", &Perm8_orderedS8_arr)
    ;
    Perm8_contract<9>::add_bindings(c);
    regina::python::add_output_basic(c);
    regina::python::add_tight_encoding(c);
    regina::python::add_eq_operators(c);
}

