#include <tetra/gl/geometry/Rect.hpp>
#include <algorithm>

using namespace tetra;
using namespace tetra::gl;
using namespace tetra::gl::geometry;

Rect::Rect( float bottom, float top, float left, float right )
  : bottom{bottom}, top{top}, left{left}, right{right} {};

void Rect::translate( float dx, float dy ) noexcept
{
  bottom += dy;
  top += dy;
  left += dx;
  right += dx;
}

float Rect::getBottom() const noexcept { return bottom; }
void Rect::setBottom( float val ) noexcept { bottom = val; }

float Rect::getTop() const noexcept { return top; }
void Rect::setTop( float val ) noexcept { top = val; }

float Rect::getLeft() const noexcept { return left; }
void Rect::setLeft( float val ) noexcept { left = val; }

float Rect::getRight() const noexcept { return right; }
void Rect::setRight( float val ) noexcept { right = val; }

Rect geometry::BoundingBox( Rect first, Rect second )
{
  Rect bb;
  bb.setBottom( std::min( first.getBottom(), second.getBottom() ) );
  bb.setTop( std::max( first.getTop(), second.getTop() ) );
  bb.setLeft( std::min( first.getLeft(), second.getLeft() ) );
  bb.setRight( std::max( first.getRight(), second.getRight() ) );

  return bb;
}
