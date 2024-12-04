#ifndef ICOMPARER_H
#define ICOMPARER_H

template <typename X>
class IComparer {
   public:
    virtual int Compare(const X& o1, const X& o2) const = 0;
    virtual ~IComparer() = default;
};

#endif  // ICOMPARER_H
