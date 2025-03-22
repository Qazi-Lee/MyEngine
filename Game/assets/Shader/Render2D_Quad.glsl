#type  Vertex

#version 460 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec2 aTextureCoord;
layout(location=2) in float aTextureIndex;
layout(location=3) in vec4 aColor;
layout(location=4) in int aEntityID; 
uniform mat4 u_ViewProjection;
out vec2 vTextureCoord;
out flat float vTextureIndex;
out vec4 vColor;
out flat int vEntityID;
void main()
{
 vTextureCoord=aTextureCoord;
 vTextureIndex=aTextureIndex;
 vColor=aColor;
 vEntityID=aEntityID;
 gl_Position = u_ViewProjection*vec4(aPos.x, aPos.y, aPos.z, 1.0);
}

#type Frage

#version 460 core
layout(location=0) out vec4 color;
layout(location=1) out int id;
uniform sampler2D u_Texture[32];
in vec2 vTextureCoord;
in flat float vTextureIndex;
in vec4 vColor;
in flat int vEntityID;
void main() 
{
	color=texture(u_Texture[int(vTextureIndex)],vTextureCoord)*vec4(vColor);
	id=vEntityID+1;
}
