/// @ref ext_ivec1_precision
/// @file glm/ext/ivec1_precision.hpp
///
/// @see core (dependence)
///
/// @defgroup ext_ivec1_precision GLM_EXT_ivec1_precision
/// @ingroup ext
///
/// Include <glm/ext/ivec1_precision.hpp> to use the features of this extension.
///
/// Add Exposes highp_ivec1, mediump_ivec1 and lowp_ivec1 types.

#pragma once

#include "../detail/type_vec1.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_uvec1_precision extension included")
#endif

namespace glm
{
	/// @addtogroup ext_ivec1_precision
	/// @{

	/// 1 component vector of signed integer values.
	///
	/// @see ext_ivec1_precision
	typedef vec<1, int, highp>			highp_ivec1;

	/// 1 component vector of signed integer values.
	///
	/// @see ext_ivec1_precision
	typedef vec<1, int, mediump>		mediump_ivec1;

	/// 1 component vector of signed integer values.
	///
	/// @see ext_ivec1_precision
	typedef vec<1, int, lowp>			lowp_ivec1;

	/// @}
}//namespace glm
