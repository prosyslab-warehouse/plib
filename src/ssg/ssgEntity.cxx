
#include "ssgLocal.h" 

void ssgEntity::copy_from ( ssgEntity *src, int clone_flags )
{
  ssgBase::copy_from ( src, clone_flags ) ;

  setName ( src -> getName () ) ;

  if ( clone_flags & SSG_CLONE_USERDATA )
    setUserData ( src -> getUserData () ) ;
  else
    setUserData ( NULL ) ;

  traversal_mask = src -> getTraversalMask () ;

  dirtyBSphere () ;
}


ssgEntity *ssgEntity::clone ( int /* clone_flags */ )
{
  fprintf ( stderr, "SSG: Cannot clone an ssgEntity base class.\n" ) ;
  assert ( FALSE ) ;
  return NULL ;
}


ssgEntity::ssgEntity (void)
{
  user_data = NULL ;
  traversal_mask = 0xFFFFFFFF ;
  name  = NULL ;
  type |= SSG_TYPE_ENTITY ;
}


ssgEntity::~ssgEntity (void)
{
  delete name ;
  ssgDeRefDelete ( user_data ) ;
}


void ssgEntity::setName ( char *nm )
{
  delete name ;

  if ( nm == NULL )
    name = NULL ;
  else
  {
    name = new char [ strlen ( nm ) + 1 ] ;
    strcpy ( name, nm ) ;
  }
}


void ssgEntity::dirtyBSphere ()
{
  bsphere_is_invalid = TRUE ;

  int np = getNumParents () ; 

  for ( int i = 0  ; i < np ; i++ )
    getParent ( i ) -> dirtyBSphere () ;
}

void ssgEntity::visualiseBSphere ()
{
  if ( bsphere.isEmpty () )
    return ;

  glDisable ( GL_LIGHTING ) ;
  glTranslatef(bsphere.getCenter()[0],bsphere.getCenter()[1],bsphere.getCenter()[2]);

  int spherebotch = (int)(this) % 9 ;

  switch ( spherebotch++ )
  {
#ifdef USE_GLUT_SPHERES
    case 0 : glutWireSphere(bsphere.getRadius(),10,10); break ;
    case 1 : glutWireSphere(bsphere.getRadius(),11,10); break ;
    case 2 : glutWireSphere(bsphere.getRadius(),12,10); break ;
    case 3 : glutWireSphere(bsphere.getRadius(),10,11); break ;
    case 4 : glutWireSphere(bsphere.getRadius(),11,11); break ;
    case 5 : glutWireSphere(bsphere.getRadius(),12,11); break ;
    case 6 : glutWireSphere(bsphere.getRadius(),10,12); break ;
    case 7 : glutWireSphere(bsphere.getRadius(),11,12); break ;
    case 8 : glutWireSphere(bsphere.getRadius(),12,12); spherebotch = 0 ; break ;
#else
#endif
  }
  glTranslatef(-bsphere.getCenter()[0],-bsphere.getCenter()[1],-bsphere.getCenter()[2]);
  glEnable ( GL_LIGHTING ) ;
}

void ssgEntity::print ( FILE *fd, char *indent )
{
  ssgBase::print ( fd, indent ) ;

  fprintf ( fd, "%s  Name = \"%s\"\n", indent, getPrintableName() ) ;
  fprintf ( fd, "%s  Num Parents=%d\n", indent, parents . getNumEntities () ) ;
}


/*
  Get the entity whos name equals match (case sensitive) or 0 if there isn't 
   This will be overridden by descendant classes to search trees, etc.
   This makes recovery of tagged parts of objects simple.
   I envisage it being used to find axles, gimbles etc and to seperate many objects
   from a single loaded model file.  The base version in ssgEntity returns the
   ssgEntity if the name refers to this entity or 0 if not.
*/

ssgEntity* ssgEntity::getByName ( char *match )
{
  return ( getName() != NULL && strcmp ( getName(), match ) == 0 ) ?
                                                this : (ssgEntity *) NULL ;
}


/*
  Get the entity specified by the path.  A path is a series of names
  seperated by '/'.  Each sub-unit is searched so long as the path matches.
  A single leading '/' on the path is ignored.
*/

ssgEntity* ssgEntity::getByPath ( char *path )
{
  /* ignore leading '/' */

  if ( *path == '/' )
    ++path ;

  /* return this if the name is the path else NULL */

  return ( getName() != NULL && strcmp ( getName(), path ) == 0 ) ?
                                                this : (ssgEntity *) NULL ;
}
  

ssgCullResult ssgEntity::cull_test ( sgFrustum *f, sgMat4 m, int test_needed )
{
  if ( (getTraversalMask() & SSGTRAV_CULL) == 0 )
    return SSG_OUTSIDE ;

  if ( ! test_needed )
    return SSG_INSIDE ;

  stats_cull_test++ ;
  sgSphere tmp = *(getBSphere()) ;

  if ( tmp.isEmpty () )
    return SSG_OUTSIDE ;

  tmp . orthoXform ( m ) ;

  if ( _ssgCurrentContext->isOrtho() )
    return SSG_STRADDLE ;   /* XXX Fix Me!! XXX */
  else
    return (ssgCullResult) f -> contains ( &tmp ) ;
}


ssgCullResult ssgEntity::isect_test ( sgSphere *s, sgMat4 m, int test_needed )
{
  if ( (getTraversalMask() & SSGTRAV_ISECT) == 0 )
    return SSG_OUTSIDE ;

  if ( ! test_needed )
    return SSG_INSIDE ;

  stats_isect_test++ ;
  sgSphere tmp = *(getBSphere()) ;

  if ( tmp.isEmpty () )
    return SSG_OUTSIDE ;

  tmp . orthoXform ( m ) ;

  /* Check axial distances for trivial reject */

  sgVec3 center_vec ;
  float sum_radii = s->getRadius() + tmp.getRadius() ;

  sgSubVec3 ( center_vec, s->getCenter(), tmp.getCenter() ) ;

  if ( center_vec[0] > sum_radii ||
       center_vec[1] > sum_radii ||
       center_vec[2] > sum_radii )
    return SSG_OUTSIDE ;

  float separation_sqd = sgScalarProductVec3 ( center_vec, center_vec ) ;

  float tmp_radius_sqd = sgSquare ( tmp.getRadius() ) ;
  float sph_radius_sqd = sgSquare ( s ->getRadius() ) ;

  float sum_radii_sqd = tmp_radius_sqd + sph_radius_sqd ;

  if ( separation_sqd >= sum_radii_sqd )
    return SSG_OUTSIDE ;

  if ( separation_sqd + tmp_radius_sqd <= sph_radius_sqd )
    return SSG_INSIDE ;

  return SSG_STRADDLE ;
}


/*
  Places the addresses of the entities whose names or paths
   are listed in bind into the variables whose addresses are
   listed in bind.  The bind array must terminate with a
   NULL name.  Returns TRUE if all names were found and
   bound to variables.
*/

int ssgEntity::bindEntities ( ssgEntityBinding *bind )
{
  int success = TRUE ;

  while ( bind -> nameOrPath != NULL )
  {
    ssgEntity *e = NULL ;

    if ( strchr ( bind -> nameOrPath, '/' ) )
      e = getByPath ( bind -> nameOrPath ) ;
    else
      e = getByName ( bind -> nameOrPath ) ;

    if ( e != NULL )
      *( bind -> entity ) = e ;
    else
      success = FALSE ;

    ++bind ;
  }
 
  return success ;
}


ssgCullResult ssgEntity::hot_test ( sgVec3 s, sgMat4 m, int test_needed )
{
  stats_hot_test++ ;

  if ( !test_needed )
{
stats_hot_triv_accept++ ;
    return SSG_INSIDE ;
}
   
  if ((getTraversalMask() & SSGTRAV_HOT) == 0 )
{
stats_hot_no_trav++ ;
    return SSG_OUTSIDE ;
}

  sgSphere tmp = *(getBSphere()) ;

  if ( tmp.isEmpty () )
    return SSG_OUTSIDE ;

  tmp . orthoXform ( m ) ;

  float d = sgSquare ( s[0] - tmp.getCenter()[0] ) +
            sgSquare ( s[1] - tmp.getCenter()[1] ) ;

  if ( d > sgSquare ( tmp.getRadius() ) )
{
stats_hot_radius_reject++ ;
    return SSG_OUTSIDE ;
}

stats_hot_straddle++ ;
  return SSG_STRADDLE ;
}


int ssgEntity::load ( FILE *fd )
{
  delete name ;
  name = NULL ;
  bsphere_is_invalid = TRUE ;

  _ssgReadString ( fd, &name ) ;
  _ssgReadInt    ( fd, &traversal_mask ) ;
  return ssgBase::load(fd) ;
}

int ssgEntity::save ( FILE *fd )
{
  _ssgWriteString ( fd, name ) ;
  _ssgWriteInt    ( fd, traversal_mask ) ;
  return ssgBase::save(fd) ;
}


