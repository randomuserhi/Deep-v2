#include "Deep.h"
#include "Deep/Math/Vec3.h"
#include "Deep/Physics3D/ContactInfo.h"
#include "Deep/Physics3D/Ray.h"

DEEP_NAMESPACE_BEGIN

struct DEEP_EXPORT Aabb3D {
    Vec3 m_center;
    Vec3 m_extents;
};

using Arg_Aabb3D = const Aabb3D&;

DEEP_EXPORT bool IsOverlapping(Vec3 in_point, Arg_Aabb3D in_a);

DEEP_EXPORT bool IsOverlapping(Arg_Aabb3D in_a, Arg_Aabb3D in_b);
DEEP_EXPORT int IsOverlapping(Arg_Aabb3D in_a, Arg_Aabb3D in_b, ContactInfo* out_contactInfo);

DEEP_EXPORT bool Raycast(Ray3D in_ray, RayHit3D* out_hit);
int RaycastAll(Ray3D in_ray, RayHit3D* out_hits);

DEEP_NAMESPACE_END
