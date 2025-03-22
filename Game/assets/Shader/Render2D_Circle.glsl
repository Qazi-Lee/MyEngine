#type  Vertex

#version 460 core
layout(location=0) in vec3  a_WorldPosition;
layout(location=1) in vec3  a_LocalPosition;
layout(location=2) in vec4  a_Color;
layout(location=3) in vec2  a_Texture;
layout(location=4) in float a_TextureIndex;
layout(location=5) in float a_Thickness;
layout(location=6) in float a_Fade; 
layout(location=7) in int a_EntityID;
uniform mat4 u_ViewProjection;
out vec3 v_LocalPosition;
out vec4 v_Color;
out vec2 v_Texture;
out flat float v_TextureIndex;
out flat float v_Thickness;
out flat float v_Fade;
out flat int v_EntityID;
void main()
{
	v_LocalPosition = a_LocalPosition;
	v_Color = a_Color;
	v_Texture=a_Texture;
	v_TextureIndex=a_TextureIndex;
	v_Thickness = a_Thickness;
	v_Fade = a_Fade;
	v_EntityID = a_EntityID;
	gl_Position = u_ViewProjection*vec4(a_WorldPosition,1.0);
}

#type Frage

#version 460 core
layout(location=0) out vec4 color;
layout(location=1) out int o_EntityID;

uniform sampler2D u_Texture[32];

in vec3 v_LocalPosition;
in vec4 v_Color;
in vec2 v_Texture;
in flat float v_TextureIndex;
in flat float v_Thickness;
in flat float v_Fade;
in flat int v_EntityID;

in flat int vEntityID;
void main() 
{

    float distance = 1.0 - length(v_LocalPosition);
    float circle = smoothstep(0.0, v_Fade, distance);
    circle *= smoothstep(v_Thickness + v_Fade, v_Thickness, distance);

	if (circle == 0.0)
		discard;
	
	color=texture(u_Texture[int(v_TextureIndex)],v_Texture)*vec4(v_Color);
	color.a*=circle;


	o_EntityID=v_EntityID+1;
}