#ifndef _RESOURCEMANAGER_H
#define _RESOURCEMANAGER_H

#include "Core/Helpers/Patterns/Manager.h"

#ifndef _STRING_H
#include "Core\Defines\string.h"
#endif

#ifndef _RESOURCE_H
#include "Core\Data\Objects\Resources\Resource.h"
#endif

#ifndef _MAP_
#include <map>
#endif
#ifndef _ALGORITHM_
#include <algorithm>
#endif

class ResourceManager : public Manager<Resource>
{
public:
	ResourceManager();
	virtual ~ResourceManager();

	virtual bool initialize();
	virtual bool shutdown();

	template<typename T> T* getResource(const std::tstring& path);
	template<typename T> T* getResource(unsigned int id);
};

template<typename T>
T* ResourceManager::getResource(const std::tstring& path)
{
	std::map<unsigned int, Resource*> map_objects = getObjects();

	std::map<unsigned int, Resource*>::const_iterator it = std::find_if(map_objects.begin(), map_objects.end(),
		[path](std::pair<unsigned int, Resource*> pair)
	{
		return path == pair.second->getResourcePath();
	});

	if (it != map_objects.end())
		return dynamic_cast<T*>((*it).second);

	T * new_r = new T(path);
	new_r->initialize();

	addObject(new_r->getResourceID(), new_r);

	return new_r;
}
template<typename T>
T* ResourceManager::getResource(unsigned int id)
{
	Resource* resource = getObject(id);
	if (resource == nullptr)
		return nullptr;

	return dynamic_cast<T*>(resource);
}

#endif
