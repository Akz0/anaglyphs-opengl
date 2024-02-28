#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCords;

uniform mat4 ViewProjectionMatrix;


void main(){
	vec4 pos = ViewProjectionMatrix * vec4(aPos,1.0f);

	gl_Position = vec4(pos.x,pos.y,pos.w,pos.w);
	TexCords = vec3(aPos.x,aPos.y,-aPos.z);

}
