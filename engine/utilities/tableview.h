
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

/*! \file utilities/tableview.h
 *  \brief Lightweight objects that offer access to multi-dimensional tables.
 */

#ifndef __REGINA_TABLEVIEW_H
#ifndef __DOXYGEN
#define __REGINA_TABLEVIEW_H
#endif

#include <array>
#include <cstddef>
#include <type_traits>
#include "regina-core.h"

namespace regina {

template <typename Element, size_t dim1, size_t... dim>
class TableView;

namespace detail {
    /**
     * Provides some implementation details for TableView.
     *
     * This helper struct exists so that we do not need to specialise the
     * entire TableView class template.
     *
     * It provides two type aliases: \a type and \a view.
     *
     * If the dimension pack \a dim contains some positive number of dimensions
     * \a d1, ..., \a dk, then:
     *
     * - \a type represents <tt>const T[d1]...[dk]</tt>;
     * - \a view represents <tt>TableView<T, d1, ..., dk></tt>.
     *
     * If the dimension pack \a dim is empty, then:
     *
     * - \a type represents <tt>const T</tt>;
     * - \a view represents <tt>const T&</tt>.
     */
    template <typename T, size_t... dim>
    struct ConstArrayOf;

#ifndef __DOXYGEN
    template <typename T>
    struct ConstArrayOf<T> {
        using type = const T;
        using view = const T&;
    };

    template <typename T, size_t dim1, size_t... dim>
    struct ConstArrayOf<T, dim1, dim...> {
        using type = typename ConstArrayOf<T, dim...>::type[dim1];
        using view = TableView<T, dim1, dim...>;
    };
#endif // __DOXYGEN
}

/**
 * A lightweight object that can be used for random access
 * to all elements of a given multi-dimensional table.
 *
 * This access is read-only, in the sense that both the table itself and the
 * table elements are read-only.  (Of course, if the table elements are
 * non-const pointers then this means that the \e pointers cannot be reassigned
 * to point to different objects, but the objects they \e point to can still
 * be modified.)
 *
 * This class is in a sense a multi-dimensional analogue to TableView
 * (though it does also support one-dimensional tables).
 *
 * Where this class differs from ListView is:
 *
 * - TableView supports multi-dimensional tables, whereas ListView only
 *   supports one-dimensional lists.
 *
 * - TableView offers a smaller set of member functions, whereas ListView
 *   has a richer interface.
 *
 * - TableView is (for now) only designed to work with fixed-size C-style
 *   arrays of the form <tt>Element[a][b]...[z]</tt>, where the array
 *   dimensions are compile-time constants.  In contrast, ListView can also
 *   work with rich C++ container classes and variable-sized C-style arrays.
 *
 * - While ListView has a purpose in C++ (to hide the "real" type used
 *   by the underlying implementation), TableView is primary for the Python
 *   bindings: its main benefit is to strictly enforce read-only access
 *   (since Python loses all knowledge of constness, and sometimes allows
 *   users to change things that they should not).  Typically TableView
 *   would be used to wrap global constant arrays (such as regina::quadDefn,
 *   or regina::Edge<3>::edgeNumber).
 *
 * TableView comes with deduction guides for tables of dimension ≤ 3.
 * This means that you can simply create a TableView using the syntax
 * <tt>TableView(array)</tt>, where \a array is the underlying C-style array,
 * without having to specify any TableView template arguments.
 *
 * TableView objects are small enough to pass by value and swap with
 * std::swap(), with no need for any specialised move operations or swap
 * functions.
 *
 * \ifacespython The TableView classes are deliberately difficult to access:
 * they live within their own private Python namespaces, and are all given the
 * same class name (\a TableView).  You would typically only interact with a
 * TableView when accessing a constant array (e.g., Edge<3>::edgeNumber).
 * In most cases you would simply use array-like operators to access the
 * elements of such a table without ever knowing its exact type.
 *
 * \tparam Element the type of element stored in the C-style array.
 * This should not be a \c const type; the \c const modifier will be added
 * automatically where necessary through the class interface.
 *
 * \tparam dim the dimensions of the C-style array.  There must always be at
 * least one dimension supplied.
 *
 * \ingroup utilities
 */
template <typename Element, size_t dim1, size_t... dim>
class TableView {
    static_assert(! std::is_const_v<Element>,
        "When declaring a TableView<Element, ...>, the type Element should not "
        "be const-qualified.  The constness will be added automatically.");

    public:
        /**
         * The type returned by the square bracket operator.  For an array
         * with multiple dimensions, this is a TableView of smaller dimension.
         * For a one-dimensional array, this is a const reference to \a Element.
         *
         * See the square bracket operator for further details.
         */
        using Subview = typename detail::ConstArrayOf<Element, dim...>::view;

        /**
         * The type of a slice of the underlying C-style array, when the
         * first array index is fixed.  If this array is one-dimensional,
         * then this is simply <tt>const Element</tt>.
         */
        using Subarray = typename detail::ConstArrayOf<Element, dim...>::type;

        /**
         * The type of the underlying C-style array, with all compile-time
         * constant dimensions specified.
         */
        using Array = typename detail::ConstArrayOf<Element, dim1, dim...>::
            type;

        /**
         * The native integer type used for array indexing.
         */
        using size_type = size_t;

        /**
         * The dimension of the underlying C-style array.
         *
         * This is the number of subscripts required to access an
         * individual array element.
         */
        static constexpr int dimension = 1 + sizeof...(dim);

        /**
         * The type of element that is stored in this array.
         */
        using value_type = Element;

        /**
         * A reference to a single array element.
         *
         * Both \a reference and \a const_reference are the same, since
         * this class only offers read-only access to the underlying array.
         */
        using reference = const Element&;

        /**
         * A reference to a single array element.
         *
         * Both \a reference and \a const_reference are the same, since
         * this class only offers read-only access to the underlying array.
         */
        using const_reference = const Element&;

    private:
        Subarray* array_;
            /**< The underlying C-style array. */

    public:
        /**
         * Returns a view for the given C-style array.
         *
         * \nopython
         *
         * @param array the pointer to the C-style array.
         */
        constexpr TableView(Subarray* array) : array_(array) {
        }

        /**
         * Creates a new copy of the given table view.
         */
        constexpr TableView(const TableView&) = default;

        /**
         * Sets this to be a copy of the given table view.
         *
         * @return a reference to this table view.
         */
        TableView& operator = (const TableView&) = default;

        /**
         * Returns the size of this array, across all of the array dimensions.
         *
         * This returns a sequence (\a s_1, \a s_2, ..., \a s_dim), where
         * \a dim is the dimension of this array, and where the <i>k</i>th
         * array subscript ranges from 0 to (\a s_k-1) inclusive.
         *
         * \ifacespython In Python, the special method __len__() returns
         * \a s_1.  That is, it indicates the allowed range for the
         * \e first array subscript.
         *
         * @return the size of this array, across all of the array dimensions.
         */
        constexpr std::array<size_t, dimension> size() const {
            return { dim1, dim... };
        }

        /**
         * Returns the requested sub-array of a multi-dimensional array,
         * or the requested element of a one-dimensional array.
         *
         * If this array is one-dimensional, then this operator simply
         * returns the (\a index)th element (as a const reference).
         *
         * If this array has more than one dimension, then this operator
         * returns a TableView of smaller dimension, representing the slice
         * of the overall table obtained when the first array index is set
         * to the given value.
         *
         * Typically this operator would just be used to access an
         * individual element using the syntax
         * <tt>array[index_1][index_2]...[index_dim]</tt>, where
         * \a dim is the dimension of this array.
         *
         * @param index indicates which element or sub-array to return;
         * this must be between 0 and (\a dim1-1) inclusive.
         * @return the (\a index)th sub-array.
         */
        constexpr Subview operator [](size_t index) const {
            return array_[index];
        }

        /**
         * Determines whether this and the given table view are accessing
         * the same underlying C-style array.
         *
         * To be considered the same array, the two arrays must have the same
         * location in memory (i.e., the pointers that define the C-style arrays
         * must be equal).  In particular, it is not enough for the two arrays
         * just to have identical contents.
         *
         * @param other the table view to compare with this.
         * @return \c true if and only if this and the given table use
         * the same underlying C-style array.
         */
        constexpr bool operator == (const TableView& other) const {
            return (array_ == other.array_);
        }

        /**
         * Determines whether this and the given table view are accessing
         * different underlying C-style arrays.
         *
         * To be considered the same array, the two arrays must have the same
         * location in memory (i.e., the pointers that define the C-style arrays
         * must be equal).  In particular, it is not enough for the two arrays
         * just to have identical contents.
         *
         * @param other the table view to compare with this.
         * @return \c true if and only if this and the given table use
         * different underlying C-style arrays.
         */
        constexpr bool operator != (const TableView& other) const {
            return (array_ != other.array_);
        }
};

// Deduction guides (hide these from Doxygen, which cannot handle them):

#ifndef __DOXYGEN
template <typename Element, size_t a>
TableView(const Element (&)[a]) -> TableView<Element, a>;

template <typename Element, size_t a, size_t b>
TableView(const Element (&)[a][b]) -> TableView<Element, a, b>;

template <typename Element, size_t a, size_t b, size_t c>
TableView(const Element (&)[a][b][c]) -> TableView<Element, a, b, c>;
#endif

} // namespace regina

#endif
