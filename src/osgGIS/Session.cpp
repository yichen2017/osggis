/**

/* osgGIS - GIS Library for OpenSceneGraph
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

#include <osgGIS/Session>
#include <osgGIS/Registry>
#include <OpenThreads/ScopedLock>

using namespace osgGIS;
using namespace OpenThreads;

Session::Session()
{
    resources = new ResourceLibrary( session_mtx );
}

Session::~Session()
{
    //NOP
}


Session*
Session::derive()
{
    ScopedLock<ReentrantMutex> sl( session_mtx );

    Session* result = new Session();
    result->scripts.insert( result->scripts.end(), scripts.begin(), scripts.end() );
    result->resources = resources;

    return result;
}


ScriptEngine*
Session::createScriptEngine()
{
    // sessions are shared across compilations that might be in 
    // separate threads.
    ScopedLock<ReentrantMutex> sl( session_mtx );

    // get an empty scripting engine from the registry's factory
    ScriptEngine* engine = Registry::instance()->createScriptEngine();

    // install all the session scripts
    for( ScriptList::iterator i = scripts.begin(); i != scripts.end(); i++ )
        engine->install( i->get() );

    return engine;
}

void
Session::addScript( Script* script )
{
    ScopedLock<ReentrantMutex> sl( session_mtx );
    scripts.push_back( script );
}

const ScriptList&
Session::getScripts() const 
{
    return scripts;
}

const ResourceLibrary*
Session::getResources() const
{
    return const_cast<const ResourceLibrary*>( resources.get() );
}

ResourceLibrary*
Session::getResources()
{
    return resources.get();
}

FilterEnv*
Session::createFilterEnv()
{
    return new FilterEnv( this );
}

void 
Session::setProperty( const Property& prop )
{
    ScopedLock<ReentrantMutex> sl( session_mtx );
    properties.set( prop );
}

Property
Session::getProperty( const std::string& name )
{
    ScopedLock<ReentrantMutex> sl( session_mtx );
    return properties.get( name );
}

OpenThreads::ReentrantMutex&
Session::getSessionMutex()
{
    return session_mtx;
}

void
Session::markResourceUsed( Resource* r )
{
    ScopedLock<ReentrantMutex> sl( session_mtx );
    if ( std::find( resources_used.begin(), resources_used.end(), r ) == resources_used.end() )
        resources_used.push_back( r );
}

ResourceList
Session::getResourcesUsed( bool reset )
{
    ScopedLock<ReentrantMutex> sl( session_mtx );

    ResourceList list;
    list.reserve( resources_used.size() );
    for( ResourceList::const_iterator i = resources_used.begin(); i != resources_used.end(); i++ )
    {
        list.push_back( i->get() );
    }

    if ( reset )
        resources_used.clear();

    return list;
}
