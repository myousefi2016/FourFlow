// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Copyright Barend Gehrels 2007-2009, Geodan, Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_CORE_EXCEPTION_HPP
#define BOOST_GEOMETRY_CORE_EXCEPTION_HPP

#include <exception>

namespace boost { namespace geometry {

/*!
\brief Base exception class for GGL
\ingroup core
*/
struct exception : public std::exception
{
};

}} // namespace boost::geometry

#endif // BOOST_GEOMETRY_CORE_EXCEPTION_HPP
