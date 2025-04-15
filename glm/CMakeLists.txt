#glm

file(GLOB_RECURSE GLM_SOURCES
"glm/common.hpp"


"glm/exponential.hpp"


"glm/ext.hpp"


"glm/fwd.hpp"


"glm/geometric.hpp"


"glm/glm.hpp"


"glm/integer.hpp"


"glm/mat2x2.hpp"


"glm/mat2x3.hpp"


"glm/mat2x4.hpp"


"glm/mat3x2.hpp"


"glm/mat3x3.hpp"


"glm/mat3x4.hpp"


"glm/mat4x2.hpp"


"glm/mat4x3.hpp"


"glm/mat4x4.hpp"


"glm/matrix.hpp"


"glm/packing.hpp"


"glm/trigonometric.hpp"


"glm/vec2.hpp"


"glm/vec3.hpp"


"glm/vec4.hpp"


"glm/vector_relational.hpp"


"glm/pch.h"


"glm/detail/_features.hpp"


"glm/detail/_fixes.hpp"


"glm/detail/_noise.hpp"


"glm/detail/_swizzle.hpp"


"glm/detail/_swizzle_func.hpp"


"glm/detail/_vectorize.hpp"


"glm/detail/compute_common.hpp"


"glm/detail/compute_vector_decl.hpp"


"glm/detail/compute_vector_relational.hpp"


"glm/detail/qualifier.hpp"


"glm/detail/setup.hpp"


"glm/detail/type_float.hpp"


"glm/detail/type_half.hpp"


"glm/detail/type_mat2x2.hpp"


"glm/detail/type_mat2x3.hpp"


"glm/detail/type_mat2x4.hpp"


"glm/detail/type_mat3x2.hpp"


"glm/detail/type_mat3x3.hpp"


"glm/detail/type_mat3x4.hpp"


"glm/detail/type_mat4x2.hpp"


"glm/detail/type_mat4x3.hpp"


"glm/detail/type_mat4x4.hpp"


"glm/detail/type_quat.hpp"


"glm/detail/type_vec1.hpp"


"glm/detail/type_vec2.hpp"


"glm/detail/type_vec3.hpp"


"glm/detail/type_vec4.hpp"


"glm/ext/_matrix_vectorize.hpp"


"glm/ext/matrix_clip_space.hpp"


"glm/ext/matrix_common.hpp"


"glm/ext/matrix_double2x2.hpp"


"glm/ext/matrix_double2x2_precision.hpp"


"glm/ext/matrix_double2x3.hpp"


"glm/ext/matrix_double2x3_precision.hpp"


"glm/ext/matrix_double2x4.hpp"


"glm/ext/matrix_double2x4_precision.hpp"


"glm/ext/matrix_double3x2.hpp"


"glm/ext/matrix_double3x2_precision.hpp"


"glm/ext/matrix_double3x3.hpp"


"glm/ext/matrix_double3x3_precision.hpp"


"glm/ext/matrix_double3x4.hpp"


"glm/ext/matrix_double3x4_precision.hpp"


"glm/ext/matrix_double4x2.hpp"


"glm/ext/matrix_double4x2_precision.hpp"


"glm/ext/matrix_double4x3.hpp"


"glm/ext/matrix_double4x3_precision.hpp"


"glm/ext/matrix_double4x4.hpp"


"glm/ext/matrix_double4x4_precision.hpp"


"glm/ext/matrix_float2x2.hpp"


"glm/ext/matrix_float2x2_precision.hpp"


"glm/ext/matrix_float2x3.hpp"


"glm/ext/matrix_float2x3_precision.hpp"


"glm/ext/matrix_float2x4.hpp"


"glm/ext/matrix_float2x4_precision.hpp"


"glm/ext/matrix_float3x2.hpp"


"glm/ext/matrix_float3x2_precision.hpp"


"glm/ext/matrix_float3x3.hpp"


"glm/ext/matrix_float3x3_precision.hpp"


"glm/ext/matrix_float3x4.hpp"


"glm/ext/matrix_float3x4_precision.hpp"


"glm/ext/matrix_float4x2.hpp"


"glm/ext/matrix_float4x2_precision.hpp"


"glm/ext/matrix_float4x3.hpp"


"glm/ext/matrix_float4x3_precision.hpp"


"glm/ext/matrix_float4x4.hpp"


"glm/ext/matrix_float4x4_precision.hpp"


"glm/ext/matrix_int2x2.hpp"


"glm/ext/matrix_int2x2_sized.hpp"


"glm/ext/matrix_int2x3.hpp"


"glm/ext/matrix_int2x3_sized.hpp"


"glm/ext/matrix_int2x4.hpp"


"glm/ext/matrix_int2x4_sized.hpp"


"glm/ext/matrix_int3x2.hpp"


"glm/ext/matrix_int3x2_sized.hpp"


"glm/ext/matrix_int3x3.hpp"


"glm/ext/matrix_int3x3_sized.hpp"


"glm/ext/matrix_int3x4.hpp"


"glm/ext/matrix_int3x4_sized.hpp"


"glm/ext/matrix_int4x2.hpp"


"glm/ext/matrix_int4x2_sized.hpp"


"glm/ext/matrix_int4x3.hpp"


"glm/ext/matrix_int4x3_sized.hpp"


"glm/ext/matrix_int4x4.hpp"


"glm/ext/matrix_int4x4_sized.hpp"


"glm/ext/matrix_integer.hpp"


"glm/ext/matrix_projection.hpp"


"glm/ext/matrix_relational.hpp"


"glm/ext/matrix_transform.hpp"


"glm/ext/matrix_uint2x2.hpp"


"glm/ext/matrix_uint2x2_sized.hpp"


"glm/ext/matrix_uint2x3.hpp"


"glm/ext/matrix_uint2x3_sized.hpp"


"glm/ext/matrix_uint2x4.hpp"


"glm/ext/matrix_uint2x4_sized.hpp"


"glm/ext/matrix_uint3x2.hpp"


"glm/ext/matrix_uint3x2_sized.hpp"


"glm/ext/matrix_uint3x3.hpp"


"glm/ext/matrix_uint3x3_sized.hpp"


"glm/ext/matrix_uint3x4.hpp"


"glm/ext/matrix_uint3x4_sized.hpp"


"glm/ext/matrix_uint4x2.hpp"


"glm/ext/matrix_uint4x2_sized.hpp"


"glm/ext/matrix_uint4x3.hpp"


"glm/ext/matrix_uint4x3_sized.hpp"


"glm/ext/matrix_uint4x4.hpp"


"glm/ext/matrix_uint4x4_sized.hpp"


"glm/ext/quaternion_common.hpp"


"glm/ext/quaternion_double.hpp"


"glm/ext/quaternion_double_precision.hpp"


"glm/ext/quaternion_exponential.hpp"


"glm/ext/quaternion_float.hpp"


"glm/ext/quaternion_float_precision.hpp"


"glm/ext/quaternion_geometric.hpp"


"glm/ext/quaternion_relational.hpp"


"glm/ext/quaternion_transform.hpp"


"glm/ext/quaternion_trigonometric.hpp"


"glm/ext/scalar_common.hpp"


"glm/ext/scalar_constants.hpp"


"glm/ext/scalar_int_sized.hpp"


"glm/ext/scalar_integer.hpp"


"glm/ext/scalar_packing.hpp"


"glm/ext/scalar_reciprocal.hpp"


"glm/ext/scalar_relational.hpp"


"glm/ext/scalar_uint_sized.hpp"


"glm/ext/scalar_ulp.hpp"


"glm/ext/vector_bool1.hpp"


"glm/ext/vector_bool1_precision.hpp"


"glm/ext/vector_bool2.hpp"


"glm/ext/vector_bool2_precision.hpp"


"glm/ext/vector_bool3.hpp"


"glm/ext/vector_bool3_precision.hpp"


"glm/ext/vector_bool4.hpp"


"glm/ext/vector_bool4_precision.hpp"


"glm/ext/vector_common.hpp"


"glm/ext/vector_double1.hpp"


"glm/ext/vector_double1_precision.hpp"


"glm/ext/vector_double2.hpp"


"glm/ext/vector_double2_precision.hpp"


"glm/ext/vector_double3.hpp"


"glm/ext/vector_double3_precision.hpp"


"glm/ext/vector_double4.hpp"


"glm/ext/vector_double4_precision.hpp"


"glm/ext/vector_float1.hpp"


"glm/ext/vector_float1_precision.hpp"


"glm/ext/vector_float2.hpp"


"glm/ext/vector_float2_precision.hpp"


"glm/ext/vector_float3.hpp"


"glm/ext/vector_float3_precision.hpp"


"glm/ext/vector_float4.hpp"


"glm/ext/vector_float4_precision.hpp"


"glm/ext/vector_int1.hpp"


"glm/ext/vector_int1_sized.hpp"


"glm/ext/vector_int2.hpp"


"glm/ext/vector_int2_sized.hpp"


"glm/ext/vector_int3.hpp"


"glm/ext/vector_int3_sized.hpp"


"glm/ext/vector_int4.hpp"


"glm/ext/vector_int4_sized.hpp"


"glm/ext/vector_integer.hpp"


"glm/ext/vector_packing.hpp"


"glm/ext/vector_reciprocal.hpp"


"glm/ext/vector_relational.hpp"


"glm/ext/vector_uint1.hpp"


"glm/ext/vector_uint1_sized.hpp"


"glm/ext/vector_uint2.hpp"


"glm/ext/vector_uint2_sized.hpp"


"glm/ext/vector_uint3.hpp"


"glm/ext/vector_uint3_sized.hpp"


"glm/ext/vector_uint4.hpp"


"glm/ext/vector_uint4_sized.hpp"


"glm/ext/vector_ulp.hpp"


"glm/gtc/bitfield.hpp"


"glm/gtc/color_space.hpp"


"glm/gtc/constants.hpp"


"glm/gtc/epsilon.hpp"


"glm/gtc/integer.hpp"


"glm/gtc/matrix_access.hpp"


"glm/gtc/matrix_integer.hpp"


"glm/gtc/matrix_inverse.hpp"


"glm/gtc/matrix_transform.hpp"


"glm/gtc/noise.hpp"


"glm/gtc/packing.hpp"


"glm/gtc/quaternion.hpp"


"glm/gtc/random.hpp"


"glm/gtc/reciprocal.hpp"


"glm/gtc/round.hpp"


"glm/gtc/type_aligned.hpp"


"glm/gtc/type_precision.hpp"


"glm/gtc/type_ptr.hpp"


"glm/gtc/ulp.hpp"


"glm/gtc/vec1.hpp"


"glm/simd/common.h"


"glm/simd/exponential.h"


"glm/simd/geometric.h"


"glm/simd/integer.h"


"glm/simd/matrix.h"


"glm/simd/neon.h"


"glm/simd/packing.h"


"glm/simd/platform.h"


"glm/simd/trigonometric.h"


"glm/simd/vector_relational.h"

"glm/detail/func_common.inl"


"glm/detail/func_common_simd.inl"


"glm/detail/func_exponential.inl"


"glm/detail/func_exponential_simd.inl"


"glm/detail/func_geometric.inl"


"glm/detail/func_geometric_simd.inl"


"glm/detail/func_integer.inl"


"glm/detail/func_integer_simd.inl"


"glm/detail/func_matrix.inl"


"glm/detail/func_matrix_simd.inl"


"glm/detail/func_packing.inl"


"glm/detail/func_packing_simd.inl"


"glm/detail/func_trigonometric.inl"


"glm/detail/func_trigonometric_simd.inl"


"glm/detail/func_vector_relational.inl"


"glm/detail/func_vector_relational_simd.inl"


"glm/detail/type_half.inl"


"glm/detail/type_mat2x2.inl"


"glm/detail/type_mat2x3.inl"


"glm/detail/type_mat2x4.inl"


"glm/detail/type_mat3x2.inl"


"glm/detail/type_mat3x3.inl"


"glm/detail/type_mat3x4.inl"


"glm/detail/type_mat4x2.inl"


"glm/detail/type_mat4x3.inl"


"glm/detail/type_mat4x4.inl"


"glm/detail/type_mat4x4_simd.inl"


"glm/detail/type_quat.inl"


"glm/detail/type_quat_simd.inl"


"glm/detail/type_vec_simd.inl"


"glm/detail/type_vec1.inl"


"glm/detail/type_vec2.inl"


"glm/detail/type_vec3.inl"


"glm/detail/type_vec4.inl"


"glm/detail/type_vec4_simd.inl"


"glm/ext/matrix_clip_space.inl"


"glm/ext/matrix_common.inl"


"glm/ext/matrix_integer.inl"


"glm/ext/matrix_projection.inl"


"glm/ext/matrix_relational.inl"


"glm/ext/matrix_transform.inl"


"glm/ext/quaternion_common.inl"


"glm/ext/quaternion_common_simd.inl"


"glm/ext/quaternion_exponential.inl"


"glm/ext/quaternion_geometric.inl"


"glm/ext/quaternion_relational.inl"


"glm/ext/quaternion_transform.inl"


"glm/ext/quaternion_trigonometric.inl"


"glm/ext/scalar_common.inl"


"glm/ext/scalar_constants.inl"


"glm/ext/scalar_integer.inl"


"glm/ext/scalar_packing.inl"


"glm/ext/scalar_reciprocal.inl"


"glm/ext/scalar_relational.inl"


"glm/ext/scalar_ulp.inl"


"glm/ext/vector_common.inl"


"glm/ext/vector_integer.inl"


"glm/ext/vector_packing.inl"


"glm/ext/vector_reciprocal.inl"


"glm/ext/vector_relational.inl"


"glm/ext/vector_ulp.inl"


"glm/gtc/bitfield.inl"


"glm/gtc/color_space.inl"


"glm/gtc/constants.inl"


"glm/gtc/epsilon.inl"


"glm/gtc/integer.inl"


"glm/gtc/matrix_access.inl"


"glm/gtc/matrix_inverse.inl"


"glm/gtc/matrix_transform.inl"


"glm/gtc/noise.inl"


"glm/gtc/packing.inl"


"glm/gtc/quaternion.inl"


"glm/gtc/quaternion_simd.inl"


"glm/gtc/random.inl"


"glm/gtc/round.inl"


"glm/gtc/type_precision.inl"


"glm/gtc/type_ptr.inl"


"glm/gtc/ulp.inl"

"glm/detail/glm.cpp"

)

add_library(glm STATIC ${GLM_SOURCES})


target_compile_definitions(glm PUBLIC 
    _LIB
)

target_include_directories(glm PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)   
