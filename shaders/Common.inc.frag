#pragma include phong.inc.frag

uniform uint shadingTypeEnum;


// Return some bullshit colour if some functionality isn't implemented
vec4 unimplemented()
{
	vec4 result = vec4(1.0, 0.0, 0.0, 1.0);
	
	if (int(gl_FragCoord.x) % 2 == 0 && int(gl_FragCoord.y) % 2 == 0)
	{
		result.x = 0.0;
		result.z = 1.0;
	}
	
	return result;
}

vec4 computeLighting(const in vec4 ambient,
                   const in vec4 diffuse,
                   const in vec4 specular,
                   const in float shininess,
                   const in vec3 worldPosition,
                   const in vec3 worldView,
                   const in vec3 worldNormal)
{
	vec4 result = vec4(0.0);

	switch (shadingTypeEnum)
	{
	case 0u: // gouraud
		result = phongFunction(ambient, diffuse, specular, shininess, worldPosition, worldView, worldNormal);
		break;
	case 1u: // material_only
		result = diffuse + specular + ambient;
		break;
	case 2u: // translucent (very rare)
	case 3u: // none (what does this mean?)
		result = unimplemented();
		break;
	}
	
	return result;
}