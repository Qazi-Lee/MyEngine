 #type  Vertex
 #version 460 core
layout(location=0) in vec4  a_Position;
layout(location=1) in vec2  a_TextureCoord;
layout(location=2) in vec4  a_Color;
uniform mat4 u_ViewProjection;
out vec2 v_TextureCoord;
out vec4 v_Color;
void main()
{
    v_TextureCoord=a_TextureCoord;
    v_Color=a_Color;
    gl_Position = u_ViewProjection*vec4(a_Position);
}

#type Frage

#version 460 core
layout(location=0) out vec4 color;
in vec2 v_TextureCoord;
in vec4 v_Color;
uniform sampler2D u_Texture[32];
void main()
{
    float distance = texture(u_Texture[31],v_TextureCoord).r;
    float alpha = smoothstep(0.5 - 0.1, 0.5 + 0.1, distance);
    color = vec4(1.0f,1.0f,1.0f,alpha);
}