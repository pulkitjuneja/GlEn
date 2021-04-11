#version 330 core
const vec2 quad_vertices[4] = vec2[4]( vec2( -1.0, -1.0), vec2( 1.0, -1.0), vec2( -1.0, 1.0), vec2( 1.0, 1.0));
const vec2 quad_tex_coords[4] = vec2[4](vec2( 0.0, 0.0), vec2( 1.0, 0.0), vec2( 0.0, 1.0), vec2( 1.0, 1.0));

out vec4 fragPos;
out vec2 fragTexcoords;

void main()
{	
	fragPos = vec4(quad_vertices[gl_VertexID], 0.0, 1.0);
	fragTexcoords = quad_tex_coords[gl_VertexID];
    gl_Position = fragPos;
}