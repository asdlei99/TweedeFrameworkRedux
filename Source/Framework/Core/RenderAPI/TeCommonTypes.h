#pragma once

namespace te
{
#undef None
#undef Convex

    /**	Factors used when blending new pixels with existing pixels. */
    enum BlendFactor
    {
        BF_ONE, /**< Use a value of one for all pixel components. */
        BF_ZERO, /**< Use a value of zero for all pixel components. */
        BF_DEST_COLOR, /**< Use the existing pixel value. */
        BF_SOURCE_COLOR, /**< Use the newly generated pixel value. */
        BF_INV_DEST_COLOR, /**< Use the inverse of the existing value. */
        BF_INV_SOURCE_COLOR, /**< Use the inverse of the newly generated pixel value. */
        BF_DEST_ALPHA, /**< Use the existing alpha value. */
        BF_SOURCE_ALPHA, /**< Use the newly generated alpha value. */
        BF_INV_DEST_ALPHA, /**< Use the inverse of the existing alpha value. */
        BF_INV_SOURCE_ALPHA /**< Use the inverse of the newly generated alpha value. */
    };

    /**	Operations that determines how are blending factors combined. */
    enum BlendOperation
    {
        BO_ADD, /**< Blend factors are added together. */
        BO_SUBTRACT, /**< Blend factors are subtracted in "srcFactor - dstFactor" order. */
        BO_REVERSE_SUBTRACT, /**< Blend factors are subtracted in "dstFactor - srcFactor" order. */
        BO_MIN, /**< Minimum of the two factors is chosen. */
        BO_MAX /**< Maximum of the two factors is chosen. */
    };

	/**	Comparison functions used for the depth/stencil buffer. */
	enum CompareFunction
	{
		CMPF_ALWAYS_FAIL, /**< Operation will always fail. */
		CMPF_ALWAYS_PASS, /**< Operation will always pass. */
		CMPF_LESS, /**< Operation will pass if the new value is less than existing value. */
		CMPF_LESS_EQUAL, /**< Operation will pass if the new value is less or equal than existing value. */
		CMPF_EQUAL, /**< Operation will pass if the new value is equal to the existing value. */
		CMPF_NOT_EQUAL, /**< Operation will pass if the new value is not equal to the existing value. */
		CMPF_GREATER_EQUAL, /**< Operation will pass if the new value greater or equal than the existing value. */
		CMPF_GREATER /**< Operation will pass if the new value greater than the existing value. */
	};

    /**
     * Types of texture addressing modes that determine what happens when texture coordinates are outside of the valid range.
     */
    enum TextureAddressingMode
    {
        TAM_WRAP, /**< Coordinates wrap back to the valid range. */
        TAM_MIRROR, /**< Coordinates flip every time the size of the valid range is passed. */
        TAM_CLAMP, /**< Coordinates are clamped within the valid range. */
        TAM_BORDER /**< Coordinates outside of the valid range will return a separately set border color. */
    };

    /**	Types of available filtering situations. */
    enum FilterType
    {
        FT_MIN, /**< The filter used when shrinking a texture. */
        FT_MAG, /**< The filter used when magnifying a texture. */
        FT_MIP /**< The filter used when filtering between mipmaps. */
    };

    /**	Filtering options for textures. */
    enum FilterOptions
    {
        FO_NONE = 0, /**< Use no filtering. Only relevant for mipmap filtering. */
        FO_POINT = 1, /**< Filter using the nearest found pixel. Most basic filtering. */
        FO_LINEAR = 2, /**< Average a 2x2 pixel area, signifies bilinear filtering for texture, trilinear for mipmaps. */
        FO_ANISOTROPIC = 3, /**< More advanced filtering that improves quality when viewing textures at a steep angle */
    };
	
    /**	Types of frame buffers. */
	enum FrameBufferType
	{
		FBT_COLOR = 0x1, /**< Color surface. */
		FBT_DEPTH = 0x2, /**< Depth surface. */
		FBT_STENCIL = 0x4 /**< Stencil surface. */
	};

	/**
	 * Types of culling that determine how (and if) hardware discards faces with certain winding order. Winding order can
	 * be used for determining front or back facing polygons by checking the order of its vertices from the render
	 * perspective.
	 */
	enum CullingMode
	{
		CULL_NONE = 0, /**< Hardware performs no culling and renders both sides. */
		CULL_CLOCKWISE = 1, /**< Hardware culls faces that have a clockwise vertex ordering. */
		CULL_COUNTERCLOCKWISE = 2 /**< Hardware culls faces that have a counter-clockwise vertex ordering. */
	};

	/**	Polygon mode to use when rasterizing. */
	enum PolygonMode
	{
		PM_WIREFRAME = 1, /**< Render as wireframe showing only polygon outlines. */
		PM_SOLID = 2 /**< Render as solid showing whole polygons. */
	};

    /**	Types of action that can happen on the stencil buffer. */
    enum StencilOperation
    {
        SOP_KEEP, /**< Leave the stencil buffer unchanged. */
        SOP_ZERO, /**< Set the stencil value to zero. */
        SOP_REPLACE, /**< Replace the stencil value with the reference value. */
        SOP_INCREMENT, /**< Increase the stencil value by 1, clamping at the maximum value. */
        SOP_DECREMENT, /**< Decrease the stencil value by 1, clamping at 0. */
        SOP_INCREMENT_WRAP, /**< Increase the stencil value by 1, wrapping back to 0 when incrementing past the maximum value. */
        SOP_DECREMENT_WRAP, /**< Decrease the stencil value by 1, wrapping when decrementing 0. */
        SOP_INVERT /**< Invert the bits of the stencil buffer. */
    };

    /**
     * Suggested queue priority numbers used for sorting objects in the render queue. Objects with higher priority will
     * be renderer sooner.
     */
    enum class QueuePriority
    {
        Opaque = 100000,
        Transparent = 90000,
        Skybox = 80000,
        Overlay = 70000
    };

    /** Type of sorting to perform on an object when added to a render queue. */
    enum class QueueSortType
    {
        FrontToBack, /**< All objects with the same priority will be rendered front to back based on their center. */
        BackToFront, /**< All objects with the same priority will be rendered back to front based on their center. */
        None /**< Objects will not be sorted and will be processed in the order they were added to the queue. */
    };

    /**	Flags that may be assigned to a shader that let the renderer know how to interpret the shader. */
    enum class ShaderFlag
    {
        Forward = 0x2 /**< Signifies the shader should use the forward rendering pipeline, if relevant. */
    };

	/** These values represent a hint to the driver when locking a hardware buffer. */
	enum GpuLockOptions
	{
		/**
		 * Allows you to write to the buffer. Can cause a CPU-GPU sync point so avoid using it often (every frame) as
		 * that might limit your performance significantly.
		 */
		GBL_READ_WRITE,
		/**
		 * Allows you to write to the buffer. Tells the driver to completely discard the contents of the buffer you are
		 * writing to. The driver will (most likely) internally allocate another buffer with same specifications (which is
		 * fairly fast) and you will avoid CPU-GPU stalls.
		 */
		GBL_WRITE_ONLY_DISCARD,
		/**
		 * Allows you to write to the buffer. Tells the driver to discard the contents of the mapped buffer range (but
		 * not the entire buffer like with GBL_WRITE_ONLY_DISCARD). Use this if you plan on overwriting all of the
		 * range. This can help avoid CPU-GPU stalls.
		 */
		GBL_WRITE_ONLY_DISCARD_RANGE,
		/**  Allows you to read from a buffer. Be aware that reading is usually a very slow operation. */
		GBL_READ_ONLY,
		/**
		 * Allows you to write to the buffer. Guarantees the driver that you will not be updating any part of the buffer
		 * that is currently used. This will also avoid CPU-GPU stalls, without requiring you to discard the entire buffer.
		 * However it is hard to guarantee when GPU has finished using a buffer.
		 */
		GBL_WRITE_ONLY_NO_OVERWRITE,
		/** Allows you to write to a buffer. */
		GBL_WRITE_ONLY	
	};

    /** Types of programs that may run on GPU. */
	enum GpuProgramType
	{
		GPT_VERTEX_PROGRAM, /**< Vertex program. */
		GPT_PIXEL_PROGRAM, /**< Fragment(pixel) program. */
		GPT_GEOMETRY_PROGRAM, /**< Geometry program. */
		GPT_DOMAIN_PROGRAM, /**< Domain (tesselation evaluation) program. */
		GPT_HULL_PROGRAM, /**< Hull (tesselation control) program. */
		GPT_COUNT // Keep at end
	};

	/**
	 * Values that represent hardware buffer usage. These usually determine in what type of memory is buffer placed in,
	 * however that depends on rendering API.
	 */
	enum GpuBufferUsage
	{
		/**
		 * Signifies that you don't plan on modifying the buffer often (or at all) after creation. Modifying such buffer
		 * will involve a larger performance hit. Mutually exclusive with GBU_DYNAMIC.
		 */
		GBU_STATIC = 1 << 0,
		/**
		 * Signifies that you will modify this buffer fairly often (e.g. every frame). Mutually exclusive with GBU_STATIC.
		 */
		GBU_DYNAMIC = 1 << 1,
		/** Siginifies that the buffer can be used for arbitrary load/store operations on the GPU. Implies GBU_STATIC. */
		GBU_LOADSTORE = GBU_STATIC | 1 << 2
	};

	/** Different types of GPU views that control how GPU sees a hardware buffer. */
	enum GpuViewUsage
	{
		/** Buffer is seen as a default shader resource, used primarily for reading. (for example a texture for sampling) */
		GVU_DEFAULT = 0x01,
		/** Buffer is seen as a render target that color pixels will be written to after pixel shader stage. */
		GVU_RENDERTARGET = 0x02,
		/** Buffer is seen as a depth stencil target that depth and stencil information is written to. */
		GVU_DEPTHSTENCIL = 0x04,
		/** Buffer that allows you to write to any part of it from within a GPU program. */
		GVU_RANDOMWRITE = 0x08
	};

    /** Combineable set of bits that describe a set of physical GPU's. */
    enum GpuDeviceFlags
    {
        /**
         * Use the default set of devices. This may be the primary device or multiple devices. Cannot be used together with
         * other device flags.
         */
        GDF_DEFAULT = 0,
        /** Use only the primary GPU. */
        GDF_PRIMARY = 0x01,
        /** Use the second GPU. */
        GDF_GPU2 = 0x02,
        /** Use the third GPU. */
        GDF_GPU3 = 0x04,
        /** Use the fourth GPU. */
        GDF_GPU4 = 0x08,
        /** Use the fifth GPU. */
        GDF_GPU5 = 0x10
    };

    /** Type of a parameter in a GPU program. */
    enum GpuParamType
    {
        GPT_DATA, /**< Raw data type like float, Vector3, Color, etc. */
        GPT_TEXTURE, /**< Texture type (2D, 3D, cube, etc.) */
        GPT_BUFFER, /**< Data buffer (raw, structured, etc.) */
        GPT_SAMPLER /**< Sampler type (2D, 3D, cube, etc.) */
    };

    /**	Type of GPU data parameters that can be used as inputs to a GPU program. */
    enum GpuParamDataType
    {
        GPDT_FLOAT1 = 1, /**< 1D floating point value. */
        GPDT_FLOAT2 = 2, /**< 2D floating point value. */
        GPDT_FLOAT3 = 3, /**< 3D floating point value. */
        GPDT_FLOAT4 = 4, /**< 4D floating point value. */
        GPDT_MATRIX_2X2 = 11, /**< 2x2 matrix. */
        GPDT_MATRIX_2X3 = 12, /**< 2x3 matrix. */
        GPDT_MATRIX_2X4 = 13, /**< 2x4 matrix. */
        GPDT_MATRIX_3X2 = 14, /**< 3x2 matrix. */
        GPDT_MATRIX_3X3 = 15, /**< 3x3 matrix. */
        GPDT_MATRIX_3X4 = 16, /**< 3x4 matrix. */
        GPDT_MATRIX_4X2 = 17, /**< 4x2 matrix. */
        GPDT_MATRIX_4X3 = 18, /**< 4x3 matrix. */
        GPDT_MATRIX_4X4 = 19, /**< 4x4 matrix. */
        GPDT_INT1 = 20, /**< 1D signed integer value. */
        GPDT_INT2 = 21, /**< 2D signed integer value. */
        GPDT_INT3 = 22, /**< 3D signed integer value. */
        GPDT_INT4 = 23, /**< 4D signed integer value. */
        GPDT_BOOL = 24, /**< Boolean value. */
        GPDT_STRUCT = 25, /**< Variable size structure. */
        GPDT_COLOR = 26, /**< Same as GPDT_FLOAT4, but can be used to better deduce usage. */
        GPDT_COUNT = 27, // Keep at end before GPDT_UNKNOWN
        GPDT_UNKNOWN = 0xffff
    };

    /**	Type of GPU object parameters that can be used as inputs to a GPU program. */
    enum GpuParamObjectType
    {
        GPOT_SAMPLER1D = 1, /**< Sampler state for a 1D texture. */
        GPOT_SAMPLER2D = 2, /**< Sampler state for a 2D texture. */
        GPOT_SAMPLER3D = 3, /**< Sampler state for a 3D texture. */
        GPOT_SAMPLERCUBE = 4, /**< Sampler state for a cube texture. */
        GPOT_SAMPLER2DMS = 5, /**< Sampler state for a 2D texture with multiple samples. */
        GPOT_TEXTURE1D = 11, /**< 1D texture. */
        GPOT_TEXTURE2D = 12, /**< 2D texture. */
        GPOT_TEXTURE3D = 13, /**< 3D texture. */
        GPOT_TEXTURECUBE = 14, /**< Cube texture. */
        GPOT_TEXTURE2DMS = 15, /**< 2D texture with multiple samples. */
        GPOT_BYTE_BUFFER = 32, /**< Buffer containing raw bytes (no interpretation). */
        GPOT_STRUCTURED_BUFFER = 33, /**< Buffer containing a set of structures. */
        GPOT_RWTYPED_BUFFER = 41, /**< Read-write buffer containing a set of primitives. */
        GPOT_RWBYTE_BUFFER = 42, /**< Read-write buffer containing raw bytes (no interpretation). */
        GPOT_RWSTRUCTURED_BUFFER = 43, /**< Read-write buffer containing a set of structures. */
        GPOT_RWSTRUCTURED_BUFFER_WITH_COUNTER = 44, /**< Read-write buffer containing a set of structures, with a counter. */
        GPOT_RWAPPEND_BUFFER = 45, /**< Buffer that can be used for appending data in a stack-like fashion. */
        GPOT_RWCONSUME_BUFFER = 46, /**< Buffer that can be used for consuming data in a stack-like fashion. */
        GPOT_RWTEXTURE1D = 50, /**< 1D texture with unordered read/writes. */
        GPOT_RWTEXTURE2D = 51, /**< 2D texture with unordered read/writes. */
        GPOT_RWTEXTURE3D = 52, /**< 3D texture with unordered read/writes. */
        GPOT_RWTEXTURE2DMS = 53, /**< 2D texture with multiple samples and unordered read/writes. */
        GPOT_TEXTURE1DARRAY = 54, /**< 1D texture with multiple array entries. */
        GPOT_TEXTURE2DARRAY = 55, /**< 2D texture with multiple array entries. */
        GPOT_TEXTURECUBEARRAY = 56, /**< Cubemap texture with multiple array entries. */
        GPOT_TEXTURE2DMSARRAY = 57, /**< 2D texture with multiple samples and array entries. */
        GPOT_RWTEXTURE1DARRAY = 58, /**< 1D texture with multiple array entries and unordered read/writes. */
        GPOT_RWTEXTURE2DARRAY = 59, /**< 2D texture with multiple array entries and unordered read/writes. */
        GPOT_RWTEXTURE2DMSARRAY = 60, /**< 2D texture with multiple array entries, samples and unordered read/writes. */
        GPOT_UNKNOWN = 0xffff
    };

    /** These values represent a hint to the driver when writing to a GPU buffer. */
    enum BufferWriteType
    {
        /**
         * Default flag with least restrictions. Can cause a CPU-GPU sync point so avoid using it often (every frame)
         * as that might limit your performance significantly.
         */
        BWT_NORMAL,
        /**
         * Tells the driver to completely discard the contents of the buffer you are writing to. The driver will (most
         * likely) internally allocate another buffer with same specifications (which is fairly fast) and you will avoid
         * CPU-GPU stalls.
         */
         BWT_DISCARD,
         /**
          * Guarantees the driver that you will not be updating any part of the buffer that is currently used. This will
          * also avoid CPU-GPU stalls, without requiring you to discard the entire buffer. However it is hard to guarantee
          * when GPU has finished using a buffer.
          */
          BTW_NO_OVERWRITE
    };

    /** Types of valid formats used for standard GPU buffers. */
    enum GpuBufferFormat
    {
        BF_16X1F, /**< 1D 16-bit floating-point format. */
        BF_16X2F, /**< 2D 16-bit floating-point format. */
        BF_16X4F, /**< 4D 16-bit floating-point format. */
        BF_32X1F, /**< 1D 32-bit floating-point format. */
        BF_32X2F, /**< 2D 32-bit floating-point format. */
        BF_32X3F, /**< 3D 32-bit floating-point format. */
        BF_32X4F, /**< 4D 32-bit floating-point format. */
        BF_8X1,   /**< 1D 8-bit normalized format. */
        BF_8X2,   /**< 2D 8-bit normalized format. */
        BF_8X4,   /**< 4D 8-bit normalized format. */
        BF_16X1,  /**< 1D 16-bit normalized format. */
        BF_16X2,  /**< 2D 16-bit normalized format. */
        BF_16X4,  /**< 4D 16-bit normalized format. */
        BF_8X1S,  /**< 1D 8-bit signed integer format. */
        BF_8X2S,  /**< 2D 8-bit signed integer format. */
        BF_8X4S,  /**< 4D 8-bit signed integer format. */
        BF_16X1S, /**< 1D 16-bit signed integer format. */
        BF_16X2S, /**< 2D 16-bit signed integer format. */
        BF_16X4S, /**< 4D 16-bit signed integer format. */
        BF_32X1S, /**< 1D 32-bit signed integer format. */
        BF_32X2S, /**< 2D 32-bit signed integer format. */
        BF_32X3S, /**< 3D 32-bit signed integer format. */
        BF_32X4S, /**< 4D 32-bit signed integer format. */
        BF_8X1U,  /**< 1D 8-bit unsigned integer format. */
        BF_8X2U,  /**< 2D 8-bit unsigned integer format. */
        BF_8X4U,  /**< 4D 8-bit unsigned integer format. */
        BF_16X1U, /**< 1D 16-bit unsigned integer format. */
        BF_16X2U, /**< 2D 16-bit unsigned integer format. */
        BF_16X4U, /**< 4D 16-bit unsigned integer format. */
        BF_32X1U, /**< 1D 32-bit unsigned integer format. */
        BF_32X2U, /**< 2D 32-bit unsigned integer format. */
        BF_32X3U, /**< 3D 32-bit unsigned integer format. */
        BF_32X4U, /**< 4D 32-bit unsigned integer format. */
        BF_COUNT, /**< Not a valid format. Keep just before BS_UNKNOWN. */
        BF_UNKNOWN = 0xffff /**< Unknown format (used for non-standard buffers, like structured or raw. */
    };

	/** Describes operation that will be used for rendering a certain set of vertices. */
	enum DrawOperationType
	{
		/** Each vertex represents a point. */
		DOT_POINT_LIST = 1,
		/** Each sequential pair of vertices represent a line. */
		DOT_LINE_LIST = 2,
		/** Each vertex (except the first) forms a line with the previous vertex. */
		DOT_LINE_STRIP = 3,
		/** Each sequential 3-tuple of vertices represent a triangle. */
		DOT_TRIANGLE_LIST = 4,
		/** Each vertex (except the first two) form a triangle with the previous two vertices. */
		DOT_TRIANGLE_STRIP = 5,
		/** Each vertex (except the first two) form a triangle with the first vertex and previous vertex. */
		DOT_TRIANGLE_FAN = 6
	};

	/**	Type of mesh indices used, used for determining maximum number of vertices in a mesh. */
	enum IndexType
	{
		IT_16BIT,	/**< 16-bit indices. */
		IT_32BIT    /**< 32-bit indices. */
	};

	/**
	 * Controls what kind of mobility restrictions a scene object has. This is used primarily as a performance hint to
	 * other systems. Generally the more restricted the mobility the higher performance can be achieved.
	 */
	enum class ObjectMobility
	{
		/** Scene object can be moved and has no mobility restrictions. */
		Movable,
		/**
		 * Scene object isn't allowed to be moved but is allowed to be visually changed in other ways (e.g. changing the
		 * displayed mesh or light intensity (depends on attached components).
		 */
		Immovable,
		/** Scene object isn't allowed to be moved nor is it allowed to be visually changed. Object must be fully static. */
		Static
	};

	/** Determines the type of the source image for generating cubemaps. */
	enum class CubemapSourceType
	{
		/** Source is a single image that will be replicated on all cubemap faces. */
		Single,

		/**
		 * Source is a list of 6 images, either sequentially next to each other or in a cross format. The system will
		 * automatically guess the layout and orientation based on the aspect ratio.
		 */
		Faces,

		/** Source is a single spherical panoramic image. */
		Spherical,

		/** Source is a single cylindrical panoramic image. */
		Cylindrical
	};

	/**	Available texture types. */
	enum TextureType
	{
		/** One dimensional texture. Just a row of pixels. */
		TEX_TYPE_1D = 1,
		/** Two dimensional texture. */
		TEX_TYPE_2D = 2,
		/** Three dimensional texture. */
		TEX_TYPE_3D = 3,
		/** Texture consisting out of six 2D textures describing an inside of a cube. Allows special sampling. */
		TEX_TYPE_CUBE_MAP = 4
	};

	/**	Projection type to use by the camera. */
	enum ProjectionType
	{
		/** Projection type where object size remains constant and parallel lines remain parallel. */
		PT_ORTHOGRAPHIC,
		/** Projection type that emulates human vision. Objects farther away appear smaller. */
		PT_PERSPECTIVE
	};

    /**	Texture addressing mode, per component. */
    struct UVWAddressingMode
    {
        UVWAddressingMode()
            :u(TAM_WRAP), v(TAM_WRAP), w(TAM_WRAP)
        { }

        bool operator==(const UVWAddressingMode& rhs) const
        {
            return u == rhs.u && v == rhs.v && w == rhs.w;
        }

        TextureAddressingMode u, v, w;
    };

    /**	References a subset of surfaces within a texture. */
    struct TextureSurface
    {
        TextureSurface(UINT32 mipLevel = 0, UINT32 numMipLevels = 1, UINT32 face = 0, UINT32 numFaces = 1)
            : MipLevel(mipLevel)
            , NumMipLevels(numMipLevels)
            , Face(face)
            , NumFaces(numFaces)
        { }

        /** First mip level to reference. */
        UINT32 MipLevel;

        /** Number of mip levels to reference. Must be greater than zero. */
        UINT32 NumMipLevels;

        /**
         * First face to reference. Face can represent a single cubemap face, or a single array entry in a
         * texture array. If cubemaps are laid out in a texture array then every six sequential faces represent a single
         * array entry.
         */
        UINT32 Face;

        /** Number of faces to reference, if the texture has more than one. */
        UINT32 NumFaces;
    };    
}
