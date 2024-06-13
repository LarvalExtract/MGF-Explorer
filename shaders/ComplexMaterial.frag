#version 330 core

#pragma include Common.inc.frag

uniform sampler2D basetexture;
uniform sampler2D multitexture;

uniform vec4 diffuse_colour;
uniform vec4 spec_colour;
uniform vec4 amb_colour;
uniform vec4 selfillum_colour;
uniform float shininess;

uniform vec3 colorshift0;
uniform vec3 colorshift1;

uniform vec3 eyePosition;

in vec3 worldPosition;
in vec2 texCoord;
in vec3 worldNormal;

out vec4 fragColour;

#define M_PI 3.14159265

vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 TransformHSV(
    vec3 inputCol,  // color to transform
    float h,          // hue shift (in degrees)
    float s,          // saturation multiplier (scalar)
    float v           // value multiplier (scalar)
) {
    float vsu = v * s * cos(h * M_PI/180.0);
    float vsw = v * s * sin(h * M_PI/180.0);
    
    vec3 ret = vec3(0.0);
    ret.r = (.299*v + .701*vsu + .168*vsw)*inputCol.r
        +   (.587*v - .587*vsu + .330*vsw)*inputCol.g
        +   (.114*v - .114*vsu - .497*vsw)*inputCol.b;
    ret.g = (.299*v - .299*vsu - .328*vsw)*inputCol.r
        +   (.587*v + .413*vsu + .035*vsw)*inputCol.g
        +   (.114*v - .114*vsu + .292*vsw)*inputCol.b;
    ret.b = (.299*v - .300*vsu + 1.25*vsw)*inputCol.r
        +   (.587*v - .588*vsu - 1.05*vsw)*inputCol.g
        +   (.114*v + .886*vsu - .203*vsw)*inputCol.b;
    
    return ret;
}

void main()
{
	vec4 base_colour = texture(basetexture, texCoord);
	vec4 mix_factor = texture(multitexture, texCoord);
	
	vec3 colorshift0_hsv = rgb2hsv(colorshift0);
	vec3 colorshift1_hsv = rgb2hsv(colorshift1);
	
	vec4 color0_shifted = base_colour + vec4(TransformHSV(base_colour.xyz, colorshift0_hsv.x, colorshift0_hsv.y, colorshift0_hsv.z), base_colour.w);
	vec4 color1_shifted = base_colour + vec4(TransformHSV(base_colour.xyz, colorshift1_hsv.x, colorshift1_hsv.y, colorshift1_hsv.z), base_colour.w);
	
	vec4 diffuse = vec4(mix(color0_shifted.xyz, color1_shifted.xyz, mix_factor.xyz), 1.0);
	
	vec4 specular = vec4(spec_colour.xyz, texture(basetexture, texCoord).w); // specular channel seems to be alpha in mechassault
	vec4 ambient = amb_colour;

	vec3 worldView = normalize(eyePosition - worldPosition);

	vec4 lit = computeLighting(ambient, diffuse, specular, shininess, worldPosition, worldView, worldNormal);
	lit += diffuse * selfillum_colour;
	fragColour = lit;
}