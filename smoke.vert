uniform float currentTime;
 
attribute vec3 velocity;

attribute float startTime;

attribute vec3 random;

varying vec4 color;

varying float timeAlive;

void main( void )
{
    vec4  vertex = gl_Vertex;

    timeAlive = currentTime - startTime;

    if ( timeAlive >= 0.0 )
    {
		if ( timeAlive < 0.5 )
		{
			vertex += vec4( velocity * timeAlive, 0.0 );
        
			// Make them float upward a little
			vertex.y += 0.1 * timeAlive * timeAlive;
		}
		else
		{
			// Get them to where they ended their life as fire
			vertex += vec4( velocity * 0.5, 0.0 );
			vertex.y += 0.1 * 0.5 * 0.5;
			
			// Make them float upward
			vertex.y += 1.0 * timeAlive;		
		}
    }
    
    color = gl_Color;
    
    // Add a little bit of randomness
    color.x *= 1.0 - ( abs( random.x ) * 0.1 );
    color.y *= 1.0 - ( abs( random.y ) * 0.1 );
    
 
    gl_Position  = gl_ModelViewProjectionMatrix * vertex;
}