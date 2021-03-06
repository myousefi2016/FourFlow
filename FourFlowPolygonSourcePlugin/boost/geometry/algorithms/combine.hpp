// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Copyright Barend Gehrels 2007-2009, Geodan, Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_ALGORITHMS_COMBINE_HPP
#define BOOST_GEOMETRY_ALGORITHMS_COMBINE_HPP

#include <cstddef>

#include <boost/numeric/conversion/cast.hpp>

#include <boost/geometry/core/coordinate_dimension.hpp>
#include <boost/geometry/geometries/concepts/check.hpp>

#include <boost/geometry/util/select_coordinate_type.hpp>

#include <boost/geometry/strategies/compare.hpp>
#include <boost/geometry/policies/compare.hpp>


/*!
\defgroup combine combine: add a geometry to a bounding box
\par Geometries:
- \b box + \b box -> \b box: the box will be combined with the other box
    \image html combine_box_box.png
    \note Also if the input box is incorrect, the box will correctly updated
- \b box + \b point -> \b box: the box will combined with the point
    \image html combine_box_point.png
- \b box + \b segment -> \b box
*/
namespace boost { namespace geometry
{

#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace combine {


template
<
    typename Box, typename Point,
    typename StrategyLess, typename StrategyGreater,
    std::size_t Dimension, std::size_t DimensionCount
>
struct point_loop
{
    typedef typename strategy::compare::detail::select_strategy
        <
            StrategyLess, 1, Point, Dimension
        >::type less_type;

    typedef typename strategy::compare::detail::select_strategy
        <
            StrategyGreater, -1, Point, Dimension
        >::type greater_type;

    typedef typename select_coordinate_type<Point, Box>::type coordinate_type;

    static inline void apply(Box& box, Point const& source)
    {
        less_type less;
        greater_type greater;

        coordinate_type const coord = get<Dimension>(source);

        if (less(coord, get<min_corner, Dimension>(box)))
        {
            set<min_corner, Dimension>(box, coord);
        }

        if (greater(coord, get<max_corner, Dimension>(box)))
        {
            set<max_corner, Dimension>(box, coord);
        }

        point_loop
            <
                Box, Point,
                StrategyLess, StrategyGreater,
                Dimension + 1, DimensionCount
            >::apply(box, source);
    }
};


template
<
    typename Box, typename Point,
    typename StrategyLess, typename StrategyGreater,
    std::size_t DimensionCount
>
struct point_loop
    <
        Box, Point,
        StrategyLess, StrategyGreater,
        DimensionCount, DimensionCount
    >
{
    static inline void apply(Box&, Point const&) {}
};


template
<
    typename Box, typename Geometry,
    typename StrategyLess, typename StrategyGreater,
    std::size_t Index,
    std::size_t Dimension, std::size_t DimensionCount
>
struct indexed_loop
{
    typedef typename strategy::compare::detail::select_strategy
        <
            StrategyLess, 1, Box, Dimension
        >::type less_type;

    typedef typename strategy::compare::detail::select_strategy
        <
            StrategyGreater, -1, Box, Dimension
        >::type greater_type;

    typedef typename select_coordinate_type
            <
                Box,
                Geometry
            >::type coordinate_type;


    static inline void apply(Box& box, Geometry const& source)
    {
        less_type less;
        greater_type greater;

        coordinate_type const coord = get<Index, Dimension>(source);

        if (less(coord, get<min_corner, Dimension>(box)))
        {
            set<min_corner, Dimension>(box, coord);
        }

        if (greater(coord, get<max_corner, Dimension>(box)))
        {
            set<max_corner, Dimension>(box, coord);
        }

        indexed_loop
            <
                Box, Geometry,
                StrategyLess, StrategyGreater,
                Index, Dimension + 1, DimensionCount
            >::apply(box, source);
    }
};


template
<
    typename Box, typename Geometry,
    typename StrategyLess, typename StrategyGreater,
    std::size_t Index, std::size_t DimensionCount
>
struct indexed_loop
    <
        Box, Geometry,
        StrategyLess, StrategyGreater,
        Index, DimensionCount, DimensionCount
    >
{
    static inline void apply(Box&, Geometry const&) {}
};



// Changes a box such that the other box is also contained by the box
template
<
    typename Box, typename Geometry,
    typename StrategyLess, typename StrategyGreater
>
struct combine_indexed
{
    static inline void apply(Box& box, Geometry const& geometry)
    {
        indexed_loop
            <
                Box, Geometry,
                StrategyLess, StrategyGreater,
                0, 0, dimension<Geometry>::type::value
            >::apply(box, geometry);

        indexed_loop
            <
                Box, Geometry,
                StrategyLess, StrategyGreater,
                1, 0, dimension<Geometry>::type::value
            >::apply(box, geometry);
    }
};

}} // namespace detail::combine
#endif // DOXYGEN_NO_DETAIL

#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{

template
<
    typename Tag,
    typename BoxOut, typename Geometry,
    typename StrategyLess, typename StrategyGreater
>
struct combine
{};


// Box + point -> new box containing also point
template
<
    typename BoxOut, typename Point,
    typename StrategyLess, typename StrategyGreater
>
struct combine<point_tag, BoxOut, Point, StrategyLess, StrategyGreater>
    : detail::combine::point_loop
        <
            BoxOut, Point,
            StrategyLess, StrategyGreater,
            0, dimension<Point>::type::value
        >
{};


// Box + box -> new box containing two input boxes
template
<
    typename BoxOut, typename BoxIn,
    typename StrategyLess, typename StrategyGreater
>
struct combine<box_tag, BoxOut, BoxIn, StrategyLess, StrategyGreater>
    : detail::combine::combine_indexed
        <BoxOut, BoxIn, StrategyLess, StrategyGreater>
{};

template
<
    typename Box, typename Segment,
    typename StrategyLess, typename StrategyGreater
>
struct combine<segment_tag, Box, Segment, StrategyLess, StrategyGreater>
    : detail::combine::combine_indexed
        <Box, Segment, StrategyLess, StrategyGreater>
{};


} // namespace dispatch
#endif // DOXYGEN_NO_DISPATCH


/***
*!
    \brief Combines a box with another geometry (box, point)
    \ingroup combine
    \tparam Box type of the box
    \tparam Geometry of second geometry, to be combined with the box
    \param box box to combine another geometry with, might be changed
    \param geometry other geometry
    \param strategy_less
    \param strategy_greater
    \note Strategy is currently ignored
    \todo Handle strategy
 *
template
<
    typename Box, typename Geometry,
    typename StrategyLess, typename StrategyGreater
>
inline void combine(Box& box, Geometry const& geometry,
            StrategyLess const& strategy_less,
            StrategyGreater const& strategy_greater)
{
    concept::check_concepts_and_equal_dimensions<Box, const Geometry>();

    dispatch::combine
        <
            typename tag<Geometry>::type,
            Box,
            Geometry,
            StrategyLess, StrategyGreater
        >::apply(box, geometry);
}
***/


/*!
    \brief Combines a box with another geometry (box, point)
    \ingroup combine
    \tparam Box type of the box
    \tparam Geometry of second geometry, to be combined with the box
    \param box box to combine another geometry with, might be changed
    \param geometry other geometry
 */
template <typename Box, typename Geometry>
inline void combine(Box& box, Geometry const& geometry)
{
    concept::check_concepts_and_equal_dimensions<Box, const Geometry>();

    dispatch::combine
        <
            typename tag<Geometry>::type,
            Box, Geometry,
            strategy::compare::default_strategy,
            strategy::compare::default_strategy
        >::apply(box, geometry);
}

}} // namespace boost::geometry

#endif // BOOST_GEOMETRY_ALGORITHMS_COMBINE_HPP
