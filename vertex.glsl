#version 400 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 3) in float aColor;

out vec3 vertexColor;
out vec3 outNormal;
out vec3 outPosition;
out float outColor;


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


vec3 floatToRgb(float v) {

   float r = (v / (256 * 256)) / 256.0;
   float g = mod(v / 256, 256) / 256.0;
   float b = mod(v, 256.0) / 256.0;
   return vec3(r, g, b);
}



float rnd(vec2 x)
{
    int n = int(x.x * 40.0 + x.y * 6400.0);
    n = (n << 13) ^ n;
    return 1.0 - float( (n * (n * n * 15731 + 789221) + \
             1376312589) & 0x7fffffff) / 1073741824.0;
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
   vec3 result = (vec3(0.2, 0.2, 0.2) + diffuse) ;
  
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

   //vertexColor = vec3(1, 1, 1);
   //vertexColor = result;
   //vertexColor = vec3(fclampf(rnd(aPos.xy), 0, 1), fclampf(rnd(aPos.zx), 0, 1), fclampf(rnd(aPos.zy), 0, 1));


   vertexColor = floatToRgb(aColor);
   outColor = aColor;
   outNormal = aNormal;
   //outPosition = vec3(persp * transform * vec4(aPos, 1.0f));
   outPosition = aPos;
}