in vec2 texCoord;
out vec4 colour;

uniform vec3 u_diffuse;
uniform vec3 u_selfillum;
uniform float u_shininess;
uniform float u_shin_strength;
uniform vec3 u_spec_color;
uniform float u_opacity;
uniform bool u_two_sided;

uniform bool u_selected = false;

const vec4 yellow = vec4(1.0, 1.0, 0.0, 1.0);

void CullBackface()
{
    if (!u_two_sided && !gl_FrontFacing)
        discard;
}

void Highlight()
{
    mix(colour, yellow, 0.5);
}