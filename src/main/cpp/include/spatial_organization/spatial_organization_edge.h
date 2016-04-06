#ifndef SPATIAL_ORGANIZATION_SPATIAL_ORGANIZATION_EDGE_H_
#define SPATIAL_ORGANIZATION_SPATIAL_ORGANIZATION_EDGE_H_

#include <memory>
#include "physics/physical_node.h"

namespace cx3d {
namespace spatial_organization {

 class SpaceNode;

/**
 * Interface to define the basic properties of an edge in the simulation.
 *
 * @param  The type of user objects associated with nodes of the triangulation.
 */

class SpatialOrganizationEdge {
 public:
  virtual ~SpatialOrganizationEdge() {
  }

  /**
   * Given one user object associated with one endpoint of this edge,
   * this function returns the user object associated with the other endpoint of this edge.
   * @param element A user object associated with one of the two endpoints of this edge.
   * @return The second user object associated to an endpoint of this edge.
   */
  virtual std::shared_ptr<physics::PhysicalNode> getOppositeElement(
      const std::shared_ptr<physics::PhysicalNode>& element) const = 0;

  /**
   * Given one endpoint of this edge, this function returns the other endpoint.
   * @param node One endpoint of this edge.
   * @return The other endpoint of this edge. Throws a RuntimeException if the node <code>first</code>
   * is not incident to this edge.
   */
  virtual std::shared_ptr<SpaceNode> getOpposite(
      const std::shared_ptr<const SpaceNode>& node) const = 0;

  /**
   * @return One of the two user objects associated to the endpoints of this edge.
   * Returns the opposite user object to the result of {@link #getSecondElement()}.
   */
  virtual std::shared_ptr<physics::PhysicalNode> getFirstElement() const = 0;

  /**
   * @return One of the two user objects associated to the endpoints of this edge.
   * Returns the opposite user object to the result of {@link #getFirstElement()}.
   */
  virtual std::shared_ptr<physics::PhysicalNode> getSecondElement() const = 0;

  /**
   * Returns the current cross section area associated with this edge.
   * @return A double value representing the cross section area between the two endpoints of this edge.
   */
  virtual double getCrossSection() const = 0;

  virtual const std::string toString() const = 0;
};

}  // namespace spatial_organization
}  // namespace cx3d

#endif  // SPATIAL_ORGANIZATION_SPATIAL_ORGANIZATION_EDGE_H_
