//
//  Face.h
//  umeshu++
//
//  Created by Chalupecky Vladimir on 3/28/11.
//  Copyright 2011 Kyushu University. All rights reserved.
//

#ifndef __FACE_H_INCLUDED__
#define __FACE_H_INCLUDED__

#include "Identifiable.h"
#include "Mesh_fwd.h"
#include "Point2.h"
#include "ExactAdaptiveKernel.h"

#include <boost/assert.hpp>

//template <typename Kernel>
class Face : public Identifiable {
public:
//    typedef Kernel kernel_type;
//    typedef Face<Kernel>* pointer;

    explicit Face(HalfEdgeHandle adj_edge)
    : adj_edge_(adj_edge), area_(0.0)
    { this->compute_area<ExactAdaptiveKernel>(); }

    inline HalfEdgeHandle adjacent_he() const { return adj_edge_; }
    inline HalfEdgeHandle& adjacent_he() { return adj_edge_; }
    
    void edge_lengths(double& el1, double& el2, double& el3) const;
    void edge_lengths_squared(double& el1, double& el2, double& el3) const;

    HalfEdgeHandle longest_edge(double *l = NULL) const;
    double longest_edge_length() const;
    double longest_edge_length_squared() const;

    HalfEdgeHandle shortest_edge(double *l = NULL) const;
    double shortest_edge_length() const;
    double shortest_edge_length_squared() const;
    
    void angles(double& a1, double& a2, double& a3) const;
    double minimum_angle() const;
    double maximum_angle() const;

    template <typename Kernel>
    double circumradius() const;
    
    Point2 barycenter() const;

    template <typename Kernel>
    Point2 circumcenter() const;

    template <typename Kernel>
    Point2 offcenter(double offconstant) const;

    template <typename Kernel>
    double area() const { return area_; }
    template <typename Kernel>
    double compute_area();

    template <typename Kernel>
    double quality() const;
    
    bool is_restricted() const;

    void nodes(NodeHandle& n1, NodeHandle& n2, NodeHandle& n3) const;
    void vertices(Point2& v1, Point2& v2, Point2& v3) const;
    
private:
    HalfEdgeHandle adj_edge_;
    double area_;
};

std::ostream& operator<< (std::ostream& os, Face const& f);

struct compare_faces {
    bool operator() (FaceHandle f1, FaceHandle f2) const;
};

template <typename Kernel>
double Face::compute_area()
{
    Point2 p1, p2, p3;
    this->vertices(p1, p2, p3);
    area_ = Kernel::signed_area(p1, p2, p3);
    BOOST_ASSERT(area_ > 0.0);
    return area_;
}

template <typename Kernel>
double Face::quality() const
{
    // TODO
    return 1.0;
}


template <typename Kernel>
Point2 Face::circumcenter() const
{
    Point2 p1, p2, p3;
    this->vertices(p1, p2, p3);
    return Kernel::circumcenter(p1, p2, p3);    
}

template <typename Kernel>
Point2 Face::offcenter(double offconstant) const
{
    Point2 p1, p2, p3;
    this->vertices(p1, p2, p3);
    return Kernel::offcenter(p1, p2, p3, offconstant);
}

template <typename Kernel>
double Face::circumradius() const
{
    Point2 p1, p2, p3;
    this->vertices(p1, p2, p3);
    return Kernel::circumradius(p1, p2, p3);
}

#endif // __FACE_H_INCLUDED__