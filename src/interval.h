#ifndef INTERVAL_H
#define INTERVAL_H

#include "rtweekend.h"

class interval {
  public:
    double min, max;

    interval() : min(+infinity), max(-infinity) {} // Default interval is empty

    interval(double min, double max) : min(min), max(max) {}

    double size() const {
        return max - min;
    }

    bool contains(double x) const {
        return min <= x && x <= max;
    }

    bool surrounds(double x) const {
        return min < x && x < max;
    }

    double clamp(double x) const {
        if(x<min) return min;
        if(x> max) return max;
        return x;
    }

    double centroid() const {
        return (max + min) / 2.0;
    }

    static interval interval_union(const interval& a, const interval& b) {
        return interval(std::min(a.min, b.min), std::max(a.max, b.max));
    }

    // expands the interval by amount amt
    void expand(double amt) {
        min -= amt;
        max += amt;
    }

    static const interval empty, universe;
};

const interval interval::empty    = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);
inline interval operator+(interval in, double offset) {
    return interval(in.min + offset, in.max+ offset);
}
inline interval operator+(double offset, interval in) {
    return in + offset;
}

#endif