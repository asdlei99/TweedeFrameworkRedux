#pragma once

#include "TeCorePrerequisites.h"
#include "Utility/TeModule.h"
#include "Utility/TeEvent.h"
#include "Importer/TeImporter.h"
#include "Threading/TeThreading.h"

namespace te
{
    /** Manager that handles resource loading.*/
    class TE_CORE_EXPORT ResourceManager: public Module<ResourceManager>
    {
        struct LoadedResourceData
        {
            ResourceHandle<Resource> resource;

            LoadedResourceData() = default;
            LoadedResourceData(ResourceHandle<Resource> resource)
                : resource(resource)
            {}
        };

    public:
        ResourceManager();
        ~ResourceManager();

        TE_MODULE_STATIC_HEADER_MEMBER(ResourceManager)

        template <class T>
        ResourceHandle<T> Load(UUID& uuid = UUID::EMPTY)
        {
            OnResourceLoaded(Get(uuid));
            return static_resource_cast<T>(Get(uuid));
        }

        template <class T>
        ResourceHandle<T> Load(const UUID& uuid = UUID::EMPTY)
        {
            OnResourceLoaded(Get(uuid));
            return static_resource_cast<T>(Get(uuid));
        }

        template <class T>
        ResourceHandle<T> Load(const String& filePath)
        {
            UUID uuid;
            GetUUIDFromFile(filePath, uuid);

            if(uuid.Empty())
            {
                ResourceHandle<T> resourceHandle = gImporter().Import<T>(filePath);
                uuid = resourceHandle.GetHandleData()->uuid;
                RegisterResource(uuid, filePath);
                _loadedResources[uuid] = static_resource_cast<Resource>(resourceHandle);
            }

            OnResourceLoaded(Get(uuid));
            return static_resource_cast<T>(Get(uuid));
        }

        template <class T>
        ResourceHandle<T> Load(const String& filePath, const SPtr<const ImportOptions>& options)
        {
            UUID uuid;
            ResourceHandle<T> resourceHandle;
            GetUUIDFromFile(filePath, uuid);

            if (uuid.Empty())
            {
                resourceHandle = gImporter().Import<T>(filePath, options);

                if (resourceHandle.GetHandleData())
                {
                    uuid = resourceHandle.GetHandleData()->uuid;
                    resourceHandle.GetInternalPtr()->_UUID = uuid;
                    RegisterResource(uuid, filePath);
                    _loadedResources[uuid] = static_resource_cast<Resource>(resourceHandle);
                }
            }

            if (resourceHandle.GetHandleData())
            {
                OnResourceLoaded(Get(uuid));
                return static_resource_cast<T>(Get(uuid));
            }

            return resourceHandle;
        }

        void Update(HResource& handle, const SPtr<Resource>& resource);

        void Release(const HResource& resource) 
        { 
            Release((ResourceHandleBase&)resource); 
        }

        void Release(ResourceHandleBase& resource);

        /** Forces unload of all resources, whether they are being used or not. */
        void UnloadAll();

        /**	Destroys a resource, freeing its memory. */
        void Destroy(ResourceHandleBase& resource);

        /** Creates a new resource handle from a resource pointer. */
        HResource _createResourceHandle(const SPtr<Resource>& obj);

        /** Creates a new resource handle from a resource pointer, with a user defined UUID. */
        HResource _createResourceHandle(const SPtr<Resource>& obj, const UUID& UUID);

        /** Allow to retrieve a resource using its uuid */
        HResource Get(const UUID& uuid);

        /** Return an unordered map containing all resources loaded at call time */
        Vector<HResource> GetAll();

        /** Find all resources based on the _coreType (Serializable) */
        Vector<HResource> FindByType(UINT32 type);

    public:
        Event<void(const HResource&)> OnResourceLoaded;

        /** Called when the resource has been destroyed. Provides UUID of the destroyed resource.*/
        Event<void(const UUID&)> OnResourceDestroyed;

        /** Called when the internal resource the handle is pointing to has changed. */
        Event<void(const HResource&)> OnResourceModified;

    private:
        friend class ResourceHandleBase;

        bool GetUUIDFromFile(const String& filePath, UUID& uuid);
        bool GetFileFromUUID(const UUID& uuid, String& filePath);
        void RegisterResource(const UUID& uuid, const String& filePath);

    private:
        UnorderedMap<UUID, LoadedResourceData> _loadedResources;
        UnorderedMap<UUID, String> _UUIDToFile;
        UnorderedMap<String, UUID> _fileToUUID;

        RecursiveMutex _loadingResourceMutex;
        RecursiveMutex _loadingUuidMutex;
    };

    TE_CORE_EXPORT ResourceManager& gResourceManager();
}