#version 400 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 iNormal;

uniform mat4	Transform;
uniform mat4	Persp;
uniform mat4	Model;
uniform vec3   kd;


out vec3 pos;
out vec3 col;
out vec3 normal;
out vec3 fragPos;

void main()
{
   col      = kd;
   pos      = aPos;
   fragPos  = vec3((Persp * Transform * Model) * vec4(aPos, 1.0));
   normal   = iNormal;
   
   gl_Position =   (Persp * Transform * Model) * vec4(aPos, 1.0f);
}