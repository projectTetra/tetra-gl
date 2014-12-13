#pragma once
#ifndef TETRA_GL_GEOMETRY_RECT_HPP
#define TETRA_GL_GEOMETRY_RECT_HPP

namespace tetra
{
namespace gl
{
namespace geometry
{

/**
 * Holds information about a rectangle in cartesian coordinates.
 **/
class Rect
{
  float bottom{0}, top{0}, left{0}, right{0};

public:
  Rect() = default;
  Rect( float bottom, float top, float left, float right );

  /**
   * Tranlates the rectangle by dx and dy.
   **/
  void translate( float dx, float dy ) noexcept;

  float getBottom() const noexcept;
  void setBottom( float bottom ) noexcept;

  float getTop() const noexcept;
  void setTop( float top ) noexcept;

  float getLeft() const noexcept;
  void setLeft( float left ) noexcept;

  float getRight() const noexcept;
  void setRight( float right ) noexcept;
};

Rect BoundingBox( Rect first, Rect second );

} /* namespace geometry */
} /* namespace gl */
} /* namespace tetra */

#endif
