// minimal vertex shader
// www.lighthouse3d.com


/*
void main()
{	

	// the following three lines provide the same result

//	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
//	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_Position = ftransform();
}

*/
uniform float time;

void main(void)
{
        vec4 v = vec4(gl_Vertex);
        
        v.z = sin(5.0*v.x + time*1.0)*0.25;

        gl_Position = gl_ModelViewProjectionMatrix * v;
} 