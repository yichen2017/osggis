/**
* osgGIS - GIS Library for OpenSceneGraph
* Copyright 2007-2008 Glenn Waldron and Pelican Ventures, Inc.
* http://osggis.org
*
* osgGIS is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#include <osgGIS/ModelResource>
#include <osgGIS/Utils>
#include <osgDB/ReadFile>
#include <osgDB/FileNameUtils>
#include <osg/ProxyNode>

using namespace osgGIS;

#include <osgGIS/Registry>
OSGGIS_DEFINE_RESOURCE(ModelResource);

static std::string EMPTY_STRING = "";


ModelResource::ModelResource()
{
    init();
}

ModelResource::ModelResource( const std::string& _name )
: Resource( _name )
{
    init();
}

void
ModelResource::init()
{
    //nop
}

ModelResource::~ModelResource()
{
    //NOP
}

void
ModelResource::setProperty( const Property& prop )
{
    if ( prop.getName() == "model_path" || prop.getName() == "path" )
        setURI( prop.getValue() ); // backwards compat - use <uri>
    else
        Resource::setProperty( prop );
}

Properties
ModelResource::getProperties() const
{
    Properties props = Resource::getProperties();
    //props.push_back( Property( "model_path", getModelPath() ) );
    return props;
}

osg::Node*
ModelResource::createNode()
{
    osg::Node* node = osgDB::readNodeFile( getAbsoluteURI() );
    return node;
};

osg::Node*
ModelResource::createProxyNode()
{
    osg::ProxyNode* proxy = new osg::ProxyNode();
    proxy->setDataVariance( osg::Object::STATIC );
    proxy->setFileName( 0, getAbsoluteURI() );
    return proxy;

    //osg::PagedLOD* plod = new osg::PagedLOD();
    //plod->addChild( dummy, 0.0f, 15000.0f, getAbsoluteURI() );
    //return plod;
}

ModelResourceQuery::ModelResourceQuery()
{
    //nop
}

void 
ModelResourceQuery::addTag( const char* tag )
{
    tags.push_back( tag );
}

const TagList& 
ModelResourceQuery::getTags() const
{
    return tags;
}


const std::string& 
ModelResourceQuery::getHashCode()
{
    //TODO
    return EMPTY_STRING;
}

