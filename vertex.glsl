#version 400 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

varying vec3 vertexColor;
out vec3 outNormal;
out vec3 outPosition;

uniform mat4	transform;
uniform mat4	persp;
uniform mat4	Model;


float fclampf(float value, float vmin, float vmax)
{
   if (value >= vmax)  
      value = vmax;
   if (value <= vmin)
      value = vmin;
   return (value);
}

void main()
{
   gl_Position =   persp * transform * vec4(aPos, 1.0f);

   vec3 posLight = vec3(15.0, 10.0, 50.0);
   

   vec3 colorByPos = vec3(
      fclampf(aPos.y * 0.4f + 0.4f, 0, 1),
		fclampf(aPos.z * 0.1 + aPos.y * 0.4f, 0, 1), 0.2f);

   vec3 dir = normalize(posLight - aPos);
   float diff = max(dot(aNormal, dir), 0.0);
   vec3 diffuse = diff * vec3(1.0, 0.7, 0.7);
   vec3 result = (vec3(0.1, 0.1, 0.1) + diffuse) * vec3(0.8, 0.8, 0.8);
  
   vec4 transfed = persp * transform * vec4(aPos, 1.0f);
/*
   vec3 modelViewVertex = vec3(persp * transform * vec4(aPos, 1.0f));
 
    // Transform the normal's orientation into eye space.
    vec3 modelViewNormal = vec3(persp * transform * vec4(aNormal, 0.0));
    float distance = length(posLight - modelViewVertex);
    vec3 lightVector = normalize(posLight - modelViewVertex);
    float diffuse = max(dot(modelViewNormal, lightVector), 0.1);
    diffuse = diffuse * (1.0 / (1.0 + (0.25 * distance * distance)));
 
    // Multiply the color by the illumination level. It will be interpolated across the triangle.
    vec3 result = aColor * diffuse;
   */

   vec3 colorByPosq = vec3(aPos.x, aPos.y, aPos.z);

   vertexColor = colorByPos;
   outNormal = aNormal;
   //outPosition = vec3(persp * transform * vec4(aPos, 1.0f));
   outPosition = aPos;
}