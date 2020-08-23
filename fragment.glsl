#version 400 core

out vec4 FragColor;

in vec3 vertexColor;

in vec3 outNormal;
in vec3 outPosition;
in float outColor;

vec3 floatToRgb(float v) {

   float r = (v / (256 * 256)) / 256.0;
   float g = mod(v / 256, 256) / 256.0;
   float b = mod(v, 256.0) / 256.0;
   return vec3(r, g, b);
}


void main()
{
   /*
   vec3 posLight = vec3(60.0, 20.0, 0.0);
   vec3 dir = normalize(posLight - outPosition);
   float diff = max(dot(outNormal, dir), 0.0);
   vec3 diffuse = diff * vec3(1.0, 0.7, 0.7);
   vec3 result = (vec3(0.1, 0.1, 0.1) + diffuse) * vertexColor;
   */
   FragColor = vec4(vertexColor, 1.0);
}