/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Tuesday, October 17, 2017 - 10:24:30
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <labraytracer/plane.h>
#include <labraytracer/triangle.h>
#include <labraytracer/util.h>
#include <memory>

namespace inviwo {

Triangle::Triangle() {}

Triangle::Triangle(const vec3& v0, const vec3& v1, const vec3& v2, const vec3& uvw0,
                   const vec3& uvw1, const vec3& uvw2) {
    mVertices[0] = v0;
    mVertices[1] = v1;
    mVertices[2] = v2;
    mUVW[0] = uvw0;
    mUVW[1] = uvw1;
    mUVW[2] = uvw2;
}

bool Triangle::closestIntersection(const Ray& ray, double maxLambda,
                                   RayIntersection& intersection) const {
    // Programming TASK 1: Implement this method
    // Your code should compute the intersection between a ray and a triangle.
    //
    // If you detect an intersection, the return type should look similar to this:
    // if(rayIntersectsTriangle)
    //{
    //  intersection = RayIntersection(ray,shared_from_this(),lambda,ray.normal,uvw);
    //  return true;
    //}
    //
    // Hints :
    // Ray origin p_r : ray.getOrigin()
    // Ray direction t_r : ray.getDirection()
    // Compute the intersection point using ray.pointOnRay(lambda)

    const vec3 t1 = mVertices[1] - mVertices[0];
    const vec3 t2 = mVertices[2] - mVertices[0];

    const vec3 normal = Util::normalize(cross(t1, t2));

    // Check intersection with triangle's containing plane
    const std::shared_ptr<PlaneX> trianglePlane = std::make_shared<PlaneX>(normal, mVertices[0]);
    RayIntersection planeIntersection;
    const bool isIntersectingPlane = trianglePlane->closestIntersection(ray, maxLambda, planeIntersection);

    if (!isIntersectingPlane) {
        return false;
    }

    // Check if intersection point is within triangle
    const bool isFirstConditionValid = dot(cross(mVertices[0] - mVertices[1], mVertices[0] - planeIntersection.getPosition()), normal) >= 0;
    const bool isSecondConditionValid = dot(cross(mVertices[1] - mVertices[2], mVertices[1] - planeIntersection.getPosition()), normal) >= 0;
    const bool isThirdConditionValid = dot(cross(mVertices[2] - mVertices[0], mVertices[2] - planeIntersection.getPosition()), normal) >= 0;
    if (isFirstConditionValid && isSecondConditionValid && isThirdConditionValid) {
        const vec3 uvw(0, 0, 0);
        intersection = RayIntersection(ray, shared_from_this(), planeIntersection.getLambda(), normal, uvw);
        return true;
    }

    return false;
}

bool Triangle::anyIntersection(const Ray& ray, double maxLambda) const {
    RayIntersection temp;
    return closestIntersection(ray, maxLambda, temp);
}

void Triangle::drawGeometry(std::shared_ptr<BasicMesh> mesh,
                            std::vector<BasicMesh::Vertex>& vertices) const {
    auto indexBuffer = mesh->addIndexBuffer(DrawType::Lines, ConnectivityType::None);

    Util::drawLineSegment(mVertices[0], mVertices[1], vec4(0.2, 0.2, 0.2, 1), indexBuffer.get(),
                          vertices);
    Util::drawLineSegment(mVertices[1], mVertices[2], vec4(0.2, 0.2, 0.2, 1), indexBuffer.get(),
                          vertices);
    Util::drawLineSegment(mVertices[2], mVertices[0], vec4(0.2, 0.2, 0.2, 1), indexBuffer.get(),
                          vertices);
}

}  // namespace inviwo
