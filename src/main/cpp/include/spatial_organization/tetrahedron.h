#ifndef SPATIAL_ORGANIZATION_TETRAHEDRON_H_
#define SPATIAL_ORGANIZATION_TETRAHEDRON_H_

#include <array>
#include <vector> //former list
#include <string>
#include <memory>
#include <stdexcept>
#include "physics/physical_node.h"

#ifdef TETRAHEDRON_DEBUG
#include "spatial_organization/debug/tetrahedron_debug.h"
#endif

namespace cx3d {
namespace spatial_organization {

 class Edge;
 class SpaceNode;
 class Triangle3D;
 class OpenTriangleOrganizer;

/**
 * This class is used to represent a tetrahedron. Each instance saves references
 * to 4 incident nodes and 4 incident triangles. Additionally,
 * <code>Tetrahedron</code> stores information about the volume_ and the
 * circumsphere of the tetrahedron defined by the incident nodes.
 *
 * Tetrahedra can either be finite or infinite. In the latter case, the first
 * node incident to this tetrahedron is set to <code>null</code>, indicating
 * that the other three endpoints of this tetrahedron are part of the convex
 * hull of all points in the current triangulation.
 *
 * @param  The type of the user objects stored in endpoints of a tetrahedron.
 */

class Tetrahedron : public std::enable_shared_from_this<Tetrahedron> {
 public:
#ifndef TETRAHEDRON_NATIVE
  Tetrahedron();
#endif

  /**
   * Creates a new Tetrahedron object and returns it within a <code>std::shared_ptr</code>
   * @see Edge(...)
   *
   * If functions return a std::shared_ptr of <code>this</code> using
   * <code>return shared_from_this();</code>, the following precondition must be met:
   * There must be at least one std::shared_ptr p that owns *this!
   * Calling <code>shared_from_this</code> on a non-shared object results in undefined behaviour.
   * http://mortoray.com/2013/08/02/safely-using-enable_shared_from_this/
   *
   * Therefore, all constructors are private and accessed through static factory methods that return
   * a std::shared_ptr.
   *
   * TODO(lukas) SWIG doesn't seem to support variadic templates and perfect forwarding system.
   * Once mapping to Java is not needed anymore, replace following create functions with:
   * <code>
   * template<typename ... T>
   * static std::shared_ptr<Tetrahedron> create(T&& ... all) {
   *   return std::shared_ptr<Tetrahedron>(new Tetrahedron(std::forward(all)...));
   * }
   * </code>
   */
  static std::shared_ptr<Tetrahedron> create(
      const std::shared_ptr<Triangle3D>& one_triangle,
      const std::shared_ptr<SpaceNode>& fourth_point,
      const std::shared_ptr<OpenTriangleOrganizer>& oto) {
#ifdef TETRAHEDRON_DEBUG
    Tetrahedron* raw_pointer = new TetrahedronDebug();
#else
    Tetrahedron* raw_pointer = new Tetrahedron();
#endif
    std::shared_ptr<Tetrahedron> tetrahedron(raw_pointer);
    tetrahedron->initializationHelper(one_triangle, fourth_point, oto);
#ifdef TETRAHEDRON_DEBUG
    logConstrFromStatic("Tetrahedron", tetrahedron, one_triangle, fourth_point, oto);
#endif
    return tetrahedron;
  }

  static std::shared_ptr<Tetrahedron> create(const std::shared_ptr<Triangle3D>& triangle_a,
                                                const std::shared_ptr<Triangle3D>& triangle_b,
                                                const std::shared_ptr<Triangle3D>& triangle_c,
                                                const std::shared_ptr<Triangle3D>& triangle_d,
                                                const std::shared_ptr<SpaceNode>& node_a,
                                                const std::shared_ptr<SpaceNode>& node_b,
                                                const std::shared_ptr<SpaceNode>& node_c,
                                                const std::shared_ptr<SpaceNode>& node_d) {
#ifdef TETRAHEDRON_DEBUG
    Tetrahedron* raw_pointer = new TetrahedronDebug();
#else
    Tetrahedron* raw_pointer = new Tetrahedron();
#endif
    std::shared_ptr<Tetrahedron> tetrahedron(raw_pointer);
    tetrahedron->initializationHelper(triangle_a, triangle_b, triangle_c, triangle_d, node_a,
                                      node_b, node_c, node_d);
#ifdef TETRAHEDRON_DEBUG
    logConstrFromStatic("Tetrahedron", tetrahedron, triangle_a, triangle_b, triangle_c, triangle_d,
                        node_a, node_b, node_c, node_d);
#endif
    return tetrahedron;
  }

  /**
   * Generates an initial tetrahedron for a new triangulation. A tetrahedron
   * is generated which has the four given nodes as endpoints and is adjacent
   * to four infinite tetrahedra.
   *
   * @param 
   *            The type of the user object stored in the incident nodes.
   * @param a
   *            The first incident node.
   * @param b
   *            The second incident node.
   * @param c
   *            The third incident node.
   * @param d
   *            The fourth incident node.
   * @param simple_oto
   *            simple open triangle organizer
   * @return A tetrahedron which is made out of the four points passed to this
   *         function. This tetrahedron will be neighbor to 4 infinite
   *         tetrahedra.
   */
  static std::shared_ptr<Tetrahedron> createInitialTetrahedron(
      const std::shared_ptr<SpaceNode>& a, const std::shared_ptr<SpaceNode>& b,
      const std::shared_ptr<SpaceNode>& c, const std::shared_ptr<SpaceNode>& d,
      const std::shared_ptr<OpenTriangleOrganizer>& simple_oto);

  virtual ~Tetrahedron() {
  }

  /**
   * Calculates the properties of this tetrahedron's circumsphere.
   */
  virtual void calculateCircumSphere();

  /**
   * Used to calculate the properties of this tetrahedron's circumsphere after
   * an endpoint has been moved. Originally used to increase the speed of
   * circumsphere calculations, but now uses the same functions as
   * {@link #calculateCircumSphere()} because the old method increased the
   * uncertainity of the circum_center_.
   *
   * In addition to calcualting the circumsphere, all incident triangles that are
   * incident to the moved node are informed about the movement.
   *
   * @param moved_node
   *            The node that was moved.
   */
  virtual void updateCirumSphereAfterNodeMovement(const std::shared_ptr<SpaceNode>& moved_node);

  /**
   * Determines whether a given point lies inside or outside the circumsphere
   * of this tetrahedron or lies on the surface of this sphere.
   *
   * @param point
   *            The point for which the orientation should be determined.
   * @return -1, if the point lies outside this tetrahedron's circumsphere, 1
   *         if it is inside the sphere and 0, if it lies on the surface of
   *         the sphere.
   */
  virtual int orientation(const std::array<double, 3>& point);

  /**
   * Determines whether a given point lies truly inside the circumsphere of
   * this tetrahedron
   *
   * @param point
   *            The point for which the orientation should be determined.
   * @return <code>true</code> if the distance of the point to the center of
   *         the circumsphere is smaller than the radius of the circumsphere
   *         and <code>false</code> otherwise.
   */
  virtual bool isTrulyInsideSphere(const std::array<double, 3>& point);

  /**
   * Determines whether a given point lies truly inside the circumsphere of
   * this tetrahedron
   *
   * @param point
   *            The point for which the orientation should be determined.
   * @return <code>true</code> if the distance of the point to the center of
   *         the circumsphere is smaller or equal to the radius of the
   *         circumsphere and <code>false</code> otherwise.
   */
  virtual bool isInsideSphere(const std::array<double, 3>& point);

  /**
   * Returns a String representation of this tetrahedron
   */
  virtual std::string toString() const;

  /**
   * Determines if two instances of this object are equal
   */
  virtual bool equalTo(const std::shared_ptr<Tetrahedron>& other);

  /**
   * @return An array of triangles containing the 4 triangles incident to this tetrahedron.
   */
  virtual std::array<std::shared_ptr<Triangle3D>, 4> getAdjacentTriangles() const;

  /**
   * Determines whether a given node is an endpoint of this tetrahedron.
   * @param node The node of interest.
   * @return <code>true</code>, if the node is an endpoint.
   */
  virtual bool isAdjacentTo(const std::shared_ptr<SpaceNode>& node) const;

  /**
   * Walks toward a specified point. If the point lies inside this
   * tetrahedron, this tetrahedron is returned. Otherwise, an adjacent
   * tetrahedron is returned that lies closer to the point.
   *
   * @param coordinate
   *            The coordinate that should be approximated.
   * @param triangle_order
   *        A small list containing a permutation of the number 0-3. This list is
   *            used to randomize the visibility walk implemented in
   * @return An adjacent tetrahedron that lies closer to the specified point
   *         than this tetrahedron, or this tetrahedron, if the point lies inside it.
   */
  virtual std::shared_ptr<Tetrahedron> walkToPoint(const std::array<double, 3>& coordinate,
                                                      const std::array<int, 4>& triangle_order);
  /**
   * @return An array containing the nodes incident to this tetrahedron.
   */
  virtual std::array<std::shared_ptr<SpaceNode >, 4> getAdjacentNodes() const;
//
// protected:
//
  /**
   * Determines the index of the edge connecting two given endpoints of the
   * tetrahedron in this tetrahedron's list of incident edges.
   *
   * @param node_number_1
   *            The index of the first endpoint of the edge.
   * @param node_number_2
   *            The index of the second endpoint of the edge.
   * @return A number between 0 and 5, giving the index of the edge of
   *         interest.
   */
  static int getEdgeNumber(int node_number_1, int node_number_2);

  /**
   * Removes two flat tetrahedra that have two common triangles.
   *
   * @param 
   *            The type of the user objects stored in the given tetrahedra.
   * @param tetrahedron_a
   *            The first flat tetrahedron.
   * @param tetrahedron_b
   *            The second flat tetrahedron.
   * @return A list of tetrahedra that were originally adjacent to either one
   *         of the two flat tetrahedra that were removed.
   */
  static std::vector<std::shared_ptr<Tetrahedron> > remove2FlatTetrahedra(
      const std::shared_ptr<Tetrahedron>& tetrahedron_a,
      const std::shared_ptr<Tetrahedron>& tetrahedron_b);

  /**
   * Performs a 2->3 Flip of two adjacent tetrahedra.
   * @param  The type of the user objects stored in the endpoints of the two tetrahedra.
   * @param tetrahedron_a The first tetrahedron to flip.
   * @param tetrahedron_b The second tetrahedron to flip.
   * @return An array of tetrahedra which were created during the process of flipping.
   */
  static std::array<std::shared_ptr<Tetrahedron>, 3> flip2to3(
      const std::shared_ptr<Tetrahedron>& tetrahedron_a,
      const std::shared_ptr<Tetrahedron>& tetrahedron_b);

  /**
   * Performs a 3->2 Flip of two adjacent tetrahedra.
   * @param  The type of the user objects stored in the endpoints of the two tetrahedra.
   * @param tetrahedron_a The first tetrahedron to flip.
   * @param tetrahedron_b The second tetrahedron to flip.
   * @param tetrahedron_c The third tetrahedron to flip.
   * @return An array of tetrahedra which were created during the process of flipping.
   */
  static std::array<std::shared_ptr<Tetrahedron>, 2> flip3to2(
      const std::shared_ptr<Tetrahedron>& tetrahedron_a,
      const std::shared_ptr<Tetrahedron>& tetrahedron_b,
      const std::shared_ptr<Tetrahedron>& tetrahedron_c);

  /**
   * Extracts the user objects associated with the four endpoints of this
   * tetrahedron.
   *
   * @return An array of objects of type <code>T</code>.
   */
  virtual std::array<std::shared_ptr<physics::PhysicalNode>, 4> getVerticeContents() const;

  /**
   * Returns whether this tetrahedron is infinite.
   *
   * @return <code>true</code>, if the tetrahedron is infinite (first
   *         endpoint is null).
   */
  virtual bool isInfinite() const;

  /**
   * Returns whether this tetrahedron is a flat tetrahedron. Used to simplify
   * distinction between the two types <code>Tetrahedron</code> and
   * <code>FlatTetrahedron</code>.
   *
   * @return <code>false</code> for all instances of
   *         <code>Tetrahedron</code>.
   */
  virtual bool isFlat() const {
    return false;
  }

  /**
   * Changes the cross section area associated with one incident edge. Informs
   * incident edges if there is a change in their cross section area.
   *
   * @param number
   *            The index of the edge which cross section area should be
   *            changed.
   * @param new_value
   *            The new value for the cross section area of the specified
   *            edge.
   */
  virtual void changeCrossSection(int number, double new_value);

  /**
   * Calculates all cross section areas of the six edges incident to this
   * tetrahedron.
   */
  virtual void updateCrossSectionAreas();

  /**
   * Calculates the volume_ of this tetrahedron and changes the stored value.
   * (The volume_ equals 1/6th of the determinant of 3 incident edges with a
   * common endpoint.)
   */
  virtual void calculateVolume();

  /**
   * Determines wether a given point lies inside or outside the circumsphere
   * of this tetrahedron or lies on the surface of this sphere. This function
   * uses precise arithmetics to calculate a reliable answer.
   *
   * @param position
   *            The position for which the orientation should be determined.
   * @return -1, if the point lies outside this tetrahedron's circumsphere, 1
   *         if it is inside the sphere and 0, if it lies on the surface of
   *         the sphere.
   */
  virtual int orientationExact(const std::array<double, 3>& position) const;

  /**
   * Replaces one of the incident triangles of this tetrahedron. Automatically
   * exchanges the affected edges, too.
   *
   * @param old_triangle
   *            The triangle that should be replaced.
   * @param new_triangle
   *            The new trianlge.
   */
  virtual void replaceTriangle(const std::shared_ptr<Triangle3D>& old_triangle,
                               const std::shared_ptr<Triangle3D>& new_triangle);

  /**
   * Determines which index a given node has in this tetrahedron's list of
   * endpoints.
   *
   * @param node
   *            The node of interest.
   * @return An index between 0 and 3.
   */
  virtual int getNodeNumber(const std::shared_ptr<SpaceNode>& node) const;

  /**
   * Determines which index a given triangle has in this tetrahedron's list of
   * incident triangles.
   *
   * @param triangle
   *            The triangle of interest.
   * @return An index between 0 and 3.
   */
  virtual int getTriangleNumber(const std::shared_ptr<Triangle3D>& triangle) const;

  /**
   * Determines the edge that connects two endpoints of this tetrahedron.
   *
   * @param node_number_1
   *            The index of the first endpoint of the edge.
   * @param node_number_2
   *            The index of the second endpoint of the edge.
   * @return The edge connecting the two endpoints with the given indices.
   */
  virtual std::shared_ptr<Edge> getEdge(int node_number_1, int node_number_2) const;

  /**
   * Determines the edge that connects two endpoints of this tetrahedron.
   *
   * @param a
   *            The first endpoint of the edge.
   * @param b
   *            The second endpoint of the edge.
   * @return The edge connecting the two given endpoints.
   */
  virtual std::shared_ptr<Edge> getEdge(const std::shared_ptr<SpaceNode>& a,
                                           const std::shared_ptr<SpaceNode>& b) const;

  /**
   * Determines the edge that connects two endpoints of this tetrahedron.
   *
   * @param a
   *            The first endpoint of the edge.
   * @param b
   *            The second endpoint of the edge.
   * @return A number between 0 and 5, giving the index of the edge of
   *         interest.
   */
  virtual int getEdgeNumber(const std::shared_ptr<SpaceNode>& a,
                            const std::shared_ptr<SpaceNode>& b) const;

  /**
   * Returns the incident triangle opposite to a given endpoint of this
   * tetrahedron.
   *
   * @param node
   *            An endpoint of this tetrahedron.
   * @return A reference to the triangle that lies opposite to
   *         <code>node</code>.
   */
  virtual std::shared_ptr<Triangle3D> getOppositeTriangle(
      const std::shared_ptr<SpaceNode>& node) const;

  /**
   * Returns the incident node opposite to a given triangle which is incident
   * to this tetrahedron.
   *
   * @param triangle
   *            An incident triangle of this tetrahedron.
   * @return The endpoint of this triangle that lies opposite to
   *         <code>triangle</code>.
   */
  virtual std::shared_ptr<SpaceNode > getOppositeNode(
      const std::shared_ptr<Triangle3D>& triangle) const;

  /**
   * Ret#include "spatial_organization/space_node.h"urns a reference to the triangle connecting this tetrahedron with
   * another one.
   *
   * @param tetrahedron
   *            An adjacent tetrahedron.
   * @return The triangle which is incident to this tetrahedron and
   *         <code>tetrahedron</code>.
   */
  virtual std::shared_ptr<Triangle3D> getConnectingTriangle(
      const std::shared_ptr<Tetrahedron>& tetrahedron) const;

  /**
   * Returns this index of the triangle connecting this tetrahedron with
   * another one.
   *
   * @param tetrahedron
   *            An adjacent tetrahedron.
   * @return An index between 0 and 3 which is the position of the triangle
   *         incident to this tetrahedron and <code>tetrahedron</code> in
   *         this tetrahedron's list of incident triangles.
   */
  virtual int getConnectingTriangleNumber(const std::shared_ptr<Tetrahedron>& tetrahedron) const;

  /**
   * Returns the three incident triangles that are adjacent to a given
   * triangle.
   *
   * @param base
   *            A triangle which is incident to this tetrahedron.
   * @return An array of three triangles.
   */
  virtual std::array<std::shared_ptr<Triangle3D>, 3> getTouchingTriangles(
      const std::shared_ptr<Triangle3D>& base) const;

  /**
   * Removes this tetrahedron from the triangulation. All the incident nodes,
   * edges and triangles are informed that this tetrahedron is being removed.
   *
   * !IMPORTANT!: No triangle organizer is informed about the removement of
   * this tetrahedron. A caller of this function must keep track of the new
   * open triangles itself!
   */
  virtual void remove();

  /**
   * Determines whether a given coordinate lies in convex position, meaning
   * that the incident triangle with list index
   * <code>connectingTriangleNumver</code> is truly cut by a line connecting
   * the given coordinate and the endpoint of this tetrahedron that lies
   * opposite to the same triangle.
   *
   * @param point
   *            The coordinate that should be tested.
   * @param connecting_triangle_number
   *            The index of the triangle facing the coordinate.
   * @return <code>true</code>, if the given coordinate truly lies in
   *         convex position and <code>false</code> otherwise.
   */
  virtual bool isPointInConvexPosition(const std::array<double, 3>& point,
                                       int connecting_triangle_number) const;

  /**
   * Determines whether a given coordinate lies in convex position, meaning
   * that the incident triangle with list index
   * <code>connectingTriangleNumver</code> is cut by a line connecting the
   * given coordinate and the endpoint of this tetrahedron that lies opposite
   * to the same triangle.
   *
   * @param point
   *            The coordinate that should be tested.
   * @param connecting_triangle_number
   *            The index of the triangle facing the coordinate.
   * @return 1, if the given coordinate lies truly in convex position to this
   *         tetrahedron (meaning that a line connecting the node opposite to
   *         the specified triangle and the given coordinate would cut the
   *         inside of the specified triangle), 0 if the point lies on the
   *         border between convex positions and non-convex position, and -1
   *         if the point lies in a non-convex position.
   */
  virtual int isInConvexPosition(const std::array<double, 3>& point,
                                 int connecting_triangle_number) const;

  /**
   * Returns the second tetrahedron that is incident to the incident triangle with index <code>number</code>.
   * @param number An index specifying a position in the list of triangles of this tetrahedron. The
   * corresponding triangle will be chosen to determine the adjacent tetrahedron.
   * @return An adjacent tetrahedron.
   */
  virtual std::shared_ptr<Tetrahedron> getAdjacentTetrahedron(int number);

  /**
   * Checks if a node may be moved to a given coordinate.
   * @param position The coordinate of interest.
   * @//fnoexceptionthrows PositionNotAllowedException If the position is equal to any endpoint of this tetrahedron.
   */
  virtual void testPosition(const std::array<double, 3>& position) const; //fnoexceptionthrow(std::exception);

  /**
   * When this tetrahedron is removed, there might still be references to
   * this tetrahedron.  Therefore, a flag is set to save that this tetrahedron was removed and
   * this can be read using this function.
   *
   * @return <code>true</code>, iff this tetrahedron is still part of the triangulation.
   */
  virtual bool isValid() const;

  /**
   * Returns whether a given tetrahedron is adjacent to this tetrahedron.
   * @param other_tetrahedron The potential neighbor of this tetrahedron.
   * @return <code>true</code>, iff this tetrahedron is adjacent to <code>otherTetrahedron</code>.
   */
  virtual bool isNeighbor(const std::shared_ptr<Tetrahedron>& other_tetrahedron) const;

  /**
   * Given two nodes incident to this tetrahedron, this function returns
   * another endpoint. The returned endpoint is different from the result of
   * {@link #getSecondOtherNode(SpaceNode, SpaceNode)}.
   *
   * @param node_a
   *            A first incident node.
   * @param node_b
   *            A second incident node.
   * @return A third incident node.
   */
  virtual std::shared_ptr<SpaceNode> getFirstOtherNode(
      const std::shared_ptr<SpaceNode>& node_a,
      const std::shared_ptr<SpaceNode>& node_b) const;

  /**
   * Given two nodes incident to this tetrahedron, this function returns
   * another endpoint. The returned endpoint is different from the result of
   * {@link #getFirstOtherNode(SpaceNode, SpaceNode)}.
   *
   * @param node_a
   *            A first incident node.
   * @param node_b
   *            A second incident node.
   * @return A third incident node.
   */
  virtual std::shared_ptr<SpaceNode> getSecondOtherNode(
      const std::shared_ptr<SpaceNode>& node_a,
      const std::shared_ptr<SpaceNode>& node_b) const;

 protected:
#ifdef TETRAHEDRON_NATIVE
  /**
   * Initialization is done within initializationHelper methods
   * Have a look at their documentation to understand why
   */
  Tetrahedron();
#endif

  /**
   * Initialization code that cannot be called inside the constructor, because it passes
   * a std::shared_ptr of itself to another function.
   * It does that using shared_from_this(). @see create function documentation for a more
   * detailed explanation of the requirements before calling shared_from_this()
   *
   * Constructs a new tetrahedron from a given triangle and a fourth point.
   * Missing triangles are created.
   *
   * @param one_triangle
   *            The triangle delivering 3 of the new tetrahedron's endpoints.
   * @param fourth_point
   *            The fourth endpoint of the new tetrahedron.
   * @param org
   *            An organizer for open triangles which is used to keep track of
   *            newly created triangles.
   */
  void initializationHelper(const std::shared_ptr<Triangle3D>& one_triangle,
                            const std::shared_ptr<SpaceNode>& fourth_point,
                            const std::shared_ptr<OpenTriangleOrganizer>& oto);

  /**
   * Initialization code that cannot be called inside the constructor, because it passes
   * a std::shared_ptr of itself to another function.
   * It does that using shared_from_this(). @see create function documentation for a more
   * detailed explanation of the requirements before calling shared_from_this()
   *
   * Creates a new tetrahedron from four triangles and four points.
   *
   * @param triangle_a
   *            The first triangle.
   * @param triangle_b
   *            The second triangle.
   * @param triangle_c
   *            The third triangle.
   * @param triangle_d
   *            The fourth triangle.
   * @param node_a
   *            The first point, must lie opposite to triangleA.
   * @param node_b
   *            The first point, must lie opposite to triangleB.
   * @param node_c
   *            The first point, must lie opposite to triangleC.
   * @param node_d
   *            The first point, must lie opposite to triangleD.
   */
  void initializationHelper(const std::shared_ptr<Triangle3D>& triangle_a,
                            const std::shared_ptr<Triangle3D>& triangle_b,
                            const std::shared_ptr<Triangle3D>& triangle_c,
                            const std::shared_ptr<Triangle3D>& triangle_d,
                            const std::shared_ptr<SpaceNode>& node_a,
                            const std::shared_ptr<SpaceNode>& node_b,
                            const std::shared_ptr<SpaceNode>& node_c,
                            const std::shared_ptr<SpaceNode>& node_d);

  /**
   * A small list containing a permutation of the number 0-3. This list is
   * used to randomize the visibility walk implemented in
   * {@link #walkToPoint(double[])}.
   */
  static std::array<int, 4> triangle_order_;
  /**
   * Contains references to the nodes incident to this tetrahedron.
   */
  std::array<std::shared_ptr<SpaceNode>, 4> adjacent_nodes_;
  /**
   * Contains references to the 6 edges incident to this tetrahedron.
   */
  std::array<std::shared_ptr<Edge>, 6> adjacent_edges_;
  /**
   * Contains references to the 4 triangles incident to this tetrahedron.
   */
  std::array<std::shared_ptr<Triangle3D>, 4> adjacent_triangles_;
  /**
   * Saves for each incident edge this tetrahedron's contribution to the
   * cross-section area of that edge.
   */
  std::array<double, 6> cross_section_areas_;
  /**
   * The center of the circumsphere of this tetrahedron.
   */
  std::array<double, 3> circum_center_;

  bool circum_center_is_null_;

  /**
   * The square of the radius of this tetrahedron's circumsphere.
   */
  double squared_radius_;

  /**
   * Defines a tolerance_ intervall which is used in
   * {@link #orientation(double[])}. If the distance of a given point to the
   * center of the circumsphere of this tetrahedron has a difference to
   * <co#include "spatial_organization/space_node.h"de>squared_radius_</code> smaller than <code>tolerance_</code>,
   * exact mathematics are used to reliably calculate a decision whether that
   * point lies inside, outside or on the circumsphere.
   */
  double tolerance_;

  /**
   * The volume_ of this tetrahedron.
   */
  double volume_;

  /**
   * Determines whether this tetrahedron is still a part of the triangulation.
   */
  bool valid_;

 private:
  /**
   * Used during initialization to make sure that edges are created only once.
   */
  virtual void registerEdges();

  /**
   * Changes the volume_ associated with this tetrahedron to a new value. The
   * incident nodes are informed about the volume_ change.
   *
   * @param new_volume
   *            The new volume_.
   */
  virtual void changeVolume(double new_volume);

  /**
   * Calculates the vectors connecting the first endpoint of this tetrahedron
   * with the other three points.
   *
   * @return A two-dimensional array of length 3x3 and type double, containing
   *         three vectors.
   */
  virtual std::array<std::array<double, 3>, 3> getPlaneNormals() const;

  /**
   * Finds the maximal absolute value in a two-dimensional array. Used for
   * tolerance_ interval calculations.
   *
   * @param values
   *            The array which should be searched.
   * @return The entry in <code>values</code> with the highest absolute
   *         value.
   */
  virtual double maxAbs(const std::array<std::array<double, 3>, 3>& values) const;

  /**
   * Finds the maximal absolute value in four arrays. Used for tolerance_
   * interval calculations.
   *
   * @param values_1
   *            The first array to be searched.
   * @param values_2
   *            The second array to be searched.
   * @param values_3
   *            The third array to be searched.
   * @param values_4
   *            The fourth array to be searched.
   * @return The entry with the highest absolute value in any of the 4 given
   *         arrays.
   */
  virtual double maxAbs(const std::array<double, 3>& values_1,
                        const std::array<double, 3>& values_2,
                        const std::array<double, 3>& values_3,
                        const std::array<double, 3>& values_4) const;

  virtual double multError2(double a, double a_err_2, double b, double b_err_2) const;

  virtual double multError2(double a, double a_err_2, double b, double b_err_2, double c,
                            double c_err_2) const;

  virtual double addError2(double a_err_2, double b_err_2, double result) const;

  virtual double addError2(double a_err_2, double b_err_2, double c_err_2, double result) const;

  /**
   * Calculates the center of the circumsphere of this tetrahedron and the
   * volume_. (The latter is simply convenient because the determinant needed
   * to calculate the volume_ is used anyways.)
   *
   * Along with the circumsphere calculation, an upper bound of the
   * uncertainty of this value is calculated.
   */
  virtual void computeCircumsphereCenterAndVolume();

  /**
   * Calculates the radius of this tetrahedron's circumsphere.
   */
  virtual void computeRadius();
};

}  // namespace spatial_organization
}  // namespace cx3d

#endif  // SPATIAL_ORGANIZATION_TETRAHEDRON_H_
