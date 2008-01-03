/**
 * osgGIS - GIS Library for OpenSceneGraph
 * Copyright 2007 Glenn Waldron and Pelican Ventures, Inc.
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

#include <osgGIS/LayerCompiler>
#include <osgSim/LineOfSight>
#include <osgSim/OverlayNode>
#include <osgDB/ReadFile>
#include <osg/TexEnv>
#include <map>
#include <string>
#include <queue>

using namespace osgGIS;




LayerCompiler::LayerCompiler()
{
    read_cb = new SmartReadCallback();
    render_bin_number = INT_MAX;
    fade_lods = false;
    overlay = false;
}

void
LayerCompiler::setTaskManager( TaskManager* _manager )
{
    task_manager = _manager;
}

TaskManager*
LayerCompiler::getTaskManager()
{
    return task_manager.get();
}

void
LayerCompiler::addScript( float min_range, float max_range, Script* script )
{
    ScriptRange slice;
    slice.min_range = min_range;
    slice.max_range = max_range;
    slice.script = script;

    script_ranges.push_back( slice );
}

LayerCompiler::ScriptRangeList&
LayerCompiler::getScripts()
{
    return script_ranges;
}

void
LayerCompiler::setTerrain(osg::Node*              _terrain,
                          const SpatialReference* _terrain_srs,
                          const GeoExtent&        _terrain_extent )
{
    terrain        = _terrain;
    terrain_srs    = (SpatialReference*)_terrain_srs;
    terrain_extent = _terrain_extent;
}


void
LayerCompiler::setTerrain(osg::Node*              _terrain,
                          const SpatialReference* _terrain_srs )
{
    setTerrain( _terrain, _terrain_srs, GeoExtent::infinite() );
}

osg::Node*
LayerCompiler::getTerrainNode()
{
    return terrain.get();
}

SpatialReference* 
LayerCompiler::getTerrainSRS() 
{
    return terrain_srs.get();
}

const GeoExtent& 
LayerCompiler::getTerrainExtent() const
{
    return terrain_extent;
}

void
LayerCompiler::setArchive( osgDB::Archive* _archive )
{
    archive = _archive;
}


osgDB::Archive*
LayerCompiler::getArchive() 
{
    return archive.get();
}


void
LayerCompiler::setRenderBinNumber( int value )
{
    render_bin_number = value;
}


int
LayerCompiler::getRenderBinNumber() const
{
    return render_bin_number;
}

void
LayerCompiler::setFadeLODs( bool value )
{
    fade_lods = value;
}

bool
LayerCompiler::getFadeLODs() const
{
    return fade_lods;
}


void
LayerCompiler::setPaged( bool value )
{
    paged = value;
}

bool
LayerCompiler::getPaged() const
{
    return paged;
}

void
LayerCompiler::setOverlay( bool value )
{
    overlay = value;
}

bool
LayerCompiler::getOverlay() const
{
    return overlay;
}

osg::Node*
LayerCompiler::convertToOverlay( osg::Node* input )
{
    osgSim::OverlayNode* o_node = new osgSim::OverlayNode();
    o_node->getOrCreateStateSet()->setTextureAttribute( 1, new osg::TexEnv( osg::TexEnv::DECAL ) );
    o_node->setOverlaySubgraph( input );
    o_node->setOverlayTextureSizeHint( 1024 );
    return o_node;
}