#include "Deep.h"
#include "Deep/Math/Vec3.h"
#include "Deep/Physics3D/ContactInfo.h"

DEEP_NAMESPACE_BEGIN

struct DEEP_EXPORT Aabb3D {
    Vec3 m_center;
    Vec3 m_extents;
};

using Aabb3DArg = const Aabb3D&;

DEEP_EXPORT bool IsOverlapping(Vec3 in_point, Aabb3DArg in_a);

DEEP_EXPORT bool IsOverlapping(Aabb3DArg in_a, Aabb3DArg in_b);
DEEP_EXPORT int IsOverlapping(Aabb3DArg in_a, Aabb3DArg in_b, ContactInfo* out_contactInfo);

DEEP_NAMESPACE_END
