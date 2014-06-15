// FRAGMENT SHADER //

uniform sampler2D colorMap;
varying vec2 texCoord;

void main()
{
	vec4 diffuse = texture2D(colorMap, texCoord);
	gl_FragColor = diffuse;
}
