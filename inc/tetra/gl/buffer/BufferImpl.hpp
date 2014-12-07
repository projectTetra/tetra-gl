template <class VertexType>
Buffer<VertexType>::Buffer()
{
  static_assert( std::is_standard_layout<VertexType>::value,
                 "Vertex data must have standard layout!" );

  glGenBuffers( 1, &this->bufferHandle );
  CheckGLError( "glGenBuffers", {"vertex buffer"} );
}

template <class VertexType>
Buffer<VertexType>::~Buffer()
{
  glDeleteBuffers( 1, &this->bufferHandle );
  glDeleteBuffers( 1, &this->elementBufferHandle );
}

template <class VertexType>
Buffer<VertexType>::Buffer( Buffer<VertexType>&& b ) NOEXCEPT
  : bufferHandle{std::move( b.bufferHandle )},
    elementBufferHandle{std::move( b.elementBufferHandle )},
    vertexCount{std::move( b.vertexCount )},
    vao{std::move( b.vao )}
{
  b.bufferHandle = 0;
  b.elementBufferHandle = 0;
  b.vertexCount = 0;
}

template <class VertexType>
Buffer<VertexType>& Buffer<VertexType>::
operator=( Buffer<VertexType>&& b )
{
  // delete my old buffers
  glDeleteBuffers( 1, &this->bufferHandle );
  CheckGLError( "glDeleteBuffers", {"vertex buffer"} );

  glDeleteBuffers( 1, &this->elementBufferHandle );
  CheckGLError( "glDeleteBuffers", {"element buffer"} );

  // use in-place new to call the move constructor
  new ( this ) Buffer<VertexType>{std::move( b )};
  return *this;
}

template <class VertexType>
void Buffer<VertexType>::setData( const std::vector<VertexType>& data,
                                  GLenum bufferUsage )
{
  glBindBuffer( GL_ARRAY_BUFFER, this->bufferHandle );
  CheckGLError( "glBindBuffer", {"setData"} );

  glBufferData( GL_ARRAY_BUFFER, data.size() * sizeof( VertexType ),
                data.data(), bufferUsage );
  CheckGLError( "glBufferData", {"setData"} );

  // if there is an element buffer, then we let the element buffer
  // determine the number of verticies
  if ( !this->elementBufferHandle )
  {
    this->vertexCount =
      static_cast<decltype( this->vertexCount )>( data.size() );
  }
}

template <class VertexType>
void Buffer<VertexType>::setElementData(
  const std::vector<unsigned short>& data, GLenum bufferUsage )
{
  if ( !this->elementBufferHandle )
  {
    glGenBuffers( 1, &this->elementBufferHandle );
    CheckGLError( "glGenBuffers", {"element buffer"} );
  }

  bind();
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, this->elementBufferHandle );
  CheckGLError( "glBindBuffer", {"element buffer"} );

  glBufferData( GL_ELEMENT_ARRAY_BUFFER,
                data.size() * sizeof( unsigned short ), data.data(),
                bufferUsage );

  this->vertexCount =
    static_cast<decltype( this->vertexCount )>( data.size() );
}

template <class VertexType>
template <std::array<float, 1>::size_type length>
Buffer<VertexType>& Buffer<VertexType>::enableVertexAttrib(
  GLuint index, std::array<float, length> VertexType::*Attrib )
{
  // Get the byte offset of the attrib from beginning of the vertex
  // struct
  auto offset = ( std::size_t ) & ( ( (VertexType*)0 )->*Attrib );

  this->vao.bind();
  glBindBuffer( GL_ARRAY_BUFFER, this->bufferHandle );
  CheckGLError( "glBindBuffer", {"vertex buffer"} );

  glEnableVertexAttribArray( index );
  CheckGLError( "glEnableVertexAttribArray", {"vertex buffer"} );

  glVertexAttribPointer( index, length, GL_FLOAT, GL_FALSE,
                         sizeof( VertexType ),
                         reinterpret_cast<const GLvoid*>( offset ) );
  CheckGLError( "glVertexAttribPointer", {"vertex buffer"} );

  return *this;
}

template <class VertexType>
void Buffer<VertexType>::draw( GLenum topology ) const
{
  bind();
  if ( this->elementBufferHandle )
  {
    glDrawElements( topology, this->vertexCount, GL_UNSIGNED_SHORT,
                    0 );
    CheckGLError(
      "glDrawElements",
      {"vertexCount: " + std::to_string( this->vertexCount )} );
  }
  else
  {
    glDrawArrays( topology, 0, this->vertexCount );
    CheckGLError(
      "glDrawArrays",
      {"vertexCount: " + std::to_string( this->vertexCount )} );
  }
}

template <class VertexType>
GLuint Buffer<VertexType>::expose() const NOEXCEPT
{
  return this->bufferHandle;
}

template <class VertexType>
void Buffer<VertexType>::bind() const NOEXCEPT
{
  this->vao.bind();
}

template <class VertexType>
unsigned Buffer<VertexType>::size() const NOEXCEPT
{
  return this->vertexCount;
}

