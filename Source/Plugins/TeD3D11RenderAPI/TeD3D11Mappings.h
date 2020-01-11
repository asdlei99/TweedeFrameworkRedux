#pragma once

#include "TeD3D11RenderAPIPrerequisites.h"
#include "RenderAPI/TeCommonTypes.h"

namespace te
{
    /**	Helper class that maps engine types to DirectX 11 types. */
	class D3D11Mappings
	{
	public:
        /**	Converts engine to DX11 specific texture addressing mode. */
        static D3D11_TEXTURE_ADDRESS_MODE Get(TextureAddressingMode tam);

		/**	Converts engine to DX11 specific blend factor. */
		static D3D11_BLEND Get(BlendFactor bf);

		/**	Converts engine to DX11 specific blend operation. */
		static D3D11_BLEND_OP Get(BlendOperation bo);

        /** Return DirectX 11 stencil operation and optionally invert it (greater than becomes less than, etc.). */
        static D3D11_STENCIL_OP Get(StencilOperation op, bool invert = false);

        /**	Converts engine to DX11 specific comparison function. */
		static D3D11_COMPARISON_FUNC Get(CompareFunction cf);

		/**	Converts engine to DX11 specific culling mode. */
		static D3D11_CULL_MODE Get(CullingMode cm);

		/**	Converts engine to DX11 specific polygon fill mode. */
		static D3D11_FILL_MODE Get(PolygonMode mode);

        /**	Returns DX11 primitive topology based on the provided draw operation type. */
		static D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveType(DrawOperationType type);

        /**
         * Converts engine texture filter type to DirectX 11 filter shift (used for combining to get actual min/mag/mip
         * filter bit location).
         */
        static DWORD Get(FilterType ft);

        /**
         * Returns DirectX 11 texture filter from the provided min, mag and mip filter options, and optionally a filter
         * with comparison support.
         */
        static D3D11_FILTER Get(const FilterOptions min, const FilterOptions mag,
            const FilterOptions mip, const bool comparison = false);

		/**	Converts engine color to DX11 color. */
		static void Get(const Color& inColor, float* outColor);
    };
}
