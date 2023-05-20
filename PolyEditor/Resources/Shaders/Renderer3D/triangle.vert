
#version 450

 const vec3 g_vertex_buffer_data[] = {
    vec3(-1.0f,-1.0f,-1.0f),  // -X side
    vec3(-1.0f,-1.0f, 1.0f),
    vec3(-1.0f, 1.0f, 1.0f),
    vec3(-1.0f, 1.0f, 1.0f),
    vec3(-1.0f, 1.0f,-1.0f),
    vec3(-1.0f,-1.0f,-1.0f),

    vec3(-1.0f,-1.0f,-1.0f),  // -Z side
    vec3( 1.0f, 1.0f,-1.0f),
    vec3( 1.0f,-1.0f,-1.0f),
    vec3(-1.0f,-1.0f,-1.0f),
    vec3(-1.0f, 1.0f,-1.0f),
    vec3( 1.0f, 1.0f,-1.0f),

    vec3(-1.0f,-1.0f,-1.0f),  // -Y side
    vec3( 1.0f,-1.0f,-1.0f),
    vec3( 1.0f,-1.0f, 1.0f),
    vec3(-1.0f,-1.0f,-1.0f),
    vec3( 1.0f,-1.0f, 1.0f),
    vec3(-1.0f,-1.0f, 1.0f),

    vec3(-1.0f, 1.0f,-1.0f),  // +Y side
    vec3(-1.0f, 1.0f, 1.0f),
    vec3( 1.0f, 1.0f, 1.0f),
    vec3(-1.0f, 1.0f,-1.0f),
    vec3( 1.0f, 1.0f, 1.0f),
    vec3( 1.0f, 1.0f,-1.0f),

     vec3(1.0f, 1.0f,-1.0f),  // +X side
     vec3(1.0f, 1.0f, 1.0f),
     vec3(1.0f,-1.0f, 1.0f),
     vec3(1.0f,-1.0f, 1.0f),
     vec3(1.0f,-1.0f,-1.0f),
     vec3(1.0f, 1.0f,-1.0f),
     
    vec3(-1.0f, 1.0f, 1.0f),  // +Z side
    vec3(-1.0f,-1.0f, 1.0f),
    vec3( 1.0f, 1.0f, 1.0f),
    vec3(-1.0f,-1.0f, 1.0f),
    vec3( 1.0f,-1.0f, 1.0f),
    vec3( 1.0f, 1.0f, 1.0f)
};

void main()
{
	//const array of positions for the triangle
	const vec3 positions[3] = vec3[3](
		vec3(1.f,1.f, 0.0f),
		vec3(-1.f,1.f, 0.0f),
		vec3(0.f,-1.f, 0.0f)
	);

	//output the position of each vertex
	gl_Position = vec4(g_vertex_buffer_data[gl_VertexIndex] * 0.5, 1.0f);
}