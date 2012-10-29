// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Copyright Barend Gehrels 2007-2009, Geodan, Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_ALGORITHMS_DETAIL_OVERLAY_VISIT_INFO_HPP
#define BOOST_GEOMETRY_ALGORITHMS_DETAIL_OVERLAY_VISIT_INFO_HPP


#ifdef BOOST_GEOMETRY_USE_MSM
#  include <boost/geometry/algorithms/overlay/msm_state.hpp>
#endif


namespace boost { namespace geometry
{

#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace overlay {


#if ! defined(BOOST_GEOMETRY_USE_MSM)

class visit_info
{
private :
    static const int NONE = 0;
    static const int STARTED = 1;
    static const int VISITED = 2;
    static const int FINISHED = 3;

    int visit_code;

public:
    inline visit_info()
        : visit_code(0)
    {}

    inline void set_none() { visit_code = NONE; }
    inline void set_visited() { visit_code = VISITED; }
    inline void set_started() { visit_code = STARTED; }
    inline void set_finished() { visit_code = FINISHED; }

    inline bool none() const { return visit_code == NONE; }
    inline bool visited() const { return visit_code == VISITED; }
    inline bool started() const { return visit_code == STARTED; }

#ifdef BOOST_GEOMETRY_DEBUG_INTERSECTION
    friend std::ostream& operator<<(std::ostream &os, visit_info const& v)
    {
        if (v.visit_code != 0)
        {
            os << " VIS: " << int(v.visit_code);
        }
        return os;
    }
#endif

};


#else


class visit_info
{

private :

#ifndef USE_MSM_MINI
    mutable
#endif
        traverse_state state;

public :
    inline visit_info()
    {
        state.start();
    }

    inline void set_none() { state.process_event(none()); } // Not Yet Implemented!
    inline void set_visited() { state.process_event(visit()); }
    inline void set_started() { state.process_event(starting()); }
    inline void set_finished() { state.process_event(finish()); }

#ifdef USE_MSM_MINI
    inline bool none() const { return state.flag_none(); }
    inline bool visited() const { return state.flag_visited(); }
    inline bool started() const { return state.flag_started(); }
#else
    inline bool none() const { return state.is_flag_active<is_init>(); }
    inline bool visited() const { return state.is_flag_active<is_visited>(); }
    inline bool started() const { return state.is_flag_active<is_started>(); }
#endif

#ifdef BOOST_GEOMETRY_DEBUG_INTERSECTION
    friend std::ostream& operator<<(std::ostream &os, visit_info const& v)
    {
        return os;
    }
#endif
};
#endif


}} // namespace detail::overlay
#endif //DOXYGEN_NO_DETAIL


}} // namespace boost::geometry


#endif // BOOST_GEOMETRY_ALGORITHMS_DETAIL_OVERLAY_VISIT_INFO_HPP
