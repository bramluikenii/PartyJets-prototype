uniform float wavetime;
uniform float wavewidth;
uniform float waveheight;
varying vec2 texCoord;

 
void main(void)
{
	vec4 v = vec4(gl_Vertex);
    
//	v.y = sin(v.x + wavetime)*waveheight;
	v.y = sin(wavewidth * v.x + wavetime) * cos(wavewidth * v.z + wavetime) * waveheight;

	texCoord = gl_MultiTexCoord0.xy;
	gl_Position = gl_ModelViewProjectionMatrix * v;
}