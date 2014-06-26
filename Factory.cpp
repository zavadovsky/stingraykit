#include <stingray/toolkit/Factory.h>


namespace stingray {
namespace Detail
{

	Factory::~Factory()
	{
		STINGRAY_TRY("Clean failed",
			MutexLock l(_registryGuard);
			for (ClassRegistry::iterator i = _registry.begin(); i != _registry.end(); ++i)
				delete i->second;
		);
	}


	IFactoryObject* Factory::Create(const std::string& name)
	{
		MutexLock l(_registryGuard);

		ClassRegistry::const_iterator it = _registry.find(name);
		TOOLKIT_CHECK(it != _registry.end(), "Class '" + name + "' was not registered!");

		const IFactoryObjectCreator& creator = *it->second;
		return creator.Create();
	}


	const IFactoryObjectCreator& Factory::GetCreator(const std::string& name)
	{
		MutexLock l(_registryGuard);

		ClassRegistry::const_iterator it = _registry.find(name);
		TOOLKIT_CHECK(it != _registry.end(), "Class '" + name + "' was not registered!");

		return *it->second;
	}


	void Factory::Register(const std::string& name, IFactoryObjectCreator* creator)
	{
		Logger::Debug() << "Registering " << name;

		MutexLock l(_registryGuard);

		const ClassRegistry::const_iterator it = _registry.find(name);
		TOOLKIT_CHECK(it == _registry.end(), "Class '" + name + "' was already registered!");

		_registry.insert(std::make_pair(name, creator));
	}

}}

