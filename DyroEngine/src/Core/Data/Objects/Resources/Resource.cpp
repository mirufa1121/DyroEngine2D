#include "Core/Data/Objects/Resources/Resource.h"

Resource::Resource(const std::tstring& resourcePath, ResourceType type)
	:resource_path(resourcePath)
	, resource_type(type)
	, resource_id(ObjectCounter<Resource>::getAmount())
{
	int start_index = this->resource_path.find_last_of('\\');
	if (start_index == (int)std::tstring::npos)
		start_index = this->resource_path.find_last_of('/');
	++start_index;
	int end_index = this->resource_path.find_last_of('.');

	this->resource_name = this->resource_path.substr(start_index, end_index - start_index);
}
Resource::~Resource()
{
}

unsigned int Resource::getResourceID() const
{
	return this->resource_id;
}

const std::tstring& Resource::getResourcePath()
{
	return this->resource_path;
}
const std::tstring& Resource::getResourceName()
{
	return this->resource_name;
}

ResourceType Resource::getResourceType()
{
	return this->resource_type;
}