#version 400 core

out vec4 FragColor;
in vec3  col;
in vec3  pos;
in vec3  normal;
in vec3  fragPos;  

void main()
{

   vec3 norm      = normalize(normal);
   vec3 lightDir  = normalize(vec3(5, 1, 5) - pos);  
   float d        = max(dot(norm, lightDir), 0.0);

   vec3 diffuse   = d * vec3(1.0, 1.0, 1.0);


   vec3 viewDir      = normalize(pos);
   vec3 reflectDir   = reflect(-lightDir, norm);
   float spec        = pow(max(dot(viewDir, reflectDir), 0.0), 32);
   vec3 specular     = 0.5 * spec * vec3(1.0, 1.0, 1.0);  

   vec3 result    = (diffuse + specular) * col;

   FragColor = vec4(result.x, result.y, result.z, 1.0f);
}