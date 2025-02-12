#pragma once

#include "Prerequisites/TePrerequisitesUtility.h"

namespace te
{
    /**
     * A string identifier that provides very fast comparisons to other string identifiers. Significantly faster than
     * comparing raw strings.
     *
     * @note	
     * Essentially a unique ID is generated for each string and then the ID is used for comparisons as if you were using
     * an integer or an enum.
     */
    class TE_UTILITY_EXPORT StringID
    {
        static constexpr const int HASH_TABLE_SIZE = 4096;
        static constexpr const int MAX_CHUNK_COUNT = 50;
        static constexpr const int ELEMENTS_PER_CHUNK = 256;
        static constexpr const int STRING_SIZE = 256;

        /** Helper class that performs string actions on both null terminated character arrays and standard strings. */
        template<class T>
        class StringIDUtil
        {
        public:
            static UINT32 size(T const& input) { return 0; }
            static void copy(T const& input, char* dest) { }
            static bool compare(T const& a, char* b) { return 0; }
        };

        /**	Internal data that is shared by all instances for a specific string. */
        struct InternalData
        {
            UINT32 id;
            InternalData* next;
            char chars[STRING_SIZE];
        };

        /**	Performs initialization of static members as soon as the library is loaded. */
        struct InitStatics
        {
            InitStatics();
        };

    public:
        StringID() = default;

        StringID(const char* name)
        {
            Construct(name);
        }

        StringID(const String& name)
        {
            Construct(name);
        }

        template<int N>
        StringID(const char name[N])
        {
            Construct((const char*)name);
        }

        /**	Compare to string ids for equality. Uses fast integer comparison. */
        bool operator== (const StringID& rhs) const
        {
            return _data == rhs._data;
        }

        /**	Compare to string ids for inequality. Uses fast integer comparison. */
        bool operator!= (const StringID& rhs) const
        {
            return _data != rhs._data;
        }

        /** Implicitly converts to a normal string. */
        operator String() const { return String(_data->chars); }

        /**	Returns true if the string id has no value assigned. */
        bool empty() const
        {
            return _data == nullptr;
        }

        /**	Returns the null-terminated name of the string id. */
        const char* c_str() const
        {
            if (_data == nullptr)
                return "";

            return _data->chars;
        }

        /** Returns the unique identifier of the string. */
        UINT32 id() const { return _data ? _data->id : -1; }

        static const StringID NONE;

    private:
        /**Constructs a StringID object in a way that works for pointers to character arrays and standard strings. */
        template<class T>
        void Construct(T const& name);

        /**	Calculates a hash value for the provided null-terminated string. */
        template<class T>
        UINT32 CalcHash(T const& input);

        /**
         * Allocates a new string entry and assigns it a unique ID. Optionally expands the chunks buffer if the new entry
         * doesn't fit.
         */
        InternalData* allocEntry();

        InternalData* _data = nullptr;

        static volatile InitStatics _initStatics;
        static InternalData* _stringHashTable[HASH_TABLE_SIZE];
        static InternalData* _chunks[MAX_CHUNK_COUNT];

        static UINT32 _nextId;
        static UINT32 _numChunks;
    };
}

namespace std
{
    /**	Hash value generator for StringID. */
    template<>
    struct hash<te::StringID>
    {
        size_t operator()(const te::StringID& value) const
        {
            return (size_t)value.id();
        }
    };
}
