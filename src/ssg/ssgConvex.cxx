#include "ssgLocal.h"

#ifdef GLU_VERSION_1_1

static GLUtriangulatorObj *tesselator     = NULL;
static ssgVtxTable        *source         = NULL;
static ssgBranch          *poly_branch    = NULL;

static GLenum             curr_gltype;
static ssgVertexArray     *curr_vertices  = NULL;
static ssgNormalArray     *curr_normals   = NULL;
static ssgColourArray     *curr_colours   = NULL;
static ssgTexCoordArray   *curr_texcoords = NULL;

static void vertexCallback( GLvoid *vertex_data ) {
  int index = *(short*)vertex_data;

  curr_vertices -> add( source -> getVertex(index) );

  if ( source->getNumColours()   > 1 ) 
    curr_colours   -> add ( source -> getColour   (index) );
  if ( source->getNumNormals()   > 1 ) 
    curr_normals   -> add ( source -> getNormal   (index) );
  if ( source->getNumTexCoords() > 1 ) 
    curr_texcoords -> add ( source -> getTexCoord (index) );
}

static void beginCallback( GLenum type ) {
  curr_gltype    = type;
  curr_vertices  = new ssgVertexArray;
  curr_normals   = new ssgNormalArray;
  curr_colours   = new ssgColourArray;
  curr_texcoords = new ssgTexCoordArray;

  if ( source->getNumColours() == 1 ) 
    curr_colours -> add ( source -> getColour (0) );
  if ( source->getNumNormals() == 1 ) 
    curr_normals -> add ( source -> getNormal (0) );
}

static void endCallback( void ) {
  ssgVtxTable *leaf = new ssgVtxTable( curr_gltype,
				       curr_vertices,
				       curr_normals,
				       curr_texcoords,
				       curr_colours );
  leaf -> setName     ( source -> getName    () );
  leaf -> setState    ( source -> getState   () );
  leaf -> setCullFace ( source -> getCullFace() );

  poly_branch -> addKid( leaf );
}

ssgEntity *ssgVtxTable::makeConvex() {
  int i;

  if ( gltype != GL_TRIANGLE_FAN && gltype != GL_POLYGON ) {
    // lines, points, triangles and quads are already convex
    return NULL;
  }

  tesselator = gluNewTess();
  if (tesselator == NULL) {
    ulSetError( UL_FATAL, "ssgVtxTable::makeConvex: Unable to create new " \
		"tesselator." );
    return NULL;
  }

  gluTessCallback( tesselator, GLU_BEGIN , 
		   (void (*)())beginCallback  );
  gluTessCallback( tesselator, GLU_END   , 
		   (void (*)())endCallback    );
  gluTessCallback( tesselator, GLU_VERTEX, 
		   (void (*)())vertexCallback );

  source      = this;
  poly_branch = new ssgBranch;

  /*
    We now have to copy all vertices in the right order into an
    array of sgdVec3, since that's how GLU takes its vertex data.
    
    The way to get the vertices is a bit intricate, since this might
    be an ssgVtxArray, where the vertices aren't ordered in the
    ssgVertexArray. We have to get the vertices in counter clockwise order
    by extracting the triangles.
  */
  sgdVec3 *dvertices    = new sgdVec3[ getNumTriangles() + 2 ];
  short   *vert_indices = new short  [ getNumTriangles() + 2 ];
  int c = 0;
  for ( i = 0; i < getNumTriangles(); i++) {
    short v1, v2, v3;
    getTriangle( i, &v1, &v2, &v3 );
    if ( i == 0 ) {
      sgdSetVec3( dvertices[c    ], 
		  getVertex(v1)[0], getVertex(v1)[1], getVertex(v1)[2] );
      sgdSetVec3( dvertices[c + 1], 
		  getVertex(v2)[0], getVertex(v2)[1], getVertex(v2)[2] );
      vert_indices[c    ] = v1;
      vert_indices[c + 1] = v2;
      c += 2;
    }

    sgdSetVec3( dvertices[c], 
		getVertex(v3)[0], getVertex(v3)[1], getVertex(v3)[2] );    
    vert_indices[c] = v3;
    c++;
  }

  /* Finally, lets feed GLU with the vertices */
  gluBeginPolygon( tesselator );
  for ( i = 0; i < getNumTriangles() + 2; i++) {
    gluTessVertex( tesselator, dvertices[i], & vert_indices[i] );
  }
  gluEndPolygon( tesselator );
  gluDeleteTess( tesselator );

  delete dvertices;
  delete vert_indices;

  if ( poly_branch -> getNumKids() > 1 ) {
    return poly_branch;
  } else {
    // since one or less kids were created, this leaf is already
    // convex.
    delete poly_branch;
    return NULL;
  }
}

#else

ssgEntity *ssgVtxTable::makeConvex() {
    ulSetError( UL_FATAL, "ssgVtxTable::makeConvex: GLU version 1.1 was not" \
		" present when compiling PLIB." );
    return NULL;
}

#endif

//  int ssgVtxTable::isConvex() {
//    return TRUE;
//  }