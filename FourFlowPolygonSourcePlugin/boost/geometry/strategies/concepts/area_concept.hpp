// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Copyright Barend Gehrels 2007-2009, Geodan, Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_STRATEGIES_CONCEPTS_AREA_CONCEPT_HPP
#define BOOST_GEOMETRY_STRATEGIES_CONCEPTS_AREA_CONCEPT_HPP



#include <boost/concept_check.hpp>


namespace boost { namespace geometry { namespace concept {


/*!
    \brief Checks strategy for area
    \ingroup area
*/
template <typename Strategy>
class AreaStrategy
{
#ifndef DOXYGEN_NO_CONCEPT_MEMBERS

    // 1) must define state_type,
    typedef typename Strategy::state_type state_type;

    // 2) must define return_type,
    typedef typename Strategy::return_type return_type;

    // 3) must define point_type, of polygon (segments)
    typedef typename Strategy::segment_point_type spoint_type;

    struct check_methods
    {
        static void apply()
        {
            Strategy const* str;
            state_type *st;

            // 4) must implement a method apply with the following signature
            spoint_type const* sp;
            str->apply(*sp, *sp, *st);

            // 5) must implement a static method result with the following signature
            return_type r = str->result(*st);

            boost::ignore_unused_variable_warning(r);
            boost::ignore_unused_variable_warning(str);
        }
    };

public :
    BOOST_CONCEPT_USAGE(AreaStrategy)
    {
        check_methods::apply();
    }

#endif
};



}}} // namespace boost::geometry::concept

#endif // BOOST_GEOMETRY_STRATEGIES_CONCEPTS_AREA_CONCEPT_HPP
