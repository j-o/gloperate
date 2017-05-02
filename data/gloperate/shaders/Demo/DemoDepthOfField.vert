#version 140
#extension GL_ARB_explicit_attrib_location : require

uniform mat4 view;
uniform mat4 projection;
uniform vec2 dofShift;

layout (location = 0) in vec3 a_vertex;
layout (location = 1) in vec3 a_color;

flat out vec3 v_color;

void main()
{
    vec4 viewPos = view * vec4(a_vertex, 1.0);
    vec4 shiftedPos = vec4(viewPos.xy + dofShift * (viewPos.z + 0.1), viewPos.zw);
    gl_Position = projection * shiftedPos;

    v_color = a_color;
}
