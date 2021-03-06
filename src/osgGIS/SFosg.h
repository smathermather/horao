/**
 *   Horao
 *
 *   Copyright (C) 2013 Oslandia <infos@oslandia.com>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Library General Public
 *   License as published by the Free Software Foundation; either
 *   version 2 of the License, or (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Library General Public License for more details.

 *   You should have received a copy of the GNU Library General Public
 *   License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */
#ifndef STACK3D_OSGGIS_SFOSG
#define STACK3D_OSGGIS_SFOSG

#ifndef CALLBACK
#define CALLBACK
#endif

#include <osg/Geometry>

namespace osgGIS {

//! just encapsulate a cont char * to give it a type since
//! WKT and WKB are both const char *
struct ConstCharWrapper {
    // default dtor and cpy are ok
    ConstCharWrapper( const char* data ):_data( data ) {}
    const char* get() const {
        return _data;
    }
private :
    const char* _data ;
};

struct WKT: ConstCharWrapper {
    WKT( const char* data ): ConstCharWrapper( data ) {}
};
struct WKB: ConstCharWrapper {
    WKB( const char* data ): ConstCharWrapper( data ) {}
};

//! @brief build an osg::Geometry from WKT or WKB represenations
//! @note this structure avoids the creation of many small osg::geometries (slow)
struct Mesh {
    //! @param layerToWord transformation from GIS CRS (layer) to OpenGL scene (world)
    //!        the aim is mainly to center the scene around origin to avoid round-off errors
    Mesh( const osg::Matrixd& layerToWord )
        : _layerToWord( layerToWord )
    {}


    void push_back( WKB geometry );
    void push_back( WKT geometry );

    void addBar( WKB center, float width, float depth, float height );

    osg::Geometry* createGeometry() const;

private:
    std::vector<osg::Vec3> _vtx;
    std::vector<osg::Vec3> _nrml;
    std::vector<unsigned> _tri;
    const osg::Matrixd _layerToWord;

    template< typename GEOM >
    void push_back( const GEOM* );  // utility fonction, specialised for several types

    //! @note this is needed for glu tesselation to avoid exposing vtx and tri members
    friend void CALLBACK tessVertexCB( const GLdouble* vtx, void* data );

};

}
#endif

