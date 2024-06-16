#define _LOG
//#define _PROFILE

//---------------------------------------------------------------------------------------
#include "Vector3.h"
#include "Segment3.h"
#include <iostream>

#ifdef _PROFILE
#include <chrono>
#endif // _PROFILE

//---------------------------------------------------------------------------------------
int main()
{
#ifdef _PROFILE
    const size_t repeats = 1e3;
    const size_t partitions = 5;

    Segment3D seg1[repeats];
    Segment3D seg2[repeats];
    
    srand(time(0));
    for (size_t i = 0; i < repeats; ++i)
    {
        seg1[i].Start.X = static_cast<double>(rand() % partitions + 1) / partitions;
        seg1[i].Start.Y = static_cast<double>(rand() % partitions + 1) / partitions;
        seg1[i].Start.Z = static_cast<double>(rand() % partitions + 1) / partitions;
        seg1[i].End  .X = static_cast<double>(rand() % partitions + 1) / partitions;
        seg1[i].End  .Y = static_cast<double>(rand() % partitions + 1) / partitions;
        seg1[i].End  .Z = static_cast<double>(rand() % partitions + 1) / partitions;
        seg2[i].Start.X = static_cast<double>(rand() % partitions + 1) / partitions;
        seg2[i].Start.Y = static_cast<double>(rand() % partitions + 1) / partitions;
        seg2[i].Start.Z = static_cast<double>(rand() % partitions + 1) / partitions;
        seg2[i].End  .X = static_cast<double>(rand() % partitions + 1) / partitions;
        seg2[i].End  .Y = static_cast<double>(rand() % partitions + 1) / partitions;
        seg2[i].End  .Z = static_cast<double>(rand() % partitions + 1) / partitions;
    }

    Vector3D result[repeats];
    size_t intersections = 0;

    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < repeats; ++i)
        result[i] = seg1[i].Intersection(seg2[i]);

    auto duration = std::chrono::high_resolution_clock::now() - start;

    for (size_t i = 0; i < repeats; ++i)
        if (result[i].IsValid())
            ++intersections;

    std::cout << "Time ns: " << duration.count() / repeats << '\n'
              << "Intersections: " << intersections << '\n';

#else
    Segment3D seg1({ 0, 0, 0 }, { 0, 2, 2 });
    Segment3D seg2({ 0, 0, 2 }, { 0, 2, 0 });
    Vector3D result;

    result = seg1.Intersection(seg2);
    std::cout << "Intersection: " << result.X << " " <<
                                     result.Y << " " << 
                                     result.Z << '\n';

#endif // _PROFILE
}
